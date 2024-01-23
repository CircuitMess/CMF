#include "Entity.h"

Entity::Entity() noexcept : Super() {

}

Entity::~Entity() noexcept{

}

bool Entity::hasBegun() const noexcept{
	return begun;
}

void Entity::postInitProperties() noexcept {

}

void Entity::begin() noexcept{
	begun = true;
}

void Entity::tick(float deltaTime) noexcept{

}

void Entity::end() noexcept{

}
