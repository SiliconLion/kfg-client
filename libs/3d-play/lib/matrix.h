#pragma once

#include <stdbool.h>

//We could of course make a more dynamic array and avoid code 
//duplication, or do some extremely hacky preprocessor tricks to 
//try and force const generics, but since bot 3x3 and 4x4 arrays are
//so common, this seems like the best answer, and comes with many 
//bennifits like being totally stack allocated, so no delete function needed

//3x3 array
typedef struct {
    //the elements of the matrix stored as a flat array. 
    float elements[9];
} mat_3x3; 

//4x4 array
typedef struct {
    //the elements of the matrix stored as a flat array. 
    float elements[16];
} mat_4x4; 


const mat_3x3 MAT_3X3_ZERO = {.elements = 
                    {0.0, 0.0, 0.0, 
                     0.0, 0.0, 0.0, 
                     0.0, 0.0, 0.0}};

const mat_3x3 MAT_3X3_IDENT = {.elements = 
                    {1.0, 0.0, 0.0,
                     0.0, 1.0, 0.0,
                     0.0, 0.0, 1.0}};

const mat_4x4 MAT_4x4_ZERO = {.elements = 
                    {0.0, 0.0, 0.0, 0.0,
                     0.0, 0.0, 0.0, 0.0, 
                     0.0, 0.0, 0.0, 0.0,
                     0.0, 0.0, 0.0, 0.0}};
            
const mat_4x4 MAT_4X4_IDENT = {.elements = 
                    {1.0, 0.0, 0.0, 0.0,
                     0.0, 1.0, 0.0, 0.0,
                     0.0, 0.0, 1.0, 0.0,
                     0.0, 0.0, 0.0, 1.0 }};

//returns whether the matricies are equal or not
bool mat_3x3_equal(mat_3x3 lhs, mat_3x3 rhs);

//does not take ownership of elements. copies it instead
mat_3x3 mat_3x3_new(float elements[9]);


//gets the value of an element of a 3x3 matrix
float mat_3x3_get( mat_3x3 matrix, int row, int col);


void mat_3x3_get_row(float dest[3], mat_3x3 matrix, int row);

void mat_3x3_get_col(float dest[3], mat_3x3 matrix, int col);

void mat_3x3_set(mat_3x3 * matrix, float value, int row, int col);

mat_3x3 mat_3x3_add(mat_3x3 lhs, mat_3x3 rhs);

mat_3x3 mat_3x3_negate(mat_3x3 input);

mat_3x3 mat_3x3_sub(mat_3x3 lhs, mat_3x3 rhs) ;

//multiplty two 3x3 matricies. 
mat_3x3 mat_3x3_mult(mat_3x3 lhs, mat_3x3 rhs);

void mat_3x3_print(mat_3x3 mat);
//The functions for a 4x4 matrix


bool mat_4x4_equal(mat_4x4 lhs, mat_4x4 rhs);

//does not take ownership of elements. copies it instead
mat_4x4 mat_4x4_new(float elements[16]);


//gets the value of an element of a 4x4 matrix
float mat_4x4_get( mat_4x4 matrix, int row, int col);

void mat_4x4_get_row(float dest[4], mat_4x4 matrix, int row);

void mat_4x4_get_col(float dest[4], mat_4x4 matrix, int col) ;

void mat_4x4_set(mat_4x4 * matrix, float value, int row, int col);

mat_4x4 mat_4x4_add(mat_4x4 lhs, mat_4x4 rhs);

mat_4x4 mat_4x4_negate(mat_4x4 input);

mat_4x4 mat_4x4_sub(mat_4x4 lhs, mat_4x4 rhs) ;

//multiplty two 4x4 matricies. 
mat_4x4 mat_4x4_mult(mat_4x4 lhs, mat_4x4 rhs);

void mat_4x4_print(mat_4x4 mat);