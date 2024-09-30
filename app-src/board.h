#pragma once

#include "omni-include.h"
#include "stdbool.h"

#include "shader.h"
#include "geometry.h"
#include "dynarr.h"
#include "scene.h"

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
    dynarr stones;
    Shader* board_shader;
    Shader* stones_shader; //might be a little weird that the board owns this,
    //but theres only one board and many stones, and this gets around either duplicating the shader
    //for each stone (bad idea), or using a more complex, multi-ownership strategy.

    FullGeometry board_geometry;
    FullGeometry stone_geometry; //there is only really one stone (geometry) loaded at once, we just draw
    //it over and over in all the right places and with the right colors + effects
} Board;


//typedef struct {
//    int move;
//    int tick;
//    u64 time; //# of seconds. Maybe theres a better way to track time but good enough for now.
//    Board b;
//} Game;


Board board_new(u32 row_count, u32 col_count);
//takes a new array of stones and deletes its old array of stones
void board_update(Board* b, dynarr stones);
//void board_draw(Board* b, Scene* s);
void board_delete(Board* b);

