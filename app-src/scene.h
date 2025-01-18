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
    dynarr model_prototypes;
    dynarr model_instances;
} Scene;

bool import_scene(Scene* scene_out, const char* pFile);