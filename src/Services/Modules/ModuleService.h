#ifndef CMF_MODULESERVICE_H
#define CMF_MODULESERVICE_H

#include <map>
#include "Entity/AsyncEntity.h"
#include "ModuleType.h"
#include "Event/EventBroadcaster.h"
#include "Drivers/Interface/InputDriver.h"
#include "Periphery/I2C.h"
#include "Drivers/Interface/OutputDriver.h"
#include "ModuleDevice.h"
#include "ModuleDefs.h"

/**
* Service for managing UMAX modules (https://www.lcsc.com/datasheet/C404108.pdf)
* that follow this pinout specification: https://docs.google.com/spreadsheets/d/1MGPnOqgmIhoZG_LD7VQy7eJBQMKvngh-zGssaOBs1kE/edit?usp=sharing
 *
 * Its job is to detect and manage instances of plugged-in modules.
 *
 * @tparam NumBus Number of independent UMAX ports (defaults to 1)
*/

using namespace Modules;

template<uint8_t NumBus = 1>
class ModuleService : public AsyncEntity {
	GENERATED_BODY(ModuleService, AsyncEntity);
public:

	enum class Action : uint8_t {
		Insert, Remove
	};

	/**
	 * Generic constructor
	 * @param busPins array of UMAX bus pin structures
	 */
	ModuleService(std::array<Modules::BusPins, NumBus> busPins = {}) : AsyncEntity(), busPins(std::move(busPins)){
		populateInputDrivers();
		for(uint8_t i = 0; i < NumBus; i++){
			registerSubAddressPinsInput(i);
		}
	}

	/**
	 * Specialized constructor for a single bus
	 * @param busPinsSingle UMAX bus pins
	 */
	ModuleService(const Modules::BusPins& busPinsSingle, std::enable_if_t<NumBus == 1, int> = 0) : ModuleService(std::array<Modules::BusPins, 1>{ busPinsSingle }){

	}

	/**
	 * uint8_t - busID
	 * Type - type of module
	 * Action - Insert or Remove
	 */
	DECLARE_EVENT(ModulesEvent, ModuleService, uint8_t, Type, Action)

	ModulesEvent modulesEvent{ this };

	StrongObjectPtr<ModuleDevice> getDevice(uint8_t bus = 0){
		if(bus >= NumBus) return nullptr;

		return busContexts[bus].instance;
	}

	Type getInserted(uint8_t bus = 0){
		if(bus >= NumBus) return Type::Unknown;

		if(!busContexts[bus].inserted){
			return Type::Unknown;
		}

		return busContexts[bus].type;
	}


private:
	const std::array<Modules::BusPins, NumBus> busPins;

	/**
	 * Model of the current bus state
	 */
	struct BusContext {
		bool inserted;
		Type type;
		StrongObjectPtr<ModuleDevice> instance;
	} busContexts[NumBus];


	void tick(float deltaTime) noexcept override{
		Super::tick(deltaTime);
		scanAllInputs();
		for(uint8_t i = 0; i < NumBus; i++){
			loopCheck(i);
		}
	}

	void loopCheck(uint8_t bus){
		const bool nowInserted = checkInserted(bus);
		if(busContexts[bus].inserted && !nowInserted){
			busContexts[bus].inserted = false;
			const auto removed = busContexts[bus].type;

			busContexts[bus].type = Type::Unknown;

			ESP_LOGI(TAG, "Module %d removed from bus %d", (int) removed, bus);
			modulesEvent.broadcast(bus, removed, Action::Remove);

			busContexts[bus].instance = nullptr;

			registerSubAddressPinsInput(bus);

		}else if(!busContexts[bus].inserted && nowInserted){
			const Type type = checkAddr(bus);

			busContexts[bus].type = type;
			busContexts[bus].inserted = true;

			ESP_LOGI(TAG, "Module %d inserted into bus %d", (int) type, bus);
			modulesEvent.broadcast(bus, type, Action::Insert);

			registerSubAddressPinsModule(bus, type);
			busContexts[bus].instance = CreateModuleDevice(this, type, busPins[bus]);
		}
	}

	bool checkInserted(uint8_t bus){
		//TODO - make DET pin reading interrupt driven instead of polling

		const auto pin1 = busPins[bus].detPins[0];
		const auto pin2 = busPins[bus].detPins[1];

		const bool det1 = pin1.driver->read(pin1.port);
		const bool det2 = pin2.driver->read(pin2.port);
		return det1 == 0 && det2 == 1;
	}

	Type checkAddr(uint8_t bus){
		Address readAddress{};

		uint8_t addr = 0;
		for(uint8_t i = 0; i < 6; i++){
			const auto pin = busPins[bus].addr[i];
			auto state = pin.driver->read(pin.port);
			if(state){
				addr |= 1 << i;
			}
		}

		ESP_LOGI(TAG, "primary address %d", addr);

		readAddress.mainAddress = addr;


		if(!SubAddressMap.contains(addr)){
			if(AddressMap.contains(readAddress)){
				return AddressMap.at(readAddress);
			}else{
				ESP_LOGE(TAG, "Unknown primary address");
				return Type::Unknown;
			}
		}

		const std::set<SubAddress>& subAddressSet = SubAddressMap.at(addr);
		SubAddress readSubAddress{};
		readSubAddress.type = SubAddress::Type::None;
		/*
		 * Caching read values to std::optional of each subAddress type.
		 *
		 * I2C values don't make sense to cache here, since every possible I2C address needs to be probed individually
		 * (instead of being read once and then compared multiple times)
		 */
		std::optional<uint8_t> ReadRMAddress;
		std::optional<uint8_t> ReadTokenAddress;

		//Check every possible subAddress for this mainAddress, stop when match is found
		for(const SubAddress& subAddress : subAddressSet){
			if(subAddress.type == SubAddress::Type::RM){
				/* RM non-I2C modules are sub-addressed using subAddr pins 4 - 6 (3 bits)  */
				if(!ReadRMAddress){
					uint8_t RMAddr = 0;
					for(uint8_t i = 0; i < 3; i++){
						const auto pin = busPins[bus].subAddressPins[i + 3];
						const auto state = pin.inputDriver->read(pin.port);
						if(state){
							RMAddr |= 1 << i;
						}
					}
					ReadRMAddress = RMAddr;
				}

				if(subAddress.RMAddress == ReadRMAddress){
					readSubAddress = subAddress;
					break;
				}
			}else if(subAddress.type == SubAddress::Type::RM_I2C || subAddress.type == SubAddress::Type::Rover_I2C){
				/* RM I2C and Rover I2C addresses determined by probing */
				if(busPins[bus].i2c->probe(subAddress.I2CAddress) == ESP_OK){
					readSubAddress = subAddress;
					break;
				}
			}else if(subAddress.type == SubAddress::Type::Token){
				/* Bit/Wacky robots are sub-addressed using all 6 SubAddress pins */
				if(!ReadTokenAddress){
					uint8_t tokenAddr = 0;
					for(uint8_t i = 0; i < 6; i++){
						const auto pin = busPins[bus].subAddressPins[i];
						const auto state = pin.inputDriver->read(pin.port);
						if(state){
							tokenAddr |= 1 << i;
						}
					}
					ReadTokenAddress = tokenAddr;
				}

				if(subAddress.tokenAddress == ReadTokenAddress){
					readSubAddress = subAddress;
					break;
				}
			}
		}

		if(readSubAddress.type == SubAddress::Type::None){
			ESP_LOGE(TAG, "Unknown main-sub address combination");
			return Type::Unknown;
		}

		readAddress.subAddress = readSubAddress;

		if(!AddressMap.contains(readAddress)){
			ESP_LOGE(TAG, "Error in main/sub address mapping (internal error)");
			return Type::Unknown;
		}

		return AddressMap.at(readAddress);
	}

	/**
	 * Mapping the whole Address to a specific Type.
	 */
	inline static const std::map<Address, Type> AddressMap = GetAddressMap();

	/**
	 * Mapping the main address to all available SubAddresses. Does not include modules with no SubAddress.
	 * Populated in the constructor using data from AddressMap
	 * Used for module detection.
	 */
	inline static const std::map<uint8_t, std::set<SubAddress>> SubAddressMap = GetSubAddressMap();

	//Set of inputDrivers prevents unnecessary polling of the same InputDriver multiple times.
	std::set<InputDriver*> inputDriverSet;

	/**
	 * Populates the inputDriverSets with their respective inputDrivers
	 */
	void populateInputDrivers(){
		for(uint8_t bus = 0; bus < NumBus; ++bus){
			auto& pins = busPins[bus];

			// Address pins
			for(const auto& pin : pins.addr){
				if(pin.driver) inputDriverSet.insert(pin.driver);
			}
			// Detection pins
			for(const auto& pin : pins.detPins){
				if(pin.driver) inputDriverSet.insert(pin.driver);
			}
			// SubAddress pins
			for(const auto& ioPin : pins.subAddressPins){
				if(ioPin.inputDriver) inputDriverSet.insert(ioPin.inputDriver);
			}
		}
	}

	/**
	 * Calls scan on every InputDriver across all registered buses.
	 */
	void scanAllInputs(){
		for(auto driver : inputDriverSet){
			driver->scan();
		}
	}

	/**
	 * Registers all subAddress pins to their InputDrivers.
	 * This enables using them for address scanning.
	 * @param bus
	 */
	void registerSubAddressPinsInput(uint8_t bus){
		for(const auto& pin : busPins[bus].subAddressPins){
			pin.inputDriver->registerInput({ pin.port });
		}
	}

	void registerSubAddressPinsModule(uint8_t bus, Modules::Type type){
		const auto& pinModes = GetPinModeMap().at(type);
		for(uint8_t i = 0; i < pinModes.size(); i++){
			const auto& pin = busPins[bus].subAddressPins[i];
			if(pinModes[i] == PinMode::Input){
				pin.inputDriver->registerInput({ pin.port });
			}else if(pinModes[i] == PinMode::Output){
				pin.outputDriver->registerOutput({ pin.port });
			}
		}
	}

	static constexpr const char* TAG = "ModuleService";
};


#endif //CMF_MODULESERVICE_H
