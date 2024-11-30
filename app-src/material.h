#pragma once

#include <assert.h>

#include "omni-include.h"
#include "dynarr.h"
#include "geometry.h"
#include "texture.h"
#include "shader.h"

typedef enum {
    PBR_CHANNEL_DIFFUSE = 0,
    PBR_CHANNEL_SPECULAR,
    // PBR_CHANNEL_AMBIENT,
    // PBR_CHANNEL_EMISSIVE,
    // PBR_CHANNEL_HEIGHT,
    PBR_CHANNEL_NORMALS, 
    // PBR_CHANNEL_SHININESS,
    // PBR_CHANNEL_OPACITY,
    // PBR_CHANNEL_DISPLACEMENT,
    // PBR_CHANNEL_LIGHTMAP,
    // PBR_CHANNEL_REFLECTION,
    // PBR_CHANNEL_BASE_COLOR,
    // PBR_CHANNEL_NORMAL_CAMERA,
    // PBR_CHANNEL_EMISSION_COLOR,
    // PBR_CHANNEL_METALNESS,
    PBR_CHANNEL_DIFFUSE_ROUGHNESS,
    // PBR_CHANNEL_AMBIENT_OCCLUSION,
    // PBR_CHANNEL_UNKNOWN,
    // PBR_CHANNEL_SHEEN,
    // PBR_CHANNEL_CLEARCOAT,
    // PBR_CHANNEL_TRANSMISSION,
    //Number of varients in this enum
    PBR_CHANNEL_COUNT
} PBRTextureChannel;

enum aiTextureType PBRTextureChannelToAI(PBRTextureChannel channel);
// PBRTextureChannel AIToPBRTextureChannel(enum aiTextureType);

static_assert(PBR_CHANNEL_COUNT <= 16, 
                "more variants in PBRTextureChannel than minimum required `GL_MAX_TEXTURE_IMAGE_UNITS` by OpenGL");

const char* PBRTextureChannelToStr(PBRTextureChannel c);

typedef struct {
    char * name;
    Texture* channels[PBR_CHANNEL_COUNT];
} PBRMaterial;


//note, the pbr model shader has to have its sampler uniforms set already for this function to work
void PBRMaterialBind(PBRMaterial* mat, Shader* shad);

// void PBRMaterialUnbind();

//deletes the material, but does not call delete on any of the textures it refrences.
void PBRMaterialSoftDelete(PBRMaterial* mat);

//deletes the material and calls delete on every texture it refrences.
void PBRMaterialDeepDelete(PBRMaterial* mat);