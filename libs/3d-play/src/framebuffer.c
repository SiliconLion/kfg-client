#include "framebuffer.h"

#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
#endif
#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
    #define STB_IMAGE_WRITE_IMPLEMENTATION
#endif

//#include "stb_image.h"
#include "stb_image_write.h"
#include <stdbool.h>

#include "error-handling.h"



//Note: Apparently there are some issues with many drivers when trying to reuse an FBO across
// things like a resize. So here, best practice is to delete and create a totally new FBO

void framebuffer_new(Framebuffer* f, int32_t pixel_format, int32_t width, int32_t height) {
    glGenFramebuffers(1, &f->fbo_id);
    glGenTextures(1, &f->color_tex_id);
    glGenTextures(1, &f->depth_tex_id);
//    glGenTextures(1, &f->stencil_tex_id);
    f->width = width;
    f->height = height;
    f->color_pixel_format = pixel_format;

    glBindFramebuffer(GL_FRAMEBUFFER, f->fbo_id);

//set up the color texture
    glBindTexture(GL_TEXTURE_2D, f->color_tex_id);
    glTexImage2D(
            GL_TEXTURE_2D, 0,
            pixel_format, width, height,0,
            pixel_format, GL_UNSIGNED_BYTE, NULL
            );

    //we dont really care about mipmaps or anything cuz it matches the viewport.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, f->color_tex_id, 0);

    //error checking
        uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if( status != GL_FRAMEBUFFER_COMPLETE) {

            printf("Error creating Frame Buffer Object in the color texture section. Error: %" PRIu32 "\n", status);
        }
        uint32_t err = glGetError();
        if( err != GL_NO_ERROR) {
            printf("Error creating Frame Buffer Object in the color texture section. Error: %" PRIu32 "\n", err);
        }

//set up the depth texture
    glBindTexture(GL_TEXTURE_2D, f->depth_tex_id);
    glTexImage2D(
            GL_TEXTURE_2D, 0,
            GL_DEPTH_COMPONENT, width, height,0,
            GL_DEPTH_COMPONENT, GL_FLOAT, NULL
    );
    GLERROR();

    //we dont really care about mipmaps or anything cuz it matches the viewport.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, f->depth_tex_id, 0);
    GLERROR();
    //error checking
        status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if( status != GL_FRAMEBUFFER_COMPLETE) {
            printf("Error creating Frame Buffer Object in the Depth texture section. Error: %" PRIu32 "\n", status);
        }
        err = glGetError();
        if( err != GL_NO_ERROR) {
            printf("Error creating Frame Buffer Object in the Depth texture section. Error: %" PRIu32 "\n", err);
        }

//Cant specify GL_STENCIL_INDEX as format or internalFormat for glTexImage2D unless using opengl 4.4 or greater.
//Now, I belive we *can* pass it GL_DEPTH_STENCIL, and have a combined depth and stencil texture. But i dont want
//to do that for now. So skipping the stencil information until something actually needs it.
//
////set up the stencil texture
//    glBindTexture(GL_TEXTURE_2D, f->depth_tex_id);
//    glTexImage2D(
//            GL_TEXTURE_2D, 0,
//            GL_STENCIL_INDEX, width, height,0,
//            GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, NULL
//    );
//    GLERROR();
//
//    //we dont really care about mipmaps or anything cuz it matches the viewport.
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, f->stencil_tex_id, 0);
//    GLERROR();
//    //error checking
//        status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//        if( status != GL_FRAMEBUFFER_COMPLETE) {
//            printf("Error creating Frame Buffer Object in Stencil Texture section. Error: %" PRIu32 "\n", status);
//        }
//        err = glGetError();
//        if( err != GL_NO_ERROR) {
//            printf("Error creating Frame Buffer Object in Stencil Texture section. Error: %" PRIu32 "\n", err);
//        }


    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}



void framebuffer_bind(Framebuffer* f) {
    glBindFramebuffer(GL_FRAMEBUFFER, f->fbo_id);
}

//void framebuffer_bind_read(Framebuffer* f) {
//    glBindFramebuffer(GL_READ_FRAMEBUFFER, f->fbo_id);
//}
//
//void framebuffer_bind_draw(Framebuffer* f) {
//    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, f->fbo_id);
//}

//usually technically does not need to be called, but prevents the wrong framebuffer being written to
void framebuffer_unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0); //might be unnecessary but doesnt hurt.
}

void framebuffer_delete(Framebuffer* f) {
    glDeleteTextures(1, &f->color_tex_id);
    glDeleteTextures(1, &f->depth_tex_id);
//    glDeleteTextures(1, &f->stencil_tex_id);
    glDeleteFramebuffers(1, &f->fbo_id);
}


void framebuffer_color_save_image(Framebuffer* f, const char* file_path) {

//    std::unique_ptr<unsigned char[]> data = std::make_unique<unsigned char[]>(width * height * 3 * sizeof(unsigned int));
// Or you can just:
//    unsigned char *data = new unsigned_char[width * height * 3];
    unsigned char *data = malloc(f->width * f->height * 4);
    glBindTexture(GL_TEXTURE_2D, f->color_tex_id);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_flip_vertically_on_write(true);
    int stride_of_pixel_row_in_bytes = 4 * f->width; // can you tell by the long name I missunderstood what this was supposed to be?
    int ret = stbi_write_png(file_path, f->width, f->height, 4, data, stride_of_pixel_row_in_bytes);
    if (ret == 0) {
        printf("Error: Cannot save color image from framebuffer. (may be many possibilities but check filepath)\n");
    }

    free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

//TODO: de-dup this function. This is just a copy of `framebuffer_color_save_image` with the framebuffer's
//depth texture bound instead. There's a more elegant way to do this but I don't have the time to be DRY rn.
void framebuffer_depth_save_image(Framebuffer* f, const char* file_path, bool normalize) {
    usize pixel_count = f->width * f->height;
    float *data = malloc(pixel_count * sizeof(float));
    glBindTexture(GL_TEXTURE_2D, f->depth_tex_id);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, data);
    GLERROR();

//    printf("Depth buffer : ");
//    for(usize i = 0; i < pixel_count; i++) {
//        printf(" %f,", data[i]);
//    }
//    printf("\n\n\n\n");

    u8* byte_data = malloc(pixel_count * sizeof(u8));
    if(normalize) {
        f32 max_depth = -INFINITY, min_depth = INFINITY;
        for (usize i = 0; i < pixel_count; i++) {
            if (data[i] > max_depth) { max_depth = data[i]; }
            else if (data[i] < min_depth) { min_depth = data[i]; }
        }
        f32 range = max_depth - min_depth;
        f32 scale = 255.f / range;

        for(usize i = 0; i < pixel_count; i++) {
            byte_data[i] = (u8)(scale * (data[i] + min_depth) );
        }
    } else {
        f32 scale = 1.f / 255.f;
        for(usize i = 0; i < pixel_count; i++) {
            byte_data[i] = (u8)(scale * data[i] );
        }
    }




    stbi_flip_vertically_on_write(true);
    int stride_of_pixel_row_in_bytes =  f->width; // can you tell by the long name I missunderstood what this was supposed to be?
    int ret = stbi_write_png(file_path, f->width, f->height, 1, byte_data, stride_of_pixel_row_in_bytes);
    if (ret == 0) {
        printf("Error: Cannot save image from framebuffer. (may be many possibilities but check filepath)\n");
    }


    free(data);
    free(byte_data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

