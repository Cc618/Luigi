cmake_minimum_required(VERSION 3.10)

project(luigi VERSION 0.1)

configure_file(src/config.h.in src/config.h)

add_subdirectory(src)
