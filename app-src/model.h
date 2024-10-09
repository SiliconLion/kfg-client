#pragma once

#include "omni-include.h"
#include "dynarr.h"
#include "geometry.h"
#include "linmath.h"

typedef struct {
    //coordinates should be normalized. Not sure anything will break if they aren't,
    //but I'm going to be assuming they are normalized.
    FullGeometry geom; //later this may become a vec of geometries

    //later, textures but might do a full materials thing

    //This matrix represents all the transformations that place the model into
    //world coordinates.
    mat4x4 world_coords_transform;

} Model;

//Model model_from_stl_file(const char* path);