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
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Devices/Camera.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Devices/Camera.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Periphery/WiFi.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Periphery/WiFi.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Services/AACSource.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Services/AACSource.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Services/Audio.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Services/Audio.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Services/AudioSource.h"
        CACHE INTERNAL "" FORCE)

foreach(SRC IN LISTS SRC_DIRS)
    file(GLOB_RECURSE DIR_SOURCES "${SRC}/*/**.h" "${SRC}/*/**.cpp" "${SRC}/*/**.hpp" "${SRC}/*/**.inl" "${SRC}/**.h" "${SRC}/**.cpp" "${SRC}/**.hpp" "${SRC}/**.inl")
    list(APPEND SOURCES ${DIR_SOURCES})
endforeach()

foreach(SRC IN LISTS EXCLUDED_ITEMS)
    list(REMOVE_ITEM SOURCES "${SRC}")
endforeach()

set(LIBS_INCL "${CMAKE_CURRENT_SOURCE_DIR}/lib/glm/glm" "${CMAKE_CURRENT_SOURCE_DIR}/lib/libhelix-aac/src" CACHE INTERNAL "" FORCE)

# LVGL specific
if(CONFIG_CMF_LVGL)
    file(GLOB_RECURSE GRAPHICS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/LV_Interface/**.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/LV_Interface/**.c" "${CMAKE_CURRENT_SOURCE_DIR}/src/LV_Interface/**.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/LV_Interface/**.h")
    list(APPEND SOURCES ${GRAPHICS_SOURCES})
endif()

# LGFX specific
if(CONFIG_CMF_LGFX)
    list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Devices/Display.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/Devices/Display.h")
endif()

# Audio specific
if(CONFIG_CMF_AUDIO)
    list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Services/Audio/AACSource.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/Services/Audio/AACSource.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/Services/Audio/Audio.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/Services/Audio/Audio.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/Services/Audio/AudioSource.h")
    file(GLOB_RECURSE LIBS "${CMAKE_CURRENT_SOURCE_DIR}/lib/libhelix-aac/src/**.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/lib/libhelix-aac/src/**.c")
else()
    set(LIBS "" CACHE INTERNAL "" FORCE)
endif()

# Camera specific
if(CONFIG_CMF_CAMERA)
    list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Devices/Camera.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/Devices/Camera.cpp")
endif()

# WiFi specific
if(CONFIG_CMF_WIFI)
    list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Periphery/WiFi.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/Periphery/WiFi.h")
endif()

set(REQUIRED_COMPONENTS "esp_timer" "driver" "spiffs" "esp_adc" "esp_wifi" CACHE INTERNAL "" FORCE)

idf_component_register(SRCS ${SOURCES}  ${LIBS} INCLUDE_DIRS "src"  ${LIBS_INCL} REQUIRES ${REQUIRED_COMPONENTS})