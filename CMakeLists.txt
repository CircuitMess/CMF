set(SOURCES "src/Object/Class.cpp" "src/Drivers/InputGPIO.cpp" "src/Drivers/OutputGPIO.cpp" "src/Devices/ButtonInput.cpp" "src/Periphery/GPIO.cpp" "src/Periphery/I2C.cpp" "src/Util/StateMachine/State.cpp" "src/Util/StateMachine/StateMachine.cpp" "src/Containers/Archive.cpp" "src/Misc/Singleton.cpp" "src/Memory/ObjectMemory.cpp" "src/Object/Object.cpp" "src/Core/Application.cpp" "src/Memory/ObjectManager.cpp" "src/Memory/GarbageCollector.cpp" "src/Entity/Entity.cpp" "src/Entity/AsyncEntity.cpp" "src/Entity/SyncEntity.cpp" "src/Statics/ApplicationStatics.cpp" "src/Thread/Threaded.cpp" "src/Util/stdafx.cpp" CACHE INTERNAL "" FORCE)

file(GLOB_RECURSE LIBS "lib/*/src/**.cpp" "lib/*/src/**.c")
set(LIBS_INCL "lib/glm/glm")

idf_component_register(SRCS ${SOURCES}  ${LIBS} INCLUDE_DIRS "src"  ${LIBS_INCL} REQUIRES esp_timer driver)