#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

uniform mat4x4 model; //local coords -> world coords
uniform mat4x4 view; //world coords -> camera coords
uniform mat4x4 perspective; //camera coords -> clip coords

uniform sampler2D Tex;

out vec3 norm;
out vec2 TexCoord;

void main() {
    norm = aNorm;
    TexCoord = aTexCoord;
    gl_Position = perspective * view * model * vec4(aPos, 1.0);
}