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
#include "board.h"
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
f32 movement_speed = 10;

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


int main() {
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





    dynarr models = DoTheImportThing("assets/scenes/Sponza/glTF/Sponza.gltf");
    if(dynarr_is_zero(&models)) {
        printf("I weep, for dispite promises made, we could not import a gltf file\n");
    } else {
        printf("HUZZAH! Assimp imported a complex gltf file\n");
    }







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





    glDisable(GL_CULL_FACE);



    glm_vec3_copy((vec3){-14.915992, 104.215508, -0.615611}, camera.pos);
    glm_vec3_copy((vec3){-13.920774, 104.313194, -0.615634}, camera.target);
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

            
//             //draw walls
//             {
//                 model_draw_all_instances(&wall, model_matrix_loc);

//             }

//             //draw board
//             {
//                 model_draw_all_instances(&board, model_matrix_loc);
//             }

//             //draw stones
//             {
// //                model_draw_all_instances(&white_stones, model_matrix_loc);
// //                model_draw_all_instances(&black_stones_model, model_matrix_loc);
//                 model_draw_instance_list(
//                     &black_stones_model, black_stones.data, black_stones.len, model_matrix_loc
//                 );

//                 model_draw_instance_list(
//                     &white_stones_model, white_stones.data, white_stones.len, model_matrix_loc
//                 );
//             }




            // //draw all models
            for(usize i = 0; i < models.len; i++) {
                Model* m = (Model*)dynarr_at(&models, i);

                model_draw_instance(m, 0, model_matrix_loc);     
                // GLERROR();          
            }

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