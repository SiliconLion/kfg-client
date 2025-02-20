#include "camera.h"
#include <stdio.h>
//#include "helpers.h"
#include <cglm/cglm.h>

vec3 up_dir_global = {0.f, 1.0f, 0.0f}; //global up

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
    glm_vec3_copy(c.pos, pos);
    glm_vec3_copy(c.target, look_at_point);

    camera_update(&c);

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

    glm_perspective(
            c->y_fov, c->aspect,
            c->near_plane, c->far_plane,
            c->perspective);

//    hidden_construct_look_at(c->camera_transform, c->pos, c->look_at_point);

    // vec3 eye;
    // glm_vec3_sub(c->target, c->pos, eye);
    // glm_vec3_norm(eye);
    // // eye = |look_at_point - pos|

    // vec3 up = {0.f, 1.0f, 0.0f}; //global up
    // glm_lookat(eye, c->pos, up, c->view);


    glm_lookat(c->pos, c->target, up_dir_global, c->view);

}

void camera_get_dir(Camera* c, vec3 dest) {
    glm_vec3_sub(c->target, c->pos, dest);
    glm_normalize(dest); 
}

void camera_print(Camera* c) {
    printf(
        "Camera: {\n"
        "           target: (%f, %f, %f),\n"
        "           y_fov:       %f,\n"
        "           aspect:      %f,\n"
        "           near_plane:  %f,\n"
        "           far_plane:   %f\n"
        "}\n",
        c->target[0], c->target[1], c->target[2],
        c->y_fov, c->aspect, c->near_plane, c->far_plane
    );
}






//ToDo: make this more abstract/cleaner, reduce code repetition 
void cc_apply_action(CameraControler* cc, CameraAction action){
    Camera* camera = cc->c;

    switch (action) {
        case CA_MOVE_FORWARD  : {
            vec3 dir, delta;
            camera_get_dir(camera, dir);

            dir[1] = 0; //set y component to 0
            glm_vec3_normalize(dir);

            glm_vec3_scale(dir, cc->forward_speed, delta);

            glm_vec3_add(camera->pos, delta, camera->pos);
            glm_vec3_add(camera->target, delta, camera->target);
            break;
        }
        case CA_MOVE_BACKWARD : {
            vec3 dir, delta;
            camera_get_dir(camera, dir);

            dir[1] = 0; //set y component to 0
            glm_vec3_normalize(dir);

            glm_vec3_scale(dir, -1.0 * cc->backward_speed, delta);

            glm_vec3_add(camera->pos, delta, camera->pos);
            glm_vec3_add(camera->target, delta, camera->target);
            break;
        }

        case CA_MOVE_LEFT   : {
            vec3 dir, delta;
            camera_get_dir(camera, dir);

            glm_vec3_cross(up_dir_global, dir, delta);

            glm_normalize(delta);
            glm_vec3_scale(delta, cc->straif_speed, delta);
            

            glm_vec3_add(camera->pos, delta, camera->pos);
            glm_vec3_add(camera->target, delta, camera->target);
            break;

        }

        case CA_MOVE_RIGHT  : {
            vec3 dir, delta;
            camera_get_dir(camera, dir);

            glm_vec3_cross(up_dir_global, dir, delta);
            glm_normalize(delta);
            //multiplying by -1.0 reverses the direction.
            glm_vec3_scale(delta, -1.0 * cc->straif_speed, delta);
            

            glm_vec3_add(camera->pos, delta, camera->pos);
            glm_vec3_add(camera->target, delta, camera->target);
            break;

        }
        case CA_FLOAT         : {
            vec3 delta;
            glm_vec3_scale(up_dir_global, cc->float_speed, delta);

            glm_vec3_add(camera->pos, delta, camera->pos);
            glm_vec3_add(camera->target, delta, camera->target);
            break;
        }

        case CA_SINK          : {
            vec3 delta;
            glm_vec3_scale(up_dir_global, -1.0* cc->float_speed, delta);

            glm_vec3_add(camera->pos, delta, camera->pos);
            glm_vec3_add(camera->target, delta, camera->target);
            break;
        }

        case CA_ZOOM_IN       : {
            // cc->c.y_fov *= 0.95;
            camera->y_fov *= (1 - cc->zoom_speed);
            break;
        }
        case CA_ZOOM_OUT      : {
            // cc->c.y_fov *= 1.05;
            camera->y_fov *= (1 + cc->zoom_speed);
            break;
        }
        case CA_TILT_UP       : {
            vec3 dir, axis; 
            camera_get_dir(camera, dir);

            glm_vec3_cross(up_dir_global, dir, axis);
            glm_normalize(axis);

            glm_vec3_rotate(dir, -1.0 * cc->tilt_speed, axis);
            glm_vec3_add(camera->pos, dir, camera->target);

            //ToDo: may want to safeguard against ever looking straight up. 
            //glm says it doesnt handle that
            break;
        }
        case CA_TILT_DOWN     : {
            vec3 dir, axis; 
            camera_get_dir(camera, dir);

            glm_vec3_cross(up_dir_global, dir, axis);
            glm_normalize(axis);

            glm_vec3_rotate(dir, cc->tilt_speed, axis);
            glm_vec3_add(camera->pos, dir, camera->target);
            break;
        }

        case CA_PAN_LEFT      : {
            vec3 dir, axis; 
            camera_get_dir(camera, dir);

            glm_vec3_rotate(dir, cc->pan_speed, up_dir_global);
            glm_vec3_add(camera->pos, dir, camera->target);
            break;
        }

        case CA_PAN_RIGHT     : {
            vec3 dir, axis; 
            camera_get_dir(camera, dir);


            glm_vec3_rotate(dir, -1.0 * cc->pan_speed, up_dir_global);
            glm_vec3_add(camera->pos, dir, camera->target);
            break;
        }

        case CA_CIRCLE_TARGET_RIGHT: {
            // vec3 dir, rev_dir; //rev_dir is vector from target to pos
            // camera_get_dir(camera, dir);

            // glm_vec3_negate_to(dir, rev_dir);
            // glm_vec3_rotate(rev_dir, .5*cc->pan_speed, up_dir_global);
            // glm_vec3_add(camera->pos, rev_dir, camera->target);

            vec3 dir;
            glm_vec3_sub(camera->pos, camera->target, dir);
            glm_vec3_rotate(dir, .1*cc->pan_speed, up_dir_global);
            glm_vec3_add(dir, camera->target, camera->pos);
            break;
        }

        case CA_CIRCLE_TARGET_LEFT : {
            vec3 dir;
            glm_vec3_sub(camera->pos, camera->target, dir);
            glm_vec3_rotate(dir, -.1*cc->pan_speed, up_dir_global);
            glm_vec3_add(dir, camera->target, camera->pos);
            break;
        }
    }

    camera_update(camera);
    return;
}



// void camera_interface_pov_update( f32 delta_u, f32 delta_v, f32 delta_zoom ) {

// }
// void camera_interface_orbit_update( f32 delta_u, f32 delta_v, f32 delta_zoom)  {

// }

// CAMERA_INTERFACE_UPDATE_FN* C_I_POV_UPDATE = camera_interface_pov_update;
// CAMERA_INTERFACE_UPDATE_FN* C_I_ORBIT_UPDATE = camera_interface_orbit_update;