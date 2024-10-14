#include "board.h"

Board board_new(u32 row_count, u32 col_count) {
    Shader* board_shader = shad_new(
            "shaders/board/board.vert",
            "shaders/board/board.frag"
    );
    Shader* stone_shader = shad_new(
            "shaders/board/stones.vert",
            "shaders/board/stones.frag"
    );
    if(!board_shader || !stone_shader) {
        printf("shader creation failure in board creation");
        exit(-5);
    }

//    FullGeometry board_geometry = full_geom_from_stl(
//            "assets/models/go-board-basic.stl", GL_STATIC_DRAW);
    FullGeometry board_geometry = full_geom_from_stl(
            "assets/models/baby-yoda.stl", GL_STATIC_DRAW);
    FullGeometry stone_geometry = full_geom_from_stl(
            "assets/models/go-stone-basic1.stl", GL_STATIC_DRAW);

    dynarr stones = dynarr_new(sizeof(Stone), row_count * col_count);

    return (Board){
        .row_count = row_count,
        .col_count = col_count,
        .stones = stones,
        .board_shader = board_shader,
        .stones_shader = stone_shader,
        .board_geometry = board_geometry,
        .stone_geometry = stone_geometry
    };
}

void board_delete(Board* b) {
    shad_delete(b->board_shader);
    shad_delete(b->stones_shader);
    full_geom_delete(&b->board_geometry);
    full_geom_delete(&b->stone_geometry);
    dynarr_delete(&b->stones);
}