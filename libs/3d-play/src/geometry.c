#include "geometry.h"

#include <string.h>

//note that vertices and indices are passed by value. The resulting geometry will own
//both vertices and indices. Note that they can be empty and updated later with a call
//to geom_replace_vertices
FullGeometry full_geom_new(
        VERTEX_BLUEPRINT vertex_blueprint, int vertex_stride,
        dynarr vertices, dynarr indices,
        GLenum primitive_type, GLenum usage
) {
    FullGeometry g;

    g.vert_blueprint = vertex_blueprint;
    g.vertex_stride = vertex_stride;
    g.vertices = vertices;
    g.indices = indices;
    g.primitive_type = primitive_type;
    g.usage = usage;

    //generate our opengl object
    glGenVertexArrays(1, &(g.VAO));
    glGenBuffers(1, &(g.VBO));
    glGenBuffers(1, &(g.EBO));

    //bind them
    glBindVertexArray(g.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, g.VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g.EBO);


    if(g.vertices.len > 0) {
        size_t len = g.vertices.len;
        size_t stride = g.vertex_stride;
        glBufferData(GL_ARRAY_BUFFER, len * stride, g.vertices.data, g.usage);
    }

    if(g.indices.len >0) {
        size_t len = g.indices.len;
        size_t stride = sizeof(INDEX_TYPE);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, len * stride, g.indices.data, g.usage);
    }

    vertex_blueprint();

    //unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return g;
}

void full_geom_bind(FullGeometry * g) {
    glBindVertexArray(g->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, g->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g->EBO);
}

void full_geom_unbind() {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//Clears and then reuploads
//note, takes ownership of new_verts and new_indices. Either can be empty, but why have a geometry
//with no vertices?
void full_geom_replace_verts_and_indices(FullGeometry* g, dynarr new_verts, dynarr new_indices) {
    //replace vertices and indices
    dynarr_delete(&g->vertices);
    dynarr_delete(&g->indices);
    g->vertices = new_verts;
    g->indices = new_indices;

//update the gpu with our new vertices
    full_geom_bind(g);

    //update vertices on gpu
    size_t len = g->vertices.len;
    size_t stride = g->vertex_stride;
    glBufferData(GL_ARRAY_BUFFER, len * stride, g->vertices.data, g->usage);

    //update indices on gpu
    len = g->indices.len;
    stride = sizeof(INDEX_TYPE);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, len * stride, g->indices.data, g->usage);

    full_geom_unbind();
}


//creates a Geometry from an stl file.


void full_geom_draw(FullGeometry * g) {
    glEnable(GL_DEPTH_TEST); //ToDo: Should this be here?
    full_geom_bind(g);

    if(g->indices.len == 0) {
        glDrawArrays(g->primitive_type, 0, g->vertices.len);
    } else {
        glDrawElements(g->primitive_type, g->indices.len, GL_UNSIGNED_INT, (void*)0);
    }

    full_geom_unbind();
    
}

void full_geom_draw_wireframe(FullGeometry * g, float line_width) {
    glEnable(GL_DEPTH_TEST);

    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glLineWidth(line_width);
    glEnable(GL_LINE_SMOOTH);

    full_geom_draw(g);

    //probably dont need to do this, but I prefer to unbind things
    glDisable(GL_LINE_SMOOTH);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void full_geom_delete(FullGeometry * g) {
    full_geom_unbind();
    dynarr_delete(&g->vertices);
    dynarr_delete(&g->indices);
    glDeleteVertexArrays(1, &g->VAO);
    glDeleteBuffers(1, &g->VBO);
    glDeleteBuffers(1, &g->EBO);
}


const int VERTEX_SIZE = 6 * sizeof(float);


//usage denotes the hint to be given for how the vertex buffer object will
//be used. should be one of GL_STREAM_DRAW, GL_STREAM_READ, 
//GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, 
//GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY
Geometry * geom_new(GLenum usage) {
    Geometry * geom = calloc(1, sizeof(Geometry));

    

    geom->verticies_count = 0;
    //this is an arbitrary initial capacity
    geom->verticies_capacity = 1 << 8;
    geom->verticies = calloc(geom->verticies_capacity, VERTEX_SIZE);

    geom->usage = usage;

    glGenVertexArrays(1, &(geom->VAO) );
    glBindVertexArray(geom->VAO);

    glGenBuffers(1, &(geom->VBO) );
    glBindBuffer(GL_ARRAY_BUFFER, geom->VBO);

    glBufferData(GL_ARRAY_BUFFER, geom->verticies_capacity * VERTEX_SIZE, NULL, usage);

    //sets up the position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (void*) 0);
    glEnableVertexAttribArray(0);
    //sets up the normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //unbinds so we dont interfere with other state
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return geom;
}

void geom_bind(Geometry * geom) {
    glBindVertexArray(geom->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, geom->VBO);
}

void geom_unbind() {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//Data is the new array of floats for verticies, and vertex_count is the number
//of verticies it contains.
//Does not take ownership of `data`. 
void geom_replace_verticies(Geometry* geom, float * data, int vertex_count) {
    geom_bind(geom);

    //copy data into geom->verticies
    geom->verticies = realloc(geom->verticies, vertex_count * VERTEX_SIZE);
    memcpy(geom->verticies, data, vertex_count * VERTEX_SIZE);

    glBufferData(GL_ARRAY_BUFFER, vertex_count * VERTEX_SIZE, geom->verticies, geom->usage);

    geom->verticies_count = vertex_count;

    geom_unbind();
}


//creates a Geometry from an stl file. 
//Path is the path to the stl file
Geometry * geom_from_stl(const char * path) {
    stl_obj * obj = stl_from_file(path);
    if(obj == NULL) {return NULL;}

    stl_normalize(obj, 1.0);


    //3 verticies per triangle.
    float * vert_data = calloc(obj->tri_count * 3, VERTEX_SIZE);


    float * curpos = vert_data;
    for(int i = 0; i < obj->tri_count; i++) {
        for (int j = 0; j < 3; j++) {

            //copy the j'th vertex in the i'th triangle to curpos
            memcpy(curpos, obj->tris[i].verts[j], sizeof(float) * 3);
            curpos += 3;

            memcpy(curpos, obj->tris[i].norm, sizeof(float) * 3);
            curpos += 3;
        }
    }

    Geometry * geom = geom_new(GL_STATIC_DRAW);

    geom_replace_verticies(geom, vert_data, obj->tri_count * 3);

    free(vert_data);

    return geom;
}

void geom_draw(Geometry * geom) {
    glEnable(GL_DEPTH_TEST);  
    geom_bind(geom);
    glDrawArrays(GL_TRIANGLES, 0, geom->verticies_count);

    geom_unbind();
}

void geom_draw_wireframe(Geometry * geom, float line_width) {
    glEnable(GL_DEPTH_TEST); 

    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glLineWidth(line_width);
    glEnable(GL_LINE_SMOOTH);

    geom_bind(geom);
    glDrawArrays(GL_TRIANGLES, 0, geom->verticies_count);

    geom_unbind();
    //probably dont need to do this, but I prefer to unbind things
    glDisable(GL_LINE_SMOOTH);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void geom_delete(Geometry * geom) {
    geom_unbind();
    free(geom->verticies);
    glDeleteBuffers(1, &(geom->VBO));
    glDeleteVertexArrays(1, &(geom->VAO));
}