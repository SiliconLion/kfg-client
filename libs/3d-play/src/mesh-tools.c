
#include "mesh-tools.h"
#include <string.h>
#include <cglm/cglm.h>

dynarr interpolate_face_normals(dynarr* verts, dynarr* indices) {

    size_t indices_len;
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
    for(size_t tri_num = 0; tri_num < indices_len / 3; tri_num++) {
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

        for(unsigned int i = 0; i < 3; i++) {
            ThreeNormTexPoint new_v;
            memcpy(&new_v.pos, &tri_verts[i].pos, sizeof(float) * 3);
            memcpy(&new_v.tex, &tri_verts[i].tex, sizeof(float) * 2);
            memcpy(&new_v.norm, &normal, sizeof(float) * 3);

            dynarr_push(&ret, &new_v);
        }
    }
    return ret;
}

void normalize_3d_vertices_to_sphere(dynarr* verts, size_t offset_of_pos, float diameter) {
    if(verts->len == 0) {return;}
    
    float max_norm = 0.0;
    for(size_t i = 0; i < verts->len; i++) {
        void* vert = dynarr_at(verts, i);
        float* pos = (float*)vert+offset_of_pos;
        float n = glm_vec3_norm( pos );
        if(n > max_norm){
            max_norm = n;
        }
    }

    float scale = (diameter * .5) / max_norm; //ToDo: protect against divide by 0?
    for(size_t i = 0; i < verts->len; i++) {
        void* vert = dynarr_at(verts, i);
        float* pos = (float*)vert+offset_of_pos;
        glm_vec3_scale(pos, scale, pos);
    }
}

void normalize_3d_vertices_to_cube(dynarr* verts, size_t offset_of_pos, float diameter) {
    if(verts->len == 0) {return;}
    

    //ToDo, factor this out into a function that returns a bounding box
    float max_x = 0, max_y = 0, max_z = 0;
    float min_x = 0, min_y = 0, min_z = 0;

    for(size_t i = 0; i < verts->len; i++) {
        void* vert = dynarr_at(verts, i);
        float* pos = (float*)vert+offset_of_pos;

        if     (pos[0] > max_x){max_x = pos[0];}
        else if(pos[0] < min_x){min_x = pos[0];}
        if     (pos[1] > max_y){max_y = pos[1];}
        else if(pos[1] < min_y){min_y = pos[1];}
        if     (pos[2] > max_z){max_z = pos[2];}
        else if(pos[2] < min_z){min_z = pos[2];}
    }

    float delta_x = max_x - min_x;
    float delta_y = max_y - min_y;
    float delta_z = max_z - min_z;

    float center[3] = { delta_x/2.f + min_x,
                        delta_y/2.f + min_y, 
                        delta_z/2.f + min_z,
                    };

    float max_len = delta_x;
    if(delta_y > max_len) {max_len = delta_y;}
    if(delta_z > max_len) {max_len = delta_z;}

    // float scale = (diameter * .5) / max_len; //ToDo: protect against divide by 0?
    float scale = (diameter) / max_len; //ToDo: protect against divide by 0?
    for(size_t i = 0; i < verts->len; i++) {
        void* vert = dynarr_at(verts, i);
        float* pos = (float*)vert+offset_of_pos;
        glm_vec3_sub(pos, center, pos);
        glm_vec3_scale(pos, scale, pos);
    }
}