#pragma once

#include "omni-include.h"
#include "stdbool.h"

#include "shader.h"
#include "geometry.h"
#include "dynarr.h"
// #include "scene.h"
#include "model.h"
#include "scene.h"

#include <cglm/cglm.h>

#define BOARD_ROW_COUNT 19
#define BOARD_COL_COUNT 19

// #define BOARD_ROW_COUNT 5
// #define BOARD_COL_COUNT 5

#define BOARD_SCALE 50.0f

enum StoneColor {WHITE_STONE, BLACK_STONE};

typedef struct {
    Scene s;
    ModelPrototype* board_proto;
    ModelPrototype* white_stone_proto;
    ModelPrototype* black_stone_proto;

    ModelInstance board_inst; 
    dynarr white_stones; //Vec<ModelInstance>
    dynarr black_stones; //Vec<ModelInstance>

    mat4 scale;
} KFG_Match;

KFG_Match match_new();

void match_add_stone(KFG_Match* m, enum StoneColor color, u32 row, u32 col, f32 height);

void match_clear_stones(KFG_Match* m);

void match_draw(KFG_Match* m, u32 world_transform_loc);