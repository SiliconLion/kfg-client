#include "transformation.h"

#include <math.h>
#include "matrix.h"
#include "dynarr.h"



//Note: gennerally the order should be scale, then rotate, then translate. 


//TODO: either get rid of the nitch warning, or clarify
//Warning, can mess up normals. If the normals are reciving the result of the
//transformation, pass the translation along seperately. 
transform trans_new_translation(float x, float y, float z) {
    transform trans = MAT_4X4_IDENT;
    mat_4x4_set(&trans, x, 0, 3);
    mat_4x4_set(&trans, y, 1, 3);
    mat_4x4_set(&trans, z, 2, 3);

    return trans;
}

//scales it in each respective axis by x,y and z.
//TODO: either get rid of the nitch warning, or clarify
//Warning, can mess up normals. If the normals are reciving the result of the
//transformation, pass the scale along seperately. 
transform trans_new_scale(float x, float y, float z) {
    transform scale = MAT_4X4_IDENT;
    mat_4x4_set(&scale, x, 0, 0);
    mat_4x4_set(&scale, y, 1, 1);
    mat_4x4_set(&scale, z, 2, 2);
    return scale;
}

//rotation around the x axis by theta (in radians)
transform trans_new_x_rot(float theta) {
    transform rot = MAT_4X4_IDENT;
    mat_4x4_set(&rot,         cosf(theta), 1, 1);
    mat_4x4_set(&rot, -1.0f * sinf(theta), 1, 2);
    mat_4x4_set(&rot,         sinf(theta), 2, 1);
    mat_4x4_set(&rot,         cosf(theta), 2, 2);
    return rot;
}

//rotation around the y axis by theta (in radians)
transform trans_new_y_rot(float theta) {
    transform rot = MAT_4X4_IDENT;
    mat_4x4_set(&rot,         cosf(theta), 0, 0);
    mat_4x4_set(&rot,         sinf(theta), 0, 2);
    mat_4x4_set(&rot, -1.0f * sinf(theta), 2, 0);
    mat_4x4_set(&rot,         cosf(theta), 2, 2);
    
    return rot;
}

//rotation around the z axis by theta (in radians)
transform trans_new_z_rot(float theta) {
    transform rot = MAT_4X4_IDENT;
    mat_4x4_set(&rot,         cosf(theta), 0, 0);
    mat_4x4_set(&rot, -1.0f * sinf(theta), 0, 1);
    mat_4x4_set(&rot,         sinf(theta), 1, 0);
    mat_4x4_set(&rot,         cosf(theta), 1, 1);

    return rot;
}

//sends the transformation as a uniform using glUniformMatrix4fv
//obviously needs all the correct context setup and everything
void trans_send_uniform(GLint location, transform trans) {
    glUniformMatrix4fv(
                location,
                1,
                //because it is in row major order, we set transpose to true.
                GL_TRUE,
                &(trans.elements)
        );
}


tran_chain tran_chain_new() {
    return (tran_chain) {
        //4 is probably the most common number of transformations at once, 
        //as it encodes an x,y, and z rotation, and a translation. 
        .transforms = dynarr_new(sizeof(transform), 4)
    };
}

//adds the transformation to the chain
void tran_chain_add(tran_chain * self, transform trans) {
    dynarr_push(&(self->transforms), (void*)&trans);
}

//takes all the transformations and multiplies them into one transformation
//multiplied from first to last.
//An empty tran_chain will return the the identity transformation
transform tran_chain_squash(tran_chain * self) {
    transform product = MAT_4X4_IDENT;
    for(int i = 0; i < self->transforms.len; i++) {
        product = mat_4x4_mult(
            product, 
            *(mat_4x4*)dynarr_get(&(self->transforms), i)
        );
    }
    return product;
}

void tran_chain_clear(tran_chain * chain) {
    dynarr_clear(&(chain->transforms));
} 

void tran_chain_delete(tran_chain * self) {
    dynarr_delete( &(self->transforms) );
}