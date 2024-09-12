#defines the target and sources for 3dplay. unfortunately, not a very identifier friendly
#name, so going with threeDplay instead
add_library(threeDplay
        #maybe should use a GLOB or a CONFIG_DEPENDS or something to automatically
        #add all files in src, but this is adequate for now
        src/dynarr.c
        src/error-handling.c
        src/framebuffer.c
        src/geometry.c
        src/matrix.c
        src/post-processing-effects.c
        src/primatives.c
        src/shader.c
        src/stlreader.c
        src/texture.c
        src/transformation.c
        src/utilities.c
        src/vertices.c
        )
#defines GL_SILENCE_DEPRECATION when threeDplay is compiled
add_compile_definitions(threeDplay GL_SILENCE_DEPRECATION)

#links the stb library to three-d-play
target_link_libraries(threeDplay stb)

#exports the headers for three-d-play
target_include_directories(threeDplay
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/lib
        )
#target_include_directories(
#        PUBLIC
#        ./lib
#        )

