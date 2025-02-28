#define GL_SILENCE_DEPRECATION

//this basically includes opengl
#include "platform.h"
#include "omni-include.h"

#include <GLFW/glfw3.h>
#define CGLM_DEFINE_PRINTS
#include "cglm/cglm.h"
#include "SDL3/SDL.h"

#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include "camera.h"
#include "scene.h"
#include "framebuffer.h"
#include "primatives.h"
#include "shader.h"
#include "texture.h"
#include "error-handling.h"

#include "image.h"



typedef enum {
    KFG_KEY_UP = 0,
    KFG_KEY_DOWN = 1
} KeyStates;

typedef struct  {
    int W, A, S, D, SPACE, SHIFT, I, J, U, V, Z, X, L, M, ESC, ENTER;
} KeyStateTracker;

void print_kst(KeyStateTracker* kst) {
    printf(
            "KST {\n"
            "       W: %u,\n"
            "       A: %u,\n"
            "       S: %u,\n"
            "       D: %u,\n"
            "       SPACE: %u,\n"
            "       SHIFT: %u,\n"
            "       I: %u,\n"
            "       J: %u,\n"
            "       U: %u,\n"
            "       V: %u,\n"
            "       Z: %u,\n"
            "       X: %u,\n"
            "       L: %u,\n"
            "       M: %u,\n"
            "       ESC: %u,\n"
            "       ENTER: %u,\n"
            "}\n",
            kst->W, kst->A, kst->S, kst->D,
            kst->SPACE, kst->SHIFT,
            kst->I, kst->J, kst->U, kst->V,
            kst->Z, kst->X,
            kst->L, kst->M,
            kst->ESC, kst->ENTER
    );
}

typedef struct {
    Framebuffer A;
    Framebuffer B;
    Framebuffer* front;
    Framebuffer* back;
} Swapchain;

void swpchain_swap(Swapchain* c) {
    Framebuffer* temp = c->front;
    c->front = c->back;
    c->back = temp;
}

//global so callbacks can see them
int windowWidth_global, windowHeight_global;
f32 window_ratio_global;

KeyStateTracker key_states;
bool camera_mode_is_manual = false;
bool lightmode = false;

u32 const target_fps = 60;
u32 const target_frame_ms = 1000.f / (float) target_fps;

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


//    camera.aspect = window_ratio_global;
//    camera_update(&camera);

    printf("missing, must remake framebuffers on resize event.\n");
}


//ToDo: It is possible a lot of this code can be replaced by directly query-ing the state of different keys through glfw.
//However, my first attempt at that did not work reliably, and I belive key events were getting dropped, even with
//sticky keys enabled. Furthermore, Cannot totally rely on this call back by itself either because we dont usually care
//just if a key signals an event, we care about how long it is down or up in relation to real time.
void register_key_press(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if(key == GLFW_KEY_W && action == GLFW_PRESS){key_states.W = KFG_KEY_DOWN;}
    if(key == GLFW_KEY_W && action == GLFW_RELEASE){key_states.W = KFG_KEY_UP;}
    if(key == GLFW_KEY_W && action == GLFW_REPEAT){ /*no op*/}

    if(key == GLFW_KEY_A && action == GLFW_PRESS){key_states.A = KFG_KEY_DOWN;}
    if(key == GLFW_KEY_A && action == GLFW_RELEASE){key_states.A = KFG_KEY_UP;}
    if(key == GLFW_KEY_A && action == GLFW_REPEAT){ /*no op*/}

    if(key == GLFW_KEY_S && action == GLFW_PRESS){key_states.S = KFG_KEY_DOWN;}
    if(key == GLFW_KEY_S && action == GLFW_RELEASE){key_states.S = KFG_KEY_UP;}
    if(key == GLFW_KEY_S && action == GLFW_REPEAT){ /*no op*/}

    if(key == GLFW_KEY_D && action == GLFW_PRESS){key_states.D = KFG_KEY_DOWN;}
    if(key == GLFW_KEY_D && action == GLFW_RELEASE){key_states.D = KFG_KEY_UP;}
    if(key == GLFW_KEY_D && action == GLFW_REPEAT){ /*no op*/}


    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS){key_states.SPACE = KFG_KEY_DOWN;}
    if(key == GLFW_KEY_SPACE && action == GLFW_RELEASE){key_states.SPACE = KFG_KEY_UP;}
    if(key == GLFW_KEY_SPACE && action == GLFW_REPEAT){ /*no op*/}

    if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS){key_states.SHIFT = KFG_KEY_DOWN;}
    if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE){key_states.SHIFT = KFG_KEY_UP;}
    if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_REPEAT){ /*no op*/}

    //just ignoring right shift for now to keep things simple
    // if(key == GLFW_KEY_RIGHT_SHIFT && action == GLFW_PRESS){key_states.W = KFG_KEY_DOWN;}
    // if(key == GLFW_KEY_RIGHT_SHIFT && action == GLFW_RELEASE){key_states.W = KFG_KEY_UP;}
    // if(key == GLFW_KEY_RIGHT_SHIFT && action == GLFW_REPEAT){ /*no op*/}

    if(key == GLFW_KEY_U && action == GLFW_PRESS){key_states.U = KFG_KEY_DOWN;}
    if(key == GLFW_KEY_U && action == GLFW_RELEASE){key_states.U = KFG_KEY_UP;}
    if(key == GLFW_KEY_U && action == GLFW_REPEAT){ /*no op*/}

    if(key == GLFW_KEY_V && action == GLFW_PRESS){key_states.V = KFG_KEY_DOWN;}
    if(key == GLFW_KEY_V && action == GLFW_RELEASE){key_states.V = KFG_KEY_UP;}
    if(key == GLFW_KEY_V && action == GLFW_REPEAT){ /*no op*/}

    if(key == GLFW_KEY_I && action == GLFW_PRESS){key_states.I = KFG_KEY_DOWN;}
    if(key == GLFW_KEY_I && action == GLFW_RELEASE){key_states.I = KFG_KEY_UP;}
    if(key == GLFW_KEY_I && action == GLFW_REPEAT){ /*no op*/}

    if(key == GLFW_KEY_J && action == GLFW_PRESS){key_states.J = KFG_KEY_DOWN;}
    if(key == GLFW_KEY_J && action == GLFW_RELEASE){key_states.J = KFG_KEY_UP;}
    if(key == GLFW_KEY_J && action == GLFW_REPEAT){ /*no op*/}

    if(key == GLFW_KEY_Z && action == GLFW_PRESS){key_states.Z = KFG_KEY_DOWN;}
    if(key == GLFW_KEY_Z && action == GLFW_RELEASE){key_states.Z = KFG_KEY_UP;}
    if(key == GLFW_KEY_Z && action == GLFW_REPEAT){ /*no op*/}

    if(key == GLFW_KEY_X && action == GLFW_PRESS){key_states.X = KFG_KEY_DOWN;}
    if(key == GLFW_KEY_X && action == GLFW_RELEASE){key_states.X = KFG_KEY_UP;}
    if(key == GLFW_KEY_X && action == GLFW_REPEAT){ /*no op*/}

    if(key == GLFW_KEY_M && action == GLFW_PRESS){key_states.M = KFG_KEY_DOWN;}
    if(key == GLFW_KEY_M && action == GLFW_RELEASE){
        key_states.M = KFG_KEY_UP;
        camera_mode_is_manual = !camera_mode_is_manual;
    }
    if(key == GLFW_KEY_M && action == GLFW_REPEAT){ /*no op*/}

    if(key == GLFW_KEY_L && action == GLFW_PRESS){key_states.L = KFG_KEY_DOWN;}
    if(key == GLFW_KEY_L && action == GLFW_RELEASE){
        key_states.L = KFG_KEY_UP;
        lightmode = !lightmode;
    }
    if(key == GLFW_KEY_M && action == GLFW_REPEAT){ /*no op*/}

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){key_states.ESC = KFG_KEY_DOWN;}
    if(key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE){key_states.ESC = KFG_KEY_UP;}
    if(key == GLFW_KEY_ESCAPE && action == GLFW_REPEAT){ /*no op*/}

    if(key == GLFW_KEY_ENTER && action == GLFW_PRESS){key_states.ENTER = KFG_KEY_DOWN;}
    if(key == GLFW_KEY_ENTER && action == GLFW_RELEASE){key_states.ENTER = KFG_KEY_UP;}
    if(key == GLFW_KEY_ENTER && action == GLFW_REPEAT){ /*no op*/}
}

int main(int argc, const char* argv[]) {
//initialize systems
    srand(time(NULL));

    if(!SDL_Init(0)) {
        printf("oh geez. sdl couldn't initialize.");
    }

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

    GLFWwindow * window = glfwCreateWindow(1024, 1024, "Transform", NULL, NULL);
    if(!window) {
        return 2;
    }
    glfwMakeContextCurrent(window);

#ifndef __APPLE__ // the apple platform doesnt use glad.
    //Note that this is AFTER `glfwMakeContextCurrent(window);` but BEFORE any gl functions
    if (!gladLoadGL() ) {
        printf("Failed to initialize glad");
    }
#endif

    glfwGetFramebufferSize(window, &windowWidth_global, &windowHeight_global);
    glViewport(0, 0, windowWidth_global, windowHeight_global);

    window_ratio_global = (float)windowWidth_global / (float)windowHeight_global;

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

    //adding callbacks for
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, register_key_press);

    //to avoid screen tearing.
    glfwSwapInterval(1);








    vec3 XAXIS = {1.0f, 0.f, 0.f};
    vec3 YAXIS = {0.0f, 1.f, 0.f};
    vec3 ZAXIS = {0.0f, 0.f, 1.f};

    mat4 IDENTITY;
    glm_mat4_identity(IDENTITY);

    //initializes all keys to be 0 aka KFG_KEY_UP
    memset(&key_states, 0, sizeof(KeyStateTracker));


    //Generate assets
    random_png(1024, 1024, 1, "assets/random-one-channel.png");
    random_png(1024, 1024, 2, "assets/random-two-channel.png");
    random_png(1024, 1024, 3, "assets/random-three-channel.png");
    random_png(1024, 1024, 4, "assets/random-four-channel.png");



    //load textures
    Texture noise = tex_new("assets/random-three-channel.png");


    Swapchain chain;
    framebuffer_new(&chain.A, GL_RGBA, windowWidth_global, windowHeight_global);
    framebuffer_new(&chain.B, GL_RGBA, windowWidth_global, windowHeight_global);
    chain.front = &chain.A;
    chain.back = &chain.B;



//This is the stuff for rendering the contents of a framebuffer or texture to the screen
    //No framebuffer because the intention is to draw to the screen, aka the default framebuffer.
    Shader* screen_shader_no_effect = shad_new("shaders/postprocessing-pallet/screen.vert", "shaders/postprocessing-pallet/no-effect.frag");
    Shader* screen_shader_blur = shad_new("shaders/postprocessing-pallet/screen.vert", "shaders/postprocessing-pallet/blur.frag");
     Shader* screen_shader_sharpen = shad_new("shaders/postprocessing-pallet/screen.vert", "shaders/postprocessing-pallet/sharpen.frag");
    GLERROR();
    FullGeometry screen_rect = prim_new_tex_rect(GL_STATIC_DRAW);
    GLERROR();
//end screen setup stuff







//Game loop variable and metrics

    u64 frames = 0;
    u32 frame_start = 0, currentTime;

    u32 last_board_update_time = 0;
    u32 last_fps_update_time = 0;

    u32 const framespan = 100;
    u32 frame_lengths[100] = {0};


//Opengl settings
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);


    framebuffer_bind(chain.front);
        glClearColor(1.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    framebuffer_unbind();
    framebuffer_bind(chain.back);
        glClearColor(0.0, 1.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    framebuffer_unbind();


    //the render loop
    while (!glfwWindowShouldClose(window) && key_states.ESC != KFG_KEY_DOWN) {

        frame_start = SDL_GetTicks();
        glfwPollEvents();

        framebuffer_bind(chain.front);
            shad_bind(screen_shader_blur);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, chain.back->color_tex_id);

            tex_bind(&noise, 0);
            full_geom_draw(&screen_rect);
        framebuffer_unbind();

        swpchain_swap(&chain);

        framebuffer_bind(chain.front);

            shad_bind(screen_shader_blur);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, chain.back->color_tex_id);

            full_geom_draw(&screen_rect);
        framebuffer_unbind();

//        swpchain_swap(&chain);


        //draw to the screen
        framebuffer_unbind(); //binds the default framebuffer, aka the screen. (a little redundant but i like the clarity)
            glClearColor(0.0, 0.0, 1.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);
            shad_bind(screen_shader_no_effect);

            glActiveTexture(GL_TEXTURE0);
//            glBindTexture(GL_TEXTURE_2D, chain.back->color_tex_id);
            glBindTexture(GL_TEXTURE_2D, noise.id);

            full_geom_draw(&screen_rect);
            GLERROR();
        framebuffer_unbind();//again, redundant, but I like the clarity



        //present the render to the window
        glfwSwapBuffers(window);





        //calculate frame anylitics and framerate adjustment
        frames += 1;

        currentTime = SDL_GetTicks();
        u32 frame_length = currentTime - frame_start;

        frame_lengths[frames % framespan] = frame_length; //this will rollover and overwrite itself which is what we want.

        i32 sleep_dir = target_frame_ms - frame_length;
        if(sleep_dir > 0) {
            SDL_Delay(sleep_dir);
        }

    }


    //TODO: memory management lol

    glfwDestroyWindow(window);
    glfwTerminate();
}