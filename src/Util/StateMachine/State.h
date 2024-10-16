#ifndef CMF_STATE_H
#define CMF_STATE_H

#include "Entity/SyncEntity.h"
#include "Object/SubclassOf.h"

/**
 * @brief State used by the StateMachine class, extends SyncEntity, can transition to another state, has access to the StateMachine that owns it and had functions triggered when the state machine transitions to it or from it.
 */
class State : public SyncEntity {
	GENERATED_BODY(State, SyncEntity)

public:
	/**
	 * @brief Triggers the owning StateMachine to transition to the returned class. If nullptr or a class that doesn't extend the State class is returned, this state remains the active one in the state machine.
	 * @return The class type of the state the state machine needs to transition to at the current time.
	 */
	virtual const Class* transitionTo() const noexcept;

	/**
	 * @brief Function that is called when the owning StateMachine transitions to this state.
	 * @param previous The type of state that was active previously to this one.
	 */
	virtual void onTransitionFrom(const Class* previous) noexcept;

	/**
	 * @brief Function that is called when the owning StateMachine transitions from this state.
	 * @param next The type of state that will be active after this one.
	 */
	virtual void onTransitionTo(const Class* next) noexcept;

	/**
	 * @return The owning StateMachine or nullptr if the State is still inactive.
	 */
	class StateMachine* getStateMachine() const noexcept;
};

#endif //CMF_STATE_H