#pragma once
/*****************************************************************//**
 * \file         MEWindow.h
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/

#include <glfw/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

typedef GLFWwindow GLFWwindow;


class MEWindow
{
public:

	MEWindow( char* title);
	~MEWindow();

	static GLFWwindow* GetWindow();
	static void SetScale(GLFWwindow* _window, float scale);
	static int GetWidth();
	static int GetHeight();
	static void SetSize(glm::vec2 size);
	static void ToggleFullscreen();
	static bool IsFullscreen();

private:
	bool Init( char* title);


	static GLFWwindow* window;
	static char* title;
	static GLFWmonitor* monitor;
	static glm::vec4 window_data;
	static bool is_fullscreen;
};
