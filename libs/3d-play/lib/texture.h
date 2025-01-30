#pragma once
#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#endif

#include <stdlib.h>
#include <stdbool.h>



typedef struct {
    //the id of the texture
    unsigned int id;
    int width;
    int height;
    //number of color channels
    int nrChannels;
} Texture;

#define EMPTY_TEXTURE (Texture){.id = 0, .width = 0, .height=0, .nrChannels=0}



//slot is the number of the texture slot to bind the texture to. 
//For instance, slot = 0 would be GL_TEXTURE0, and slot = 3 would be GL_TEXTURE3
void tex_bind(Texture * texture, int slot);

void tex_unbind() ;

//takes the path to the texture file and creates an OpenGL texture
//automatically detects alpha
Texture tex_new(const char * path);

void tex_delete(Texture * texture);

