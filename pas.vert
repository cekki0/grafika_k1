#version 330 core

layout(location = 0) in vec2 inPos;
uniform vec2 uPos;
uniform float uRotationAngle;

void main()
{
    mat4 translation = mat4(1.0, 0.0, 0.0, 0.0,
                            0.0, 1.0, 0.0, 0.0,
                            0.0, 0.0, 1.0, 0.0,
                            uPos.x, uPos.y, 0.0, 1.0);

    mat4 rotation = mat4(cos(uRotationAngle), 0.0, -sin(uRotationAngle), 0.0,
                         0.0, 1.0, 0.0, 0.0,
                         sin(uRotationAngle), 0.0, cos(uRotationAngle), 0.0,
                         0.0, 0.0, 0.0, 1.0);

    gl_Position = translation * rotation * vec4(inPos, 0.0, 1.0);
}
