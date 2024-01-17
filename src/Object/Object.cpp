#include "Object.h"
#include "Event/EventHandle.h"

void Object::destroy() noexcept{
	markedForDestroy = true;
	onDestroy();
}

void Object::onDestroy() noexcept {}

void Object::setOwner(Object* object) noexcept {
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

void Object::forEachChild(const std::function<bool(Object*)>& function) const noexcept {
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
	Object* oldInstigator = instigator.get();
	instigator = object;
	onInstigatorChanged(oldInstigator);
}

void Object::onInstigatorChanged(Object* oldInstigator) noexcept {}

void Object::scanEvents() noexcept {
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
}

void Object::registerEventHandle(class EventHandleBase* handle) noexcept {
	if(handle == nullptr){
		return;
	}

	ownedEventHandles.insert(handle);
}

void Object::unregisterEventHandle(EventHandleBase* handle) noexcept {
	ownedEventHandles.erase(handle);
}

inline void Object::registerChild(Object* child) noexcept {
	WeakObjectPtr<Object> newChild = child;
	if(!newChild.isValid()){
		return;
	}

	childrenObjects.insert(newChild);
	onChildAdded(child);
}

inline void Object::removeChild(Object* child) noexcept {
	if(child == nullptr){
		return;
	}

	if(childrenObjects.erase(child) == 0){
		return;
	}

	onChildRemoved(child);
}
