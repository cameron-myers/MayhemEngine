/*****************************************************************//**
 * \file         MEResources.h
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#pragma once
#include "glfw/glfw3.h"
#include "glm/glm.hpp"

typedef GLFWwindow GLFWwindow;

typedef struct Vertex
{
    glm::vec2 position;         // The position of the vertex relative to the mesh
    glm::vec2 textureCoord;     // The texture coordinate associated with this vertex
    glm::vec4 color;            // The color to be applied to this vertex
    Vertex()
    {
	    
    }
    // Constructor for ease of use
    Vertex(const glm::vec2& position_, const glm::vec2& uv_, const glm::vec4& color_) :
        position(position_), textureCoord(uv_), color(color_)
    {
    }
    
}Vertex;

bool CreateShader(GLuint* program, const char* vertexShader, const char* pixelShader);

void CreateTexture(GLuint* texture, int width, int height);

void CreateTextureImage(GLuint* texture, const char* filename, int* width, int* height);

void CreateMesh(GLuint* mesh, GLuint* meshVertexBuffer, Vertex* verticies, unsigned vertexCount);

void FreeTexture(const GLuint* texture);

void FreeShader(const GLuint* program);

void FreeMesh(const GLuint* mesh, const GLuint* meshVertexBuffer);
