#version 330 core

in vec2 TexCoord;
in vec4 Color;

out vec4 pixelColor;

void main(void)
{
    pixelColor = Color;
}