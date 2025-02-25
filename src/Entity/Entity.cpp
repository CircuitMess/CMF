#include "Entity.h"
#include "Memory/ObjectMemory.h"
#include "Statics/ApplicationStatics.h"
#include "Core/Application.h"

Entity::Entity() noexcept : Super() {
	beginMutex.lock();
}

Entity::~Entity() noexcept = default;

bool Entity::hasBegun() const noexcept{
	return begun;
}

void Entity::postInitProperties() noexcept {
	Super::postInitProperties();

	beginMutex.unlock();
}

void Entity::begin() noexcept{
	std::lock_guard lock(beginMutex);
	begun = true;
}

void Entity::tick(float deltaTime) noexcept{
	std::lock_guard lock(beginMutex);
}

void Entity::end(EndReason reason) noexcept{
	std::lock_guard lock(beginMutex);
}
