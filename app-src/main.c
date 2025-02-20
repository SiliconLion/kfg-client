#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#endif

#include "omni-include.h"

// #include "../libs/3d-play/lib/omni-include.h"

#include <stdbool.h>
#include <string.h>

#include "camera.h"
#include "board.h"
#include "scene.h"
#include "record.h"


#include "error-handling.h"

#define CGLM_DEFINE_PRINTS
#include "cglm/cglm.h"

#include "SDL3/SDL.h"

#include <unistd.h>

typedef enum {
    KFG_KEY_UP = 0,
    KFG_KEY_DOWN = 1 
} KeyStates;

typedef struct  {
    int W, A, S, D, SPACE, SHIFT, I, J, U, V, Z, X, M, ESC, ENTER;
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
        "       M: %u,\n"
        "       ESC: %u,\n"
        "       ENTER: %u,\n"
        "}\n",
        kst->W, kst->A, kst->S, kst->D,
        kst->SPACE, kst->SHIFT,
        kst->I, kst->J, kst->U, kst->V,
        kst->Z, kst->X,
        kst->M,
        kst->ESC, kst->ENTER
    );
}


//global so callbacks can see them
int windowWidth_global, windowHeight_global;
f32 window_ratio_global;

u32 counter_global;

Camera camera;
KeyStateTracker key_states;
bool camera_mode_is_manual = false;

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

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){key_states.ESC = KFG_KEY_DOWN;}
    if(key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE){key_states.ESC = KFG_KEY_UP;}
    if(key == GLFW_KEY_ESCAPE && action == GLFW_REPEAT){ /*no op*/}
    
    if(key == GLFW_KEY_ENTER && action == GLFW_PRESS){key_states.ENTER = KFG_KEY_DOWN;}
    if(key == GLFW_KEY_ENTER && action == GLFW_RELEASE){key_states.ENTER = KFG_KEY_UP;}
    if(key == GLFW_KEY_ENTER && action == GLFW_REPEAT){ /*no op*/}
}


void main_update_camera(CameraControler* cc, KeyStateTracker* kst, GLFWwindow* window) {


    if(camera_mode_is_manual) {
    //Keyboard controls
        if(kst->W == KFG_KEY_DOWN) {cc_apply_action(cc, CA_MOVE_FORWARD, NULL);}
        if(kst->A == KFG_KEY_DOWN) {cc_apply_action(cc, CA_MOVE_LEFT, NULL);}
        if(kst->S == KFG_KEY_DOWN) {cc_apply_action(cc, CA_MOVE_BACKWARD, NULL);}
        if(kst->D == KFG_KEY_DOWN) {cc_apply_action(cc, CA_MOVE_RIGHT, NULL);}

        if(kst->SPACE == KFG_KEY_DOWN) {cc_apply_action(cc, CA_FLOAT, NULL);}
        if(kst->SHIFT == KFG_KEY_DOWN) {cc_apply_action(cc, CA_SINK, NULL);}

        if(kst->U == KFG_KEY_DOWN) {cc_apply_action(cc, CA_TILT_UP, NULL);}
        if(kst->V == KFG_KEY_DOWN) {cc_apply_action(cc, CA_TILT_DOWN, NULL);}
        if(kst->I == KFG_KEY_DOWN) {cc_apply_action(cc, CA_PAN_RIGHT, NULL);}
        if(kst->J == KFG_KEY_DOWN) {cc_apply_action(cc, CA_PAN_LEFT, NULL);}

        if(kst->Z == KFG_KEY_DOWN) {cc_apply_action(cc, CA_ZOOM_IN, NULL);}
        if(kst->X == KFG_KEY_DOWN) {cc_apply_action(cc, CA_ZOOM_OUT, NULL);}

    //Gamepad controls
        //this checks that `GLF_JOYSTICK_1` is present and is a game pad and gets its state. I guess there could be a problem if a different
        //joystick were connected that was not a gamepad but thats a problem for another time for hardware i dont have.

        GLFWgamepadstate state;
        if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state)) {
            // if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
            // {
            //     input_jump();
            // }
        
            // input_speed(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER]);

            float l_x_axis = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
            float abs_l_x = fabs(l_x_axis);
            if(l_x_axis < -.1) {cc_apply_action(cc, CA_MOVE_LEFT, &abs_l_x);} else
            if(l_x_axis > .1) {cc_apply_action(cc, CA_MOVE_RIGHT, &abs_l_x);}

            float l_y_axis = state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
            float abs_l_y = fabs(l_y_axis);
            if(l_y_axis < -.1) {cc_apply_action(cc, CA_MOVE_FORWARD, &abs_l_y);} else
            if(l_y_axis > .1) {cc_apply_action(cc, CA_MOVE_BACKWARD, &abs_l_y);}

            float r_x_axis = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
            float abs_r_x = fabs(r_x_axis);
            if(r_x_axis < -.1) {cc_apply_action(cc, CA_PAN_LEFT, &abs_r_x);} else
            if(r_x_axis > .1) {cc_apply_action(cc, CA_PAN_RIGHT, &abs_r_x);}

            float r_y_axis = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
            float abs_r_y = fabs(r_y_axis);
            if(r_y_axis < -.1) {cc_apply_action(cc, CA_TILT_UP, &abs_r_y);} else
            if(r_y_axis > .1) {cc_apply_action(cc, CA_TILT_DOWN, &abs_r_y);}

            if(state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] && state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER]) {/*do nothing*/} else 
            if(state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER]) {cc_apply_action(cc, CA_SINK, NULL);} else 
            if(state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER]) {cc_apply_action(cc, CA_FLOAT, NULL);}

            if(state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP] && state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN]) {/*do nothing*/} else 
            if(state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP]) {cc_apply_action(cc, CA_ZOOM_IN, NULL);} else 
            if(state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN]) {cc_apply_action(cc, CA_ZOOM_OUT, NULL);}

            // for(int i = 0; i < GLFW_GAMEPAD_BUTTON_LAST; i++) {
            //     if(state.buttons[i]) {printf("Gamepad button pressed: %u\n", i);}
            // }
            
        }


    } else {
        cc_apply_action(cc, CA_CIRCLE_TARGET_RIGHT, NULL);
    }

    if(kst->ENTER == KFG_KEY_DOWN){camera_print(cc->c);}
}


int main(int argc, const char* argv[]) {

    if(!SDL_Init(0)) {
        printf("oh geez. sdl couldn't initialize.");
    }

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

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

    //adding callbacks for
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, register_key_press);

    //to avoid screen tearing.
    glfwSwapInterval(1);


 

    



    vec3 XAXIS = {1.0f, 0.f, 0.f};
    vec3 YAXIS = {0.0f, 1.f, 0.f};
    vec3 ZAXIS = {0.0f, 0.f, 1.f};

    //initializes all keys to be 0 aka KFG_KEY_UP
    memset(&key_states, 0, sizeof(KeyStateTracker)); 


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

    const char* record_path = "assets/game-records/game_export";
    GameRecord record = game_record_new(record_path);
    // while(record.curr_line != NULL) {
    //     game_record_next_iter(&record);
    // }
    // printf("All lines in game record parsed");
    

    KFG_Match match = match_new();
    // game_record_apply_to_board(&record, &match);
    
    


    glDisable(GL_CULL_FACE);



    glm_vec3_copy((vec3){150, 165, 150}, camera.pos);
    glm_vec3_copy((vec3){0, 0, 0}, camera.target);
    camera.y_fov = 0.3;
    camera.near_plane = 1.00000; 
    camera.far_plane = 3000.000000;
    camera.aspect = window_ratio_global;

    camera_update(&camera);

    CameraControler cam_control = {
        .c = &camera,
        .forward_speed = 4, 
        .backward_speed = 4,
        .straif_speed = 4,
        .float_speed = 4,
        .tilt_speed = M_PI / 200.0,
        .pan_speed = M_PI / 200.0,
        .zoom_speed = 0.02
    };



    u64 frames = 0;

    // u32 stone_count = 0;

    // while(stone_count < BOARD_ROW_COUNT * BOARD_COL_COUNT) {
    //     if(rand() % 2 == 0) {
    //         match_add_stone(&match, BLACK_STONE, stone_count / BOARD_ROW_COUNT, stone_count % BOARD_ROW_COUNT, 0);
    //     } else {
    //         match_add_stone(&match, WHITE_STONE, stone_count / BOARD_ROW_COUNT, stone_count % BOARD_ROW_COUNT, 0);
    //     }

    //     stone_count += 1;
    // }

    
    unsigned int lastTime = 0, currentTime;
    
    


    //the render loop
    while (!glfwWindowShouldClose(window) && key_states.ESC != KFG_KEY_DOWN) {
        glfwPollEvents();
        // print_kst(&key_states);
        main_update_camera(&cam_control, &key_states, window);



        // Print a report once per second
        currentTime = SDL_GetTicks();
        if (currentTime > lastTime + 20) {
            // advance_match(&match, )
            game_record_next_iter(&record);
            game_record_apply_to_board(&record, &match);
            lastTime = currentTime;
        }



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

            match_draw(&match, model_matrix_loc);
        }

        //present the render to the window 
        glfwSwapBuffers(window);
        

        // GLERROR();

        frames += 1;
        usleep(1000/180);
    }

    //TODO: Delete models

    glfwDestroyWindow(window);
    glfwTerminate();
}