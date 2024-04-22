/*!********************************************************************************************************************
  \file              MEFramerateController.cpp
  \author            Cameron Myers and Nate White
  \par               Email: cameron.myers\@digipen.edu
  \par               Email: nate.white\@digipen.edu
  \par               Course: GAM200
  \par               Copyright © 2022 DigiPen, All rights reserved.

  \brief
	the framerate controller for all systems
 *********************************************************************************************************************/

#include <chrono>
#include <iostream>
#include <MEFramerateController.h>
#include <string>
#include <glfw/glfw3.h>
int frames = 0;
double currentFrame = 0.0;
double lastFrame = 0.0;
double printTime = 0.0;
float MEFramerateController::fps = 0;
 /*!********************************************************************************************************************
   \brief
     helper function to clamp dt
   \returns
     nothing
 *********************************************************************************************************************/
void ClampDT(float* dt ,float min, float max)
{
    if (*dt < min)
    {
       *dt = min;
    }
    if (*dt > max)
    {
      *dt = max;
    }
}

/*!******************************************************************************************************************** 
  \brief
    controlls the framerate for the engine
  \returns
    the delta time in between frames
*********************************************************************************************************************/
float MEFramerateController::GetFrameTime()
{
    using chrono_hrc = std::chrono::steady_clock; //still not sure what this is doing
    currentFrame = 0.0;
    double dt = 0;


    currentFrame = glfwGetTime();
    dt = (currentFrame - lastFrame);
    //ClampDT(&dt, 0.0f, 0.1f);

    lastFrame = currentFrame;

    return dt;
}
/// <summary>
/// get the frames per second
/// </summary>
/// <returns>float of frames per second</returns>
float MEFramerateController::GetFPS()
{
    return MEFramerateController::fps;
}
/// <summary>
/// sets the frames per second
/// </summary>
/// <param name="Fps"></param>
void MEFramerateController::SetFPS(int Fps)
{ 
    MEFramerateController::fps = Fps;
}
