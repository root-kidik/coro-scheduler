cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_STANDARD 20)

project(Coro)

add_executable(${PROJECT_NAME} main.cpp scheduler.hpp scheduler.cpp task.hpp
                               task.cpp)
