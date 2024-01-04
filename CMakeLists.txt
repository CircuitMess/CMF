set(SOURCES "src/Object/Class.cpp" "src/Core/Application.cpp" CACHE INTERNAL "" FORCE)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS "src")