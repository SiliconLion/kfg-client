#pragma once
#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#endif

void print_gl_error(int line, char* file);

#define GLERROR() \
        do{       \
            print_gl_error(__LINE__, __FILE__); \
        }while(0)

//just makes print debugging a little less tedious
#ifdef NDEBUG
    #define HERE do{}while(0) //do nothing
#else
    #define HERE() do{ printf("here at %s:%i\n", __FILE__, __LINE__); }while(0)
#endif