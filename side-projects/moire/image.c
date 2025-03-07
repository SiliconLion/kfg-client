#include "image.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif

#include "stb_image_write.h"
#include <stdbool.h>
#include <stdlib.h>

#include "error-handling.h"
#include "utilities.h"





void random_png(u32 width, u32 height, u32 NChannels, const char* path_to_save) {
    usize data_size = width * height * NChannels;
    u8 *data = malloc(data_size);

    for(usize i = 0; i < data_size; i++) {
        data[i] = rand() % 255;
    }

    stbi_flip_vertically_on_write(true);
    int stride_of_pixel_row_in_bytes = NChannels * width; // can you tell by the long name I missunderstood what this was supposed to be?
    int ret = stbi_write_png(path_to_save, width, height, NChannels, data, stride_of_pixel_row_in_bytes);
    if (ret == 0) {
        printf("Error: Cannot save color image from framebuffer. (may be many possibilities but check filepath)\n");
    }

    free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void random_png_percent(u32 width, u32 height, u32 NChannels, const char* path_to_save, f32 percent) {
    usize pixel_count = width * height;
    u8 *data = calloc(pixel_count, sizeof(u8) * NChannels);

    for(usize i = 0; i < pixel_count; i++) {
        if(norm_rand() <= percent) {
            for(usize j = 0; j < NChannels; j++) {
                data[(i*NChannels) + j] = rand() % 255;
            }
        }

    }

    stbi_flip_vertically_on_write(true);
    int stride_of_pixel_row_in_bytes = NChannels * width; // can you tell by the long name I missunderstood what this was supposed to be?
    int ret = stbi_write_png(path_to_save, width, height, NChannels, data, stride_of_pixel_row_in_bytes);
    if (ret == 0) {
        printf("Error: Cannot save color image from framebuffer. (may be many possibilities but check filepath)\n");
    }

    free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}


