#include "camera.h"
#include <stdio.h>
//#include "helpers.h"

void mat4x4_print(mat4x4 M) {
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

Camera camera_new(
        vec3 pos, vec3 look_at_point,
        float y_fov, float aspect, float near_plane, float far_plane
) {
    Camera c = {
        .y_fov = y_fov,
        .aspect = aspect,
        .near_plane = near_plane,
        .far_plane = far_plane
    }; //initializes everything thats trivial
    vec3_dup(c.pos, pos);
    vec3_dup(c.look_at_point, look_at_point);

    camera_update(&c);

    printf("look-at matrix \n");
    mat4x4_print(c.camera_transform);

    printf("perspective matrix \n");
    mat4x4_print(c.perspective_transform);


    return c;
}

//I'll try to make this all work another time for my own satisfaction. leaving a
//TODO: verify my linear algebra skilz
//so I can find it easy
////listen, i did all the math before discovering linmath, which has its own lookat
////matrix implimentation. But I want to try mine.
//void hidden_construct_look_at(mat4x4 ret, vec3 pos, vec3 look_at_point) {
//    //d represents the direction the camera is looking at.
//    vec3 d = {0};
//    vec3_sub(d, look_at_point, pos);
//    vec3_norm(d, d);
//    // d = |look_at_point - pos|
//
//    vec3 g = {0.f, 1.0f, 0.0f}; //global up
//
//    vec3 u = {0}; //camera up
//    float g_dot_d = vec3_mul_inner(g,d);
//    vec3_scale(u, g, g_dot_d); // (g DOT d)g
//    vec3_add(u, u, d);
//    vec3_norm(u, u);
//    // u = | (g DOT d)g + d |
//
//    vec3 w = {0};
//    vec3_mul_cross(w, d, u);
//    // w = d x u
//    //dont need to take the norm because u has been constructed to be
//    //perpendicular to d.
//
//    //g, u, w form the "camera" basis
//
//
//    mat4x4 M; //this matrix gives us the angle we need to look at the point
//    M[0][0] = w[0]; M[1][0] = u[0]; M[2][0] = -1.f * d[0]; M[3][0] = 0;
//    M[0][1] = w[1]; M[1][1] = u[1]; M[2][1] = -1.f * d[0]; M[3][1] = 0;
//    M[0][2] = w[2]; M[1][2] = u[2]; M[2][2] = -1.f * d[0]; M[3][2] = 0;
//    M[0][3] = 0;    M[1][3] = 0   ; M[2][3] = 0;           M[3][3] = 1;
//
//    mat4x4_invert(M, M);
//    //the hope is this is the homogenous equivalent of M = [w u -d]^-1
//
//    mat4x4 T;
//    mat4x4_translate(T, pos[0], pos[1], pos[2]);
//
//    mat4x4_mul(ret, T, M);
//}


//call this on the camera any time you set a variable in the camera
void camera_update(Camera* c) {
    mat4x4_perspective(
            c->perspective_transform,
            c->y_fov, c->aspect, c->near_plane, c->far_plane
    );
//    hidden_construct_look_at(c->camera_transform, c->pos, c->look_at_point);

    //d represents the direction the camera is looking at.
    vec3 eye = {0};
    vec3_sub(eye, c->look_at_point, c->pos);
    vec3_norm(eye, eye);
    // eye = |look_at_point - pos|

    vec3 up = {0.f, 1.0f, 0.0f}; //global up
    mat4x4_look_at(c->camera_transform, eye, c->pos, up);
}

