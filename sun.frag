#version 330 core

out vec4 outCol;

uniform float uA;

void main()
{
    float pulse = 0.5 + 0.5 * sin(uA);
    vec3 color = mix(vec3(1.0, 1.0, 0.0), vec3(1.0, 0.5, 0.0), pulse);
    outCol = vec4(color, 1.0);
}