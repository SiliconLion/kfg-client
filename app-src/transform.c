#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include <GLFW/glfw3.h>
#endif

#include <stdbool.h>

#include "stlreader.h"
#include "geometry.h"
#include "shader.h"
#include "texture.h"
#include "transformation.h"

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


    //load the geometry, the shaders, and the texture
    Geometry * ball = geom_from_stl("assets/models/meshed_sphere.stl");
    Geometry * cube = geom_from_stl("assets/models/meshed_cube.stl");
    Shader * shad = shad_new("shaders/transform/vertex.vert", "shaders/transform/fragment.frag");
    Texture * cube_texture = tex_new("assets/matcap/normalsmatcap.png", true);
    Texture * ball_texture = tex_new("assets/matcap/clay_brown.png", true);

    //location of the transformation uniform
    unsigned int transform_loc = glGetUniformLocation(shad->program, "transformation");
    unsigned int scale_loc = glGetUniformLocation(shad->program, "scale");



    //will be used for rotation based on mouse movement. See more in render loop
    float xrot = 0;
    float yrot = 0;
    //the components of the previous mouse position. 
    //starting at the center so everything will be rotated relative to the center of 
    //the window. 
    float  prev_xpos = windowHeight_global / 2;
    float  prev_ypos = windowWidth_global / 2; 

    //the transformation chain used to keep track of the scale
    tran_chain scale = tran_chain_new();
    //the transformation chain used to collect all the transformations
    tran_chain trans = tran_chain_new();

    //bind the shader program
    shad_bind(shad);


    //the render loop
    while (!glfwWindowShouldClose(window)) {

        //just clears the screen for rendering
        // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

        

        //maintains a square aspect ratio when window isn't square
        //not sure it's 100% sound but works pretty well
        tran_chain_add(&scale, trans_new_scale(
                (float)windowHeight_global/(float)windowWidth_global,
                1.0f,
                1.0f
            ) 
        );
        //just adjusts the scale of the geometry to be half the size
        tran_chain_add(&scale, trans_new_scale(0.5, 0.5, 0.5));


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
            yrot += -1.0f * (curr_xpos - prev_xpos) / (windowWidth_global / 2);
            xrot += -1.0f * (curr_ypos - prev_ypos) / (windowHeight_global / 2);

            //
            prev_xpos = curr_xpos;
            prev_ypos = curr_ypos;

            //add the x and y rotations to the chain
            tran_chain_add(&trans, trans_new_y_rot(yrot) );
            tran_chain_add(&trans, trans_new_x_rot(xrot) );
        }

        //combine all the transformations into one and send to gpu. 
        trans_send_uniform(transform_loc, tran_chain_squash(&trans));
        trans_send_uniform(scale_loc, tran_chain_squash(&scale));
       
        //bind the texture and draw the geometry
        tex_bind(ball_texture, 0);
        geom_draw(ball);
        tex_bind(cube_texture, 0);
        geom_draw_wireframe(cube, 0.8);

        tran_chain_clear(&trans);
        tran_chain_clear(&scale);

        //present the render to the window and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();

        
    }      

    //standard cleanup code
    tran_chain_delete(&trans);
    tran_chain_delete(&scale);

    geom_delete(cube);
    geom_delete(ball);
    shad_delete(shad);
    tex_delete(ball_texture);
    tex_delete(cube_texture);

    glfwDestroyWindow(window);
    glfwTerminate();
}