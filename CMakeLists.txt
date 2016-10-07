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
    ./src/filemanager.cpp
    ./src/punchfile.cpp
    ./src/reader.cpp
    ./src/writer.cpp
    ./src/main.cpp
    )

add_executable(pch2csv ${MY_SOURCES})

#-----------------------------------------------------------------------------
# Add file(s) to CMake Install
#-----------------------------------------------------------------------------

# Deploy MinGW *.DLL files for Windows
# Note: This is a dirty 'hack' for MinGW
if(WIN32 AND MINGW)
    find_path( LIBGCC_S_DW2_DLL_DIR    libgcc_s_dw2-1.dll    DOC "Location of libgcc_s_dw2-1.dll")
    find_path( LIBSTDC_6_DLL_DIR       libstdc++-6.dll       DOC "Location of libstdc++-6.dll")
    find_path( LIBWINPTHREAD_DLL_DIR   libwinpthread-1.dll   DOC "Location of libwinpthread-1.dll")
    set(MY_MINGW_DEPENDANCIES
        ${LIBGCC_S_DW2_DLL_DIR}/libgcc_s_dw2-1.dll
        ${LIBSTDC_6_DLL_DIR}/libstdc++-6.dll
        ${LIBWINPTHREAD_DLL_DIR}/libwinpthread-1.dll
    )
    install (
        FILES ${MY_MINGW_DEPENDANCIES}
        DESTINATION ${CMAKE_INSTALL_PREFIX}
        COMPONENT mingw_dependancies
    )
    message(STATUS "Found MINGW compiler: ${MINGW}")
endif(WIN32 AND MINGW)


# Deploy documentation files
set(MY_RELEASE_DOCS
    ./deployment/example.pch
    ./deployment/LICENSE.txt
    ./deployment/README.txt
    )

install (
    FILES ${MY_RELEASE_DOCS}
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    COMPONENT release_docs
)

# Deploy the executable
install(TARGETS pch2csv RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

