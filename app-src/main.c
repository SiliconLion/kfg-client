#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#endif

#include "omni-include.h"

#include <stdbool.h>
#include <string.h>

#include "stlreader.h"
#include "geometry.h"
#include "shader.h"
#include "texture.h"
#include "model.h"
#include "camera.h"
// #include "board.h"
#include "error-handling.h"
#include "primatives.h"
#include "helpers.h"
#include "dreadful-hacks.h"
#include "scene.h"

#define CGLM_DEFINE_PRINTS
#include "cglm/cglm.h"


//global so callbacks can see them
int windowWidth_global, windowHeight_global;
f32 window_ratio_global;

u32 counter_global;

Camera camera;
f32 zoom_fac = 1.0;
f32 rotation_fac = M_PI / 100.0;
f32 movement_speed = 8;

f32 board_scale = 20.0;

//on a GLFW error, will print the error
void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

//updates the glViewport when the window is resized.
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    windowWidth_global = width;
    windowHeight_global = height;
    window_ratio_global = (float)width / (float)height;


    camera.aspect = window_ratio_global;
    camera_update(&camera);
}

//closes the application when escape key is pressed
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if(key == GLFW_KEY_Z) {
        camera.y_fov *= 0.95;
        zoom_fac *= 0.95;
    }
    if(key == GLFW_KEY_X) {
        camera.y_fov *= 1.05;
        zoom_fac *= 1.05;
    }


    if(key == GLFW_KEY_W) {
        vec3 dir, delta;
        camera_get_dir(&camera, dir);

        dir[1] = 0; //set y component to 0

        glm_vec3_scale(dir, zoom_fac*movement_speed, delta);

        glm_vec3_add(camera.pos, delta, camera.pos);
        glm_vec3_add(camera.target, delta, camera.target);

    }
    if(key == GLFW_KEY_S) {
        vec3 dir, delta;
        camera_get_dir(&camera, dir);

        dir[1] = 0; //set y component to 0

        glm_vec3_scale(dir, -1.0 * zoom_fac*movement_speed, delta);

        glm_vec3_add(camera.pos, delta, camera.pos);
        glm_vec3_add(camera.target, delta, camera.target);
    }

    if(key == GLFW_KEY_A) {
        vec3 dir, delta;
        camera_get_dir(&camera, dir);

        glm_vec3_cross(up_dir_global, dir, delta);
        glm_normalize(delta);
        glm_vec3_scale(delta, movement_speed * zoom_fac, delta);
        

        glm_vec3_add(camera.pos, delta, camera.pos);
        glm_vec3_add(camera.target, delta, camera.target);

    }
    if(key == GLFW_KEY_D) {
        vec3 dir, delta;
        camera_get_dir(&camera, dir);

        glm_vec3_cross(up_dir_global, dir, delta);
        glm_normalize(delta);
        //multiplying by -1.0 reverses the direction.
        glm_vec3_scale(delta, -1.0 * movement_speed * zoom_fac, delta);
        

        glm_vec3_add(camera.pos, delta, camera.pos);
        glm_vec3_add(camera.target, delta, camera.target);

    }

    if(key == GLFW_KEY_SPACE) {
        vec3 delta;
        glm_vec3_scale(up_dir_global, movement_speed * zoom_fac, delta);

        glm_vec3_add(camera.pos, delta, camera.pos);
        glm_vec3_add(camera.target, delta, camera.target);
    }

    if(key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) {
        vec3 delta;
        glm_vec3_scale(up_dir_global, -1.0* movement_speed * zoom_fac, delta);

        glm_vec3_add(camera.pos, delta, camera.pos);
        glm_vec3_add(camera.target, delta, camera.target);
    }

    if(key == GLFW_KEY_U) {
        vec3 dir, axis; 
        camera_get_dir(&camera, dir);

        glm_vec3_cross(up_dir_global, dir, axis);
        glm_normalize(axis);

        glm_vec3_rotate(dir, -1.0 * rotation_fac, axis);
        glm_vec3_add(camera.pos, dir, camera.target);

        //may want to safeguard against ever looking straight up. 
        //glm says it doesnt handle that
    
    }
    if(key == GLFW_KEY_V) {
        vec3 dir, axis; 
        camera_get_dir(&camera, dir);

        glm_vec3_cross(up_dir_global, dir, axis);
        glm_normalize(axis);

        glm_vec3_rotate(dir, rotation_fac, axis);
        glm_vec3_add(camera.pos, dir, camera.target);

        //may want to safeguard against ever looking straight up. 
        //glm says it doesnt handle that
    }


    if(key == GLFW_KEY_I) {
        vec3 dir, axis; 
        camera_get_dir(&camera, dir);

        glm_vec3_rotate(dir, rotation_fac, up_dir_global);
        glm_vec3_add(camera.pos, dir, camera.target);

        //may want to safeguard against ever looking straight up. 
        //glm says it doesnt handle that
    }

    if(key == GLFW_KEY_J) {
        vec3 dir, axis; 
        camera_get_dir(&camera, dir);


        glm_vec3_rotate(dir, -1.0 * rotation_fac, up_dir_global);
        glm_vec3_add(camera.pos, dir, camera.target);

        //may want to safeguard against ever looking straight up. 
        //glm says it doesnt handle that
    }


    if(key == GLFW_KEY_ENTER) {
        printf("Camera stats\n");
        printf("camera pos: [%f, %f, %f]\n", camera.pos[0], camera.pos[1], camera.pos[2]);
        printf("camera target: [%f, %f, %f]\n", camera.target[0], camera.target[1], camera.target[2]);
        printf("camera fov: %f\n", camera.y_fov);
        printf("camera near plane %f\n", camera.near_plane);
        printf("camera far plane %f\n", camera.far_plane);
        printf("\n");
    }

    camera_update(&camera);

}


int main(int argc, const char* argv[]) {

    f32 BOARD_WIDTH = 8.0;
    f32 BOARD_HEIGHT = 2.0;

    mat4 IDENTITY;
    glm_mat4_identity(IDENTITY);

    counter_global = 0;

    //Just plain GLFW window setup and opengl context creation.
    if(!glfwInit()) {
        return 2;
    }
    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //needed for mac
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow * window = glfwCreateWindow(640, 480, "Transform", NULL, NULL);
    if(!window) {
        return 2;
    }
    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &windowWidth_global, &windowHeight_global);
    glViewport(0, 0, windowWidth_global, windowHeight_global);

    window_ratio_global = (float)windowWidth_global / (float)windowHeight_global;

    //adding callbacks for
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    //to avoid screen tearing.
    glfwSwapInterval(1);



    



    vec3 XAXIS = {1.0f, 0.f, 0.f};
    vec3 YAXIS = {0.0f, 1.f, 0.f};
    vec3 ZAXIS = {0.0f, 0.f, 1.f};





    Shader* model_shader = shad_new("shaders/model/model.vert", "shaders/model/model.frag");
    GLERROR();
    u32 model_matrix_loc = glGetUniformLocation(model_shader->program, "model");
    u32 model_view_loc = glGetUniformLocation(model_shader->program, "view");
    u32 model_perspective_loc = glGetUniformLocation(model_shader->program, "perspective");

    u32 model_diffuse_loc = glGetUniformLocation(model_shader->program, "DIFFUSE");
    u32 model_normals_loc = glGetUniformLocation(model_shader->program, "NORMALS");

    shad_bind(model_shader);
    glUniform1i(model_diffuse_loc, 0);
    glUniform1i(model_normals_loc, 1);
    shad_unbind();

    
    GLERROR();



    const char* scene_path;
    if(argc < 2) {
        scene_path = "assets/scenes/Sponza/glTF/Sponza.gltf";
    } else {
        scene_path = argv[1];
    }

    Scene setting;
    if(!import_scene(&setting, scene_path, true)){
        printf("I weep, for dispite promises made, we could not import a gltf file\n");
    } else {
        printf("HUZZAH! Assimp imported a complex gltf file\n");
    }

    GLERROR();



    // Board board = board_new(19, 19);

    // Scene kfg_match = scene_new_empty();

    // FullGeometry g;
    // Texture t; // REALLY gotta deal with this texture pointer nonsense. NEXT COMMIT I SWEAR


    // g = full_geom_from_stl(
    //        "assets/models/go-board-basic.stl", GL_STATIC_DRAW);
    // g = add_text_coords_to_geom(&g);
    // dynarr_push(&kfg_match.geometries, &g);

    // g = full_geom_from_stl(
    //         "assets/models/go-stone-basic1.stl", GL_STATIC_DRAW);
    // g = add_text_coords_to_geom(&g);
    // dynarr_push(&kfg_match.geometries, &g);

    // t = tex_new("assets/misc-textures/light-wood.jpg", false);
    // dynarr_push(&kfg_match.diffuse_textures, &t);

    // t = tex_new("assets/misc-textures/black-stone-texture.jpg", false);
    // dynarr_push(&kfg_match.diffuse_textures, &t);

    // t = tex_new("assets/misc-textures/white-stone-texture.jpg", false);
    // dynarr_push(&kfg_match.diffuse_textures, &t);

    // ModelPrototype board_proto = {
    //     .geom = dynarr_at(&kfg_match.geometries, 0),
    //     .diffuse = dynarr_at(&kfg_match.diffuse_textures, 0),
    //     .normals = NULL
    // };
    // dynarr_push(&kfg_match.model_prototypes, &board_proto);
    
    // ModelPrototype black_stone_proto = {
    //     .geom = dynarr_at(&kfg_match.geometries, 1),
    //     .diffuse = dynarr_at(&kfg_match.diffuse_textures, 1),
    //     .normals = NULL
    // };
    // dynarr_push(&kfg_match.model_prototypes, &black_stone_proto);

    // ModelPrototype white_stone_proto = {
    //     .geom = dynarr_at(&kfg_match.geometries, 1),
    //     .diffuse = dynarr_at(&kfg_match.diffuse_textures, 2),
    //     .normals = NULL
    // };
    // dynarr_push(&kfg_match.model_prototypes, &white_stone_proto);


    // ModelInstance board_inst;
    // board_inst.prototype = dynarr_at(&kfg_match.model_prototypes, 0);
    // glm_scale_make(board_inst.world_transform, (vec3){40, 40, 40});
    // glm_rotate(board_inst.world_transform, -1.f * M_PI_2, XAXIS);

    // dynarr_push(&kfg_match.model_instances, &board_inst);


    Scene kfg_match;
    // if(!import_scene(&kfg_match, "assets/scenes/go-board-basic.glb", true)) {
    //     printf("error, could not import go match scene.\n");
    // }
    if(!import_scene(&kfg_match, "assets/scenes/go-board/go-board.gltf", true)) {
        printf("error, could not import go match scene.\n");
    }

    // //puts the board, and the black and white stones at the origin
    // for(usize i = 0; i < kfg_match.model_prototypes.len; i++) {
    //     ModelInstance* m = dynarr_at(&kfg_match.model_prototypes, i);
    //     glm_mat4_identity(m->world_transform);
    // }




    mat4 scale_match;
    glm_scale_make(scale_match, (vec3){board_scale, board_scale, board_scale});



    glDisable(GL_CULL_FACE);



    // glm_vec3_copy((vec3){-14.915992, 104.215508, -0.615611}, camera.pos);
    // glm_vec3_copy((vec3){-13.920774, 104.313194, -0.615634}, camera.target);
    // camera.y_fov = 0.954656;
    // camera.near_plane = 0.100000; 
    // camera.far_plane = 3000.000000;
    // camera.aspect = window_ratio_global;

    // glm_vec3_zero(camera.target);
    // glm_vec3_copy((vec3){5, 5, -5}, camera.pos);
    // camera.y_fov = 0.954656;
    // camera.near_plane = 0.100000; 
    // camera.far_plane = 300.000000;
    // camera.aspect = window_ratio_global;


    glm_vec3_copy((vec3){-132.255875, 120.215515, -5.458469}, camera.pos);
    glm_vec3_copy((vec3){-131.606308, 119.455688, -5.431660}, camera.target);
    camera.y_fov = 0.954656;
    camera.near_plane = 0.100000; 
    camera.far_plane = 3000.000000;
    camera.aspect = window_ratio_global;

    camera_update(&camera);


    u64 frames = 0;

    //the render loop
    while (!glfwWindowShouldClose(window)) {


        //just clears the screen for rendering
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClearColor(0.4f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);



    //begin drawing models
        {
            shad_bind(model_shader);
            // GLERROR();

            //bind view matrix
            glUniformMatrix4fv(
                    model_view_loc,
                    1,
                    GL_FALSE,// column major order
                    camera.view
            );
            // GLERROR();
            //bind perspective matrix
            glUniformMatrix4fv(
                    model_perspective_loc,
                    1,
                    GL_FALSE,// column major order
                    camera.perspective
            );
            // GLERROR();

    // draw setting
            draw_all_model_instances(&setting.model_instances, model_matrix_loc);

    // draw the match
            draw_all_model_instances_with_mat(&kfg_match.model_instances, scale_match, model_matrix_loc);
        }

        //present the render to the window and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();

        // GLERROR();

        frames += 1;

    }

    //TODO: Delete models

    glfwDestroyWindow(window);
    glfwTerminate();
}