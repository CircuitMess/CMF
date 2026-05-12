#include "EventScanner.h"
#include "EventHandle.h"

EventScanner::EventScanner() : Super(CONFIG_CMF_EVENTSCANNER_TICK_INTERVAL / portTICK_PERIOD_MS, CONFIG_CMF_EVENTSCANNER_STACK_SIZE, CONFIG_CMF_EVENTSCANNER_THREAD_PRIORITY, CONFIG_CMF_EVENTSCANNER_CPU_CORE) {
    semaphore = xSemaphoreCreateBinary();
}

EventScanner::~EventScanner() {
    vSemaphoreDelete(semaphore);
}

void EventScanner::unlock() const noexcept {
    xSemaphoreGive(semaphore);
}

void EventScanner::registerHandle(EventHandleBase *handle) noexcept {
    std::lock_guard lock(registerMutex);

    if(handle == nullptr){
        return;
    }

    eventHandles.insert(handle);
}

void EventScanner::unregisterHandle(EventHandleBase *handle) noexcept {
    std::lock_guard lock(registerMutex);

    if(handle == nullptr){
        return;
    }

    eventHandles.erase(handle);
}

void EventScanner::tick(float deltaTime) noexcept {
    Super::tick(deltaTime);

    if(xSemaphoreTake(semaphore, portMAX_DELAY) != pdTRUE){
        return;
    }

    for(EventHandleBase *handle : eventHandles){
        if(handle == nullptr){
            continue;
        }

        if(!handle->probe(0)) {
            continue;
        }

        Object* owner = handle->getOwningObject();
        if(owner == nullptr){
            continue;
        }

        if(handle->getOwningObject()->getOutermostOwner() != nullptr) {
            owner = handle->getOwningObject()->getOutermostOwner();
        }

        owner->readyEventHandle(handle);
    }
}
