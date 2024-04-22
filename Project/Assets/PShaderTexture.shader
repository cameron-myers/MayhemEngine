#version 330 core

in vec2 TexCoord;
in vec4 Color;

out vec4 pixelColor;

uniform sampler2D inTexture;

void main(void)
{
    vec4 texColor = texture(inTexture, TexCoord);
     if (texColor.a < 0.1)
         discard;
    
    
    pixelColor = texColor * Color;
}