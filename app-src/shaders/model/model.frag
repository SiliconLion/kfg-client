#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 VertexNorm;
in vec2 TexCoord;

uniform sampler2D DIFFUSE;
uniform sampler2D SPECULAR;
uniform sampler2D NORMALS;
uniform sampler2D ROUGHESS;


void main() {

//light variables. TODO: make these more robust/dynamic
    vec3 lightDir = normalize(vec3(1, 4, 1));
    vec3 lightColor = normalize(vec3(1, 1, 0.8));

    vec3 ambientLightColor = normalize(vec3(0.8, 0.8, 1));
    float ambientStrength = 0.5;


    // vec3 norm = VertexNorm + texture(NORMALS, TexCoord).xyz;

    vec3 norm = VertexNorm;

    vec3 baseColor = texture(DIFFUSE, TexCoord).rgb;

    vec3 ambient = ambientStrength * ambientLightColor;
    vec3 diffuse = max(dot(norm, lightDir), 0.0) * lightColor;

    vec3 result = (ambient + diffuse) * baseColor;
    FragColor = vec4(result, 1.0);






//    FragColor = texture(Tex, TexCoord);
   FragColor = vec4(1.0);
//    FragColor = vec4(norm, 1.0);
    // FragColor = vec4(TexCoord, 1.0, 1.0);
    // FragColor = vec4(1.0, .5, 1.0, 1.0);

    // FragColor = texture(DIFFUSE, TexCoord);

}