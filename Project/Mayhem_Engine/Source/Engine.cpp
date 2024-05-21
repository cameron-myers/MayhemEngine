/*********************************************************************************************************************/
/*!
  \file              Engine.cpp
  \author            Nick Leben, Cameron Myers
  \par               Email:   nick.leben\@digipen.edu, cameron.myers\@digipen.edu
  \par               Course: GAM200
  \par               Copyright © 2022 DigiPen, All rights reserved.

  \brief
	base engine class to manage the engine
 */
 /*********************************************************************************************************************/

/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/

#include <Engine.h>
#include <iostream>
#include <windows.h>
#include <Log.h>
#include <glad/glad.h>
#include <MERendering.h>
#include "MEAudio.h"
#include "MEInput.h"
#include "MeshManager.h"
#include "SpriteSource.h"
#include "SpriteSourceManager.h"


#include "MEWindow.h"
#include "MESpaceManager.h"
#include "MEtimer.h"
#include "InterpolManager.h"
#include "MEFunctionalTesting.h"

float total = 0.f;
MEWindow* window = NULL;
#ifdef _DEBUG
#include "MEEditor.h"
MEEditor* editor = NULL;
#endif
MEWindow* GetWindow();
/// <summary>
/// constructor
/// </summary>
/// <param name="windowptr"></param>
Engine::Engine(MEWindow* windowptr)
{
	window = windowptr;
	Initialize();

}
/// <summary>
/// destructor
/// </summary>
Engine::~Engine()
{
	DestroySystems();
}

MEWindow* Engine::GetWindow()
{
	return window;
}
/**
 * \brief Initializes the engine and it's systems
 */
void Engine::Initialize()
{
	Log::Init();

	//glfwMaximizeWindow(window->GetWindow());

	InitRenderer(window->GetWindow());
	MEFunctionalTesting::Init();

	MEAudio::Init();
	MEAudio::Load();

	MESpaceManager::Init();


	//if debug toggle to windowed on boot
#ifdef _DEBUG
	MEWindow::ToggleFullscreen();
	MEEditor::Initialize(window);

#endif


}

/**
 * \brief updates the engine
 * \param dt 
 */
void Engine::Update(float dt)
{
	MEFunctionalTesting::Update(dt);

	InterpolManager::Update(dt);
	Input::InputUpdate();
	/*
	if (Input::IsKeyPressed(GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window->GetWindow(), TRUE);
	}
	*/
	//Mute/UnMute Audio
	if(Input::IsKeyPressed(GLFW_KEY_0))
	{
		MEAudio::ToggleMute();
	}

	MEAudio::Update();

	METimerManager::Update(dt);

	MESpaceManager::Update(dt);
#ifdef _DEBUG

	if (Input::IsKeyPressed(GLFW_KEY_MINUS))
	{
		MEEditor::ToggleOpen();
	}


	MEEditor::Update(dt);
#endif
	if (Input::IsKeyPressed(GLFW_KEY_LEFT_BRACKET))
	{
		window->ToggleFullscreen();
	}
}

/**
 * \brief renders the engine
 */
void Engine::Render()
{
	if(isHeadless) return;

	StartRenderFrame();

	MESpaceManager::Render();
	
#ifdef _DEBUG
	MEEditor::Render();
#endif

	PresentFrame();
}

/*LRESULT Engine::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

}*/
/**
 * \brief Shuts down all the engine's systems
 */
void Engine::DestroySystems()
{

	SpriteSourceManager::FreeAll();
	SpriteSourceManager::Shutdown();
	METimerManager::Shutdown();
	MeshManager::FreeAll();
	MESpaceManager::Shutdown();
	MEAudio::Shutdown();
#ifdef _DEBUG
	MEEditor::Shutdown();
#endif

	ShutdownRenderer();
}

