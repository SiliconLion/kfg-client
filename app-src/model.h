#pragma once

#include "omni-include.h"
#include "dynarr.h"
#include "geometry.h"
#include "texture.h"
#include <cglm/cglm.h>

//Can kinda do a poor-man's single inheritance by having a struct include Model as it's first
//member. Because C's data repr. follows the order of declaration, you can just cast a pointer
//to the child struct to a (Model*), and you'll get the model to act on. This is not
//full inheritance which includes v-tables and function overlaoding, but it lets you
//define structs that in some ways "are" a model, and pass them to functions that want a model
//(like a drawing function)
typedef struct {
    //coordinates should be normalized. Not sure anything will break if they aren't,
    //but I'm going to be assuming they are normalized.
    FullGeometry geom; //later this may become a vec of geometries

    //Vec<Mat4>
    //These matrices represents all the transformations that place each instance into
    //world coordinates.
    dynarr model_instances;

    //eventually will probably be multiple textures (optional), but for now, we will do one
    //optional texture. If null, no texture.
    Texture* tex;

} Model;

//tex can be null
Model model_new(FullGeometry geom, Texture* tex);

//Model model_from_stl_file(const char* path);

//model_matrix_loc is the location of the uniform in the currently
//bound shader that transforms local coordinates to world coordinates
void model_draw_instances(Model* m, u32 model_matrix_loc);


//actually wait on this because we might want to be smarter about
//how textures are deleted
//void model_delete(Model* m);
