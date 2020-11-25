cmake_minimum_required(VERSION 3.17)
project(Assignment2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++14 -Wa,-mbig-obj")

add_executable(Assignment2 main.cpp city.cpp city.hpp tour.cpp tour.hpp genetic_algorithm.cpp genetic_algorithm.hpp)