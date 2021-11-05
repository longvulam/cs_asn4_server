cmake_minimum_required(VERSION 3.20)
project(cs_asn4_server C)

set(CMAKE_C_STANDARD 11)

add_executable(multithreadedserver multithreadedserver.c)