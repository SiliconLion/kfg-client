


set(CMAKE_C_STANDARD 11)




#This is the actual end product program
add_executable(Moire main.c image.c)

target_link_libraries(Moire glfw)
target_link_libraries(Moire OpenGL::GL)
target_link_libraries(Moire cglm_headers)
target_link_libraries(Moire assimp)
target_link_libraries(Moire glad)
target_link_libraries(Moire stb)
target_link_libraries(Moire SDL3::SDL3)

target_link_libraries(Moire threeDplay)

#add_subdirectory(external/cglm/ EXCLUDE_FROM_ALL)



#copies assets and shaders to the build dir
file(COPY assets DESTINATION ./)
file(COPY shaders DESTINATION ./)