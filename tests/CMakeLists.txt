cmake_minimum_required(VERSION 3.21)
project(cod4-plugpp-unittests VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

FetchContent_Declare(boost-ut
    GIT_REPOSITORY https://github.com/boost-ext/ut.git
    GIT_TAG        v2.1.1
)
FetchContent_MakeAvailable(boost-ut)

add_executable(unittests
    src/main.cpp
    src/cod4x-mock.cpp
    src/arithmetic.cpp
)

target_include_directories(unittests
    PRIVATE include
)

target_link_libraries(unittests
    PRIVATE Boost::ut
    PRIVATE cod4-plugpp
)

target_compile_options(unittests PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wnull-dereference
    -Wformat=2
    -Wsign-conversion
)

