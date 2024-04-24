cmake_minimum_required(VERSION 3.16..3.24)
project(terminal_webcam VERSION 1
  DESCRIPTION "Shows webcam images in a pixelated version in the terminal"
  LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

# Create a pseudo-library to propagate the needed flags.
add_library(cxx_setup INTERFACE)
target_compile_options(cxx_setup INTERFACE -Wall -Wpedantic -Wextra)
target_compile_features(cxx_setup INTERFACE cxx_std_17)
target_include_directories(cxx_setup INTERFACE ${PROJECT_SOURCE_DIR})

FIND_PACKAGE( OpenCV REQUIRED )                              
INCLUDE_DIRECTORIES( ${OpenCV_INCLUDE_DIRS} )
# Update the submodules here
include(external/UpdateSubmodules.cmake)

# Enable testing for this project
include(CTest)

# Add code in subdirectories
add_subdirectory(external)
add_subdirectory(${PROJECT_NAME})
add_subdirectory(examples)
