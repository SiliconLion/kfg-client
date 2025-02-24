#pragma once
#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#endif
#include <inttypes.h>

#include "omni-include.h"

//TODO: Support renderbuffer attachments, not just texture attachments
//TODO: Support depth and stencil textures - in progress
//TODO: Support multitexture Frambuffers?
typedef struct {
    u32 fbo_id; //frameBuffer
    u32 color_tex_id;
    u32 depth_tex_id;
//    u32 stencil_tex_id;
    u32 width;
    u32 height;
    int32_t color_pixel_format;
} Framebuffer; //FBO for short. "frame buffer object"


//Note: Apparently there are some issues with many drivers when trying to reuse an FBO across
// things like a resize. So here, best practice is to delete and create a totally new FBO

void framebuffer_new(Framebuffer* f, int32_t pixel_format, int32_t width, int32_t height);

void framebuffer_new_depth(Framebuffer* f, int32_t width, int32_t height);

void framebuffer_bind(Framebuffer* f);

//void framebuffer_bind_read(Framebuffer* f) {
//    glBindFramebuffer(GL_READ_FRAMEBUFFER, f->fbo_id);
//}
//
//void framebuffer_bind_draw(Framebuffer* f) {
//    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, f->fbo_id);
//}

//usually technically does not need to be called, but prevents the wrong framebuffer being written to
void framebuffer_unbind();

void framebuffer_delete(Framebuffer* f);

void framebuffer_color_save_image(Framebuffer* f, const char* file_path);
void framebuffer_depth_save_image(Framebuffer* f, const char* file_path, bool normalize);


