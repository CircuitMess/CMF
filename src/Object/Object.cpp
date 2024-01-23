#include "Object.h"
#include "Event/EventHandle.h"

Object::Object() noexcept{
	destroyMutex.lock();
}

void Object::postInitProperties() noexcept {}

void Object::onCreated() noexcept {}

bool Object::canDelete() noexcept{
	return destroyMutex.try_lock();
}

void Object::destroy() noexcept{
	markedForDestroy = true;
}

void Object::onDestroy() noexcept {
	destroyMutex.unlock();
}

void Object::setOwner(Object* object) noexcept {
	std::lock_guard lock(ownershipMutex);

	Object* oldOwner = owner.get();

	if(owner.isValid()){
		owner->removeChild(this);
		owner = nullptr;
	}

	if(object != nullptr && object != this){
		owner = object;
		owner->registerChild(this);
	}

	onOwnerChanged(oldOwner);
}

void Object::onOwnerChanged(Object* oldOwner) noexcept {}

void Object::onChildAdded(Object* child) noexcept {}

void Object::onChildRemoved(Object* child) noexcept {}

void Object::forEachChild(const std::function<bool(Object*)>& function) noexcept {
	std::lock_guard lock(ownershipMutex);

	for(const WeakObjectPtr<Object>& child : childrenObjects){
		if(!child.isValid()){
			continue;
		}

		if(function(child.get())){
			return;
		}
	}
}

void Object::setInstigator(Object* object) noexcept {
	std::lock_guard lock(instigatorMutex);
	Object* oldInstigator = instigator.get();
	instigator = object;
	onInstigatorChanged(oldInstigator);
}

void Object::onInstigatorChanged(Object* oldInstigator) noexcept {}

void Object::scanEvents() noexcept {
	if(!eventScanningMutex.try_lock()){
		// TODO: log error for circular ownership
		eventScanningMutex.unlock();
		return;
	}

	std::lock_guard lock(eventHandleMutex);

	for(class EventHandleBase* handle : ownedEventHandles){
		handle->scan(0); // TODO: wait time
	}

	// WARNING: This will not work, or will create an infinite loop if owner system is abused, this is intentional, events are dependent on their owner to scan events, outermost owner must be an async entity for this to work
	for(const WeakObjectPtr<Object>& child : childrenObjects){
		if(!child.isValid()){
			continue;
		}

		child->scanEvents();
	}

	eventScanningMutex.unlock();
}

void Object::registerEventHandle(class EventHandleBase* handle) noexcept {
	if(handle == nullptr){
		return;
	}

	std::lock_guard lock(eventHandleMutex);
	ownedEventHandles.insert(handle);
}

void Object::unregisterEventHandle(EventHandleBase* handle) noexcept {
	std::lock_guard lock(eventHandleMutex);
	ownedEventHandles.erase(handle);
}

inline void Object::registerChild(Object* child) noexcept {
	std::lock_guard lock(ownershipMutex);

	WeakObjectPtr<Object> newChild = child;
	if(!newChild.isValid()){
		return;
	}

	childrenObjects.insert(newChild);
	onChildAdded(child);
}

inline void Object::removeChild(Object* child) noexcept {
	std::lock_guard lock(ownershipMutex);

	if(child == nullptr){
		return;
	}

	if(childrenObjects.erase(child) == 0){
		return;
	}

	onChildRemoved(child);
}
