
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 norm;
in vec2 TexCoord;

uniform sampler2D Tex;

void main() {
    vec3 ambientLightColor = normalize(vec3(0.8, 0.8, 1));
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * ambientLightColor;

    vec3 objectColor = texture(Tex, TexCoord).rgb;
//    vec3 objectColor = vec3(1, 1, 1);

    vec3 lightDir = normalize(vec3(1, -4, 1));
    vec3 lightColor = normalize(vec3(1, 1, 0.8));

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
//    FragColor = vec4(result, 1.0);





//    FragColor = texture(Tex, TexCoord);
//    FragColor = vec4(1.0);
    FragColor = vec4(norm, 1.0);
}