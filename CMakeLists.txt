set(SOURCES "src/Event/EventScanner.cpp" "src/Object/Class.cpp" "src/Devices/AW9523.cpp" "src/Drivers/Input/InputAW.cpp" "src/Drivers/Output/OutputDigAW.cpp" "src/Drivers/Output/OutputCurrAW.cpp" "src/Drivers/Input/InputGPIO.cpp" "src/Drivers/Input/InputTouchGPIO.cpp" "src/Drivers/Output/OutputGPIO.cpp" "src/Services/ButtonInput.cpp" "src/Periphery/GPIOPeriph.cpp" "src/Periphery/I2C.cpp" "src/Util/StateMachine/State.cpp" "src/Util/StateMachine/StateMachine.cpp" "src/Containers/Archive.cpp" "src/Misc/Singleton.cpp" "src/Memory/ObjectMemory.cpp" "src/Object/Object.cpp" "src/Core/Application.cpp" "src/Memory/ObjectManager.cpp" "src/Memory/GarbageCollector.cpp" "src/Entity/Entity.cpp" "src/Entity/AsyncEntity.cpp" "src/Entity/SyncEntity.cpp" "src/Statics/ApplicationStatics.cpp" "src/Thread/Threaded.cpp" "src/Util/stdafx.cpp" CACHE INTERNAL "" FORCE)

file(GLOB_RECURSE FS_SOURCES "src/FS/**.cpp" "src/FS/**.c")

if (CONFIG_CMF_GRAPHICS)
    file(GLOB_RECURSE GRAPHICS_SOURCES "src/LV_Interface/**.cpp" "src/LV_Interface/**.c")
    list(APPEND GRAPHICS_SOURCES "src/Devices/Display.cpp")

    list(APPEND SOURCES ${GRAPHICS_SOURCES})
endif()

list(APPEND SOURCES ${FS_SOURCES})


file(GLOB_RECURSE LIBS "lib/*/src/**.cpp" "lib/*/src/**.c")
set(LIBS_INCL "lib/glm/glm")

idf_component_register(SRCS ${SOURCES}  ${LIBS} INCLUDE_DIRS "src"  ${LIBS_INCL} REQUIRES esp_timer driver spiffs)