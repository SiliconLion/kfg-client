
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D Tex;

void main() {
    FragColor = texture(Tex, TexCoord);
//    FragColor = vec4(1.0);
}