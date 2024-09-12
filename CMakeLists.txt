cmake_minimum_required(VERSION 3.20)
project(client C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_subdirectory(libs)
add_subdirectory(app-src)

