#include "primatives.h"
#include <string.h>

//uses TwoPoint vertex type. Counter-clockwise winding direction
FullGeometry prim_new_rect(GLenum usage) {
    dynarr vertices = dynarr_new(sizeof(TwoPoint), 4);
    dynarr indices = dynarr_new(sizeof(INDEX_TYPE), 6);

    TwoPoint vertices_data [4] = {
            {-1.0f, 1.0f}, {1.0f, 1.0f},
            {-1.0f, -1.0f}, {1.0f, -1.0f}
    };

    INDEX_TYPE indices_data [6] = {
            1, 0, 2,
            1, 2, 3
    };

    dynarr_append_slice(&vertices, vertices_data, 4);
    dynarr_append_slice(&indices, indices_data, 6);

    FullGeometry g = full_geom_new(
            TwoPointBlueprint, sizeof(TwoPoint), vertices, indices, GL_TRIANGLES, usage
    );
    return g;
    //no need to free vertices_data or indices_data because they are stack allocated.
    //no need to delete vertices or indices because their ownership is passed into g
}

//uses TwoTexPoint vertex type. Counter-clockwise winding direction
FullGeometry prim_new_tex_rect(GLenum usage) {
    dynarr vertices = dynarr_new(sizeof(TwoTexPoint), 4);
    dynarr indices = dynarr_new(sizeof(INDEX_TYPE), 6);

    TwoTexPoint vertices_data [4] = {
            {{-1.0f, 1.0f}, {0.f, 1.f} }, {{1.0f, 1.0f}, {1.f, 1.f}},
            {{-1.0f, -1.0f}, {0.f, 0.f}}, {{1.0f, -1.0f}, {1.f, 0.f}}
    };

//    //this is sometimes helpful for debugging
//    for(int i = 0; i < 4; i++) {
//        TwoTexPoint* vert = vertices_data + i;
//        vert->pos[0] *= 0.9;
//        vert->pos[1] *= 0.9;
//    }

    INDEX_TYPE indices_data [6] = {
            1, 0, 2,
            1, 2, 3
    };

    dynarr_append_slice(&vertices, vertices_data, 4);
    dynarr_append_slice(&indices, indices_data, 6);

    FullGeometry g = full_geom_new(
            TwoTexPointBlueprint, sizeof(TwoTexPoint), vertices, indices, GL_TRIANGLES, usage
    );
    return g;
    //no need to free vertices_data or indices_data because they are stack allocated.
    //no need to delete vertices or indices because their ownership is passed into g
}

FullGeometry prim_new_tex_rect_3d(GLenum usage) {
    dynarr vertices = dynarr_new(sizeof(ThreeTexPoint), 4);
    dynarr indices = dynarr_new(sizeof(INDEX_TYPE), 6);

    ThreeTexPoint vertices_data [4] = {
            {{-1.0f, 1.0f, 0.f}, {0.f, 1.f} },
            {{1.0f, 1.0f, 0.f}, {1.f, 1.f}},
            {{-1.0f, -1.0f, 0.f}, {0.f, 0.f}},
            {{1.0f, -1.0f, 0.f}, {1.f, 0.f}}
    };

//    //this is sometimes helpful for debugging
//    for(int i = 0; i < 4; i++) {
//        TwoTexPoint* vert = vertices_data + i;
//        vert->pos[0] *= 0.9;
//        vert->pos[1] *= 0.9;
//    }

    INDEX_TYPE indices_data [6] = {
            1, 0, 2,
            1, 2, 3
    };

    dynarr_append_slice(&vertices, vertices_data, 4);
    dynarr_append_slice(&indices, indices_data, 6);

    FullGeometry g = full_geom_new(
            ThreeTexPointBlueprint, sizeof(ThreeTexPoint), vertices, indices, GL_TRIANGLES, usage
    );
    return g;
    //no need to free vertices_data or indices_data because they are stack allocated.
    //no need to delete vertices or indices because their ownership is passed into g
}


FullGeometry prim_new_tex_cube(GLenum usage) {

    //TODO, there are duplicates that can be reduced in the vertices
    dynarr vertices = dynarr_new(sizeof(ThreeTexPoint), 36);
    dynarr indices = dynarr_new(sizeof(INDEX_TYPE), 36);

    float vertices_data[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    dynarr_append_slice(&vertices, vertices_data, 36);

    //indices becomes [0..36)
    for(int i = 0; i< 36; i++) {
        dynarr_push(&indices, &i);
    }

    FullGeometry g = full_geom_new(
            ThreeTexPointBlueprint, sizeof(ThreeTexPoint), vertices, indices, GL_TRIANGLES, usage
    );
    return g;
    //no need to free vertices_data or indices_data because they are stack allocated.
    //no need to delete vertices or indices because their ownership is passed into g
}


//TODO: Maybe move this into a new file. It's not really a "primitive" is it?

//Returns a Geometry that is a flat triangular mesh with dimensions width, height. 
//the mesh is divided into rectangles 
//with w_resolution number of rectangles across the width, and 
//h_resolution number of rectangles across the height.
//then each rectangle has it's diagonal connected to make them tris. 
//Orientation is along the x and y plane
//usage is a paramater passed to the geometry constructor telling it how it's vertex buffer will be used.
//see Geometry in geometry.h to see whats allowed. 
Geometry * prim_new_plane(float width, float height, int w_resolution, int h_resolution, GLenum usage) {
 
    int rect_count = w_resolution * h_resolution;

    //the grid dimensions of the vertices. Just clustring the logically related variables.
    //total number of points in the grid, the number of points width-wise, and height-wise
    struct {
        int total_count;
        int w_count;
        int h_count;
    } griddims = {
        (w_resolution +1)  * (h_resolution +1),
        (w_resolution +1),
        (h_resolution +1)
    };
    
    float w_start = 0.0 - (width / 2);
    float h_start = 0.0 - (height / 2);

    float w_step = width / (float)w_resolution;
    float h_step = height / (float)h_resolution;

    //formatted such that 3 consecutive floats form an (x,y,z) coordinate
    float * positions = calloc(griddims.total_count, sizeof(float) * 3);
    //pointer used to walk along positions
    float * pos_walker = positions;

    for(int row = 0; row < griddims.h_count; row++) {
        for(int col = 0; col < griddims.w_count; col++) {
            *pos_walker = w_start + ((float)col * w_step); 
            pos_walker++;
            *pos_walker = h_start + ((float)row * h_step);
            pos_walker++;
            *pos_walker = 0.0;
            pos_walker++;
        }
    }

    int vertex_count = rect_count * 2 * 3;

    //the order in which we will copy the positions into verticies in order to make triangles.
    //if in the future Geometry supports indexing, may want to change this to an `unsigned int *`. 
    int * indicies = calloc(vertex_count, sizeof(int));
    //pointer used to walk along indicies
    int * ind_walker = indicies;

    
    //steps over each row and column except the last of each, and generates indicies of 2 triangles 
    //starting at the bottom left and top right respectively, with counter clockwise winding. 
    for(int row = 0; row < griddims.h_count - 1; row++) {
        for(int col = 0; col < griddims.w_count - 1; col++) {
            //first tri
            *ind_walker = (int)util_flatten_index(row, col, griddims.w_count);
            ind_walker++;
            *ind_walker = (int)util_flatten_index(row, col + 1, griddims.w_count);
            ind_walker++;
            *ind_walker = (int)util_flatten_index(row + 1, col + 1, griddims.w_count);
            ind_walker++;

            //second tri
            *ind_walker = (int)util_flatten_index(row + 1, col + 1, griddims.w_count);
            ind_walker++;
            *ind_walker = (int)util_flatten_index(row +1, col, griddims.w_count);
            ind_walker++;
            *ind_walker = (int)util_flatten_index(row, col, griddims.w_count);
            ind_walker++;
        }
    }

    //should probably assert that VERTEX_SIZE = sizeof(float) * 6
    //properly formatted verticies for Geometry
    float * verticies = calloc(vertex_count, VERTEX_SIZE);
    //pointer used to walk along verticies
    float * vert_walker = verticies;

    //since all the tris are coplaner, they share a normal
    float normal[3] = {0.0, 0.0, 1.0};

    for(int i = 0; i < vertex_count; i++) {
        //since each position takes 3 floats, we have to step by 3. Hence `indicies[i] * 3`.
        memcpy(vert_walker, positions + (indicies[i] * 3), sizeof(float) * 3);
        vert_walker += 3;
        memcpy(vert_walker, normal, sizeof(float) * 3);
        vert_walker += 3; 
    }


    Geometry * geom = geom_new(usage);
    geom_replace_verticies(geom, verticies, vertex_count);

    free(positions);
    free(indicies);
    free(verticies);

    return geom;
}


// returns a dynarr<vec3> of positions of a grid broken into rows cols and ranks
// everything centered in a 2x2x2 cube. "rank" here means vertical "rows"
//the cube itself is centered on 0, so all dimensions span -1 to 1.
dynarr prim_grid_centers(int row_count, int col_count, int rank_count) {
    float x_step = 2.f / (float)row_count;
    float y_step = 2.f / (float)rank_count;
    float z_step = 2.f / (float)col_count;

    vec3 origin = {-1, -1, -1};

    //when we construct the grid, instead of everything being centered, we gonna have
    //a row, column and rank all on their respective 0's. If this were a chessboard,
    //we want the coordinates for where the chesspieces sit. So offset by half a step
    vec3 offset = {.5f * x_step, .5f * y_step, .5f * z_step};

    dynarr positions = dynarr_new(sizeof(vec3), row_count * col_count * rank_count);
    for(int row = 0; row < row_count; row++) {
        for(int rank = 0; rank < rank_count; rank++) {
            for(int col = 0; col < col_count; col++) {
                vec3 center = {x_step * (float)row, y_step * (float)rank, (float)z_step * col };
                glm_vec3_add(origin, center, center);
                glm_vec3_add(center, offset, center);

                dynarr_push(&positions, &center);
            }
        }
    }
    return positions;
}
