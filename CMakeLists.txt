set(SOURCES "src/Object/Class.cpp" "src/Object/Object.cpp" CACHE INTERNAL "" FORCE)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS "src")