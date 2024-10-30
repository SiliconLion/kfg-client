
#version 330 core
out vec4 FragColor;

in vec3 norm;
in vec2 TexCoord;

uniform sampler2D Tex;

void main() {
    FragColor = texture(Tex, TexCoord);
//    FragColor = vec4(1.0);
//    FragColor = vec4(norm, 1.0);
}