#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

uniform mat4x4 transformation;
uniform mat4x4 scale;
uniform mat4x4 perspective;

// out vec3 pos;
out vec3 norm;

void main() {
    norm = (perspective *transformation * vec4(aNorm, 1.0)).xyz;
    gl_Position = perspective * transformation * scale * vec4(aPos, 1.0);
}