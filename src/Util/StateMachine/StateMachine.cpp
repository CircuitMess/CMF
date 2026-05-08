#include "StateMachine.h"
#include "Memory/ObjectMemory.h"

StateMachine::StateMachine(TickType_t interval /*= CONFIG_CMF_STATEMACHINE_TICK_INTERVAL / portTICK_PERIOD_MS*/, size_t stackSize /*= CONFIG_CMF_STATEMACHINE_STACK_SIZE*/,
	uint8_t threadPriority /*= CONFIG_CMF_STATEMACHINE_THREAD_PRIORITY*/, int8_t cpuCore /*= CONFIG_CMF_STATEMACHINE_CPU_CORE*/) noexcept :
		Super(interval, stackSize, threadPriority, cpuCore) {}

void StateMachine::setStartingStateType(const SubclassOf<State>& type) noexcept{
	if(current.isValid()){
		return;
	}

	if(type == nullptr){
		return;
	}

	startingStateType = type;
}

State* StateMachine::getActiveState() const noexcept {
	return current.get();
}

TickType_t StateMachine::getEventScanningTime() const noexcept{
	if(startingStateType != nullptr){
		return 0;
	}

	if(current.isValid() && current->transitionTo() != nullptr){
		return 0;
	}

	return AsyncEntity::getEventScanningTime();
}

void StateMachine::tick(float deltaTime) noexcept{
	Super::tick(deltaTime);

	if(current.isValid()){
		const SubclassOf<State> nextStateType = current->transitionTo();
		if(nextStateType != nullptr){
			const Class* previousType = current->getStaticClass();

			current->onTransitionTo(*nextStateType);
			delete *current;
			current = newObject<State>(*nextStateType, this);
			current->onTransitionFrom(previousType);
		}
	}else if(startingStateType != nullptr) {
		current = newObject<State>(*startingStateType, this);
		startingStateType = nullptr;
	}
}
