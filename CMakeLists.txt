cmake_minimum_required(VERSION 3.23)
project(fa22_csf C)

set(CMAKE_C_STANDARD 11)

include_directories(hw1)

add_executable(fa22_csf
        hw1/fixedpoint.c
        hw1/fixedpoint.h
        hw1/fixedpoint_tests.c
        hw1/tctest.c
        hw1/tctest.h)
