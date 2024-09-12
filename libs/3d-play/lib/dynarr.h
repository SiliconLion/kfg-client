#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// #include <unistd.h>

//basically a Vec<T>
typedef struct {
    //a pointer to the data stored
    void * data;
    //the width of the elements to be stored in bytes
    size_t stride;
    //the number of elements stored
    size_t len;
    //the number of elements able to be stored without reallocating
    size_t capacity;
} dynarr;

const dynarr DYNARR_ZERO;

bool dynarr_is_zero(dynarr * d);


//stride is the size of the element to be stored in bytes
//capacity is the number of elements to allocate for
dynarr dynarr_new(size_t stride, size_t capacity);

void dynarr_expand(dynarr * self);

//exits with 1 if index is out of bounds
void dynarr_check_index(dynarr * self, size_t index);

//copies the element to the end of the the array
void dynarr_push(dynarr * self, void const * const element);

//Will never be null. Will exit with 1 when called with empty dynarr
//Frustraingly has to allocate due to the nature of void pointers
void * dynarr_pop(dynarr * self);

//returns pointer to copy of element at index
//bounds checked
//frustratingly allocates + copies.
void * dynarr_get(dynarr * self, size_t index);

//copies the element inot the dyn array
void dynarr_set(dynarr * self, size_t index, void const * const element);

// //inserts element into array
// void dynarr_insert(dynarr * self, size_t index, void const * const element) {

// }

//calls a function on every element in the dyn array
//operation is a function that takes an element of the dyn array
void dynarr_foreach(dynarr * self, void operation (void * element) );

//clears the data and resets the length to 0.
//Properties like stride and capacity are preserved.
void dynarr_clear(dynarr * arr);

void dynarr_delete(dynarr * self);

//maybe should write a test for this, but looking to switch to CAVE_VEC soon
void dynarr_append_slice(dynarr * self, void* src, size_t element_count);