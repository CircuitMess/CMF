#ifndef CMF_APPLICATION_H
#define CMF_APPLICATION_H

#include "Entity/AsyncEntity.h"

class Application : public AsyncEntity {
	GENERATED_BODY(Application, AsyncEntity)

public:
	Application() noexcept;
	Application(const Application&) = delete;
	Application(Application&&) = delete;

	virtual ~Application() noexcept override;

	Application& operator =(const Application&) = delete;
	Application& operator =(Application&&) = delete;

protected:
	virtual void PostInitProperties() noexcept override;
	virtual void Begin() noexcept override;
	virtual void Tick(float deltaTime) noexcept override;
	virtual void End(/*TODO reason*/) noexcept override;
};

#endif //CMF_APPLICATION_H