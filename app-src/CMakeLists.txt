

#add_executable(client main.c)


add_executable(Matcap matcap.c)

#this way finds the system version of glfw.
find_package(glfw3 REQUIRED) #pick more specfic version in future
target_link_libraries(Matcap glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(Matcap OpenGL::GL)

target_link_libraries(Matcap threeDplay stb )

#copies assets and shaders to the build dir
file(COPY assets DESTINATION ./)
file(COPY shaders DESTINATION ./)