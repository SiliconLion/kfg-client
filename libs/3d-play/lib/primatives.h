#pragma once

#include "geometry.h"
#include "utilities.h"
#include "vertices.h"

//uses TwoPoint vertex type. Spans -1 to 1 for x and y Counter-clockwise winding direction
FullGeometry prim_new_rect(GLenum usage);

//uses TwoTexPoint vertex type. Spans -1 to 1 for x and y Counter-clockwise winding direction
FullGeometry prim_new_tex_rect(GLenum usage);

//uses ThreeTexPoint vertex type. Spans -1 to 1 for x and y. Z is 0.
// Counter-clockwise winding direction
FullGeometry prim_new_tex_rect_3d(GLenum usage);

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
