#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

uniform mat4x4 model; //local coords -> world coords
uniform mat4x4 view; //world coords -> camera coords
uniform mat4x4 perspective; //camera coords -> clip coords

uniform sampler2D Tex;

out vec3 FragPos;
out vec3 VertexNorm;
out vec2 TexCoord;

void main() {
    VertexNorm = normalize(aNorm); //probably already normalized but nice to be safe. could omit for speed.
    TexCoord = aTexCoord;
    FragPos = vec3(model * vec4(aPos, 1.0));

    gl_Position = perspective * view * model * vec4(aPos, 1.0);
}