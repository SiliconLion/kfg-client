#pragma once

#include "geometry.h"
#include "utilities.h"
#include "vertices.h"

#include "cglm/cglm.h"

//uses TwoPoint vertex type. Spans -1 to 1 for x and y Counter-clockwise winding direction
FullGeometry prim_new_rect(GLenum usage);

//uses TwoTexPoint vertex type. Spans -1 to 1 for x and y Counter-clockwise winding direction
FullGeometry prim_new_tex_rect(GLenum usage);

//uses ThreeTexPoint vertex type. Spans -1 to 1 for x and y. Z is 0.
// Counter-clockwise winding direction
FullGeometry prim_new_tex_rect_3d(GLenum usage);

//uses ThreeTexPoint vertex type. Spans -1 to 1 for x and y. Z is 0.
// Counter-clockwise winding direction
FullGeometry prim_new_tex_cube(GLenum usage);



//TODO: Maybe move this into a new file. It's not really a "primitive" is it?

//Returns a Geometry that is a flat triangular mesh with dimensions width, height. 
//the mesh is divided into rectangles 
//with w_resolution number of rectangles across the width, and 
//h_resolution number of rectangles across the height.
//then each rectangle has it's diagonal connected to make them tris. 
//Orientation is along the x and y plane
//usage is a paramater passed to the geometry constructor telling it how it's vertex buffer will be used.
//see Geometry in geometry.h to see whats allowed. 
Geometry * prim_new_plane(float width, float height, int w_resolution, int h_resolution, GLenum usage);

// returns a dynarr<vec3> of positions of a grid broken into rows cols and ranks
// everything centered in a 1x1x1 cube. "rank" here means vertical "rows"
dynarr prim_grid_centers(int row_count, int col_count, int rank_count);