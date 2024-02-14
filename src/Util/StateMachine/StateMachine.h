#ifndef CMF_STATEMACHINE_H
#define CMF_STATEMACHINE_H

#include "State.h"
#include "Entity/AsyncEntity.h"
#include "Object/SubclassOf.h"
#include "Event/EventBroadcaster.h"

class StateMachine : public AsyncEntity {
	GENERATED_BODY(StateMachine, AsyncEntity)

public:
	StateMachine() noexcept;

	virtual TickType_t getEventScanningTime() const noexcept override;
	void setStartingStateType(const SubclassOf<State>& type) noexcept;

protected:
	virtual void tick(float deltaTime) noexcept override;

private:
	DECLARE_EVENT(StartingTypeSetEvent, StateMachine);
	StartingTypeSetEvent onStartingTypeSet = StartingTypeSetEvent(this);

private:
	SubclassOf<State> startingStateType = nullptr;
	StrongObjectPtr<State> current;

private:
	void onStartingTypeSetCallback() noexcept;
};

#endif //CMF_STATEMACHINE_H