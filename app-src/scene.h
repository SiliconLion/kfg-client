#pragma once

#include "omni-include.h"

#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags

#include "dynarr.h"


typedef struct {
    dynarr models; //dynarr<Model>
    dynarr materials; //dynarr<Material*>
} Scene;

//returns -1 on error, 1 on success 
int scene_from_file(Scene* dest, const char* path);