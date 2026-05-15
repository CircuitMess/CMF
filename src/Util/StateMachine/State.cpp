#include "State.h"
#include "StateMachine.h"
#include "Log/Log.h"

DEFINE_LOG(State);

void State::onTransitionFrom(const Class* previous) noexcept{

}

void State::onTransitionTo(const Class* next) noexcept{

}

bool State::skipStateMachineTickInterval() const noexcept{
	return false;
}

StateMachine* State::getStateMachine() const noexcept{
	return cast<StateMachine>(getOwner());
}

void State::transitionTo(const Class* state) const{
	if(state == nullptr){
		CMF_LOG(State, LogLevel::Error, "Transition but given state class parameter is nullptr.");
		return;
	}

	if(!state->isA(staticClass())){
		CMF_LOG(State, LogLevel::Error, "Transition but given state class parameter is not of Class type.");
		return;
	}

	StateMachine* stateMachine = getStateMachine();
	if(stateMachine == nullptr){
		CMF_LOG(State, LogLevel::Error, "Transition but StateMachine is nullptr");
		return;
	}

	stateMachine->transitionTo(state);
}
