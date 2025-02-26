#version 410
out vec4 FragColor;

in vec3 WorldPos;
in vec3 norm;
in vec2 TexCoord;

in vec4 FragLightPos;

uniform sampler2D DIFFUSE;
uniform sampler2D NORMALS;
uniform sampler2D SHADOWS;

const float SHADOW_BIAS = 0.001;
float ShadowValue() {
    vec3 proj_coords = FragLightPos.xyz / FragLightPos.w;
    proj_coords = proj_coords * 0.5 + 0.5; // takes [-1,1] => [0, 1]

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(SHADOWS, proj_coords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = proj_coords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main() {
//    vec3 ambientLightColor = normalize(vec3(0.8, 0.8, 1));
//    float ambientStrength = 0.1;
//    vec3 ambient = ambientStrength * ambientLightColor;
//
//    vec3 objectColor = texture(DIFFUSE, TexCoord).rgb;
//
//
//    // vec3 objectColor = vec3(TexCoord, 1.0);
//    // vec3 objectColor = vec3(1.0, 1.0, 1.0);
//
//    vec3 texture_normals = texture(NORMALS, TexCoord).rgb;
//    vec3 compound_norm = norm + texture_normals;
//
//    // vec3 lightDir = normalize(vec3(1, 4, 1));
//    vec3 lightDir = normalize(vec3(1, 5, 3));
//    vec3 lightColor = normalize(vec3(1, 1, 0.8));
//
//    float diff = max(dot(compound_norm, lightDir), 0.0);
//    vec3 diffuse = diff * lightColor;
//
//    vec3 result = (ambient + diffuse) * objectColor;
//    FragColor = vec4(result, 1.0);



//    FragColor = vec4(texture(DIFFUSE, TexCoord).rgb, 1.0);
    FragColor = vec4(
        vec3( ShadowValue() )
        , 1.0
    );

}