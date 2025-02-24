#pragma once



#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
#endif

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <glad/glad.h>
#endif

