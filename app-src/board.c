#include "board.h"

KFG_Match match_new() {
    KFG_Match match;
    if(!import_scene(&match.s, "assets/scenes/go-board/go-board.gltf", true)) {
        printf("error, could not import go match scene.\n");
    }

    dynarr_clear(&match.s.model_instances);

    match.black_stone_proto = dynarr_at(&match.s.model_prototypes, 0);
    match.white_stone_proto = dynarr_at(&match.s.model_prototypes, 1);
    match.board_proto       = dynarr_at(&match.s.model_prototypes, 2);

    full_geom_normalize_verts_to(match.black_stone_proto->geom, 1.f/(BOARD_ROW_COUNT + 2));
    full_geom_normalize_verts_to(match.white_stone_proto->geom, 1.f/(BOARD_ROW_COUNT + 2));
    full_geom_normalize_verts_to(match.board_proto      ->geom, 1.f);



    match.black_stones = dynarr_new(sizeof(ModelInstance), BOARD_ROW_COUNT * BOARD_COL_COUNT);
    match.white_stones = dynarr_new(sizeof(ModelInstance), BOARD_ROW_COUNT * BOARD_COL_COUNT);

    match.board_inst.prototype = match.board_proto;
    glm_mat4_identity(match.board_inst.world_transform);

    glm_scale_make(match.scale, (vec3){BOARD_SCALE, BOARD_SCALE, BOARD_SCALE});
    // glm_mat4_identity(match.scale);


    return match;
}

void match_add_stone(KFG_Match* m, enum StoneColor color, u32 row, u32 col, f32 height) {

    //this code thinks of the board as being 1x1 centered on (0,0). so spanning -.5 to .5 in x and z respectively.

    //the distance between each grid square
    //the +2 gives space for an empty row/col around the whole board.s
    float delta_x = 1.f/ (float)(BOARD_ROW_COUNT + 2);
    float delta_z = 1.f/ (float)(BOARD_COL_COUNT + 2);

    //the edges of the board (could maybe be called "left/right and top/bottom" but i didn't want 
    //to figure out which was which lmao)
    float x_start = -.5f;
    float z_start = -.5f;


    float relative_x = delta_x * (row + 1) + x_start;
    float relative_z = delta_z * (col + 1) + z_start;

    float x = relative_x;
    float z = relative_z;

    // float x = relative_x;
    // float z = relative_z;

    ModelInstance inst;
    glm_mat4_identity(inst.world_transform);
    //ToDo: the .15 is a magic value that might not work with board sizes other than 19x19
    glm_translate_to(inst.world_transform, (vec3){x, .15+ height, z}, inst.world_transform);

    if(color == BLACK_STONE) {
        inst.prototype = m->black_stone_proto;
        dynarr_push(&m->black_stones, &inst);
    } else {
        inst.prototype = m->white_stone_proto;
        dynarr_push(&m->white_stones, &inst);
    }
}

void match_draw(KFG_Match* m, u32 world_transform_loc) {
    draw_model_instance_with_mat(&m->board_inst, m->scale, world_transform_loc);
    draw_all_model_instances_with_mat(&m->black_stones, m->scale, world_transform_loc);
    draw_all_model_instances_with_mat(&m->white_stones, m->scale, world_transform_loc);
}