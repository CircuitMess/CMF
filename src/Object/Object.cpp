#include "Object.h"
#include "Event/EventHandle.h"
#include "Memory/ObjectMemory.h"
#include "Thread/Threaded.h"
#include "Log/Log.h"
#include "Statics/ApplicationStatics.h"
#include "Core/Application.h"

Object::Object() noexcept : id(ObjectIndex++){
	destroyMutex.lock();
}

void Object::postInitProperties() noexcept {}

bool Object::canDelete() noexcept{
	return destroyMutex.try_lock();
}

void Object::destroy() noexcept{
	markedForDestroy = true;
}

void Object::onDestroy() noexcept {
	forEachChild([](Object* child){
		if(isValid(child)){
			return false;
		}

		Application* app = ApplicationStatics::getApplication();
		if(isValid(app) && !app->isShuttingDown()){
			child->setOwner(ApplicationStatics::getApplication());
		}else{
			child->destroy();
			child->onDestroy();
		}

		return false;
	});

	destroyMutex.unlock();
}

void Object::setOwner(Object* object) noexcept {
	Object* oldOwner = nullptr;

	{
		std::lock_guard lock(ownershipMutex);

		oldOwner = owner.get();

		if(owner.isValid()){
			owner->removeChild(this);
			owner = nullptr;
		}

		if(object != nullptr && object != this){
			owner = object;
			owner->registerChild(this);
		}
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

void Object::scanEvents(TickType_t wait) noexcept {
	if(!eventScanningMutex.try_lock()){
		CMF_LOG(LogCMF, Warning, "Circular object ownership detected. Object: %s, Owner: %s", getName().c_str(), getOwner()->getName().c_str());
		eventScanningMutex.unlock();
		return;
	}

	const uint64_t begin = millis();
	wait = std::min(wait, getEventScanningTime());

	std::lock_guard lock(eventHandleMutex);

	for(EventHandleBase* handle = nullptr; readyEventHandles.pop(handle, std::max((int64_t) 0, (int64_t) wait - ((int64_t) millis() - (int64_t) begin))); ){
		if(handle == nullptr){
			continue;
		}

		handle->scan(0);
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

	const std::string threadName = getName().append("_EventHandle").append(std::to_string(eventIndex++)).append("_Thread");

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

TickType_t Object::getEventScanningTime() const noexcept {
	return 0;
}

Archive& Object::serialize(Archive& archive) noexcept {
	return archive;
}

Application* Object::getApp() noexcept {
	return ApplicationStatics::getApplication();
}

Object* Object::getOutermostOwner() const noexcept{
	Object* outermost = getOwner();
	if(outermost == nullptr){
		return nullptr;
	}

	while(outermost != nullptr && outermost->getOwner() != nullptr) {
		outermost = outermost->getOwner();
	}

	return outermost;
}

Object* Object::getOutermostInstigator() const noexcept{
	Object* outermost = getInstigator();
	if(outermost == nullptr){
		return nullptr;
	}

	while(outermost != nullptr && outermost->getInstigator() != nullptr) {
		outermost = outermost->getInstigator();
	}

	return outermost;
}

inline void Object::registerChild(Object* child) noexcept {
	// NOTE: ownershipMutex must be locked before calling this function to avoid multithreading issues. If it is not locked, bugs can occur.
	WeakObjectPtr<Object> newChild = child;
	if(!newChild.isValid()){
		return;
	}

	childrenObjects.insert(newChild);
	onChildAdded(child);
}

inline void Object::removeChild(Object* child) noexcept {
	// NOTE: ownershipMutex must be locked before calling this function to avoid multithreading issues. If it is not locked, bugs can occur.
	if(child == nullptr){
		return;
	}

	if(childrenObjects.erase(child) == 0){
		return;
	}

	onChildRemoved(child);
}
