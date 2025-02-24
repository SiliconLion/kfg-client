#pragma once

#include "omni-include.h"
#include "dynarr.h"
#include "board.h"

#define MAXTIMER 60

typedef struct {
    i32 iter;  //not an iterator, but which iteration in the game record is being handled
    u32 id;
    enum StoneColor color;
    u32 row; 
    u32 col;
    u32 state;
    u32 timer;
} LogicalStone; //not the stone itself but tracks its properties

typedef struct {
    char* data;
    usize file_size;
    char* curr_line;
    u64 line_number;


    i32 cur_iter;  //not an iterator, but which iteration in the game record is being handled
    dynarr black_stones; //Vec<LogicalStone>
    dynarr white_stones; //Vec<LogicalStone>
} GameRecord; //ToDo: This is a really bad name. its not the 

GameRecord game_record_new(const char* path);
void game_record_next_iter(GameRecord* g);
void game_record_apply_to_board(GameRecord* g, KFG_Match* match);

