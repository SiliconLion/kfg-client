cmake_minimum_required(VERSION 3.20)
project(client C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
enable_language(OBJC)


find_package(OpenGL REQUIRED)



add_subdirectory(libs)
add_subdirectory(app-src)
#add_subdirectory(tests)
add_subdirectory(side-projects)

