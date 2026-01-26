#include "State.h"
#include "StateMachine.h"

const Class* State::transitionTo() const noexcept{
	return next;
}

void State::onTransitionFrom(const Class* previous) noexcept{

}

void State::onTransitionTo(const Class* next) noexcept{

}

StateMachine* State::getStateMachine() const noexcept{
	return cast<StateMachine>(getOwner());
}

void State::setNextState(const Class* state) noexcept {
	if(!state->isA(staticClass())){
		return;
	}

	next = state;
}
