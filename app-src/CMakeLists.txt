




#this way finds the *system* version of glfw. May switch to the precompiled binary in libs in the future
find_package(glfw3 REQUIRED) #pick more specfic version in future
find_package(OpenGL REQUIRED)

#This is a test executable that confirms many things are working
add_executable(Tests tests.c)

target_link_libraries(Tests glfw)
target_link_libraries(Tests OpenGL::GL)
target_link_libraries(Tests threeDplay stb )

#This is a demo executable that confirms many things are working
add_executable(Matcap matcap.c)

target_link_libraries(Matcap glfw)
target_link_libraries(Matcap OpenGL::GL)
target_link_libraries(Matcap threeDplay stb )


#This is a demo executable that confirms many things are working
add_executable(Transform transform.c)

target_link_libraries(Transform glfw)
target_link_libraries(Transform OpenGL::GL)
target_link_libraries(Transform threeDplay stb )

#This is a demo executable that confirms many things are working
add_executable(Random-walker random-walker.c)

target_link_libraries(Random-walker glfw)
target_link_libraries(Random-walker OpenGL::GL)
target_link_libraries(Random-walker threeDplay stb )





#This is the actual end product program
add_executable(Client main.c board.c scene.c camera.c model.c helpers.c mesh-tools.c  dreadful-hacks.c dreadful-hacks.c)

target_link_libraries(Client glfw)
target_link_libraries(Client OpenGL::GL)
target_link_libraries(Client threeDplay stb linmath)
target_link_libraries(Client cglm_headers)

#add_subdirectory(external/cglm/ EXCLUDE_FROM_ALL)



#copies assets and shaders to the build dir
file(COPY assets DESTINATION ./)
file(COPY shaders DESTINATION ./)