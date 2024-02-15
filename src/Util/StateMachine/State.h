#ifndef CMF_STATE_H
#define CMF_STATE_H

#include "Entity/SyncEntity.h"
#include "Object/SubclassOf.h"

class State : public SyncEntity {
	GENERATED_BODY(State, SyncEntity)

public:
	virtual const Class* transitionTo() const noexcept;
	virtual void onTransitionFrom(const Class* previous) noexcept;
	virtual void onTransitionTo(const Class* next) noexcept;
	class StateMachine* getStateMachine() const noexcept;
};

#endif //CMF_STATE_H