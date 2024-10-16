//
// Created by David Sullivan on 10/7/24.
//

#include "helpers.h"
#include <stdio.h>

void vec3_print(vec3 V) {
    printf("[%f,%f,%f]", V[0], V[1], V[2]);
}
void vec4_print(vec4 V) {
    printf("[%f,%f,%f,%f]", V[0], V[1], V[2], V[3]);
}
void mat4_print(mat4 M) {
    printf(
            " | %f, %f, %f, %f | \n"
            " | %f, %f, %f, %f | \n"
            " | %f, %f, %f, %f | \n"
            " | %f, %f, %f, %f | \n"
            "\n"
            ,
            M[0][0], M[1][0], M[2][0], M[3][0],
            M[0][1], M[1][1], M[2][1], M[3][1],
            M[0][2], M[1][2], M[2][2], M[3][2],
            M[0][3], M[1][3], M[2][3], M[3][3]
    );
}