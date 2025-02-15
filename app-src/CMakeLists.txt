


set(CMAKE_C_STANDARD 11)




#This is the actual end product program
add_executable(Client main.c board.c )

target_link_libraries(Client glfw)
target_link_libraries(Client OpenGL::GL)
target_link_libraries(Client cglm_headers)
target_link_libraries(Client assimp)
target_link_libraries(Client stb)

target_link_libraries(Client threeDplay)


#add_subdirectory(external/cglm/ EXCLUDE_FROM_ALL)



#copies assets and shaders to the build dir
file(COPY assets DESTINATION ./)
file(COPY shaders DESTINATION ./)