#pragma once
#include "platform.h"

INCLUDE_OPENGL

#include <math.h>
#include "matrix.h"
#include "dynarr.h"

//we will be representing transformations with 4x4 matrices
typedef mat_4x4 transform; 


//Note: gennerally the order should be scale, then rotate, then translate. 


//TODO: either get rid of the nitch warning, or clarify
//Warning, can mess up normals. If the normals are reciving the result of the
//transformation, pass the translation along seperately. 
transform trans_new_translation(float x, float y, float z);

//scales it in each respective axis by x,y and z.
//TODO: either get rid of the nitch warning, or clarify
//Warning, can mess up normals. If the normals are reciving the result of the
//transformation, pass the scale along seperately. 
transform trans_new_scale(float x, float y, float z);

//rotation around the x axis by theta (in radians)
transform trans_new_x_rot(float theta);

//rotation around the y axis by theta (in radians)
transform trans_new_y_rot(float theta);

//rotation around the z axis by theta (in radians)
transform trans_new_z_rot(float theta);

//sends the transformation as a uniform using glUniformMatrix4fv
//obviously needs all the correct context setup and everything
void trans_send_uniform(GLint location, transform trans);


//represents a sequence of transformantions
typedef struct {
    //each individual transformation
    dynarr transforms;
} tran_chain; 

tran_chain tran_chain_new();

//adds the transformation to the chain
void tran_chain_add(tran_chain * self, transform trans) ;

//takes all the transformations and multiplies them into one transformation
//multiplied from first to last.
//An empty tran_chain will return the the identity transformation
transform tran_chain_squash(tran_chain * self);

void tran_chain_clear(tran_chain * chain);

void tran_chain_delete(tran_chain * self);