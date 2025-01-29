#pragma once

#include "omni-include.h"
#include "stdbool.h"

#include "shader.h"
#include "geometry.h"
#include "dynarr.h"
// #include "scene.h"
#include "model.h"

#include <cglm/cglm.h>


typedef struct {
    u32 color; //unsigned int for graphics convenience, but just black (0) or white (0xFFFFFF)
    //more fields for the real-time "kung-foo" aspect of all of this

    //coordinates
    u32 row;
    u32 col;
    float height; //[0...1]
    bool locked;
} Stone;

typedef struct {
    u32 row_count;
    u32 col_count;
    dynarr stones; //Vec<Stone>
    // Shader* board_shader;
    // Shader* stones_shader; 

    FullGeometry stone_geom;
    FullGeometry board_geom;

    Texture* board_difuse_tex;
    Texture* black_stone_tex;
    Texture* white_stone_tex;

    ModelPrototype board_prototype;
    ModelPrototype stone_prototype;

    mat4 world_transform; //Where the board is in the world
} Board;

#define BOARD_HEIGHT 2.0


//typedef struct {
//    int move;
//    int tick;
//    u64 time; //# of seconds. Maybe theres a better way to track time but good enough for now.
//    Board b;
//} Game;


Board board_new(u32 row_count, u32 col_count);
//takes a new array of stones and deletes its old array of stones
void board_update(Board* b, dynarr stones);
// void board_delete(Board* b);

void board_draw(Board* b, u32 world_transform_loc);

