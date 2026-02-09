cmake_minimum_required(VERSION 3.23)
project(Imgumplementation)

file(GLOB imguimpl_SRC
     "*.h"
     "*.cpp"
)


add_library(imgui STATIC ${imguimpl_SRC})
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_CUSTOM)
target_include_directories(imgui PUBLIC .)
target_link_libraries(imgui PUBLIC glfw glad)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux" OR "${CMAKE_SYSTEM_NAME}" STREQUAL "DragonFly")
  find_package(X11 REQUIRED)
  target_link_libraries(imgui PUBLIC X11)
endif()
