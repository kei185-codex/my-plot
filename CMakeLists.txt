CMAKE_MINIMUM_REQUIRED(VERSION 4.3.4)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -o0")

project(myplot LANGUAGES CXX)

# file(GLOB MAIN src/*.cpp)
file(GLOB MAIN src/main.cpp)

add_executable(main ${MAIN})

target_include_directories(main PUBLIC src)
