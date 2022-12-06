cmake_minimum_required(VERSION 3.12)

###### Change this attributs

set(PROJECT_NAME PROJECT_EXAMPLE)

######

message("==== Project ====")
message("=")

message("= Project name: ${PROJECT_NAME}")
message("=")

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
message("= Root Directory: ${ROOT_DIR}")
message("=")

if(NOT COMMAND pico_sdk_init)
    if(NOT "$ENV{PICO_SDK_PATH}" STREQUAL "")
        set(PICO_SDK $ENV{PICO_SDK_PATH})
    else()
        set(PICO_SDK ${ROOT_DIR}/3rdparty/pico-sdk)
    endif()

    message("= Pico SDK Path: ${PICO_SDK}")
    message("=")

    include(${PICO_SDK}/pico_sdk_init.cmake)
endif()

project(${PROJECT_NAME} C CXX ASM)

pico_sdk_init()

message("= Version: ${PROJECT_VERSION}")
message("= ")

# C++ Standarts
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Create project
message("= C++ Version: ${CMAKE_CXX_STANDARD}")
message("= C++ Compiler: ${CMAKE_CXX_COMPILER_ID}")
message("= C++ Compiler Path: ${CMAKE_CXX_COMPILER}")
message("=")
message("=== Architecture ===")
message("=")

# Project architecture
if(${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    set(PROJECT_ARCHITECTURE x64)
else()
    set(PROJECT_ARCHITECTURE x86)
endif()

message("= Architecture: ${PROJECT_ARCHITECTURE}")

# Project OS
if(WIN32 OR MSVC OR MSYS OR MINGW)
    set(WINDOWS true)    

    if(PROJECT_ARCHITECTURE STREQUAL x64)
        set(WINDOWS64 true)
        set(PROJECT_OS "Win64")
    else()
        set(WINDOWS32 true)
        set(PROJECT_OS "Win32")
    endif()
elseif(UNIX AND NOT APPLE)
    set(LINUX true)

    if(PROJECT_ARCHITECTURE STREQUAL x64)
        set(LINUX64 true)
        set(PROJECT_OS "Linux64")
    else()
        set(LINUX32 true)
        set(PROJECT_OS "Linux32")
    endif()
endif()

message("= OS: ${PROJECT_OS}")
message("=")

message("= Build Type: ${CMAKE_BUILD_TYPE}")
message("=")

if (TARGET tinyusb_device)

    add_subdirectory(main)
    
    message("=================")
elseif(PICO_ON_DEVICE)
    message("")
    message(ERROR "Not building project because TinyUSB submodule is not initialized in the SDK")
    message("=================")
endif()
