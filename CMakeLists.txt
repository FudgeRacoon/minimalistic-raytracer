cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(minimalistic-raytracer LANGUAGES C CXX)

add_executable(main main.cpp PPMHandler.cpp)

target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/Source/Public)