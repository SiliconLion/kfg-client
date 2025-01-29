#pragma once

#include "omni-include.h"

#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags

#include "dynarr.h"


typedef struct {
    dynarr geometries; //Vec<Geometry>
    dynarr diffuse_textures; //Vec<Texture*>
    dynarr normals_textures; //Vec<Texture*>
    dynarr model_prototypes; //Vec<ModelPrototype>
    dynarr model_instances; //Vec<ModelInstance>
} Scene;

Scene scene_new_empty();

bool import_scene(Scene* scene_out, const char* pFile, bool permissive);

// void add_to_scene()