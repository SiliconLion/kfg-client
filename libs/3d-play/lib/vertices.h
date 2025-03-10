#pragma once

//Note: It is not valid for vertices to hold pointers to data outside the memory held by their
//containing struct


typedef struct {
    float pos [2];
} TwoPoint;

typedef struct {
    float pos [3];
} ThreePoint;

typedef struct {
    float pos [4];
} FourPoint;


typedef struct {
    float pos [2];
    float tex [2];
} TwoTexPoint;

typedef struct {
    float pos [3];
    float tex [2];
} ThreeTexPoint;

typedef struct {
    float pos [4];
    float tex [2];
} FourTexPoint;



typedef struct {
    float pos [3];
    float norm [3];
} ThreeNormPoint;

typedef struct {
    float pos [4];
    float norm [3];
} FourNormPoint;



typedef struct {
    float pos [3];
    float norm [3];
    float tex [2];
} ThreeNormTexPoint;

typedef struct {
    float pos [4];
    float norm [3];
    float tex [2];
} FourNormTexPoint;



// Note: A lot of these functions are totally untested, and writing them is very error prone.
// So if using one of them for the first time, may want to sanity check it somehow


void vert_enable_two_point();

void vert_enable_three_point();

void vert_enable_four_point();

void vert_enable_two_tex_point();

void vert_enable_three_tex_point();

void vert_enable_four_tex_point();

void vert_enable_three_norm_point();

void vert_enable_four_norm_point();

void vert_enable_three_norm_tex_point();

void vert_enable_four_norm_tex_point();



typedef void VERTEX_BLUEPRINT();

extern VERTEX_BLUEPRINT* TwoPointBlueprint;
extern VERTEX_BLUEPRINT* ThreePointBlueprint;
extern VERTEX_BLUEPRINT* FourPointBlueprint;
extern VERTEX_BLUEPRINT* TwoTexPointBlueprint;
extern VERTEX_BLUEPRINT* ThreeTexPointBlueprint;
extern VERTEX_BLUEPRINT* FourTexPointBlueprint;
extern VERTEX_BLUEPRINT* ThreeNormPointBlueprint;
extern VERTEX_BLUEPRINT* FourNormPointBlueprint;
extern VERTEX_BLUEPRINT* ThreeNormTexPointBlueprint;
extern VERTEX_BLUEPRINT* FourNormTexPointBlueprint;

