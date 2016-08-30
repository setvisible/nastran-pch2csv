project(pch2csv)

### Versioning
set(pch2csv_VERSION_MAJOR "1")
set(pch2csv_VERSION_MINOR "0")
set(pch2csv_VERSION_PATCH "0")
set(pch2csv_VERSION "${pch2csv_VERSION_MAJOR}.${pch2csv_VERSION_MINOR}.${pch2csv_VERSION_PATCH}")

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
    ./src/scanner.h
    ./src/scanner.cpp
    ./src/systemdetection.h
    ./src/version.h
    ./src/main.cpp
    )

add_executable(pch2csv WIN32 ${MY_SOURCES})

