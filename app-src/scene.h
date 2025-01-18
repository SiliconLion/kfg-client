#pragma once

#include "omni-include.h"

#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags

#include "dynarr.h"


typedef struct {
    dynarr geometries;
    dynarr diffuse_textures;
    dynarr normals_textures;
    dynarr models;
} Scene;

bool import_scene(Scene* scene_out, const char* pFile);