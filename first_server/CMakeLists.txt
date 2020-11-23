cmake_minimum_required(VERSION 3.17)
project(first_server C)

set(CMAKE_C_STANDARD 99)

add_executable(first_server main.c client.c)