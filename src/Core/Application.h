#ifndef CMF_APPLICATION_H
#define CMF_APPLICATION_H

#include <concepts>
#include <set>
#include "Entity/AsyncEntity.h"
#include "Object/SubclassOf.h"
#include "Memory/GarbageCollector.h"
#include "Memory/ObjectMemory.h"
#include "Misc/Singleton.h"
#include "Object/Interface.h"
#include "Event/EventScanner.h"

/**
 * @brief The base class used to define the core applications,
 * as well as all variable modules it uses to control the hardware and software on the device.
 * The user of the framework must extend this class to define their own application that is custom to their hardware and software.
 */
class Application : public AsyncEntity {
	GENERATED_BODY(Application, AsyncEntity)

	friend class ApplicationStatics;

public:
	/**
	 * @brief The default constructor. Sets the static instance pointer to the first constructed instance of the Application class that is still alive.
	 */
	Application(TickType_t interval = 0, size_t stackSize = 12 * 1024) noexcept;

	/**
	 * @brief Deleted copy constructor.
	 */
	Application(const Application&) = delete;

	/**
	 * @brief Deleted move constructor.
	 */
	Application(Application&&) = delete;

	/**
	 * @brief Destructor. Sets the static instance pointer to nullptr if it was set to the instance being destroyed.
	 */
	virtual ~Application() noexcept override;

	/**
	 * @brief Deleted copy operator.
	 */
	Application& operator =(const Application&) = delete;

	/**
	 * @brief Deleted move operator.
	 */
	Application& operator =(Application&&) = delete;

	/**
	 * @brief Allows the user of the framework to specify their own garbage collector type.
	 * If nullptr is returned, the default CMF garbage collector will be used.
	 * @return The type of garbage collector the application uses.
	 */
	virtual SubclassOf<GarbageCollector> getGarbageCollectorClass() const noexcept;

	/**
	 * @return The EventScanner instance for this application.
	 */
	inline constexpr EventScanner* getEventScanner() const noexcept { return eventScanner.get(); }

	/**
	 * @brief Registers a singleton to the application.
	 * Only one of each type can ever exist registered within the application.
	 * @tparam T The type of singleton being registered and returned.
	 * @param cls The class of the singleton, in case a base class pointer should be returned,
	 * but a derived class type should be created.
	 * @return The created singleton if successful, nullptr otherwise.
	 */
	template<typename T, typename = std::enable_if<std::derived_from<T, Singleton>, T>::type>
	T* registerSingleton(const Class* cls = nullptr) noexcept {
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

		singletons.insert(cast<Singleton>(newSingleton.get()));
		return *newSingleton;
	}

	/**
	 * @brief A singleton getter found by class from the argument or template if argument not provided.
	 * @tparam T The type of singleton to return.
	 * @param cls The singleton class used to find the registered singleton.
	 * @return The found singleton if successful, nullptr otherwise.
	 */
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

	/**
	 * @brief Marks if the application is in the middle of shutting down or not.
	 * @return If the application is shutting down, true, otherwise false.
	 */
	inline bool isShuttingDown() const noexcept {
		return shuttingDown;
	}

	/**
	 * @brief Used to register object instances that should be kept alive passively as long as the application exists.
	 * @param object The object instance to keep alive.
	 */
	void registerLifetimeObject(Object* object) noexcept;

	/**
	 * @brief
	 * @tparam T
	 * @param fn
	 * @return
	 */
	template<typename T>
	T* getPeriphery(const std::function<bool(const Object*)>& fn =
	[](const Object* periph) {return cast<T>(periph) != nullptr;})
	const noexcept requires(std::derived_from<T, Object>) {
		if(!fn){
			return nullptr;
		}

		for(const StrongObjectPtr<Object>& periph : periphery){
			if(fn(*periph)){
				return cast<T>(*periph);
			}
		}

		return nullptr;
	}

	/**
	 * @brief
	 * @tparam T
	 * @param fn
	 * @return
	 */
	template<typename T>
	T* getDevice(const std::function<bool(const Object*)>& fn =
	[](const Object* device) {return cast<T>(device) != nullptr;})
	const noexcept requires(std::derived_from<T, Object>) {
		if(!fn){
			return nullptr;
		}

		for(const StrongObjectPtr<Object>& device : devices){
			if(fn(*device)){
				return cast<T>(*device);
			}
		}

		return nullptr;
	}

	/**
	 * @brief
	 * @tparam T
	 * @param fn
	 * @return
	 */
	template<typename T>
	T* getService() const noexcept requires(std::derived_from<T, Object>) {
		for(const StrongObjectPtr<Object>& service : services){
			if(!service.isValid()){
				continue;
			}

			if(T* serviceObject = cast<T>(*service)){
				return serviceObject;
			}
		}

		return nullptr;
	}

	/**
	 * @brief
	 * @tparam T
	 * @param fn
	 * @return
	 */
	template<typename T>
	T* getDriver() const noexcept requires(std::derived_from<T, Object>) {
		for(const StrongObjectPtr<Object>& driver : drivers){
			if(!driver.isValid()){
				continue;
			}

			if(T* driverObject = cast<T>(*driver)){
				return driverObject;
			}
		}

		return nullptr;
	}

protected:
	/**
	 * @brief
	 */
	virtual void postInitProperties() noexcept override;

	/**
	 * @brief
	 */
	virtual void begin() noexcept override;

	/**
	 * @brief
	 * @param deltaTime The time passed since the last tick call.
	 */
	virtual void tick(float deltaTime) noexcept override;

	/**
	 * @brief
	 * @param reason The reason why the entity instance stopped ticking and is getting destroyed.
	 */
	virtual void end(EndReason reason) noexcept override;

	/**
	 * @return The maximum wait time for receiving events.
	 */
	virtual TickType_t getEventScanningTime() const noexcept override;

	/**
	 * @return The frequency of ticks of the entity instance.
	 */
	virtual TickType_t getTickingInterval() const noexcept override;

	/**
	 * @brief
	 * @tparam T
	 * @tparam Args
	 * @param args
	 * @return
	 */
	template<typename T, typename ...Args>
	T* registerPeriphery(Args&&... args) noexcept requires(std::derived_from<T, Object>) {
		StrongObjectPtr<T> object = newObject<T>(this, args...);
		if(!object.isValid()){
			return nullptr;
		}

		periphery.insert(*object);

		return *object;
	}

	/**
	 * @brief
	 * @tparam T
	 * @tparam Args
	 * @param args
	 * @return
	 */
	template<typename T, typename ...Args>
	T* registerDevice(Args&&... args) noexcept requires(std::derived_from<T, Object>) {
		StrongObjectPtr<T> object = newObject<T>(this, args...);
		if(!object.isValid()){
			return nullptr;
		}

		devices.insert(*object);

		return *object;
	}

	/**
	 * @brief
	 * @tparam T
	 * @tparam Args
	 * @param args
	 * @return
	 */
	template<typename T, typename ...Args>
	T* registerDriver(Args&&... args) noexcept requires(std::derived_from<T, Object>){
		StrongObjectPtr<T> object = newObject<T>(this, args...);
		if(!object.isValid()){
			return nullptr;
		}

		for(const StrongObjectPtr<Object>& driver : drivers){
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

	/**
	 * @brief
	 * @tparam T
	 * @tparam Args
	 * @param args
	 * @return
	 */
	template<typename T, typename ...Args>
	T* registerService(Args&&... args) noexcept requires(std::derived_from<T, Object>){
		StrongObjectPtr<T> object = newObject<T>(this, args...);
		if(!object.isValid()){
			return nullptr;
		}

		for(const StrongObjectPtr<Object>& service : services){
			if(!service.isValid()){
				continue;
			}

			if(service->getStaticClass() == object->getStaticClass()){
				return nullptr;
			}
		}

		services.insert(*object);

		return *object;
	}

private:
	inline static Application* ApplicationInstance = nullptr;

private:
	std::set<StrongObjectPtr<Singleton>> singletons;
	std::set<StrongObjectPtr<Object>> lifetimeObjects;
	bool shuttingDown;
	std::mutex registrationMutex;

	std::set<StrongObjectPtr<Object>> periphery;
	std::set<StrongObjectPtr<Object>> devices;
	std::set<StrongObjectPtr<Object>> drivers;
	std::set<StrongObjectPtr<Object>> services;
	StrongObjectPtr<EventScanner> eventScanner;
};

#endif //CMF_APPLICATION_H