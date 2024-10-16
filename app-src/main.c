#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#endif

#include <stdbool.h>

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


    Board board = board_new(19, 19);
    Texture* board_texture = tex_new("assets/matcap/normalsmatcap.png", false);


    //location of the transformation uniform
    unsigned int transform_loc = glGetUniformLocation(board.board_shader->program, "transformation");
    unsigned int scale_loc = glGetUniformLocation(board.board_shader->program, "scale");
    unsigned int perspective_loc = glGetUniformLocation(board.board_shader->program, "perspective");


    vec3 XAXIS = {1.0f, 0.f, 0.f};
    vec3 YAXIS = {0.0f, 1.f, 0.f};
    vec3 ZAXIS = {0.0f, 0.f, 1.f};



    Model floor;
    floor.geom = prim_new_tex_rect_3d(GL_STATIC_DRAW);
    floor.tex = tex_new("assets/misc-textures/wood-floor-texture.jpg", false);

    glm_mat4_identity(floor.model_matrix);
    glm_scale(floor.model_matrix, (vec3){20, 20, 20});
    glm_rotate(floor.model_matrix, M_PI_2, XAXIS);
    //floor is now a 100x100 plane in x/z, with y = 0.

    printf("floor model matrix: \n");
    mat4_print(floor.model_matrix);
    printf("\n");



    Model wall[8];
    Texture* wall_tex = tex_new("assets/misc-textures/wallpaper-texture.jpg", false);
    for(u32 i = 0; i < 8; i++) {
        wall[i].geom = prim_new_tex_rect_3d(GL_STATIC_DRAW);
        wall[i].tex = wall_tex;

        glm_mat4_identity(wall[i].model_matrix);
        glm_translate(wall[i].model_matrix, (vec3) {0, 0, 10});
//        glm_rotate_at(
//                wall[i].model_matrix,
//                (vec3){0, 0, 0},
//                M_PI_2 * i,
//                YAXIS);
//        glm_rotate(wall[i].model_matrix, M_PI_2 * i, YAXIS);

        glm_scale(wall[i].model_matrix, (vec3) {10, 10, 10});
//        printf("wall model matrix %u: \n", i);
//        mat4_print(wall[i].model_matrix);
//        printf("\n");


        mat4 test;
        glm_mat4_identity(test);
        glm_rotate(test, M_PI_4 * i, YAXIS);
        printf("test matrix %u \n", i);
        mat4_print(test);
        printf("\n");

        glm_mat4_mul(test, wall[i].model_matrix, wall[i].model_matrix);
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


    //bind the shader program
    shad_bind(board.board_shader);


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



        //draw board
        {
            glCullFace(GL_BACK);

            shad_bind(board.board_shader);
            GLERROR();

            //combine all the transformations into one and send to gpu.
//            trans_send_uniform(transform_loc, tran_chain_squash(&trans));
//            trans_send_uniform(scale_loc, tran_chain_squash(&scale));

            glUniformMatrix4fv(
                    scale_loc,
                    1,
                    GL_FALSE, //column major order
                    scale
            );
            GLERROR();

//            glUniformMatrix4fv(
//                    transform_loc,
//                    1,
//                    GL_FALSE,// column major order
//                    rotation
//            );
            glUniformMatrix4fv(
                    transform_loc,
                    1,
                    GL_FALSE,// column major order
                    camera.view
            );
            GLERROR();
            glUniformMatrix4fv(
                    perspective_loc,
                    1,
                    GL_FALSE,// column major order
                    camera.perspective
            );
            GLERROR();

            tex_bind(board_texture, 0);
            GLERROR();
            full_geom_draw(&board.board_geometry);
            GLERROR();
        }

        //draw stones
        {
//            shad_bind(board.stones_shader);
            GLERROR();

//            //combine all the transformations into one and send to gpu.
//            trans_send_uniform(transform_loc, tran_chain_squash(&trans));
//            trans_send_uniform(scale_loc, tran_chain_squash(&scale));

            glUniformMatrix4fv(
                    scale_loc,
                    1,
                    //because it is in row major order, we set transpose to true.
                    GL_TRUE,
                    scale
            );
            GLERROR();

//            glUniformMatrix4fv(
//                    transform_loc,
//                    1,
//                    //because it is in row major order, we set transpose to true.
//                    GL_TRUE,
//                    rotation
//            );
            glUniformMatrix4fv(
                    transform_loc,
                    1,
                    GL_FALSE,// column major order
                    camera.view
            );
            GLERROR();
            glUniformMatrix4fv(
                    perspective_loc,
                    1,
                    GL_FALSE,// column major order
                    camera.perspective
            );
            GLERROR();

            tex_bind(board_texture, 0);
            full_geom_draw(&board.stone_geometry);
        }


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
                model_draw(&floor, model_matrix_loc);
                GLERROR();
            }

            //draw walls
            {
                glCullFace(GL_FRONT);
                for(int i = 0; i < 8; i++) {
//                    glm_translate(wall.model_matrix,
//                      (vec3){
//                            sinf(M_PI_2 * i),
//                            0,
//                            cosf(M_PI_2 * i)
//                        }
//                    );
//                    glm_rotate_at(
//                            wall.model_matrix,
//                            (vec3){0, 0, 0},
//                            M_PI_4 * i,
//                            XAXIS);
                    model_draw(&wall[i], model_matrix_loc);
                }

            }
        }

        //present the render to the window and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();

        GLERROR();

        frames += 1;

    }


    board_delete(&board);

    glfwDestroyWindow(window);
    glfwTerminate();
}