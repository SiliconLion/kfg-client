#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 Pos;
out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos, 0.f, 1.0);

    Pos = aPos;
    TexCoords = aTexCoords;

}
