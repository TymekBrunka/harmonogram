cmake_minimum_required(VERSION 3.23)
project(betterGL LANGUAGES CXX C)

add_library(betterGL lib.cpp
  Debugging.cpp
  Program.cpp
)
target_include_directories(betterGL PUBLIC include)
target_link_libraries(betterGL glad)
target_compile_features(betterGL PUBLIC cxx_std_20)
