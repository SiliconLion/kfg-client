#include "dynarr.h"

#include <string.h>

const dynarr DYNARR_ZERO = {.data = NULL, .stride = 0, .len = 0, .capacity = 0};

bool dynarr_is_zero(dynarr * d) {
    return d->data == NULL && d->stride == 0 && d->len == 0 && d->capacity == 0 ;
}


//stride is the size of the element to be stored in bytes
//capacity is the number of elements to allocate for
dynarr dynarr_new(size_t stride, size_t capacity) {

    void* data; 
    //allocating length 0 is implimentaion defined so we avoid it. 
    if(capacity == 0) {
        data = NULL;
    } else {
        data = calloc(capacity, stride);
    }

    return (dynarr) {
        .data = data,
        .stride = stride,
        .len = 0,
        .capacity = capacity
    };
}

void dynarr_expand(dynarr * self) {
    if( self->capacity != 0) {
        self->data = realloc(self->data, self->capacity * 2 * self->stride);
        self->capacity *= 2;
    } else {
        self->data = calloc(8, self->stride);
        self->capacity = 8;
    }
}

//exits with 1 if index is out of bounds
inline void dynarr_check_index(dynarr * self, size_t index) {
    //we dont need to check if index < 0 because size_t is unsigned.
    if (index >= self->len) {
        //may be possible to specify what the function this is called from is in 
        //the error message if we force inlining. Probably not though as I think
        //thats strictly a compiler optimization and wont be picked up by the preprocessor. 
        //Alternatively could maybe pass in the name of the function and inlining could mean
        //it doesn't have to load up another register with the pointer to the string 
        //contating the function name. Will investigate if I get this error a lot.
        fprintf(stderr, "\nError: dynarr index out bounds\n");
        exit(1);
    }
    return;
}

//copies the element to the end of the the array
void dynarr_push(dynarr * self, void const * const element) {
    if (self->len == self->capacity) {dynarr_expand(self);}

    memcpy( ((char*)self->data) + (self->len * self->stride), element, self->stride);
    self->len += 1; 
}

//Will never be null. Will exit with 1 when called with empty dynarr
//Frustraingly has to allocate due to the nature of void pointers
void * dynarr_pop(dynarr * self) {
    if(self->len == 0) {
        fprintf(stderr, "\nError: dynarr called pop on empty dyn_array\n");
        exit(1);
    }

    self->len -= 1; 
    void* element = calloc(1, self->stride);
    memcpy(element, (char*)self->data + (self->len * self->stride), self->stride);
    return element;
}

//returns pointer to copy of element at index
//bounds checked
//frustratingly allocates + copies.
void * dynarr_get(dynarr * self, size_t index) {
    dynarr_check_index(self, index);

    void* element = calloc(1, self->stride);
    memcpy(element, ( (char*)self->data ) + (index * self->stride), self->stride);
    return element;
}

//copies the element inot the dyn array
void dynarr_set(dynarr * self, size_t index, void const * const element) {
    dynarr_check_index(self, index);

    memcpy( (char*)self->data + (index * self->stride), element, self->stride);
    return;
}

// //inserts element into array
// void dynarr_insert(dynarr * self, size_t index, void const * const element) {

// }

//calls a function on every element in the dyn array
//operation is a function that takes an element of the dyn array
void dynarr_foreach(dynarr * self, void operation (void * element) ) {
    //just makes it easier to do arithmetic on the data.
    char * bytes = (char*)self->data;

    for(int i = 0; i < self->len; i++) {
        operation( (void*)( bytes + (i * self->stride) ) );
    }
    return;
}

//clears the data and resets the length to 0.
//Properties like stride and capacity are preserved.
void dynarr_clear(dynarr * arr) {
    memset(arr->data, 0, arr->stride * arr->len);
    arr->len = 0; 
}

void dynarr_delete(dynarr * self) {
    free(self->data);
}

//maybe should write a test for this, but looking to switch to CAVE_VEC soon
void dynarr_append_slice(dynarr * self, void* src, size_t element_count) {
//this is kinda bad, but more confident that it is correct, and seriously sweitching to CAVE_VEC soon
    while(self->capacity < self->len + element_count) {
        dynarr_expand(self);
    }

    memcpy((char*)self->data + (self->len * self->stride), src, (element_count * self->stride));
    self->len += element_count;
}