#include "StateMachine.h"
#include "Memory/ObjectMemory.h"

StateMachine::StateMachine(TickType_t interval/* = 0*/) noexcept : Super(interval, 4 * 1024) {}

TickType_t StateMachine::getEventScanningTime() const noexcept{
	return 0;
}

void StateMachine::tick(float deltaTime) noexcept{
	Super::tick(deltaTime);

	if(current.isValid()){

		const auto nextStateType = current->transitionTo();
		if(nextStateType != nullptr){
			const Class* previousType = current->getStaticClass();

			current->onTransitionTo(nextStateType);
			current->destroy();
			current = newObject<State>(nextStateType, this);
			current->onTransitionFrom(previousType);
		}
	}else if(startingStateType != nullptr) {
		current = newObject<State>(*startingStateType, this);
	}
}