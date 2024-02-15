#include "State.h"
#include "StateMachine.h"

const Class* State::transitionTo() const noexcept{
	return nullptr;
}

void State::onTransitionFrom(const Class* previous) noexcept{

}

void State::onTransitionTo(const Class* next) noexcept{

}

StateMachine* State::getStateMachine() const noexcept{
	return cast<StateMachine>(getOwner());
}
