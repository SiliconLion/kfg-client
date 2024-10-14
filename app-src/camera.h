#pragma once

#include <cglm/cglm.h>

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
    float y_fov;
    float aspect;
    float near_plane;
    float far_plane;
} Camera;

Camera camera_new(
        vec3 pos, vec3 look_at_point,
        float y_fov, float aspect, float near_plane, float far_plane
    );

//call this on the camera any time you set a variable in the camera
//Note: should probably have a way to indicate an error, but ehh, later.
void camera_update(Camera* c);
