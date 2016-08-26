project(pch2csv)

set(PCH2CSV_VERSION "1.0.0")
set(PACKAGE_VERSION_MAJOR "1")
set(PACKAGE_VERSION_MINOR "0")
set(PACKAGE_VERSION_PATCH "0")
set(PACKAGE_VERSION "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_PATCH}")

cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
            "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
            FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(MY_SOURCES
    ./src/scanner.h
    ./src/scanner.cpp
    ./src/version.h
    ./src/main.cpp
    )

add_executable(pch2csv WIN32 ${MY_SOURCES})


### Windows shell lightweight utility functions
if(WIN32)
    set(YOUR_LIBRARIES ${YOUR_LIBRARIES} "-lShlwapi")
else()
    set(SHLWAPI)
endif()


target_link_libraries(pch2csv ${YOUR_LIBRARIES})

