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
#include "Services/Service.h"

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

protected:
	virtual void postInitProperties() noexcept override;
	virtual void begin() noexcept override;
	virtual void tick(float deltaTime) noexcept override;
	virtual void end(EndReason reason) noexcept override;

	virtual TickType_t getEventScanningTime() const noexcept override;
	virtual TickType_t getTickingInterval() const noexcept override;

	template<typename T, typename ...Args>
	T* registerPeriphery(Args&&... args) noexcept requires(std::derived_from<T, Periphery>) {
		StrongObjectPtr<T> object = newObject<T>(this, args...);
		if(!object.isValid()){
			return nullptr;
		}

		periphery.insert(*object);

		return *object;
	}

	template<typename T>
	T* getPeriphery(const std::function<bool(const Periphery*)>& fn =
			[](const Periphery* periph) {return cast<T>(periph) != nullptr;})
			const noexcept requires(std::derived_from<T, Periphery>) {
		if(!fn){
			return nullptr;
		}

		for(const StrongObjectPtr<Periphery>& periph : periphery){
			if(fn(*periph)){
				return cast<T>(*periph);
			}
		}

		return nullptr;
	}

	template<typename T, typename ...Args>
	T* registerDevice(Args&&... args) noexcept requires(std::derived_from<T, Device>) {
		StrongObjectPtr<T> object = newObject<T>(this, args...);
		if(!object.isValid()){
			return nullptr;
		}

		devices.insert(*object);

		return *object;
	}

	template<typename T>
	T* getDevice(const std::function<bool(const Device*)>& fn =
			[](const Device* device) {return cast<T>(device) != nullptr;})
			const noexcept requires(std::derived_from<T, Device>) {
		if(!fn){
			return nullptr;
		}

		for(const StrongObjectPtr<Device>& device : devices){
			if(fn(*device)){
				return cast<T>(*device);
			}
		}

		return nullptr;
	}

	template<typename T, typename ...Args>
	T* registerDriver(Args&&... args) noexcept requires(std::derived_from<T, Driver>){
		StrongObjectPtr<T> object = newObject<T>(this, args...);
		if(!object.isValid()){
			return nullptr;
		}

		for(const StrongObjectPtr<Driver>& driver : drivers){
			if(!driver.isValid()){
				continue;
			}

			if(driver->getStaticClass() == object->getStaticClass()){
				return nullptr;
			}
		}

		drivers.insert(*object);

		return *object;
	}

	template<typename T>
	T* getDriver() const noexcept requires(std::derived_from<T, Driver>) {
		for(const StrongObjectPtr<Driver>& driver : drivers){
			if(!driver.isValid()){
				continue;
			}

			if(T* driverObject = cast<T>(*driver)){
				return driverObject;
			}
		}

		return nullptr;
	}

	template<typename T, typename ...Args>
	T* registerService(Args&&... args) noexcept requires(std::derived_from<T, Service>){
		StrongObjectPtr<T> object = newObject<T>(this, args...);
		if(!object.isValid()){
			return nullptr;
		}

		for(const StrongObjectPtr<Service>& service : services){
			if(!service.isValid()){
				continue;
			}

			if(service->getStaticClass() == object->getStaticClass()){
				return nullptr;
			}
		}

		drivers.insert(*object);

		return *object;
	}

	template<typename T>
	T* getService() const noexcept requires(std::derived_from<T, Service>) {
		for(const StrongObjectPtr<Service>& service : services){
			if(!service.isValid()){
				continue;
			}

			if(T* serviceObject = cast<T>(*service)){
				return serviceObject;
			}
		}

		return nullptr;
	}

private:
	inline static Application* ApplicationInstance = nullptr;

private:
	std::set<StrongObjectPtr<Singleton>> singletons;
	std::set<StrongObjectPtr<Object>> lifetimeObjects;
	bool shuttingDown;
	std::mutex registrationMutex;

	std::set<StrongObjectPtr<Periphery>> periphery;
	std::set<StrongObjectPtr<Device>> devices;
	std::set<StrongObjectPtr<Driver>> drivers;
	std::set<StrongObjectPtr<Service>> services;
};

#endif //CMF_APPLICATION_H