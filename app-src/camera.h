#pragma once

#include "omni-include.h"

#include <cglm/cglm.h>

//define a direction "up" for the world. (gonna be the positive y direction)
extern vec3 up_dir_global; 

typedef struct {
    mat4 view; //matrix that converts world coords to camera coords
   //these are the variables needed to construction the camera transform.
   //anytime these are updated, the camera_transform should be updated
    vec3 pos;
    vec3 target;

    //matrix that converts camera coords to clip coords and applies
    // a perspective effect
    mat4 perspective;

    //these are the varibles needed to construct the perspective transform.
    //anytime they are set, the persepective_transform should be updated.
    f32 y_fov;
    f32 aspect;
    f32 near_plane;
    f32 far_plane;
} Camera;

Camera camera_new(
        vec3 pos, vec3 look_at_point,
        float y_fov, float aspect, float near_plane, float far_plane
    );

//call this on the camera any time you set a variable in the camera
//Note: should probably have a way to indicate an error, but ehh, later.
void camera_update(Camera* c);

void camera_get_dir(Camera* c, vec3 dest); 




typedef void CAMERA_INTERFACE_UPDATE_FN( f32 delta_u, f32 delta_v, f32 delta_zoom );



typedef enum {
    C_I_ORBIT, C_I_POV, /*C_I_TRACK */
} CameraInterfaceType;


typedef struct {
    Camera c;
    CameraInterfaceType type;
    CAMERA_INTERFACE_UPDATE_FN* update_fn;
} CameraInterface;


void camera_interface_pov_update( f32 delta_u, f32 delta_v, f32 delta_zoom );
void camera_interface_orbit_update( f32 delta_u, f32 delta_v, f32 delta_zoom );

extern CAMERA_INTERFACE_UPDATE_FN* C_I_ORBIT_UPDATE;
extern CAMERA_INTERFACE_UPDATE_FN* C_I_POV_UPDATE;