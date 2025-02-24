//DEPRECATED




#include "matrix.h"
#include <string.h>
#include "utilities.h"


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
bool mat_3x3_equal(mat_3x3 lhs, mat_3x3 rhs) {
    int cmp = memcmp(lhs.elements, rhs.elements, sizeof(float) * 9);
    if (cmp == 0) {
        return true;
    } else {
        return false;
    }
}

//does not take ownership of elements. copies it instead
mat_3x3 mat_3x3_new(float elements[9]) {
    mat_3x3 matrix;
    //we dont have to allocate elements first because its stack allocated already
    memcpy(matrix.elements, elements, sizeof(float) * 9);
    return matrix;
}


//gets the value of an element of a 3x3 matrix
float mat_3x3_get( mat_3x3 matrix, int row, int col) {
    return matrix.elements[util_flatten_index(row, col, 3)];
}


void mat_3x3_get_row(float dest[3], mat_3x3 matrix, int row) {
    memcpy(dest, matrix.elements + (row * 3), sizeof(float) * 3);
}

void mat_3x3_get_col(float dest[3], mat_3x3 matrix, int col) {
    dest[0] = matrix.elements[col];
    dest[1] = matrix.elements[col + 3];
    dest[2] = matrix.elements[col + 6];
}

void mat_3x3_set(mat_3x3 * matrix, float value, int row, int col) {
    unsigned int i = util_flatten_index(row, col, 3);
    matrix->elements[i] = value;
}

mat_3x3 mat_3x3_add(mat_3x3 lhs, mat_3x3 rhs) {
    mat_3x3 matrix = MAT_3X3_ZERO;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            float lhs_element = mat_3x3_get(lhs, row, col);
            float rhs_element = mat_3x3_get(rhs, row, col);
            mat_3x3_set(&matrix, lhs_element + rhs_element, row, col);
        }
    }
    return matrix;
}

mat_3x3 mat_3x3_negate(mat_3x3 input) {
    mat_3x3 matrix = input; 
    for(int i = 0; i < 9; i++) {
        matrix.elements[i] = -1.0 * matrix.elements[i];
    }
    return matrix;
}

mat_3x3 mat_3x3_sub(mat_3x3 lhs, mat_3x3 rhs) {
    return mat_3x3_add(lhs, mat_3x3_negate(rhs));
}

//multiplty two 3x3 matricies. 
mat_3x3 mat_3x3_mult(mat_3x3 lhs, mat_3x3 rhs) {

    mat_3x3 matrix = MAT_3X3_ZERO;

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            float lhsrow[3], rhscol[3];
            mat_3x3_get_row(lhsrow, lhs, row);
            mat_3x3_get_col(rhscol, rhs, col);


            float element = 0;
            for ( int i = 0; i < 3; i++) {
                element += lhsrow[i] * rhscol[i];
            }

            mat_3x3_set(&matrix, element, row, col);
        }
    }
    return matrix;
}

void mat_3x3_print(mat_3x3 mat) {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            printf("%f, ", *(mat.elements + (i * 3) + j) );
        }
        printf("\n");
    }
    printf("\n");
}


//The functions for a 4x4 matrix


bool mat_4x4_equal(mat_4x4 lhs, mat_4x4 rhs) {
    int cmp = memcmp(lhs.elements, rhs.elements, sizeof(float) * 16);
    if (cmp == 0) {
        return true;
    } else {
        return false;
    }
}

//does not take ownership of elements. copies it instead
mat_4x4 mat_4x4_new(float elements[16]) {
    mat_4x4 matrix;
    //we dont have to allocate elements first because its stack allocated already
    memcpy(matrix.elements, elements, sizeof(float) * 16);
    return matrix;
}


//gets the value of an element of a 4x4 matrix
float mat_4x4_get( mat_4x4 matrix, int row, int col) {
    return matrix.elements[util_flatten_index(row, col, 4)];
}


void mat_4x4_get_row(float dest[4], mat_4x4 matrix, int row) {
    memcpy(dest, matrix.elements + (row * 4), sizeof(float) * 4);
}

void mat_4x4_get_col(float dest[4], mat_4x4 matrix, int col) {
    dest[0] = matrix.elements[col];
    dest[1] = matrix.elements[col + 4];
    dest[2] = matrix.elements[col + 8];
    dest[3] = matrix.elements[col + 12];
}

void mat_4x4_set(mat_4x4 * matrix, float value, int row, int col) {
    unsigned int i = util_flatten_index(row, col, 4);
    matrix->elements[i] = value;
}

mat_4x4 mat_4x4_add(mat_4x4 lhs, mat_4x4 rhs) {
    mat_4x4 matrix = MAT_4x4_ZERO;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            float lhs_element = mat_4x4_get(lhs, row, col);
            float rhs_element = mat_4x4_get(rhs, row, col);
            mat_4x4_set(&matrix, lhs_element + rhs_element, row, col);
        }
    }
    return matrix;
}

mat_4x4 mat_4x4_negate(mat_4x4 input) {
    mat_4x4 matrix = input; 
    for(int i = 0; i < 16; i++) {
        matrix.elements[i] = -1.0 * matrix.elements[i];
    }
    return matrix;
}

// mat_3x3 mat_3x3_negate(mat_3x3 input) {
//     mat_3x3 matrix = input; 
//     for(int i = 0; i < 9; i++) {
//         matrix.elements[i] = -1.0 * matrix.elements[i];
//     }
//     return matrix;
// }

mat_4x4 mat_4x4_sub(mat_4x4 lhs, mat_4x4 rhs) {
    return mat_4x4_add(lhs, mat_4x4_negate(rhs));
}

//multiplty two 4x4 matricies. 
mat_4x4 mat_4x4_mult(mat_4x4 lhs, mat_4x4 rhs) {

    mat_4x4 matrix = MAT_4x4_ZERO;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            float lhsrow[4], rhscol[4];
            mat_4x4_get_row(lhsrow, lhs, row);
            mat_4x4_get_col(rhscol, rhs, col);


            float element = 0;
            for ( int i = 0; i < 4; i++) {
                element += lhsrow[i] * rhscol[i];
            }

            mat_4x4_set(&matrix, element, row, col);
        }
    }
    return matrix;
}

void mat_4x4_print(mat_4x4 mat) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            printf("%f, ", *(mat.elements + (i * 4) + j) );
        }
        printf("\n");
    }
    printf("\n");
}