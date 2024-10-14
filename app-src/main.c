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
//#include "transformation.h"
#include "camera.h"
#include "board.h"
#include "error-handling.h"

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

//    unsigned int transform_loc = glGetUniformLocation(board.board_shader->program, "transformation");
//    unsigned int scale_loc = glGetUniformLocation(board.board_shader->program, "scale");
//    unsigned int perspective_loc = glGetUniformLocation(board.board_shader->program, "perspective");


    vec3 camera_pos = {100.f};
    vec3 camera_look_at = {0.f};
    Camera camera = camera_new(
            camera_pos, camera_look_at,
            M_PI_4, // PI/4 rad = 45 degrees
            M_PI_4,
            -0.1f,
            -100.f
        );




    //will be used for rotation based on mouse movement. See more in render loop
    float xrot = 0;
    float yrot = 0;
    //the components of the previous mouse position.
    //starting at the center so everything will be rotated relative to the center of
    //the window.
    float  prev_xpos = windowHeight_global / 2;
    float  prev_ypos = windowWidth_global / 2;

//    //the transformation chain used to keep track of the scale
//    tran_chain scale = tran_chain_new();
//    //the transformation chain used to collect all the transformations
//    tran_chain trans = tran_chain_new();





    //bind the shader program
    shad_bind(board.board_shader);


    //the render loop
    while (!glfwWindowShouldClose(window)) {

        mat4x4 IDENTITY;
        mat4x4_identity(IDENTITY);
        mat4x4 scale;
        mat4x4_dup(scale, IDENTITY);
        mat4x4 rotation;
        mat4x4_dup(rotation, IDENTITY);

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
        mat4x4_scale_aniso(scale, scale,
                           (float)windowHeight_global/(float)windowWidth_global,
                           1.0f,
                           1.0f
                       );

//        //just adjusts the scale of the geometry to be half the size
//        tran_chain_add(&scale, trans_new_scale(0.5, 0.5, 0.5));

        mat4x4_scale(scale, scale, 0.5f);


        //here we're using the mouse movement to generate a rotation for the geometry
        //(the block is purely aesthetic to visually seperate this calculation from the rest
        //of the render code)
        {
            //stores the current cursor position
            double curr_xpos, curr_ypos;
            glfwGetCursorPos(window, &curr_xpos, &curr_ypos);

            //This is basically the mouse movement delta, but normalized so it fits
            //the openGL coordinates better.
            //A movement along the x axis corresponds to a rotation around the y axis and visaversa.
//            yrot += -1.0f * (curr_xpos - prev_xpos) / (windowWidth_global / 2);
//            xrot += -1.0f * (curr_ypos - prev_ypos) / (windowHeight_global / 2);

            yrot +=  (curr_xpos - prev_xpos) / (windowWidth_global / 2);
            xrot +=  (curr_ypos - prev_ypos) / (windowHeight_global / 2);

            prev_xpos = curr_xpos;
            prev_ypos = curr_ypos;

//            camera.look_at_point[0] = xrot * 500000.f ;
//            camera.look_at_point[1] = yrot * 500000.f;
            camera.pos[0] = xrot ;
            camera.pos[1] = yrot ;
            camera_update(&camera);

        }


        //draw board
        {
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
                    camera.camera_transform
            );
            GLERROR();
            glUniformMatrix4fv(
                    perspective_loc,
                    1,
                    GL_FALSE,// column major order
                    camera.perspective_transform
            );
            GLERROR();

            tex_bind(board_texture, 0);
            GLERROR();
            full_geom_draw(&board.board_geometry);
            GLERROR();
        }

//        //draw stones
//        {
//            shad_bind(board.stones_shader);
//            GLERROR();
//
////            //combine all the transformations into one and send to gpu.
////            trans_send_uniform(transform_loc, tran_chain_squash(&trans));
////            trans_send_uniform(scale_loc, tran_chain_squash(&scale));
//
//            glUniformMatrix4fv(
//                    scale_loc,
//                    1,
//                    //because it is in row major order, we set transpose to true.
//                    GL_TRUE,
//                    scale
//            );
//            GLERROR();
//
////            glUniformMatrix4fv(
////                    transform_loc,
////                    1,
////                    //because it is in row major order, we set transpose to true.
////                    GL_TRUE,
////                    rotation
////            );
//            glUniformMatrix4fv(
//                    transform_loc,
//                    1,
//                    GL_FALSE,// column major order
//                    camera.camera_transform
//            );
//            GLERROR();
//            glUniformMatrix4fv(
//                    perspective_loc,
//                    1,
//                    GL_FALSE,// column major order
//                    camera.perspective_transform
//            );
//            GLERROR();
//
//            tex_bind(board_texture, 0);
//            full_geom_draw(&board.stone_geometry);
//        }




//        tran_chain_clear(&trans);
//        tran_chain_clear(&scale);

        //present the render to the window and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();

        GLERROR();

    }

    //standard cleanup code
//    tran_chain_delete(&trans);
//    tran_chain_delete(&scale);

//    geom_delete(cube);
//    geom_delete(ball);
//    shad_delete(shad);
//    tex_delete(ball_texture);
//    tex_delete(cube_texture);

    board_delete(&board);

    glfwDestroyWindow(window);
    glfwTerminate();
}