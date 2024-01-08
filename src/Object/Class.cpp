#include "Class.h"

uint32_t Class::getID() const noexcept{
	return classID;
}

Class::Class(uint32_t ID) noexcept : classID(ID) {}
