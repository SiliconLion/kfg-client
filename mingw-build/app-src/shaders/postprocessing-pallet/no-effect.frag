#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec2 Pos;

uniform sampler2D screenTexture;

uniform vec2 screenDims; //Screen width, Screen height

void main()
{
    FragColor = texture(screenTexture, TexCoords);
}
