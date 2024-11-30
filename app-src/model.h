#pragma once

#include "omni-include.h"
#include "dynarr.h"
#include "geometry.h"
#include "texture.h"
#include "material.h"
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

    //eventually might be other kinds of materials, but PBR for now
    PBRMaterial* mat;

} Model;

//tex can be null
Model model_new(FullGeometry geom, PBRMaterial* mat);

//Model model_from_stl_file(const char* path);


//model_matrix_loc is the location of the uniform in the currently
//bound shader that transforms local coordinates to world coordinates
//draws the instance at `selected` index
void model_draw_instance(Model* m, u32 selected,  u32 model_matrix_loc);

//model_matrix_loc is the location of the uniform in the currently
//bound shader that transforms local coordinates to world coordinates
//draws every instance
void model_draw_all_instances(Model* m, u32 model_matrix_loc);



//model_matrix_loc is the location of the uniform in the currently
//bound shader that transforms local coordinates to world coordinates
//calls `model_draw_instance` for each index in `selected`
//ideally selected should have no duplicates, but probably not gonna explode if it does
void model_draw_instance_list(Model* m, u32* selected, usize selected_len, u32 model_matrix_loc);


//actually wait on this because we might want to be smarter about
//how textures are deleted
//void model_delete(Model* m);
