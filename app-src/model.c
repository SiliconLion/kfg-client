
#include "model.h"

Model model_new(FullGeometry geom, PBRMaterial* mat) {
    dynarr model_instances = dynarr_new(sizeof(mat4), 1);
    return (Model){
        .geom = geom, .model_instances = model_instances, .mat = mat
    };
}

void model_draw_instance(Model* m, u32 selected,  u32 model_matrix_loc) {
    mat4* instance = dynarr_get(&m->model_instances, selected);

    glUniformMatrix4fv(
            model_matrix_loc,
            1,
            GL_FALSE,// column major order
            **instance
    );

    if(m->mat) {
        PBRMaterialBind(m->mat, 0);
    }

    full_geom_draw(&m->geom);

    free(instance);
}

//TODO: make this use the proper opengl instanced draw calls
void model_draw_all_instances(Model* m, u32 model_matrix_loc) {
    for(int i = 0; i < m->model_instances.len; i++) {
        model_draw_instance(m, i, model_matrix_loc);
    }
}

void model_draw_instance_list(
        Model* m, u32* selected, usize selected_len, u32 model_matrix_loc
) {
    for(u32 i = 0; i < selected_len; i++) {
        u32 idx = selected[i];
        model_draw_instance(m, idx, model_matrix_loc);
    }
}

//void model_delete(Model* m) {
//    full_geom_delete(&m->geom);
//    //actually wait on this because we might want to be smarter about
//    //how textures are deleted
////    tex_delete(m->tex);
//}