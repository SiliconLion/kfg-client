#include "error-handling.h"
#include <stdio.h>

void print_gl_error(int line, char* file) {
    unsigned int err = glGetError();
    if(err != 0) {
        printf("GL Error: %s:line %i : %i\n", file, line,  err);
    }
}
