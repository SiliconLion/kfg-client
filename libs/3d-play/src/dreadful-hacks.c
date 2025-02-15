#include "dreadful-hacks.h"
#include "mesh-tools.h"
#include "dynarr.h"
#include "vertices.h"


//takes ownership of g (of type FullGeometry<ThreeTexPoint>) and deletes
// it, returning a FullGeometry<ThreeNormTexPoint>
FullGeometry add_normals_to_geom(FullGeometry* g) {
    dynarr new_verts = interpolate_face_normals(&g->vertices, &g->indices);
//    dynarr new_indices = dynarr_new(sizeof(INDEX_TYPE), new_verts.len);
//    for(INDEX_TYPE i = 0; i < new_verts.len; i++) {
//        dynarr_push(&new_indices, &i);
//    }

    FullGeometry new_geom = full_geom_new(
            ThreeNormTexPointBlueprint, sizeof(ThreeNormTexPoint),
            new_verts, DYNARR_ZERO,
            g->primitive_type, g->usage
            );

    full_geom_delete(g);
    return new_geom;

}

//takes ownership of g (of type FullGeometry<ThreeNormPoint>) and deletes
// it, returning a FullGeometry<ThreeNormTexPoint>
FullGeometry add_text_coords_to_geom(FullGeometry* g) {
    dynarr new_verts;
}