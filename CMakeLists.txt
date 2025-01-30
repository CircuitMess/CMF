set(SRC_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Containers"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Core"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Devices"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Drivers"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Entity"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Event"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Log"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Memory"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Misc"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Object"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Periphery"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Services"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Statics"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Thread"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Util"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/FileSystem"
        CACHE INTERNAL "" FORCE)

set(EXCLUDED_ITEMS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Devices/Display.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Devices/Display.cpp"
        CACHE INTERNAL "" FORCE)

foreach(SRC IN LISTS SRC_DIRS)
    file(GLOB_RECURSE DIR_SOURCES "${SRC}/**.h" "${SRC}/**.cpp" "${SRC}/**.hpp" "${SRC}/**.inl")
    list(APPEND SOURCES ${DIR_SOURCES})
endforeach()

foreach(SRC IN LISTS EXCLUDED_ITEMS)
    list(REMOVE_ITEM SOURCES "${SRC}")
endforeach()

if (CONFIG_CMF_GRAPHICS)
    file(GLOB_RECURSE GRAPHICS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/LV_Interface/**.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/LV_Interface/**.c" "${CMAKE_CURRENT_SOURCE_DIR}/src/LV_Interface/**.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/LV_Interface/**.h")
    list(APPEND GRAPHICS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Devices/Display.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/Devices/Display.h")

    list(APPEND SOURCES ${GRAPHICS_SOURCES})
endif()

file(GLOB_RECURSE LIBS "lib/*/src/**.cpp" "lib/*/src/**.c")

set(LIBS_INCL "lib/glm/glm" CACHE INTERNAL "" FORCE)

set(REQUIRED_COMPONENTS "esp_timer" "driver" "spiffs" CACHE INTERNAL "" FORCE)

idf_component_register(SRCS ${SOURCES}  ${LIBS} INCLUDE_DIRS "src"  ${LIBS_INCL} REQUIRES ${REQUIRED_COMPONENTS})