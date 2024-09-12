#pragma once
#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#endif
#include <inttypes.h>

//TODO: Support renderbuffer attachments, not just texture attachments
//TODO: Support depth and stencil textures
//TODO: Support multitexture Frambuffers?
typedef struct {
    unsigned int fbo_id;
    unsigned int tex_id; //should probably modify this to be just a "Texture" object
    unsigned int width;
    unsigned int height;
    int32_t pixel_format;
} Framebuffer; //FBO for short. "frame buffer object"


//Note: Apparently there are some issues with many drivers when trying to reuse an FBO across
// things like a resize. So here, best practice is to delete and create a totally new FBO

void framebuffer_new(Framebuffer* f, int32_t pixel_format, int32_t width, int32_t height);

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

void framebuffer_save_image(Framebuffer* f, const char* file_path);


