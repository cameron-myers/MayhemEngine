#pragma once
/*!********************************************************************************************************************
  \file              MEFramerateController.h
  \author            Cameron Myers, Nate White
  \par               Email: nate.white\@digipen.edu, cameron.myers\@digipen.edu
  \par               Course: GAM200
  \par               Copyright © 2022 DigiPen, All rights reserved.

  \brief
	the framerate controller for all systems

*********************************************************************************************************************/

/*!********************************************************************************************************************
  \brief
	controlls the framerate for the engine
  \returns
	the delta time in between frames
*********************************************************************************************************************/
class MEFramerateController
{
public:
	static float GetFrameTime();
	static int GetFrame();
	static float GetTimeElapsed();
	static float GetFPS();
	static void SetFPS(int Fps);
private:
	 static float fps;
};
