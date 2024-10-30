
#include "mesh-tools.h"
#include <string.h>

dynarr interpolate_face_normals(dynarr* verts, dynarr* indices) {

    usize indices_len;
    if(!indices) {
        indices_len = verts->len;
    } else {
        indices_len = indices->len;
    }


    if(indices_len % 3 != 0 ) {
        printf("indicies do not form triangles when trying to interpolate normals\n");
        exit(-1);
    }


    dynarr ret = dynarr_new(sizeof(ThreeNormTexPoint), indices_len);

    //tri_num is which triangle we are on.
    for(usize tri_num = 0; tri_num < indices_len / 3; tri_num++) {
        //for each triangle, there are 3 vertices. These vertices are often
        //shared in multiple triangles. So triangles are defined by indices
        //of these vertices. These indices are listed in order in indices.
        //vert_idx is an array of the indices that make up the current triangle.
        //tri_verts is an array of the vertices of the current triangle.
        INDEX_TYPE vert_idx[3];
        ThreeTexPoint tri_verts[3];


        //if we dont have indices, we know the vertices are just stored in triangle order,
        //and we calculate their indices very straightforwardly.
        if(!indices) {
            vert_idx[0] = (tri_num * 3) + 0;
            vert_idx[1] = (tri_num * 3) + 1;
            vert_idx[2] = (tri_num * 3) + 2;
        //otherwise we look up what the indices are for the current tri, and
        //copy them into vert_idx
        } else {
            dynarr_cpy_into(indices, (tri_num * 3) + 0, &vert_idx[0]);
            dynarr_cpy_into(indices, (tri_num * 3) + 1, &vert_idx[1]);
            dynarr_cpy_into(indices, (tri_num * 3) + 2, &vert_idx[2]);

        }
        //here we copy the vertices into tri_verts based on the indices we
        //just compiled
        dynarr_cpy_into(verts, vert_idx[0], tri_verts + 0);
        dynarr_cpy_into(verts, vert_idx[1], tri_verts + 1);
        dynarr_cpy_into(verts, vert_idx[2], tri_verts + 2);


    //the following is some simple linear algebra to find the face normal of
    //the triangle.
    //TODO: make sure the normal is pointing in the right direction based on winding order.
        vec3 A, B, C; // form ∆ABC
        //technically these copies are unnecessary but they make the
        //code nicer.
        glm_vec3_copy(tri_verts[0].pos, A);
        glm_vec3_copy(tri_verts[1].pos, B);
        glm_vec3_copy(tri_verts[2].pos, C);

        vec3 AB, AC;
        glm_vec3_sub(A, B, AB);
        glm_vec3_sub(A, C, AC);

        vec3 normal;
        glm_cross(AB, AC, normal);
        glm_vec3_normalize(normal);

        for(u32 i = 0; i < 3; i++) {
            ThreeNormTexPoint new_v;
            memcpy(&new_v.pos, &tri_verts[i].pos, sizeof(float) * 3);
            memcpy(&new_v.tex, &tri_verts[i].tex, sizeof(float) * 2);
            memcpy(&new_v.norm, &normal, sizeof(float) * 3);

            dynarr_push(&ret, &new_v);
        }
    }
    return ret;
}