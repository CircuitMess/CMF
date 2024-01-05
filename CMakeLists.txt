set(SOURCES "src/Object/Class.cpp" "src/Core/Application.cpp" "src/Entity/Entity.cpp" "src/Entity/AsyncEntity.cpp" "src/Entity/SyncEntity.cpp" CACHE INTERNAL "" FORCE)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS "src")