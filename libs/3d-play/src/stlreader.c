#include "stlreader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "utilities.h"


void stl_norm_print(stl_normal norm) {
    printf("[%f,%f,%f]\n",norm[0], norm[1], norm[2] );
}

void stl_vert_print(stl_vertex vert) {
    printf("[%f,%f,%f]\n",vert[0], vert[1], vert[2] );
}

void stl_tri_print(stl_tri * tri) {
    printf("stl_tri: {\n");
    printf("norm: ");
    stl_norm_print(tri->norm);
    printf("verts: \n");
    stl_vert_print(tri->verts[0]);
    stl_vert_print(tri->verts[1]);
    stl_vert_print(tri->verts[2]);
    printf("attribute byte count %ui\n", tri->attribute_byte_count);
}


stl_obj * stl_from_file(const char * path) {
    
    int8_t * bytes = readBytes(path, NULL);
    if(bytes == NULL) {
        printf("%s is not a vaild path to an stl file.\n", path);
        return NULL;
    }

    stl_obj * obj = calloc(1, sizeof(stl_obj));

    //the first 80 bytes are the header
    memcpy(obj->header, bytes, 80);
    //the next 4 bytes are the number of triangles
    memcpy( &(obj->tri_count) , bytes + 80, 4);

    // printf("tricount = %i\n", obj->tri_count);

    obj->tris = calloc(obj->tri_count, sizeof(stl_tri));

    memcpy(obj->tris, bytes + 84, obj->tri_count * sizeof(stl_tri));

    return obj;
}

//will write an stl_obj to a file named `path`. if that file already exists,
//it will override it.
bool stl_to_file(stl_obj * obj, const char * path) {
    
    assert(sizeof(stl_tri) == 50);

    //header is 80 bytes, tri_count is 4, and each tri is 50 bytes.
    int filesize = 84 + (obj->tri_count * 50); 
    int8_t * bytes = calloc(filesize, sizeof(int8_t));

    memcpy(bytes, obj->header, 80);
    memcpy(bytes + 80, &(obj->tri_count), 4);
    memcpy(bytes + 84, obj->tris, obj->tri_count * sizeof(stl_tri));

    return writeBytes(path, bytes, filesize);
}

stl_obj * stl_copy(stl_obj * obj) {

    stl_obj * copy = calloc(1, sizeof(stl_obj));

    //make a shallow copy. 
    memcpy(copy, obj, sizeof(stl_obj));

    //have copy->tris point to a new allocation, then copy the tris
    //into it.
    copy->tris = calloc(copy->tri_count, sizeof(stl_tri));
    memcpy(copy->tris, obj->tris, obj->tri_count * sizeof(stl_tri));

    return copy;
}


//To Do: make it account for negative coordinates as well. Though those are 
//disallowed in the spec, they seem to be common. 

//takes an stl_obj, and normalizes it so that the largest coordinate is equal to 
//max, and all other coords are scaled proportionately.
//also makes all normals unit normals
void stl_normalize(stl_obj * obj, float max) {
    unsigned long tri_count = obj->tri_count;
    stl_tri * tris = obj->tris; 

    //just set the max_coord to an initial value
    float max_coord = tris[0].verts[0][0];
    
    //find the largest coord in all the coordinates
    for(long i = 0; i < tri_count; i++) {
        //the above needs to be a long because the possible number of tris.
        //the below is long just for consistency
        for(long j = 0; j < 3; j++) {
            for(long k = 0; k < 3; k++) {
                if (tris[i].verts[j][k] > max_coord) {
                    max_coord = tris[i].verts[j][k];
                }
            }
        }
    }

    float scale = max / max_coord;

    for(long i = 0; i < tri_count; i++) {
        //normalize the verticies in the tri
        for(long j = 0; j < 3; j++) {
            for(long k = 0; k < 3; k++) {
                tris[i].verts[j][k] *= scale;
            }
        }

        //normalize the normal of the tri

        float norm_scale = sqrt( 
            powf(tris[i].norm[0], 2) +
            powf(tris[i].norm[1], 2) +
            powf(tris[i].norm[2], 2)
        );

        tris[i].norm[0] *= norm_scale;
        tris[i].norm[1] *= norm_scale;
        tris[i].norm[2] *= norm_scale;
    }
}


void stl_obj_delete(stl_obj * obj) {
    free(obj->tris);
}