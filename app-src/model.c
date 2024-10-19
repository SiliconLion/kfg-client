
#include "model.h"

Model model_new(FullGeometry geom, Texture* tex) {
    dynarr model_instances = dynarr_new(sizeof(mat4), 1);
    return (Model){
        .geom = geom, .model_instances = model_instances, .tex = tex
    };
}

//TODO: make this use the proper opengl instanced draw calls
void model_draw_instances(Model* m, u32 model_matrix_loc) {
    for(int i = 0; i < m->model_instances.len; i++) {

        mat4* instance = dynarr_get(&m->model_instances, i);

        glUniformMatrix4fv(
                model_matrix_loc,
        1,
        GL_FALSE,// column major order
        **instance
        );

        if(m->tex) {
            tex_bind(m->tex, 0);
        }

        full_geom_draw(&m->geom);

        free(instance);
    }
}

//void model_delete(Model* m) {
//    full_geom_delete(&m->geom);
//    //actually wait on this because we might want to be smarter about
//    //how textures are deleted
////    tex_delete(m->tex);
//}