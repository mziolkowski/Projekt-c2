cmake_minimum_required(VERSION 3.9.5)
project(Projekt C)

set(CMAKE_C_STANDARD 99)

add_executable(Projekt main.c ipv4/src/ipv4.c ipv4/src/ipv4.h ipv4/src/icmp_hdr.h)