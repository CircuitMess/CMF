#ifndef CMF_STATEMACHINE_H
#define CMF_STATEMACHINE_H

#include "State.h"
#include "Entity/AsyncEntity.h"
#include "Object/SubclassOf.h"
#include "Event/EventBroadcaster.h"

/**
 * @brief State machine abstraction AsyncEntity that ticks on its own and transitions between the states depending on the given state type.
 */
class StateMachine : public AsyncEntity {
	GENERATED_BODY(StateMachine, AsyncEntity)

public:
	/**
	 * @brief Constructs the state machine which starts ticking immediately and waits for the starting state type to be set.
	 * @param interval The ticking interval of the state machine.
	 */
	explicit StateMachine(TickType_t interval = 0) noexcept;

	/**
	 * @brief Sets the event scanning type of the AsyncEntity to 0 if the starting state type is set,
	 * otherwise waits indefinitely to avoid ticking without need when no initial state is valid.
	 * @return The delay when scanning for events. 0 if starting type is set.
	 */
	virtual TickType_t getEventScanningTime() const noexcept override;

	/**
	 * @brief Sets the starting state type and triggers the state machine to start ticking.
	 * @param type The type of the starting state.
	 */
	void setStartingStateType(const SubclassOf<State>& type) noexcept;

protected:
	/**
	 * @brief Checks if the current state is ready to transition to another one.
	 * If that is the case, the state transition is triggered.
	 * @param deltaTime Time passed between the last tick and this tick.
	 */
	virtual void tick(float deltaTime) noexcept override;

private:
	SubclassOf<State> startingStateType;
	StrongObjectPtr<State> current;
};

#endif //CMF_STATEMACHINE_H