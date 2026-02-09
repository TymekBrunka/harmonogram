cmake_minimum_required(VERSION 3.12)
project(vizualizator LANGUAGES CXX C)

add_library(glad glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

find_package(glfw3 REQUIRED)

add_subdirectory(src/bettergl)
add_subdirectory(imgui)

add_executable(main src/main.cpp)
target_include_directories(main PRIVATE src)
target_link_libraries(main PRIVATE glad glfw betterGL imgui)
