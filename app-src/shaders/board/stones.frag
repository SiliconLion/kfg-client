#version 330 core
in vec3 norm;

uniform sampler2D matcap;

out vec4 FragColor;

void main() {
//    vec2 matcapPos = vec2( (norm.x / 2.0) + 0.5, (norm.y / 2.0) + 0.5);
//    FragColor = texture(matcap, matcapPos);
//    FragColor = vec4(norm, 1.0);
    vec3 light_direction = vec3(1.0, 1.0, 1.0);
    float percent_shadow = dot(norm, light_direction);
    FragColor = vec4(vec3(percent_shadow), 1.0);
}