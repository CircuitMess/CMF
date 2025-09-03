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

template<uint8_t NumBus = 1>
class ModuleService : public AsyncEntity {
	GENERATED_BODY(ModuleService, AsyncEntity);
public:

	enum class Action : uint8_t {
		Insert, Remove
	};

	/**
	 * Helper struct for pins which can be input and output, depending on Module type.
	 */
	struct IOPin {
		InputDriverBase* inputDriver;
		OutputDriverBase* outputDriver;
		int port;
	};

	struct BusPins {
		InputPin addr[6], detPins[2];
		I2C* i2c;
		IOPin subAddressPins[6];
	};


	/**
	 * Generic constructor
	 * @param busPins array of UMAX bus pin structures
	 */
	ModuleService(std::array<BusPins, NumBus> busPins = {}) : AsyncEntity(), busPins(std::move(busPins)){
		populateInputDrivers();


	}

	/**
	 * Specialized constructor for a single bus
	 * @param busPinsSingle UMAX bus pins
	 */
	ModuleService(const BusPins& busPinsSingle, std::enable_if_t<NumBus == 1, int> = 0) : ModuleService(std::array<BusPins, 1>{ busPinsSingle }){

	}

	/**
	 * uint8_t - busID
	 * ModuleType - type of module
	 * Action - Insert or Remove
	 */
	DECLARE_EVENT(ModulesEvent, ModuleService, uint8_t, ModuleType, Action)

	ModulesEvent modulesEvent{ this };

	StrongObjectPtr<ModuleDevice> getDevice(uint8_t bus){
		if(bus >= NumBus) return nullptr;

		return busContexts[bus].instance;
	}

	ModuleType getInserted(uint8_t bus){
		if(bus >= NumBus) return ModuleType::Unknown;

		if(!busContexts[bus].inserted){
			return ModuleType::Unknown;
		}

		return busContexts[bus].type;
	}


private:
	const std::array<BusPins, NumBus> busPins;

	/**
	 * Model of the current bus state
	 */
	struct BusContext {
		bool inserted;
		ModuleType type;
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

			busContexts[bus].type = ModuleType::Unknown;

			ESP_LOGI(TAG, "Module %d removed from bus %d", (int) removed, bus);
			modulesEvent.broadcast(bus, removed, Action::Remove);

			busContexts[bus].instance = nullptr;

		}else if(!busContexts[bus].inserted && nowInserted){
			const ModuleType addr = checkAddr(bus);

			busContexts[bus].type = addr;
			busContexts[bus].inserted = true;

			ESP_LOGI(TAG, "Module %d inserted into bus %d", (int) addr, bus);
			modulesEvent.broadcast(bus, addr, Action::Insert);


			if(DeviceMap.contains(addr)){
				busContexts[bus].instance = newObject<ModuleDevice>(*DeviceMap.at(addr), this);

				//Set the input/output pins and I2C for newly created ModuleDevice
				std::array<InputPin, 6> inputs;
				std::array<OutputPin, 6> outputs;
				for(uint8_t i = 0; i < 6; ++i){
					inputs[i] = InputPin{ busPins[bus].subAddressPins[i].inputDriver, busPins[bus].subAddressPins[i].port };
					outputs[i] = OutputPin{ busPins[bus].subAddressPins[i].outputDriver, busPins[bus].subAddressPins[i].port };
				}
				busContexts[bus].instance->setInputs(inputs);
				busContexts[bus].instance->setOutputs(outputs);
				busContexts[bus].instance->setI2C(busPins[bus].i2c);
			}
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

	ModuleType checkAddr(uint8_t bus){
		ModuleAddress readAddress{};

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
				return ModuleType::Unknown;
			}
		}

		const std::set<ModuleSubAddress>& subAddressSet = SubAddressMap.at(addr);
		ModuleSubAddress readSubAddress{};
		readSubAddress.type = ModuleSubAddress::Type::None;
		/*
		 * Caching read values to std::optional of each subAddress type.
		 *
		 * I2C values don't make sense to cache here, since every possible I2C address needs to be probed individually
		 * (instead of being read once and then compared multiple times)
		 */
		std::optional<uint8_t> ReadRMAddress;
		std::optional<uint8_t> ReadTokenAddress;

		//Check every possible subAddress for this mainAddress, stop when match is found
		for(const ModuleSubAddress& subAddress : subAddressSet){
			if(subAddress.type == ModuleSubAddress::Type::RM){
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
			}else if(subAddress.type == ModuleSubAddress::Type::RM_I2C || subAddress.type == ModuleSubAddress::Type::Rover_I2C){
				/* RM I2C and Rover I2C addresses determined by probing */
				if(busPins[bus].i2c->probe(subAddress.I2CAddress) == ESP_OK){
					readSubAddress = subAddress;
					break;
				}
			}else if(subAddress.type == ModuleSubAddress::Type::Token){
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

		if(readSubAddress.type == ModuleSubAddress::Type::None){
			ESP_LOGE(TAG, "Unknown main-sub address combination");
			return ModuleType::Unknown;
		}

		readAddress.subAddress = readSubAddress;

		if(!AddressMap.contains(readAddress)){
			ESP_LOGE(TAG, "Error in main/sub address mapping (internal error)");
			return ModuleType::Unknown;
		}

		return AddressMap.at(readAddress);
	}

	/**
	 * Mapping the whole ModuleAddress to a specific ModuleType.
	 */
	inline static const std::map<ModuleAddress, ModuleType> AddressMap = GetAddressMap();

	/**
	 * Mapping ModuleType to specific module device class
	 */
	inline static const std::map<ModuleType, SubclassOf<ModuleDevice>> DeviceMap = GetDeviceMap();

	/**
	 * Mapping the main address to all available SubAddresses. Does not include modules with no SubAddress.
	 * Populated in the constructor using data from AddressMap
	 * Used for module detection.
	 */
	inline static const std::map<uint8_t, std::set<ModuleSubAddress>> SubAddressMap = GetSubAddressMap();

	//Set of inputDrivers prevents unnecessary polling of the same InputDriver multiple times.
	std::set<InputDriverBase*> inputDriverSet;

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

	inline static const char* TAG = "ModuleService";
};


#endif //CMF_MODULESERVICE_H
