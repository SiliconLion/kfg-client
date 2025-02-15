#pragma once

#include "omni-include.h"
#include "geometry.h"

//takes ownership of g (of type FullGeometry<ThreeTexPoint>) and deletes
// it, returning a FullGeometry<ThreeNormTexPoint>
FullGeometry add_normals_to_geom(FullGeometry* g);

//takes ownership of g (of type FullGeometry<ThreeNormPoint>) and deletes
// it, returning a FullGeometry<ThreeNormTexPoint>
FullGeometry add_text_coords_to_geom(FullGeometry* g);