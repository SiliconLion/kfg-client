#pragma once
#include "platform.h"

INCLUDE_OPENGL

#include <stdlib.h>
#include "stlreader.h"
#include "dynarr.h" //switch to CAVE_Vec when possible
#include "vertices.h"


#define INDEX_TYPE unsigned int

typedef struct {
    VERTEX_BLUEPRINT * vert_blueprint;
    size_t vertex_stride; //number of bytes one vertex takes up.

    unsigned int VAO; //vertex array object
    unsigned int VBO; //vertex buffer object
    unsigned int EBO; //element buffer object (indices basically)

    dynarr vertices;
    dynarr indices;

    //what type of primitive is created by the vertices + indices. Eg,
    //GL_TRIANGLES, GL_TRIANGLE_FAN, GL_LINE_STRIP, etc
    GLenum primitive_type;

    //what mode the vertex buffer associated with VBO is. 
    //eg, GL_STATIC_DRAW, GL_DYNAMIC_DRAW
    GLenum usage;

} FullGeometry;
//ToDo: Once everything is switched over to this, rename to just `Geometry`.


//note that vertices and indices are passed by value. The resulting geometry will own
//both vertices and indices. Note that they can be empty and updated later with a call
//to geom_replace_vertices
FullGeometry full_geom_new(
        VERTEX_BLUEPRINT vertex_blueprint, int vertex_stride,
        dynarr vertices, dynarr indices,
        GLenum primitive_type, GLenum usage
);

void full_geom_bind(FullGeometry * g) ;

void full_geom_unbind();

//Clears and then reuploads
//note, takes ownership of new_verts and new_indices. Either can be empty, but why have a geometry
//with no vertices?
void full_geom_replace_verts_and_indices(FullGeometry* g, dynarr new_verts, dynarr new_indices);
//creates a FullGeometry<ThreeNormPoint> from an stl file. path is the path to the stl file
//Note: does not de-dupe vertices (ie, does not index the vertices, for the
// stl format has everthing in terms of triangle. It can be done but isn't here).
FullGeometry full_geom_from_stl(const char * path, GLenum usage);

void full_geom_draw(FullGeometry * g);
void full_geom_draw_wireframe(FullGeometry * g, float line_width);

void full_geom_delete(FullGeometry * g);


//ToDo: Deprecating everything below this in favor of FullGeometry

//each vertex consists of 6 floats. The first 3 for position, then next 3 for normal
const int VERTEX_SIZE;

typedef struct {
    VERTEX_BLUEPRINT * vert_blueprint;
    int vertex_stride; //number of bytes one vertex takes up.

    unsigned int VAO; //vertex array object
    unsigned int VBO; //vertex buffer object

    //ToDo: Fix typo (vertices not verticies)
    //the number of verticies `verticies` can hold
    int verticies_capacity;
    int verticies_count;

    float * verticies;


    //what mode the vertex buffer associated with VBO is.
    //eg, GL_STATIC_DRAW, GL_DYNAMIC_DRAW
    GLenum usage;

} Geometry;



//usage denotes the hint to be given for how the vertex buffer object will
//be used. should be one of GL_STREAM_DRAW, GL_STREAM_READ, 
//GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, 
//GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY
Geometry * geom_new(GLenum usage);

void geom_bind(Geometry * geom) ;

void geom_unbind();

//Data is the new array of floats for verticies, and vertex_count is the number
//of verticies it contains.
//Does not take ownership of `data`. 
void geom_replace_verticies(Geometry* geom, float * data, int vertex_count);

//creates a Geometry from an stl file. 
//Path is the path to the stl file
Geometry * geom_from_stl(const char * path);

void geom_draw(Geometry * geom);

void geom_draw_wireframe(Geometry * geom, float line_width);

void geom_delete(Geometry * geom);