#version 330 core

layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec4 inColor;


out vec2 TexCoord;
out vec4 Color;

uniform mat4 world;
uniform mat4 projection;
uniform mat4 view;
uniform vec2 u_v;
uniform vec2 u_v_size;
uniform vec4 colorVal;

void main(void)
{
    gl_Position = projection * world * view * vec4(inPosition, 0, 1);
    
    TexCoord = inTexCoord * u_v_size + u_v;
    Color = inColor * colorVal;
}
