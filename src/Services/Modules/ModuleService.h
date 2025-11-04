#ifndef CMF_MODULESERVICE_H
#define CMF_MODULESERVICE_H

#include <map>
#include "Entity/AsyncEntity.h"
#include "ModuleType.h"
#include "Event/EventBroadcaster.h"
#include "Drivers/Interface/InputDriver.h"
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


template<uint8_t NumBus = 1>
class ModuleService : public AsyncEntity {
	GENERATED_BODY(ModuleService, AsyncEntity, CONSTRUCTOR_PACK(std::array<Modules::BusPins, NumBus>));
public:

	enum class Action : uint8_t {
		Insert, Remove
	};

	/**
	 * Generic constructor
	 * @param busPins array of UMAX bus pin structures
	 */
	ModuleService(std::array<Modules::BusPins, NumBus> busPins = {}) : Super(), busPins(std::move(busPins)){
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
	DECLARE_EVENT(ModulesEvent, ModuleService, uint8_t, Modules::Type, Action)
	ModulesEvent modulesEvent{ this };

	StrongObjectPtr<ModuleDevice> getDevice(uint8_t bus = 0){
		if(bus >= NumBus) return nullptr;

		return busContexts[bus].instance;
	}

	Modules::Type getInserted(uint8_t bus = 0){
		if(bus >= NumBus) return Modules::Type::Unknown;

		if(!busContexts[bus].inserted){
			return Modules::Type::Unknown;
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
		Modules::Type type;
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

			busContexts[bus].type = Modules::Type::Unknown;

			ESP_LOGI(TAG, "Module %d removed from bus %d", (int) removed, bus);
			modulesEvent.broadcast(bus, removed, Action::Remove);

			busContexts[bus].instance = nullptr;

			registerSubAddressPinsInput(bus);
		}else if(!busContexts[bus].inserted && nowInserted){
			const Modules::Type type = checkAddr(bus);

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

	Modules::Type checkAddr(uint8_t bus){
		Modules::Address readAddress{};

		uint8_t addr = 0;
		for(uint8_t i = 0; i < 6; i++){
			const InputPin pin = busPins[bus].addr[i];

			if(pin.driver->read(pin.port) > 0.0f){
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
				return Modules::Type::Unknown;
			}
		}

		const std::set<Modules::SubAddress>& subAddressSet = SubAddressMap.at(addr);
		Modules::SubAddress readSubAddress{};
		readSubAddress.type = Modules::SubAddress::Type::None;
		/*
		 * Caching read values to std::optional of each subAddress type.
		 *
		 * I2C values don't make sense to cache here, since every possible I2C address needs to be probed individually
		 * (instead of being read once and then compared multiple times)
		 */
		std::optional<uint8_t> ReadRMAddress;
		std::optional<uint8_t> ReadTokenAddress;

		//Check every possible subAddress for this mainAddress, stop when match is found
		for(const Modules::SubAddress& subAddress : subAddressSet){
			if(subAddress.type == Modules::SubAddress::Type::RM){
				/* RM non-I2C modules are sub-addressed using subAddr pins 4 - 6 (3 bits)  */
				if(!ReadRMAddress){
					uint8_t RMAddr = 0;
					for(uint8_t i = 0; i < 3; i++){
						const Modules::IOPin pin = busPins[bus].subAddressPins[i + 3];

						if(pin.inputDriver->read(pin.port) > 0.0f){
							RMAddr |= 1 << i;
						}
					}
					ReadRMAddress = RMAddr;
				}

				if(subAddress.RMAddress == ReadRMAddress){
					readSubAddress = subAddress;
					break;
				}
			}else if(subAddress.type == Modules::SubAddress::Type::RM_I2C || subAddress.type == Modules::SubAddress::Type::Rover_I2C){
				/* RM I2C and Rover I2C addresses determined by probing */
				if(busPins[bus].i2c->probe(subAddress.I2CAddress) == ESP_OK){
					readSubAddress = subAddress;
					break;
				}
			}else if(subAddress.type == Modules::SubAddress::Type::Token){
				/* Bit/Wacky robots are sub-addressed using all 6 SubAddress pins */
				if(!ReadTokenAddress){
					uint8_t tokenAddr = 0;
					for(uint8_t i = 0; i < 6; i++){
						const Modules::IOPin pin = busPins[bus].subAddressPins[i];

						if(pin.inputDriver->read(pin.port) > 0.0f){
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

		if(readSubAddress.type == Modules::SubAddress::Type::None){
			ESP_LOGE(TAG, "Unknown main-sub address combination");
			return Modules::Type::Unknown;
		}

		readAddress.subAddress = readSubAddress;

		if(!AddressMap.contains(readAddress)){
			ESP_LOGE(TAG, "Error in main/sub address mapping (internal error)");
			return Modules::Type::Unknown;
		}

		return AddressMap.at(readAddress);
	}

	/**
	 * Mapping the whole Address to a specific Type.
	 */
	inline static const std::map<Modules::Address, Modules::Type> AddressMap = Modules::GetAddressMap();

	/**
	 * Mapping the main address to all available SubAddresses. Does not include modules with no SubAddress.
	 * Populated in the constructor using data from AddressMap
	 * Used for module detection.
	 */
	inline static const std::map<uint8_t, std::set<Modules::SubAddress>> SubAddressMap = Modules::GetSubAddressMap();

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
	void scanAllInputs() const {
		for(InputDriver* driver : inputDriverSet){
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
		const auto& pinModes = Modules::GetPinModeMap().at(type);
		for(uint8_t i = 0; i < pinModes.size(); i++){
			const auto& pin = busPins[bus].subAddressPins[i];
			if(pinModes[i] == Modules::PinMode::Input){
				pin.inputDriver->registerInput({ pin.port });
			}else if(pinModes[i] == Modules::PinMode::Output){
				pin.outputDriver->registerOutput({ pin.port });
			}
		}
	}

	static constexpr const char* TAG = "ModuleService";
};


#endif //CMF_MODULESERVICE_H
