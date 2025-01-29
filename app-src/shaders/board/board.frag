#version 410
out vec4 FragColor;

in vec3 FragPos;
in vec3 norm;
in vec2 TexCoord;

uniform sampler2D DIFFUSE;
uniform sampler2D NORMALS;

void main() {
    vec3 ambientLightColor = normalize(vec3(0.8, 0.8, 1));
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * ambientLightColor;

    vec3 objectColor = texture(DIFFUSE, TexCoord).rgb;
    // vec3 objectColor = vec3(TexCoord, 1.0);
    // vec3 objectColor = vec3(1.0, 1.0, 1.0);

    vec3 texture_normals = texture(DIFFUSE, TexCoord).rgb;
    vec3 compound_norm = norm + texture_normals;

    // vec3 lightDir = normalize(vec3(1, 4, 1));
    vec3 lightDir = normalize(vec3(1, 5, 3));
    vec3 lightColor = normalize(vec3(1, 1, 0.8));

    float diff = max(dot(compound_norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);





//    FragColor = texture(Tex, TexCoord);
//    FragColor = vec4(1.0);
//    FragColor = vec4(norm, 1.0);
    // FragColor = vec4(TexCoord, 1.0, 1.0);
    // FragColor = vec4(1.0, .5, 1.0, 1.0);

    // FragColor = texture(Tex, TexCoord);
}