
#include "model.h"


void model_draw(Model* m, u32 model_matrix_loc) {
    glUniformMatrix4fv(
            model_matrix_loc,
            1,
            GL_FALSE,// column major order
            m->model_matrix
    );

    tex_bind(m->tex, 0);
    full_geom_draw(&m->geom);
}

//void model_delete(Model* m) {
//    full_geom_delete(&m->geom);
//    //actually wait on this because we might want to be smarter about
//    //how textures are deleted
////    tex_delete(m->tex);
//}