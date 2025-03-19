#include "Object.h"
#include "Event/EventHandle.h"
#include "Memory/ObjectMemory.h"
#include "Thread/Threaded.h"
#include "Log/Log.h"
#include "Statics/ApplicationStatics.h"
#include "Core/Application.h"

Object::Object() noexcept : id(ObjectIndex++){}

void Object::postInitProperties() noexcept{}

bool Object::canDelete() noexcept{
	std::lock_guard lock(destroyMutex);

	return canBeDeleted;
}

void Object::destroy() noexcept{
	std::lock_guard lock(destroyMutex);

	markedForDestroy = true;

	onDestroy();
}

void Object::onDestroy() noexcept{
	forEachChild([](Object* child){
		if(isValid(child)){
			return false;
		}

		Application* app = ApplicationStatics::getApplication();
		if(isValid(app) && !app->isShuttingDown()){
			child->setOwner(ApplicationStatics::getApplication());
		}else{
			child->destroy();
		}

		return false;
	});

	canBeDeleted = true;
}

void Object::setOwner(Object* object) noexcept{
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

void Object::onOwnerChanged(Object* oldOwner) noexcept{}

void Object::onChildAdded(Object* child) noexcept{}

void Object::onChildRemoved(Object* child) noexcept{}

void Object::forEachChild(const std::function<bool(Object*)>& function) noexcept{
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

void Object::setInstigator(Object* object) noexcept{
	Object* oldInstigator = nullptr;

	{
		std::lock_guard lock(instigatorMutex);
		oldInstigator = instigator.get();
		instigator = object;
	}
	onInstigatorChanged(oldInstigator);
}

void Object::onInstigatorChanged(Object* oldInstigator) noexcept{}

void Object::scanEvents(TickType_t wait) noexcept{
	if(!eventScanningMutex.try_lock()){
		CMF_LOG(CMF, Warning, "Circular object ownership detected. Object: %s, Owner: %s", getName().c_str(), getOwner()->getName().c_str());
		eventScanningMutex.unlock();
		return;
	}

	const uint64_t begin = millis();
	wait = std::min(wait, getEventScanningTime());

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

void Object::registerEventHandle(EventHandleBase* handle) const noexcept{
	if(handle == nullptr){
		return;
	}

	if(handle->getOwningObject() != this){
		return;
	}

	if(const Application* app = getApp()){
		if(EventScanner* scanner = app->getEventScanner()){
			scanner->registerHandle(handle);
		}
	}
}

void Object::unregisterEventHandle(EventHandleBase* handle) noexcept{
	if(handle == nullptr) {
		return;
	}

	if(handle->getOwningObject() != this){
		return;
	}

	readyEventHandles.remove(handle);

	if(const Application* app = getApp()){
		if(EventScanner* scanner = app->getEventScanner()){
			scanner->unregisterHandle(handle);
		}
	}
}

void Object::readyEventHandle(EventHandleBase *handle) noexcept{
	readyEventHandles.push(handle);
}

TickType_t Object::getEventScanningTime() const noexcept{
	return eventScanningTime;
}

void Object::setEventScanningTime(TickType_t value) noexcept {
	eventScanningTime = value;
}

Archive& Object::serialize(Archive& archive) noexcept{
	return archive;
}

Application* Object::getApp() noexcept{
	return ApplicationStatics::getApplication();
}

Object* Object::getOutermostOwner() const noexcept{
	Object* outermost = getOwner();
	if(outermost == nullptr){
		return nullptr;
	}

	while(outermost != nullptr && outermost->getOwner() != nullptr){
		outermost = outermost->getOwner();
	}

	return outermost;
}

Object* Object::getOutermostInstigator() const noexcept{
	Object* outermost = getInstigator();
	if(outermost == nullptr){
		return nullptr;
	}

	while(outermost != nullptr && outermost->getInstigator() != nullptr){
		outermost = outermost->getInstigator();
	}

	return outermost;
}

inline void Object::registerChild(Object* child) noexcept{
	// NOTE: ownershipMutex must be locked before calling this function to avoid multithreading issues. If it is not locked, bugs can occur.
	WeakObjectPtr<Object> newChild = child;
	if(!newChild.isValid()){
		return;
	}

	childrenObjects.insert(newChild);
	onChildAdded(child);
}

inline void Object::removeChild(Object* child) noexcept{
	// NOTE: ownershipMutex must be locked before calling this function to avoid multithreading issues. If it is not locked, bugs can occur.
	if(child == nullptr){
		return;
	}

	if(childrenObjects.erase(child) == 0){
		return;
	}

	onChildRemoved(child);
}
