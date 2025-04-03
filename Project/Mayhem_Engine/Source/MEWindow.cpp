/*****************************************************************//**
 * \file         MEWindow.cpp
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief This file handles all the functions for the Engine window
 * 
 *********************************************************************/
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "Log.h"
#include "MEWindow.h"

#include <iostream>

#include "Engine.h"
#include "MEAudio.h"
#include "MEEditor.h"
#include "MERendering.h"
#include "MESpace.h"
#include "MESpaceManager.h"
#include "OpenGLError.h"


char* MEWindow::title;
GLFWwindow* MEWindow::window;

GLFWmonitor* MEWindow::monitor;
glm::vec4 MEWindow::window_data;
bool MEWindow::is_fullscreen;

float _scale;

void focused(GLFWwindow* window, int _focused);
void drop_callback(GLFWwindow* window, int count, const char** paths);



/**
 * \brief Window constructor
 * \param width width of the window in pixels
 * \param height height of the window in pixels
 * \param title title of the window
 */
MEWindow::MEWindow( char* title)
{
	ME_CORE_INFO("Cpnstruct Window")
	Init(title);

}

/**
 * \brief Window deconstructor
 */
MEWindow::~MEWindow()
{
	glfwDestroyWindow(window);
}

/**
 * \brief Initialize the window
 * \param width 
 * \param height 
 * \param title 
 * \return 
 */
bool MEWindow::Init(char* title)
{

	if (!glfwInit())
	{
		//add assert
		ME_CORE_ERROR("GLFW failed to initialize");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	GLFWwindow* windowHandle = glfwCreateWindow(vidmode->width, vidmode->height, title, glfwGetPrimaryMonitor(), nullptr);

	ME_CORE_INFO("window created")
		std::cout << "window created" << std::endl;

	if (!windowHandle)
	{
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(windowHandle);
	glfwFocusWindow(windowHandle);


	//vsync
	glfwSwapInterval(0);
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		glfwDestroyWindow(windowHandle);
		glfwTerminate();
		ME_CORE_ERROR("GLAD failed to initialize");
		return false;
	}

	CheckError();

	glDisable(GL_CULL_FACE);
	CheckError();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	window = windowHandle;

	int width, height, x, y;
	glfwGetWindowSize(window, &width, &height);

	glfwGetWindowPos(window, &x, &y);


#ifndef _DEBUG
	glfwSetWindowFocusCallback(window, focused);
#endif

#ifndef _DISTRIBUTE
	glfwSetDropCallback(window, drop_callback);
#endif

	ME_CORE_INFO("collect window data")
		std::cout << "collect window data" << std::endl;

	window_data = { vidmode->width, vidmode->height, x, y };

	return true;
}

/**
 * \brief Gets the window
 * \return a pointer to the glfw window
 */
GLFWwindow* MEWindow::GetWindow()
{
	return window;
}
/// <summary>
/// set the scale of the window
/// </summary>
/// <param name="_window"></param>
/// <param name="scale"></param>
void MEWindow::SetScale(GLFWwindow* _window, float scale)
{
	_scale = scale;
}
/// <summary>
/// get the width of the windoww
/// </summary>
/// <returns>int of the window width</returns>
int MEWindow::GetWidth()
{
	return window_data.x;
}
/// <summary>
/// get the height of the window
/// </summary>
/// <returns>int of window height</returns>
int MEWindow::GetHeight()
{
	return window_data.y;
}
/// <summary>
/// set the size of the window
/// </summary>
/// <param name="size"></param>
void MEWindow::SetSize(glm::vec2 size)
{
	window_data.x = size.x;
	window_data.y = size.y;
}

/**
 * \brief Toggles bewtween fullscreen and windowed
 */
void MEWindow::ToggleFullscreen()
{
	is_fullscreen = glfwGetWindowMonitor(window) != NULL;

	if (is_fullscreen)
	{

		//make window windowed
		glfwSetWindowMonitor(window, nullptr, window_data.z, window_data.w, window_data.x, window_data.y, 0);
		glfwSetWindowAspectRatio(window, 16, 9);
		CheckError();

		//check if window is screen size, if so maximize to make window fit right
		if(window_data.x == glfwGetVideoMode(glfwGetPrimaryMonitor())->width && window_data.y == glfwGetVideoMode(glfwGetPrimaryMonitor())->height)
		{

			glfwMaximizeWindow(window);
		}
	}
	else
	{

		//capture previous settings
		int width;
		int height;
		int x;
		int y;

		glfwGetWindowSize(window, &width, &height);
		glfwGetWindowPos(window, &x, &y);

		window_data = { width, height, x, y };

		monitor = glfwGetPrimaryMonitor();
		//make fullscreen
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		glfwSetWindowAspectRatio(window, 16, 9);
		CheckError();
	}
	
	
}

bool MEWindow::IsFullscreen()
{
	return is_fullscreen;
}

void focused(GLFWwindow* window, int _focused)
{
	if (!_focused)
	{
		//check if in game state
		if (MESpaceManager::GetSpace("Sandbox") != nullptr)
		{
			//do pause stuff
			MESpaceManager::SetSpaceInclusive("PauseMenu");
			MESpaceManager::MoveSpaceToBack("PauseMenu");
		}

		MEAudio::MuteAll();

	}
	//regaining focus
	else
	{
		MEAudio::UnMuteAll();
	}
}


void drop_callback(GLFWwindow* window, int count, const char** paths)
{
	//handle the dropped files
	MEEditor::ReceiveDrop(paths, count);
}
