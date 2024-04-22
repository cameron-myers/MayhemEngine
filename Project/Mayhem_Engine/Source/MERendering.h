#pragma once
/*****************************************************************//**
 * \file         MERendering.h
 * \author       Cameron Myers, Nate White
 * \par          Email: cameron.myers\@digipen.edu,nate.white\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/

#include "SpriteSource.h"
#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include "glm/vec4.hpp"

class SpriteSource;
class Transform;
class Mesh;

void InitRenderer(GLFWwindow* windowHandle);

void StartRenderFrame();

void PresentFrame();

void MEDraw(SpriteSource& spriteSource, Mesh& mesh, Transform* transform, unsigned frameIndex, glm::vec4 rgba);
void  MEDrawParticle(SpriteSource& spriteSource, Mesh& mesh, glm::mat4& transformMatrix, unsigned frameIndex, glm::vec4 rgba);
METexture* MEDrawToTexture(SpriteSource& spriteSource, Mesh& mesh, unsigned frameIndex, glm::mat4& transformMatrix);

void ResizeWindowInit(GLFWwindow* window, int width, int height);

void ResizeEditor();

void ResizeWindow(GLFWwindow* window, int width, int height);

GLuint GetObjectBuffer();

GLuint GetViewportTexture();

void ShutdownRenderer();

