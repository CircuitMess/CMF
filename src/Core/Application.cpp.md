## Application.h Documentation

### Table of Contents

| Section | Description |
|---|---|
| [Class: Application](#class-application) |  |
| [Constructor: Application()](#constructor-application) |  |
| [Destructor: ~Application()](#destructor-application) |  |
| [Method: registerLifetimeObject(Object\* object)](#method-registerlifetimeobject) |  |
| [Method: postInitProperties()](#method-postinitproperties) |  |
| [Method: begin()](#method-begin) |  |
| [Method: tick(float deltaTime)](#method-tick) |  |
| [Method: end(EndReason reason)](#method-end) |  |
| [Method: getGarbageCollectorClass()](#method-getgarbagecollectorclass) |  |
| [Method: getEventScanningTime()](#method-geteventscanningtime) |  |
| [Method: getTickingInterval()](#method-gettickinginterval) |  |


### Class: Application

The `Application` class represents the main application. It is responsible for managing the application's lifecycle and providing a central point for accessing other core functionalities.

**Inheritance:**

- Inherits from `Super` (unspecified base class)

**Properties:**

- **`ApplicationInstance`**: A static pointer to the current instance of the `Application` class.

- **`lifetimeObjects`**: A set that stores pointers to objects whose lifetime is managed by the application.

- **`shuttingDown`**: A boolean flag indicating whether the application is shutting down.

**Methods:**

#### Constructor: Application()

This constructor initializes the `ApplicationInstance` pointer to `this` if it is not already set. This ensures that only one instance of the `Application` class exists at a time.

```c++
Application::Application() noexcept : Super() {
    if(ApplicationInstance == nullptr){
        ApplicationInstance = this;
    }
}
```

#### Destructor: ~Application()

The destructor sets the `ApplicationInstance` pointer to `nullptr` if it points to `this`. This ensures that the static pointer is correctly reset when the `Application` instance is destroyed.

```c++
Application::~Application() noexcept {
    if(ApplicationInstance == this){
        ApplicationInstance = nullptr;
    }
}
```

#### Method: registerLifetimeObject(Object\* object)

This method registers an object with the application, adding it to the `lifetimeObjects` set. This allows the application to manage the object's lifetime.

```c++
void Application::registerLifetimeObject(Object* object) noexcept{
    lifetimeObjects.insert(object);
}
```

#### Method: postInitProperties()

This method is called after the application's properties have been initialized. It calls the `postInitProperties()` method of the base class.

```c++
void Application::postInitProperties() noexcept {
    Super::postInitProperties();
}
```

#### Method: begin()

This method is called when the application begins its execution. It calls the `begin()` method of the base class.

```c++
void Application::begin() noexcept {
    Super::begin();
}
```

#### Method: tick(float deltaTime)

This method is called every frame to update the application's state. It calls the `tick()` method of the base class.

```c++
void Application::tick(float deltaTime) noexcept {
    Super::tick(deltaTime);
}
```

#### Method: end(EndReason reason)

This method is called when the application is ending. It sets the `shuttingDown` flag to `true` and calls the `end()` method of the base class. Additionally, it iterates through the registered objects and calls the `destroy()` method for any `AsyncEntity` objects found.

```c++
void Application::end(EndReason reason) noexcept {
    reason = EndReason::Quit; //  This line appears to be redundant as it reassigns reason to EndReason::Quit.
    shuttingDown = true;

    Super::end(reason);

    ObjectManager::get()->forEachObject([](Object* object) {
        if(AsyncEntity* entity = cast<AsyncEntity>(object)){
            entity->destroy();
        }

        return false;
    });
}
```

#### Method: getGarbageCollectorClass()

This method returns the static class of the `GarbageCollector`.

```c++
SubclassOf<GarbageCollector> Application::getGarbageCollectorClass() const noexcept{
    return GarbageCollector::staticClass();
}
```

#### Method: getEventScanningTime()

This method returns the time in ticks for event scanning. Currently, it returns 0, implying no specific event scanning time is configured.

```c++
TickType_t Application::getEventScanningTime() const noexcept{
    return 0;
}
```

#### Method: getTickingInterval()

This method returns the time in ticks between application updates. Currently, it returns 0, implying no specific ticking interval is configured.

```c++
TickType_t Application::getTickingInterval() const noexcept{
    return 0;
}
```

**Usage:**

The `Application` class serves as the central hub for managing the application's lifecycle. It provides methods for initialization, execution, and shutdown. It also provides access to the application's garbage collector and manages the lifetime of registered objects.

**Example:**

```c++
// Create an instance of the Application class.
Application* app = new Application();

// Register an object with the application.
app->registerLifetimeObject(new MyObject());

// Start the application.
app->begin();

// ... Application runs ...

// End the application.
app->end(EndReason::Quit);
```