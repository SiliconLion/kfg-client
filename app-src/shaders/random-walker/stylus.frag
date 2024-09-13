#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec2 Pos;

uniform sampler2D stylusHistory;

//uniform vec2 screenDims; //Screen width, Screen height

uniform vec2 stylusPos;
uniform vec3 framecolor;

void main()
{

//    FragColor = texture(stylusHistory, TexCoords);

    vec3 historyColor = texture(stylusHistory, TexCoords).xyz;
    historyColor -= vec3(0.001); //fades the color slightly. Later might want to change
    //this approach slightly if want to have multiple trails with different colors that blend nicely

    FragColor = vec4(historyColor, 1.0);


    if(length(Pos - stylusPos) < 0.02) {
//        FragColor = vec4(0.8, 0.4, 0.2, 1.0);
        FragColor = vec4(framecolor, 1.0);
    }

}