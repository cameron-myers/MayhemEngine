#pragma once
/*********************************************************************************************************************/
/*!
  \file              MEInput.h
  \author            Nate White
  \par               Email: nate.white\@digipen.edu
  \par               Course: GAM200
  \par               Copyright © 2022 DigiPen, All rights reserved.

  \brief
	input controller
 */
 /*********************************************************************************************************************/

#include "glm/glm.hpp"
#include "glm/vec2.hpp"
#include "MEWindow.h"

class MEWindow;

	class Input

	{
	public:
		static void GrabWindow(MEWindow* window) {window1 = window->GetWindow(); };
		static bool IsMousePressed(int button); 
		static bool IsMouseHeld(int button); 
		static bool IsMouseReleased(int button); 
		static glm::vec2 GetMouse();
		static float GetMouseX();
		static float GetMouseY();
		static bool IsKeyPressed(int keycode);
		static bool IsKeyHeld(int keycode);
		static bool IsKeyReleased(int keycode);
		static void InputUpdate();//do not touch
	private:
		
		static struct KeyInfo
		{
			int LastFrame = 0;
			int CurrentFrame = 0;
		};
		
		static KeyInfo Keystate[348];
		static GLFWwindow* window1;

	};
