cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(DeltaruneTextboxes VERSION 1.5.2)

#if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS OR (ANDROID AND ANDROID_ABI STREQUAL "armeabi-v7a"))
    add_compile_definitions(DISABLE_KEYBOARD)
#endif()

add_library(${PROJECT_NAME} SHARED
    src/FLAlertLayer.cpp
    src/DialogLayer.cpp
    src/CCKeyboardDispatcher.cpp
    src/changeDesign.cpp
    src/PurchaseItemPopup.cpp
    src/ImageNode.cpp
    # Add any extra C++ source files here
)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})
