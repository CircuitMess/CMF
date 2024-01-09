set(SOURCES "src/Object/Class.cpp" "src/Core/Application.cpp" "src/Memory/ObjectManager.cpp" "src/Memory/GarbageCollector.cpp" "src/Entity/Entity.cpp" "src/Entity/AsyncEntity.cpp" "src/Entity/SyncEntity.cpp" "src/Statics/ApplicationStatics.cpp" "src/Thread/Threaded.cpp" "src/Util/stdafx.cpp" CACHE INTERNAL "" FORCE)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS "src" REQUIRES esp_timer)