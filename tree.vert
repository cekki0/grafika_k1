#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec4 aColor;

out vec4 vColor;
uniform float uColorProgress;

void main()
{
    float colorInterpolation = smoothstep(0.5, 0.1, aPosition.y);
    vec4 interpolatedColor = mix(aColor, vec4(1.0), uColorProgress * colorInterpolation);

    gl_Position = vec4(aPosition, 0.0, 1.0);
    vColor = interpolatedColor;
}
