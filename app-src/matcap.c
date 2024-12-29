#include "platform.h"

INCLUDE_OPENGL
    #include <GLFW/glfw3.h>

#include <stdbool.h>

#include "stlreader.h"
#include "geometry.h"
#include "shader.h"
#include "texture.h"



struct {
    float x_offset;
    float y_offset;
    float x_rotation;
    float y_rotation;
} globalstate = {0.0, 0.0, 0.0, 0.0};



//updates the glViewport when the window is resized.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
} 

//nothing special, just processes input. its a function to keep things nice.
void processInput(GLFWwindow *window)
{
    
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        globalstate.x_offset += 0.01;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        globalstate.x_offset -= 0.01;
    }
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        globalstate.y_offset += 0.01;
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        globalstate.y_offset -= 0.01;
    }

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    //this feels backwards, but remember we're rotating along axis, so its right.
    globalstate.x_rotation = 2 * (ypos / width) - 1 ;
    globalstate.y_rotation = 2 * (xpos / height) - 1;

}

int main(int argc, char *argv[])
{
    
    const char*  model_path;
    if (argc == 2) {
        model_path = argv[1];
    } else {
        model_path = "assets/models/mushu.stl";
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //needed for mac
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "3D Play :D", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glViewport(0, 0, 800, 600);
    glfwMakeContextCurrent(window);

    //adds the callback we defined above to a change in framebuffer size
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);





    Geometry * geom = geom_from_stl(model_path);

    Shader * shad = shad_new("shaders/matcap/vertex.vert", "shaders/matcap/fragment.frag");

    Texture * texture = tex_new("assets/matcap/clay_brown.png", true);

    printf("verticies count: %i\n", geom->verticies_count);


    
    unsigned int x_offset_loc   = glGetUniformLocation(shad->program, "x_offset");
    unsigned int y_offset_loc   = glGetUniformLocation(shad->program, "y_offset");
    unsigned int x_rotation_loc = glGetUniformLocation(shad->program, "x_rotation");
    unsigned int y_rotation_loc = glGetUniformLocation(shad->program, "y_rotation");

    unsigned int matcap_loc     = glGetUniformLocation(shad->program, "matcap");
    glUniform1i(matcap_loc, 0);

    shad_bind(shad);
    while(!glfwWindowShouldClose(window)) {

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

        // angle += 0.01;
        glUniform1f(x_offset_loc  , globalstate.x_offset  );
        glUniform1f(y_offset_loc  , globalstate.y_offset  );
        glUniform1f(x_rotation_loc, globalstate.x_rotation);
        glUniform1f(y_rotation_loc, globalstate.y_rotation);

        tex_bind(texture, 0);

        geom_draw(geom);

        glfwSwapBuffers(window);
        
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;


}



