#ifndef CMF_APPLICATION_H
#define CMF_APPLICATION_H

#include "Entity/AsyncEntity.h"

class Application : public AsyncEntity {
	GENERATED_BODY(Application, AsyncEntity)

	friend class ApplicationStatics;

public:
	Application() noexcept;
	Application(const Application&) = delete;
	Application(Application&&) = delete;

	virtual ~Application() noexcept override;

	Application& operator =(const Application&) = delete;
	Application& operator =(Application&&) = delete;

protected:
	virtual void postInitProperties() noexcept override;
	virtual void begin() noexcept override;
	virtual void tick(float deltaTime) noexcept override;
	virtual void end(/*TODO reason*/) noexcept override;

private:
	inline static Application* ApplicationInstance = nullptr;
};

#endif //CMF_APPLICATION_H