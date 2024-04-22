/*****************************************************************//**
 * \file         OpenGLError.h
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#pragma once

#include "glad/glad.h"
//int OpenGLMain();
void CheckError();
void PrintShaderError(GLuint shaderID);
//GLuint CreateShader(const char* vertexFile, const char* pixelFile);
//void CreateMesh();
//void CreateTexture(const char* fileName);
//void Draw(GLuint shader, float scale, float rotation, glm::vec3 position, GLuint mesh, int vertexCount, GLuint texture);
//void resizeWindow(GLFWwindow* window, int height, int width);