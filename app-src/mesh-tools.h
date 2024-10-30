#pragma once

#include "omni-include.h"
#include "vertices.h"
#include "dynarr.h"
#include "geometry.h"
#include <cglm/cglm.h>

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