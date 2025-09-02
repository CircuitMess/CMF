#ifndef CMF_MODULEDEFS_H
#define CMF_MODULEDEFS_H

#include <Object/SubclassOf.h>
#include "ModuleType.h"
#include "ModuleDevice.h"
#include <map>

/**
 * Model of Module sub-address.
 * Necessary since sub-addressing follows various types and methods of addressing.
 */
struct ModuleSubAddress {
	enum class Type : uint8_t {
		None, RM, RM_I2C, Token, Rover_I2C
	} type;
	union {
		uint8_t RMAddress; //Rick&Morty Butterbot non-I2C modules
		uint8_t I2CAddress; //Rick&Morty Butterbot, Rover I2C modules
		uint8_t tokenAddress; //STEM adventure tokens (Bit)
	};

	bool operator<(const ModuleSubAddress& rhs) const{
		if(type < rhs.type)
			return true;
		if(rhs.type < type)
			return false;
		if(RMAddress < rhs.RMAddress)
			return true;
		if(rhs.RMAddress < RMAddress)
			return false;
		if(I2CAddress < rhs.I2CAddress)
			return true;
		if(rhs.I2CAddress < I2CAddress)
			return false;
		return tokenAddress < rhs.tokenAddress;
	}

	bool operator>(const ModuleSubAddress& rhs) const{
		return rhs < *this;
	}

	bool operator<=(const ModuleSubAddress& rhs) const{
		return !(rhs < *this);
	}

	bool operator>=(const ModuleSubAddress& rhs) const{
		return !(*this < rhs);
	}
};

/**
 * Unique representation of a module, through a main and sub address.
 */
struct ModuleAddress {
	uint8_t mainAddress;
	ModuleSubAddress subAddress;

	bool operator<(const ModuleAddress& rhs) const{
		if(mainAddress < rhs.mainAddress)
			return true;
		if(rhs.mainAddress < mainAddress)
			return false;
		return subAddress < rhs.subAddress;
	}

	bool operator>(const ModuleAddress& rhs) const{
		return rhs < *this;
	}

	bool operator<=(const ModuleAddress& rhs) const{
		return !(rhs < *this);
	}

	bool operator>=(const ModuleAddress& rhs) const{
		return !(*this < rhs);
	}
};

const std::map<ModuleAddress, ModuleType>& GetAddressMap();
const std::map<ModuleType, SubclassOf<ModuleDevice>>& GetDeviceMap();
const std::map<uint8_t, std::set<ModuleSubAddress>> GetSubAddressMap();

#endif //CMF_MODULEDEFS_H
