#include "record.h"
#include <SDL3/SDL_iostream.h>
#include <stdlib.h>

//returns a pointer to the next line if there is one, and NULL if there is not
char* find_next_char(char* data, usize file_size, char* curr_pos, char token) {
    char* c = curr_pos;
    while(c < data + file_size) {
        if(*c == token) {
            return c;
        }
        c += 1;
    }
    return NULL;
}



GameRecord game_record_new(const char* path) {
    usize file_size;
    char* data = SDL_LoadFile(path, &file_size);
    if(!data) {printf("Could not read game record.\n"); exit(-1);}

    //skip the first line
    char* line_end = find_next_char(data, file_size, data, '\n');
    if(!line_end){
        printf("Could not read game record.\n");
        exit(-1);
    } 

    char* line_start = line_end + 1;
    if(line_start > data + file_size) {
        printf("Could not read game record.\n");
        exit(-1);
    }


    dynarr white_stones = dynarr_new(sizeof(LogicalStone), BOARD_COL_COUNT * BOARD_ROW_COUNT);
    dynarr black_stones = dynarr_new(sizeof(LogicalStone), BOARD_COL_COUNT * BOARD_ROW_COUNT);
    
    return (GameRecord){
        .data = data,
        .file_size = file_size,
        .curr_line = line_start, 
        .line_number = 1, // we skiped the 0th line

        .cur_iter = -1, //the records start at 0, but we havent started parsing it yet, so -1
        .black_stones = black_stones,
        .white_stones = white_stones
    };

}


void game_record_next_iter(GameRecord* g) {
    if(g->curr_line == NULL) {
        // printf("no further lines in game data\n");
        return;
    }

    dynarr_clear(&g->black_stones);
    dynarr_clear(&g->white_stones);
    g->cur_iter++;

    char *line_end, *token_start, *token_end;
    
    

    

    char* number_end;


    while(g->curr_line != NULL) {
    //set up parsing the first token
        line_end = find_next_char(g->data, g->file_size, g->curr_line, '\n');
        token_start = g->curr_line;
        token_end = find_next_char(g->data, g->file_size, token_start, ',');
        if(!token_end ||token_end > line_end ) {printf("Line formated incorrectly.\n"); exit(-1);}

    //read iter
        u64 iter = strtol(token_start, &number_end, 10);
        //this is how strtol reports an error.
        if(token_start == number_end ) {printf("could not read iter.\n"); exit(-1);}
    //this is the governing condition of the loop
        if(iter != g->cur_iter) {break;}

        //move to next token
        token_start = token_end + 1;
        token_end = find_next_char(g->data, g->file_size, token_start, ',');
        if(token_start > line_end || token_end > line_end) {printf("missing token.\n"); exit(-1);}

    //read stone_id
        u64 id = strtol(token_start, &number_end, 10);
        if(token_start == number_end) {printf("could not read id.\n"); exit(-1);}

        //move to next token
        token_start = token_end + 1;
        token_end = find_next_char(g->data, g->file_size, token_start, ',');
        if(token_start > line_end || token_end > line_end) {printf("missing token.\n"); exit(-1);}

    //read stone_color
        //copying the token into a null terminated string to check for equality.
        //calloc takes care of the '\0'
        char* color_token = calloc(token_end - token_start + 1, sizeof(char));
        memcpy(color_token, token_start, token_end - token_start);

        enum StoneColor color;
        if ( 0 == strcmp(color_token, "Black") ){ color = BLACK_STONE;} else 
        if ( 0 == strcmp(color_token, "White") ){ color = WHITE_STONE;} 
        else {printf("Cannot read stone color.\n"); exit(-1);}

        free(color_token);

        //move to next token
        token_start = token_end + 1;
        token_end = find_next_char(g->data, g->file_size, token_start, ',');
        if(token_start > line_end || token_end > line_end) {printf("missing token.\n"); exit(-1);}

    //read row
        u64 row = strtol(token_start, &number_end, 10);
        if(token_start == number_end ) {printf("could not read row.\n"); exit(-1);}

        //move to next token
        token_start = token_end + 1;
        token_end = find_next_char(g->data, g->file_size, token_start, ',');
        if(token_start > line_end || token_end > line_end) {printf("missing token.\n"); exit(-1);}

    //read col
        u64 col = strtol(token_start, &number_end, 10);
        if(token_start == number_end ) {printf("could not read col.\n"); exit(-1);}

        //move to next token
        token_start = token_end + 1;
        token_end = find_next_char(g->data, g->file_size, token_start, ',');
        if(token_start > line_end || token_end > line_end) {printf("missing token.\n"); exit(-1);}

    //read state
        u64 state = strtol(token_start, &number_end, 10);
        if(token_start == number_end ) {printf("could not read token.\n"); exit(-1);}

        //move to next token
        token_start = token_end + 1;
        token_end = line_end; //its the last token on the line. Cou.d maybe error check that to be the case but...

    //read timer
        u64 timer = strtol(token_start, &number_end, 10);
        if(token_start == number_end ) {printf("could not read token.\n"); exit(-1);}


    //next line
        if(line_end + 1 >= g->data + g->file_size) {g->curr_line = NULL;}
        else{ g->curr_line = line_end + 1;}

    //consolidate all the fields we read into a LogicalStone and add it to the GameRecord 
        LogicalStone stone = {
            .iter = iter,
            .id = id,
            .color = color,
            .row = row,
            .col = col,
            .state = state,
            .timer = timer
        };

        if(color == BLACK_STONE){dynarr_push(&g->black_stones, &stone);} 
        else                    {dynarr_push(&g->white_stones, &stone);}
    }

    return;
}



void game_record_apply_to_board(GameRecord* g, KFG_Match* match) {
    dynarr_clear(&match->black_stones);
    dynarr_clear(&match->white_stones);

    for(usize i = 0; i < g->black_stones.len; i++) {
        LogicalStone* stone = dynarr_at(&g->black_stones, i);

        f32 height = stone->timer == -1 ? 0 : (1./MAXTIMER) * stone->timer;
        match_add_stone(
            match, BLACK_STONE, stone->row, stone->col, height
        );
    }
    for(usize i = 0; i < g->white_stones.len; i++) {
        LogicalStone* stone = dynarr_at(&g->white_stones, i);
        f32 height = stone->timer == -1 ? 0 : (1./MAXTIMER) * stone->timer;
        match_add_stone(
             match, WHITE_STONE, stone->row, stone->col, height
        );
    }
}

