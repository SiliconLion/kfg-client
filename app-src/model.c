
#include "model.h"

ModelPrototype model_prototype_new(FullGeometry* geom, Texture** diffuse, Texture** normals) {
    return (ModelPrototype){
        .geom = geom,
        .diffuse = diffuse, 
        .normals = normals
    };
}

void draw_model_from_mat(ModelPrototype* m, mat4 transform, u32 world_matrix_loc) {
    glUniformMatrix4fv(
        world_matrix_loc,
        1,
        GL_FALSE,// column major order
        transform
    );

    if(m->diffuse) {
        tex_bind(*m->diffuse, 0);
    } 
    if(m->normals) {
        tex_bind(*m->diffuse, 1);
    }

    full_geom_draw(m->geom);
}

// void model_draw_instance(ModelPrototype* m, u32 selected, u32 model_matrix_loc) {
//     mat4* instance = dynarr_get(&m->model_instances, selected);

//     glUniformMatrix4fv(
//             model_matrix_loc,
//             1,
//             GL_FALSE,// column major order
//             **instance
//     );

//     if(*m->diffuse) {
//         tex_bind(*m->diffuse, 0);
//     } 
//     if(*m->normals) {
//         tex_bind(*m->normals, 1);
//     }

//     full_geom_draw(m->geom);

//     free(instance);
// }

// //TODO: make this use the proper opengl instanced draw calls
// void model_draw_all_instances(ModelPrototype* m, u32 model_matrix_loc) {
//     for(int i = 0; i < m->model_instances.len; i++) {
//         model_draw_instance(m, i, model_matrix_loc);
//     }
// }

// void model_draw_instance_list(
//         ModelPrototype* m, u32* selected, usize selected_len, u32 model_matrix_loc
// ) {
//     for(u32 i = 0; i < selected_len; i++) {
//         u32 idx = selected[i];
//         model_draw_instance(m, idx, model_matrix_loc);
//     }
// }

// //void model_delete(Model* m) {
// //    full_geom_delete(&m->geom);
// //    //actually wait on this because we might want to be smarter about
// //    //how textures are deleted
// ////    tex_delete(m->tex);
// //}

ModelInstance model_instance_new(ModelPrototype* p, mat4 world_transform) {
    ModelInstance m;
    m.prototype = p;
    glm_mat4_copy(world_transform, m.world_transform);
    return m;
}


void draw_model_instance(ModelInstance* inst, u32 world_matrix_loc) {
    draw_model_from_mat(inst->prototype, inst->world_transform, world_matrix_loc);
}

void draw_all_model_instances(dynarr* instances, u32 world_matrix_loc) {
    for(usize i = 0; i < instances->len; i++) {
        draw_model_instance(dynarr_at(instances, i), world_matrix_loc);
    }
}