#include "State.h"

const Class* State::transitionTo() const noexcept{
	return nullptr;
}

void State::onTransitionFrom(const Class* previous) noexcept{

}

void State::onTransitionTo(const Class* next) noexcept{

}
