#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#endif

#include <stdbool.h>
#include <string.h>

//#include "linmath.h"

#include "omni-include.h"

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

#define CGLM_DEFINE_PRINTS
#include "cglm/cglm.h"


//global so callbacks can see them
int windowWidth_global, windowHeight_global;

//on a GLFW error, will print the error
void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

//updates the glViewport when the window is resized.
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    windowWidth_global = width;
    windowHeight_global = height;
}

//closes the application when escape key is pressed
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}


int main() {

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


    //adding callbacks for
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    //to avoid screen tearing.
    glfwSwapInterval(1);

    vec3 XAXIS = {1.0f, 0.f, 0.f};
    vec3 YAXIS = {0.0f, 1.f, 0.f};
    vec3 ZAXIS = {0.0f, 0.f, 1.f};




    FullGeometry floor_geom = prim_new_tex_rect_3d(GL_STATIC_DRAW);
    Texture* floor_tex = tex_new("assets/misc-textures/wood-floor-texture.jpg", false);
    Model floor = model_new(floor_geom, floor_tex);

    mat4 instance;
    glm_mat4_identity(instance);


    glm_mat4_identity(instance);
    glm_scale(instance, (vec3){20, 20, 20});
    glm_rotate(instance, M_PI_2, XAXIS);
    //floor is now a 100x100 plane in x/z, with y = 0.

    printf("floor model matrix: \n");
    mat4_print(instance);
    printf("\n");

    dynarr_push(&floor.model_instances, &instance);

    glm_mat4_identity(instance);//clears `instance` cuz we reuse it l8r



    Texture* wall_tex = tex_new("assets/misc-textures/wallpaper-texture.jpg", false);
    FullGeometry wall_geom = prim_new_tex_rect_3d(GL_STATIC_DRAW);
    Model wall = model_new(wall_geom, wall_tex);
    for(u32 i = 0; i < 8; i++) {
        glm_mat4_identity(instance);

        glm_translate(instance, (vec3) {0, 0, 10});
        glm_scale(instance, (vec3) {10, 10, 10});

        mat4 test;
        glm_mat4_identity(test);
        glm_rotate(test, M_PI_4 * i, YAXIS);
        printf("test matrix %u \n", i);
        mat4_print(test);
        printf("\n");

        glm_mat4_mul(test, instance, instance);

        dynarr_push(&wall.model_instances, &instance);
    }



    Shader* model_shader = shad_new("shaders/model/model.vert", "shaders/model/model.frag");
    GLERROR();
    u32 model_matrix_loc = glGetUniformLocation(model_shader->program, "model");
    u32 model_view_loc = glGetUniformLocation(model_shader->program, "view");
    u32 model_perspective_loc = glGetUniformLocation(model_shader->program, "perspective");
    GLERROR();

//TODO: the fact that we need to negate the y component is a bug
    vec3 camera_pos = {0.f, -15.f, 0.f};
    vec3 camera_look_at = {0.f};
    Camera camera = camera_new(
            camera_pos, camera_look_at,
            M_PI_4, // PI/4 rad = 45 degrees
            M_PI_4,
            0.1f,
            100.f
        );

    f32 camera_path_radius = 20;
    f32 camera_speed = (1.f / 60.f) * 0.5; //assuming 60fps, .5 meters/second
    //its bad assumptions, but good ballpark

    u64 frames = 0;

    glEnable(GL_CULL_FACE);

    //the render loop
    while (!glfwWindowShouldClose(window)) {

        f32 c_pos_x = camera_path_radius * sinf(camera_speed * frames);
        f32 c_pos_y = -15.f;
        f32 c_pos_z = camera_path_radius * cosf(camera_speed * frames);

        vec3 c_pos = {c_pos_x, c_pos_y, c_pos_z};

        glm_vec3_copy(c_pos, camera.pos);
        camera_update(&camera);


        //TODO: remove these (will need to modify the shader source)
        mat4 scale;
        glm_mat4_identity(scale);
        mat4 rotation;
        glm_mat4_identity(rotation);

        //just clears the screen for rendering
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClearColor(0.4f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);






//        //maintains a square aspect ratio when window isn't square
//        //not sure it's 100% sound but works pretty well
//        tran_chain_add(&scale, trans_new_scale(
//                               (float)windowHeight_global/(float)windowWidth_global,
//                               1.0f,
//                               1.0f
//                       )
//        );
//        mat4x4_scale_aniso(scale, scale,
//                           (float)windowHeight_global/(float)windowWidth_global,
//                           1.0f,
//                           1.0f
//                       );


//        f32 window_ratio = (float)windowHeight_global/(float)windowWidth_global;





    //begin drawing models
        {
            shad_bind(model_shader);
            GLERROR();

            glUniformMatrix4fv(
                    model_view_loc,
                    1,
                    GL_FALSE,// column major order
                    camera.view
            );
            GLERROR();
            glUniformMatrix4fv(
                    model_perspective_loc,
                    1,
                    GL_FALSE,// column major order
                    camera.perspective
            );
            GLERROR();

            //draw floor
            {
                glCullFace(GL_FRONT);
                model_draw_instances(&floor, model_matrix_loc);
                GLERROR();
            }

            //draw walls
            {
                glCullFace(GL_FRONT);
                model_draw_instances(&wall, model_matrix_loc);

            }
        }

        //present the render to the window and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();

        GLERROR();

        frames += 1;

    }

    //TODO: Delete models

    glfwDestroyWindow(window);
    glfwTerminate();
}