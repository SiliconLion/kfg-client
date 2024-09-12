#stb libs are header only, which we can hint to cmake by setting the "INTERFACE" option,
#and then we just export this directory as a header directory (below)

add_library(stb INTERFACE)

 target_include_directories(stb
         INTERFACE
         ${CMAKE_CURRENT_LIST_DIR} #this directory
         )
