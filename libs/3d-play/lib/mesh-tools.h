#pragma once

#include "vertices.h"
#include "dynarr.h"
#include "geometry.h"
#include <cglm/cglm.h>
#include <stdint.h>

//verts is a dynarr<ThreeTexPoint> and dest will be a dynarr<ThreeTexNormPoint>
//may generalize this function in the future if it is useful, but for now, not gonna
//indices is of type INDEX_TYPE. But if it is null, then will assume vertices
//form triangles in order . (0..2 => tri 0 ; 3..5 => tri 0 ; etc)
//returns a dynarr<ThreeNormTexPoint> that is in triangle order (ie, indices
//are just a list from 0 to return.len).
//Note: NoCurrently only works on geometries based on triangles
//Note: there are different ways to calculate normals, namely face normal
//vs surface normal. This function generates normals based just on each individual
//triangle, but a surface normal is orthoginal to the surface formed by
//every triangle a vertex is in.
dynarr interpolate_face_normals(dynarr* verts, dynarr* indices);


//takes a dynarr of any type of vertex with a 3d position component, and scales them all towards the origin
//such that they all fit in a sphere with diameter 1 (radius .5), with at least one point touching the sphere.
//`offset_of_pos` is the location of the position component in the vertex. eg, what `offsetof(VertexType)` would 
//return. 
void normalize_3d_vertices_to_sphere(dynarr* verts, size_t offset_of_pos, float diameter);



void normalize_3d_vertices_to_cube(dynarr* verts, size_t offset_of_pos, float diameter);
