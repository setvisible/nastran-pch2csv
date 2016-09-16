project(pch2csv)

### Versioning
set(APP_VERSION_MAJOR "1")
set(APP_VERSION_MINOR "0")
set(APP_VERSION_PATCH "0")
set(APP_VERSION "${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_PATCH}")

configure_file( ./src/config.h.cmake ./src/config.h )
include_directories(${CMAKE_CURRENT_BINARY_DIR})

### CMake Compiler Options
cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
            "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
            FORCE)
endif(NOT CMAKE_BUILD_TYPE)

### Sources
set(MY_SOURCES
    ./src/filemanager.h
    ./src/punchfile.h
    ./src/reader.h
    ./src/qsystemdetection.h
    ./src/writer.h
    ./src/version.h
    ./src/filemanager.cpp
    ./src/punchfile.cpp
    ./src/reader.cpp
    ./src/writer.cpp
    ./src/main.cpp
    )

add_executable(pch2csv WIN32 ${MY_SOURCES})

