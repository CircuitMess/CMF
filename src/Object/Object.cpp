#include "Object.h"
#include "Event/EventHandle.h"
#include "Memory/ObjectMemory.h"
#include "Thread/Threaded.h"

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

void Object::scanEvents(TickType_t wait/* = 0*/) noexcept {
	if(!eventScanningMutex.try_lock()){
		// TODO: log error for circular ownership
		eventScanningMutex.unlock();
		return;
	}

	const uint64_t begin = millis();
	wait = std::min(wait, getEventScanningTime());

	std::lock_guard lock(eventHandleMutex);

	for(EventHandleBase* handle = nullptr; readyEventHandles.pop(handle, std::max((uint64_t)0, wait - (millis() - begin))); ){
		handle->scan(wait);
	}

	// WARNING: This will not work, or will create an infinite loop if owner system is abused, this is intentional, events are dependent on their owner to scan events, outermost owner must be an async entity for this to work
	for(const WeakObjectPtr<Object>& child : childrenObjects){
		if(!child.isValid()){
			continue;
		}

		child->scanEvents(std::max((uint64_t)0, wait - (millis() - begin)));
	}

	eventScanningMutex.unlock();
}

void Object::registerEventHandle(EventHandleBase* handle) noexcept {
	static uint32_t eventIndex = 0;

	if(handle == nullptr){
		return;
	}

	std::lock_guard lock(eventHandleMutex);

	const std::string threadName = std::string("EventThread_").append(std::to_string(eventIndex++));

	EventHandleContainer container = {};
	container.ownedEventHandle = handle;
	container.eventThread = newObject<Threaded>(this, [this, handle](){
		if(handle->probe(portMAX_DELAY)){
			readyEventHandles.push(handle);
		}
	}, threadName, 0, 256);

	ownedEventHandles.insert(container);
}

void Object::unregisterEventHandle(EventHandleBase* handle) noexcept {
	std::lock_guard lock(eventHandleMutex);

	for(const EventHandleContainer& container : ownedEventHandles){
		if(container.ownedEventHandle == handle){
			container.eventThread->setInterval(portMAX_DELAY);
			container.eventThread->pause();
			ownedEventHandles.erase(container);
			break;
		}
	}
}

TickType_t Object::getEventScanningTime() const noexcept{
	return 0;
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
