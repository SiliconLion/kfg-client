cmake_minimum_required(VERSION 3.20)
project(client C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

#this way finds the *system* version of glfw. May switch to the precompiled binary in libs in the future
find_package(glfw3 REQUIRED) #pick more specfic version in future
find_package(OpenGL REQUIRED)



add_subdirectory(libs)
add_subdirectory(app-src)

