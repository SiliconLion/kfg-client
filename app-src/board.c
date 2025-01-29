#include "board.h"

void stone_to_mat(mat4** ret, Stone* s, u32 row_count, u32 col_count) {
    float row_width = 1.f / (float) row_count;
    float col_width = 1.f / (float) col_count;

    float x = s->row * row_width + (.5 * row_width); 
    float y = s->col * col_width + (.5 * col_width); 
    float z = s->height + BOARD_HEIGHT;

    float v[3] = {x, y, z};

    glm_translate(*ret, v);
}

Board board_new(u32 row_count, u32 col_count) {
    // Shader* board_shader = shad_new(
    //         "shaders/board/board.vert",
    //         "shaders/board/board.frag"
    // );
    // Shader* stone_shader = shad_new(
    //         "shaders/board/stones.vert",
    //         "shaders/board/stones.frag"
    // );
    // if(!board_shader || !stone_shader) {
    //     printf("shader creation failure in board creation");
    //     exit(-5);
    // }

   FullGeometry board_geometry = full_geom_from_stl(
           "assets/models/go-board-basic.stl", GL_STATIC_DRAW);
    // FullGeometry board_geometry = full_geom_from_stl(
    //         "assets/models/baby-yoda.stl", GL_STATIC_DRAW);
   FullGeometry stone_geometry = full_geom_from_stl(
            "assets/models/go-stone-basic1.stl", GL_STATIC_DRAW);

//    ModelPrototype board_prototype = {
//     .geom = board_geometry,
//    }




    dynarr stones = dynarr_new(sizeof(Stone), row_count * col_count);

    return (Board){
        .row_count = row_count,
        .col_count = col_count,
        .stones = stones,
        // .board_shader = board_shader,
        // .stones_shader = stone_shader,

    };
}

// void board_delete(Board* b) {
//     shad_delete(b->board_shader);
//     shad_delete(b->stones_shader);
//     full_geom_delete(&b->board_geometry);
//     full_geom_delete(&b->stone_geometry);
//     dynarr_delete(&b->stones);
// }

// void board_draw(Board* b, u32 world_transform_loc) {
//     draw_model_from_mat(b->board_prototype, b->world_transform, world_transform_loc);

//     for(usize i = 0; i < b->stones.len; i++) {
//         Stone* s = dynarr_get(&b->stones, i);
//         mat4 stone_matrix;
//         stone_to_mat(&stone_matrix, s, b->row_count, b->col_count);

//         mat4 stone_to_world;
//         glm_mat4_mul(b->world_transform, stone_matrix, stone_to_world);

//         draw_model_from_mat(b->stone_prototype, stone_to_world, world_transform_loc);
//     }

// }