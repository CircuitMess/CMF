#include "StateMachine.h"
#include "Memory/ObjectMemory.h"

// TODO: make sure the state machine can control async entity properties trough its parameters
// TODO: to make sure not to have to do this anymore, make a object initializer class that will handle all the necessary params in one class
StateMachine::StateMachine() noexcept : Super() {
	onStartingTypeSet.bind(this, &StateMachine::onStartingTypeSetCallback);
}

TickType_t StateMachine::getEventScanningTime() const noexcept{
	return current.isValid() ? 0 : portMAX_DELAY;
}

void StateMachine::setStartingStateType(const SubclassOf<State>& type) noexcept{
	if(current.isValid()){
		return;
	}

	if(type == nullptr){
		return;
	}

	startingStateType = type;
	onStartingTypeSet.broadcast();
}

void StateMachine::tick(float deltaTime) noexcept{
	Super::tick(deltaTime);

	if(current.isValid()){
		SubclassOf<State> nextStateType = current->transitionTo();
		if(nextStateType != nullptr){
			const Class* previousType = current->getStaticClass();

			current->onTransitionTo(*nextStateType);
			current->destroy();
			current = newObject<State>(*nextStateType, this);
			current->onTransitionFrom(previousType);
		}
	}
}

void StateMachine::onStartingTypeSetCallback() noexcept{
	if(!current.isValid() && startingStateType != nullptr){
		current = newObject<State>(*startingStateType, this);
	}
}
