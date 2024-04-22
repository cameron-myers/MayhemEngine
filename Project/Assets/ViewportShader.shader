#version 330 core
layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inTexCoords;

out vec2 TexCoords;

uniform vec2 editorToggle;

uniform mat4 world;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    
    gl_Position = world  * vec4(inPos, 0, 1);
    TexCoords = inTexCoords;
}  