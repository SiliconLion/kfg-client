#include "utilities.h"

//path is a null terminated string of the file path
//sets length to be length of the file in bytes. Pass in NULL to ignore this 
char * readFile(const char* path, int* length) {
    FILE * file = fopen(path, "r");
    if (!file) {
        char * curdirectory = calloc(2000, sizeof(char));
        getcwd(curdirectory, 2000);
        printf("Unable to open file \"%s\" from current directory \"%s\"\n", path, curdirectory);
        length = NULL;
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    int size = ftell(file);

    char * outString = calloc(sizeof(char), size);
    
    fseek(file, 0, 0);
    fread(outString, sizeof(char), size, file); 
    fclose(file);

    if(length) {
        *length = size;
    }

    return outString;
}

// shaders/waves/darkwater.frag
// shaders/waves/darkwater.frag

//takes a path and returns a pointer to its contents. 
//sets *count to be the number of bytes pointed to. Pass in NULL to ignore this.
int8_t * readBytes(const char* path, int* count) {
    if(!path) {
        printf("error: `path` is a null pointer.\n");
    }
    FILE * file = fopen(path, "rb");
    if (!file) {
        char * curdirectory = calloc(2000, sizeof(char));
        getcwd(curdirectory, 2000);
        printf("Unable to open file \"%s\" from current directory \"%s\"\n", path, curdirectory);
        count = NULL;
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    int size = ftell(file);

    int8_t * bytes = calloc(sizeof(int8_t), size);

    fseek(file, 0, 0);
    fread(bytes, sizeof(int8_t), size, file); 
    fclose(file);

    if(count) {
        *count = size;
    }
    return bytes;
}

//writes to the file `path`. writes `count` bytes from `bytes` to the file.
//returns false if fail, true if success.
bool writeBytes(const char* path, int8_t * bytes, int count) {
    if(!path) {
        printf("error: `path` is a null pointer.\n");
        return false;
    }
    FILE * file = fopen(path, "wb+");
    if (!file) {
        char * curdirectory = calloc(2000, sizeof(char));
        getcwd(curdirectory, 2000);
        printf("Unable to open file \"%s\" from current directory \"%s\"\n", path, curdirectory);
        return false;
    }

    fwrite(bytes, 1, count, file);
    fclose(file);
    return true;
}

//takes coordinates in a 2D grid, and outputs what the index in a 1D array would be. 
//width is the width of the grid. 
inline unsigned int util_flatten_index(int row, int col, int width) {
    return row * width + col;
}

inline float norm_rand() { return (float)rand() / (float)RAND_MAX;}

void printf_triple_f(float* t) {
    printf("[ %f, %f, %f ]", *t, *(t+1), *(t+2));
}
