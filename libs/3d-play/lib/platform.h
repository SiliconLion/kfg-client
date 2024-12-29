#pragma once

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
#endif

//#ifdef __APPLE__
//    #define INCLUDE_OPENGL include <OpenGL/gl3.h>
//#else 
//#define INCLUDE_OPENGL #include <glad/glad.h>
//#endif

#define INCLUDE_OPENGL


#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else 
    #include <glad/glad.h>
    //#include "glad/glad.h"
    // #include "glad.h"
#endif

