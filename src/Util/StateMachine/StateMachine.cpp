#include "StateMachine.h"
#include "Memory/ObjectMemory.h"

StateMachine::StateMachine(TickType_t interval/* = 0*/, size_t stackSize /*= 8 * 1024*/, uint8_t threadPriority /*= 0*/, int8_t cpuCore /*= -1*/) noexcept : Super(interval, stackSize, threadPriority, cpuCore) {}

void StateMachine::setStartingStateType(const SubclassOf<State>& type) noexcept{
	if(current.isValid()){
		return;
	}

	if(type == nullptr){
		return;
	}

	startingStateType = type;
}

void StateMachine::tick(float deltaTime) noexcept{
	Super::tick(deltaTime);

	if(current.isValid()){
		const SubclassOf<State> nextStateType = current->transitionTo();
		if(nextStateType != nullptr){
			const Class* previousType = current->getStaticClass();

			current->onTransitionTo(*nextStateType);
			current->destroy();
			current = newObject<State>(*nextStateType, this);
			current->onTransitionFrom(previousType);
		}
	}else if(startingStateType != nullptr) {
		current = newObject<State>(*startingStateType, this);
	}
}