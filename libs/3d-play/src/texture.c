#include "texture.h"



#include <stdlib.h> 
#include <stdlib.h>
#include <stdbool.h>

#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb_image.h"
#include "utilities.h"



//slot is the number of the texture slot to bind the texture to. 
//For instance, slot = 0 would be GL_TEXTURE0, and slot = 3 would be GL_TEXTURE3
void tex_bind(Texture * texture, int slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void tex_unbind() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

//takes the path to the texture file and creates an OpenGL texture
//alpha tells whether or not there the image format has alpha
Texture tex_new(const char * path) {
    stbi_set_flip_vertically_on_load(true); 

    Texture texture;
    unsigned char* data = stbi_load(
        path, 
        &(texture.width),
        &(texture.height),
        &(texture.nrChannels),
        0
    );

    if(!data) {
        printf("failed to load texture from path %s\n", path);
        return EMPTY_TEXTURE;
    }

    glGenTextures(1, &(texture.id));
    tex_bind(&texture, 0);


    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (texture.nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    } else if (texture.nrChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else {
        printf("image has %u channels (which is not 3 or 4).\n", texture.nrChannels);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
 
    glGenerateMipmap(GL_TEXTURE_2D);



    stbi_image_free(data);
    tex_unbind();

    return texture;
}

void tex_delete(Texture * texture) {
    tex_unbind();
    glDeleteTextures(1, &(texture->id));
}

