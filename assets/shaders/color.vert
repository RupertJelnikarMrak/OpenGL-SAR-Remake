#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec4 aColor;

out vec4 Color;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    Color = aColor;
}
