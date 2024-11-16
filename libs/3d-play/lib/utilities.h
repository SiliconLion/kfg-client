#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>


//path is a null terminated string of the file path
//sets length to be length of the file in bytes. Pass in NULL to ignore this 
char * readFile(const char* path, int* length);

// shaders/waves/darkwater.frag
// shaders/waves/darkwater.frag

//takes a path and returns a pointer to its contents. 
//sets *count to be the number of bytes pointed to. Pass in NULL to ignore this.
int8_t * readBytes(const char* path, int* count);

//writes to the file `path`. writes `count` bytes from `bytes` to the file.
//returns false if fail, true if success.
bool writeBytes(const char* path, int8_t * bytes, int count) ;

//takes coordinates in a 2D grid, and outputs what the index in a 1D array would be. 
//width is the width of the grid. 
unsigned int util_flatten_index(int row, int col, int width);

float norm_rand();

void printf_triple_f(float* t);

char * get_dir_from_file_path(char* file_path);


//TODO: doesn't quite work i dont think
//takes the expression `expr` of type `type` and allocates it on the heap 
// #define HEAPIFY(type, expr) \
//     do{ \
//         void * ptr = calloc(1, sizeof(type)); \
//         *ptr = expr; \
//         return ptr; \
//     }while(0)

