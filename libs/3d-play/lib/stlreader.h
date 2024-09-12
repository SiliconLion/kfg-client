#pragma once

#include <stdbool.h>


typedef float stl_normal[3];
typedef float stl_vertex[3];


typedef  struct {
    stl_normal norm;
    stl_vertex verts[3];
    unsigned short int attribute_byte_count;
} __attribute__((__packed__)) stl_tri; 


void stl_norm_print(stl_normal norm);

void stl_vert_print(stl_vertex vert);

void stl_tri_print(stl_tri * tri);


typedef struct {
    char header[80];
    unsigned long tri_count;
    stl_tri * tris;
} stl_obj; 



stl_obj * stl_from_file(const char * path);

//will write an stl_obj to a file named `path`. if that file already exists,
//it will override it.
bool stl_to_file(stl_obj * obj, const char * path);

stl_obj * stl_copy(stl_obj * obj);


//To Do: make it account for negative coordinates as well. Though those are 
//disallowed in the spec, they seem to be common. 

//takes an stl_obj, and normalizes it so that the largest coordinate is equal to 
//max, and all other coords are scaled proportionately.
//also makes all normals unit normals
void stl_normalize(stl_obj * obj, float max);


void stl_obj_delete(stl_obj * obj);