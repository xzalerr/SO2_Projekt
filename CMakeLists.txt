cmake_minimum_required(VERSION 3.27)
project(SO2_Projekt)

set(CMAKE_CXX_STANDARD 20)

add_executable(SO2_Projekt main.cpp
            DiningPhilosophersProblem.cpp
            DiningPhilosophersProblem.h)