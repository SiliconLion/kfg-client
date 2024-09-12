#include "shader.h"
#include "utilities.c"



Shader * shad_new(const char * vert_path, const char * frag_path) {
    const char * vert_source = readFile(vert_path, NULL);
    const char * frag_source = readFile(frag_path, NULL);
    

    unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertShader, 1, &vert_source, NULL);
    glCompileShader(vertShader);

    glShaderSource(fragShader, 1, &frag_source, NULL);
    glCompileShader(fragShader);


    int  success;
    char infoLog[512];

    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE) {
        glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
        printf( "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE) {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        printf( "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }

    Shader * shader = calloc(1, sizeof(Shader));

    shader->program = glCreateProgram();
    glAttachShader(shader->program, vertShader);
    glAttachShader(shader->program, fragShader);
    glLinkProgram(shader->program);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return shader;
}

void shad_bind(Shader * shader) {
    glUseProgram(shader->program);
}

void shad_unbind() {
    glUseProgram(0);
}

void shad_delete(Shader * shad) {
    shad_unbind();
    glDeleteShader(shad->program);
}