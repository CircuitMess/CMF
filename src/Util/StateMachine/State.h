#ifndef CMF_STATE_H
#define CMF_STATE_H

#include "Event/EventBroadcaster.h"
#include "Object/Class.h"
#include "Entity/SyncEntity.h"

/**
 * @brief State used by the StateMachine class, extends SyncEntity, can transition to another state,
 * has access to the StateMachine that owns it and had functions triggered when the state machine transitions to it or from it.
 */
class State : public SyncEntity {
	GENERATED_BODY(State, SyncEntity, void)

public:

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
	 *
	 * @return True if next state tick should be instant instead of following the StateMachine tick interval
	 */
	virtual bool skipStateMachineTickInterval() const noexcept;

	/**
	 * @return The owning StateMachine or nullptr if the State is still inactive.
	 */
	class StateMachine* getStateMachine() const noexcept;

	/**
	 *
	 * @param state
	 */
	void transitionTo(const Class* state) const;
};

#endif //CMF_STATE_H