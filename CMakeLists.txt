cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(minimalistic-raytracer LANGUAGES C CXX)

set(SRC_FILES
	main.cpp
	Source/Private/Image/Image.cpp
	Source/Private/Image/PPMHandler.cpp
)

add_executable(App ${SRC_FILES})

target_include_directories(App PRIVATE ${CMAKE_SOURCE_DIR}/Source/Public)