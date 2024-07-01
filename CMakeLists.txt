cmake_minimum_required(VERSION 3.21)
project(lux)
add_executable(lux "source/main.cpp")
target_include_directories(lux PUBLIC "source")

