/*!********************************************************************************************************************
  \file              MEInput.cpp
  \author            Nate White
  \par               Email: nate.white\@digipen.edu
  \par               Course: GAM200
  \par               Copyright © 2022 DigiPen, All rights reserved.

  \brief
	the Input controller
 *********************************************************************************************************************/

/*
#include <chrono>
#include <iostream>
#include <string>
*/
#include "MEInput.h"
#include <glfw/glfw3.h>
#include "Log.h"
#include "MEFramerateController.h"

Input::KeyInfo Input::Keystate[] = { 0 };
GLFWwindow* Input::window1 = nullptr;
int winXSize = 0;
int winYSize = 0;

/*returns if the key is pressed on the current frame*/
bool Input::IsKeyPressed(int keycode)
{
	
	if (Keystate[keycode].CurrentFrame == GLFW_PRESS && Keystate[keycode].LastFrame == GLFW_RELEASE)
	{
		
		ME_CORE_INFO("key Pressed");
		return true;
	}
	else
	{
		
		return false;
	}
}
/// <summary>
/// check if key is held
/// </summary>
/// <param name="keycode"></param>
/// <returns>true of key is held, false otherwise</returns>
bool Input::IsKeyHeld(int keycode)
{

	if (Keystate[keycode].CurrentFrame == GLFW_PRESS && Keystate[keycode].LastFrame == GLFW_PRESS)
	{

		//ME_CORE_INFO("key Held");
		return true;
	}
	else
	{

		return false;
	}
}
//only returns true if key is released after being pressed
bool Input::IsKeyReleased(int keycode)
{
	
	
	if (Keystate[keycode].CurrentFrame == GLFW_RELEASE && Keystate[keycode].LastFrame == GLFW_PRESS)
	{
		
		ME_CORE_INFO("key Released");
		return true;
	}
	else
	{
	
		return false;
	}
}
/// <summary>
/// update the input
/// </summary>
void Input::InputUpdate()
{
	for (int i = 0; i < 8; i++)
	{
		int mouseState = glfwGetMouseButton(window1, i);
		Keystate[i].LastFrame = Keystate[i].CurrentFrame;
		Keystate[i].CurrentFrame = mouseState;
	}
	for (int i = 8; i < 348; i++)
	{
		int state = glfwGetKey(window1, i);
		Keystate[i].LastFrame = Keystate[i].CurrentFrame;
		Keystate[i].CurrentFrame = state;
	}
	
}
/// <summary>
/// check if mouse is pressed
/// </summary>
/// <param name="button">:which mouse button</param>
/// <returns>true if mouse is pressed, false otherwise</returns>
bool Input::IsMousePressed(int button)/*button = which mouse button to check*/
{
	if (Keystate[button].CurrentFrame == GLFW_PRESS && Keystate[button].LastFrame == GLFW_RELEASE)
	{
		ME_CORE_INFO("Mouse Pressed");
		return true;
	}
	return false;
}
/// <summary>
/// check if mouse button is held
/// </summary>
/// <param name="button">:mouse button to check</param>
/// <returns>true if mouse button is held, false otherwise</returns>
bool Input::IsMouseHeld(int button)/*button = which mouse button to check*/
{
	if (Keystate[button].CurrentFrame == GLFW_PRESS && Keystate[button].LastFrame == GLFW_PRESS)
	{
		//ME_CORE_INFO("Mouse Held");
		return true;
	}
	return false;
}
/// <summary>
/// checks if the mouse button is released
/// </summary>
/// <param name="button">:mouse button to check</param>
/// <returns>true if the button is released, false otherwise</returns>
bool Input::IsMouseReleased(int button)/*button = which mouse button to check*/
{
	if (Keystate[button].CurrentFrame == GLFW_RELEASE && Keystate[button].LastFrame == GLFW_PRESS)
	{
		return true;
		ME_CORE_INFO("Mouse Released");
	}
	return false;
}
/// <summary>
/// get the mouse position
/// </summary>
/// <returns>vec2 of mouse position</returns>
glm::vec2 Input::GetMouse()
{
	int halfW, halfH;
	double x, y;
	glfwGetWindowSize(window1, &halfW, &halfH);
	halfW /= 2.0;
	halfH /= 2.0;
	glfwGetCursorPos(window1, &x, &y);
	x = x -halfW;
	y = y - halfH;
	glm::vec2 vector = glm::vec2(x, y);
	return vector;/*returns the mouse x and y position*/
}
/// <summary>
/// get mouse x position
/// </summary>
/// <returns>float of mouse x position</returns>
float Input::GetMouseX()
{
	
	double halfW = 0.0, halfH = 0.0;
	double x = 0.0, y = 0.0;
	halfW = MEWindow::GetWidth();
	halfW /= 2.0;
	halfH /= 2.0;
	glfwGetCursorPos(MEWindow::GetWindow(), &x, &y);
	x = x - halfW;
	y = y - halfH;
	//glfwGetWindowFrameSize()
	return (float)x;/*returns the mouse x position*/
}
/// <summary>
/// get mouse y position
/// </summary>
/// <returns>float of mouse y position</returns>
float Input::GetMouseY()
{
	double halfW = 0.0 , halfH = 0.0;
	double x = 0.0, y = 0.0;
	halfH = MEWindow::GetHeight();
	halfW /= 2.0;
	halfH /= 2.0;
	glfwGetCursorPos(window1, &x, &y);
	x = x - halfW;
	y = y - halfH;
	return -(float)y;/*returns the mouse y position*/
}



