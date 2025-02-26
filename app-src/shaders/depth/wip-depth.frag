#version 410
out vec4 FragColor;

in vec3 FragPos;
in vec3 norm;
in vec2 TexCoord;

uniform sampler2D DIFFUSE;
uniform sampler2D NORMALS;

void main() {
    //gl_FragDepth = gl_FragCoord.z;
}