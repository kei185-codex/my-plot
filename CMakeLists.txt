CMAKE_MINIMUM_REQUIRED(VERSION 4.3.4)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g")

project(myplot LANGUAGES CXX)

file(GLOB MAIN src/*.cpp)
file(GLOB IMGUI
    src/lib/imgui-master/*.cpp
    src/lib/imgui-master/*.h)
file(GLOB IMPLOT
    src/lib/implot/*.h
    src/lib/implot/*.cpp)

add_executable(main ${MAIN})

add_library(imgui SHARED  ${IMGUI})
add_library(implot SHARED ${IMPLOT})

target_include_directories(imgui PUBLIC src/lib/imgui-master)
target_include_directories(implot PUBLIC src/lib/implot)

target_link_libraries(implot imgui)
target_link_libraries(main imgui implot)
