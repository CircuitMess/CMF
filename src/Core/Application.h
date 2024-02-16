#ifndef CMF_APPLICATION_H
#define CMF_APPLICATION_H

#include <concepts>
#include <set>
#include "Entity/AsyncEntity.h"
#include "Object/SubclassOf.h"
#include "Memory/GarbageCollector.h"
#include "Memory/ObjectMemory.h"
#include "Misc/Singleton.h"
#include "Periphery/Periphery.h"
#include "Object/Interface.h"
#include "Devices/Device.h"
#include "Drivers/Driver.h"

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

	virtual SubclassOf<GarbageCollector> getGarbageCollectorClass() const noexcept;

	template<typename T, typename = std::enable_if<std::derived_from<T, Singleton>, T>::type>
	T* registerSingleton(const Class* cls = nullptr) const noexcept {
		if(T* existingSingleton = getSingleton<T>(cls)){
			return existingSingleton;
		}

		StrongObjectPtr<T> newSingleton;

		if(cls == nullptr){
			newSingleton = newObject<T>(T::staticClass());
		}else{
			newSingleton = newObject<T>(cls);
		}

		if(!newSingleton.isValid()){
			return nullptr;
		}

		singletons.insert(newSingleton);
		return *newSingleton;
	}

	template<typename T, typename = std::enable_if<std::derived_from<T, Singleton>, T>::type>
	T* getSingleton(const Class* cls = nullptr) const noexcept {
		if(cls == nullptr){
			cls = T::staticClass();
		}

		for(const StrongObjectPtr<Singleton>& singleton : singletons){
			if(singleton->getStaticClass()->getID() == cls->getID()){
				return cast<T>(*singleton);
			}
		}

		return nullptr;
	}

	inline bool isShuttingDown() const noexcept {
		return shuttingDown;
	}

	void registerLifetimeObject(Object* object) noexcept;

	template<typename T, typename ...Args>
	T* registerPeriphery(Args&&... args) noexcept {
		StrongObjectPtr<T> object = newObject<T>(this, args...);
		if(!object.isValid()){
			return nullptr;
		}

		Interface<Periphery> periph = *object;
		if(!periph){
			return nullptr;
		}

		registerLifetimeObject(*object);
		periphery.insert(periph);

		return *object;
	}

	template<typename T>
	T* getPeriphery(const std::function<bool(const Interface<Periphery>&)> fn =
			[](const Interface<Periphery>& interface) {return cast<T>(interface.getObject()) != nullptr;})
			const noexcept requires(std::derived_from<T, Object>) {
		if(!fn){
			return nullptr;
		}

		for(const Interface<Periphery>& periph : periphery){
			if(fn(periph)){
				return cast<T>(periph.getObject());
			}
		}

		return nullptr;
	}

	template<typename T, typename ...Args>
	T* registerDevice(Args&&... args) noexcept {
		StrongObjectPtr<T> object = newObject<T>(this, args...);
		if(!object.isValid()){
			return nullptr;
		}

		Interface<Device> device = *object;
		if(!device){
			return nullptr;
		}

		registerLifetimeObject(*object);
		devices.insert(device);

		return *object;
	}

	template<typename T>
	T* getDevice(const std::function<bool(const Interface<Device>&)> fn =
			[](const Interface<Device>& interface) {return cast<T>(interface.getObject()) != nullptr;})
			const noexcept requires(std::derived_from<T, Object>) {
		if(!fn){
			return nullptr;
		}

		for(const Interface<Device>& device : devices){
			if(fn(device)){
				return cast<T>(device.getObject());
			}
		}

		return nullptr;
	}

	template<typename T, typename ...Args>
	T* registerDriver(Args&&... args) noexcept {
		StrongObjectPtr<T> object = newObject<T>(this, args...);
		if(!object.isValid()){
			return nullptr;
		}

		Interface<Driver> driver = *object;
		if(!driver){
			return nullptr;
		}

		for(const Interface<Driver>& interface : drivers){
			if(!interface){
				continue;
			}

			if(interface.getObject()->getStaticClass() == object->getStaticClass()){
				return nullptr;
			}
		}

		registerLifetimeObject(*object);
		drivers.insert(driver);

		return *object;
	}

	template<typename T>
	T* getDriver() const noexcept requires(std::derived_from<T, Object>) {
		for(const Interface<Driver>& driver : drivers){
			if(!driver){
				continue;
			}

			if(T* driverObject = cast<T>(driver.getObject())){
				return driverObject;
			}
		}

		return nullptr;
	}

protected:
	virtual void postInitProperties() noexcept override;
	virtual void begin() noexcept override;
	virtual void tick(float deltaTime) noexcept override;
	virtual void end(EndReason reason) noexcept override;
	
	virtual TickType_t getEventScanningTime() const noexcept override;
	virtual TickType_t getTickingInterval() const noexcept override;

private:
	inline static Application* ApplicationInstance = nullptr;

private:
	std::set<StrongObjectPtr<Singleton>> singletons;
	std::set<StrongObjectPtr<Object>> lifetimeObjects;
	bool shuttingDown;

	// TODO: think if there is a need for this to be in interfaces, or can this simply be set of object pointers
	std::set<Interface<Periphery>> periphery;
	std::set<Interface<Device>> devices;
	std::set<Interface<Driver>> drivers;
};

#endif //CMF_APPLICATION_H