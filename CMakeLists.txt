cmake_minimum_required(VERSION 3.0.0)

project(BallAndSocketArm VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory(simulations)
