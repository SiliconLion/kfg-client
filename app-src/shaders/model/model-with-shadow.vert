#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

uniform mat4x4 model; //local coords -> world coords
uniform mat4x4 view; //world coords -> camera coords
uniform mat4x4 perspective; //camera coords -> clip coords

uniform mat4x4 lightView;
uniform mat4x4 lightPerspective;

uniform sampler2D Tex;

out vec3 WorldPos;
out vec3 norm;
out vec2 TexCoord;

out vec4 FragLightPos;

void main() {
    norm = normalize(aNorm);
    TexCoord = aTexCoord;
    WorldPos = vec3(model * vec4(aPos, 1.0)); //just puts the vertex in world coords

    mat4x4 CAMERA = perspective * view;
    mat4x4 LIGHT = lightPerspective * lightView;

    mat4x4 CtL = LIGHT * inverse(CAMERA); //Camera to Light

    vec4 CameraPos = CAMERA * model * vec4(aPos, 1.0);
    FragLightPos = CtL * CameraPos;

    gl_Position = CAMERA * model * vec4(aPos, 1.0);


}