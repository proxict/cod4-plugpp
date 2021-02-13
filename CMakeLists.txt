cmake_minimum_required(VERSION 3.0)
project(cod4-plugpp VERSION 1.0.0 LANGUAGES CXX)

if(NOT DEFINED COD4_PLUGIN_DIR)
    message(FATAL_ERROR "Please, define directory to the CoD4x_Server/plugins directory by setting the COD4_PLUGIN_DIR variable.\nExample: cmake -DCOD4_PLUGIN_DIR=/home/cod4/CoD4x_Server/plugins ..")
endif()

set(COMMON_FLAGS "-m32 -mtune=core2 -Wall -fPIC -fvisibility=hidden")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 ${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O1 ${COMMON_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g -m32")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(external/lib-optional)

add_library(cod4-plugpp OBJECT
    src/Command.cpp
    src/PluginEntry.cpp
)
target_link_libraries(cod4-plugpp
    PUBLIC lib-optional
)
target_include_directories(cod4-plugpp
    PUBLIC include
    PUBLIC ${COD4_PLUGIN_DIR}
)

