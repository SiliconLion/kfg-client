#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#endif


typedef struct {
    unsigned int program;
} Shader;

//ToDo: change signature to not return a pointer
Shader * shad_new(const char * vert_path, const char * frag_path);

void shad_bind(Shader * shader);

void shad_unbind();

void shad_delete(Shader * shad);