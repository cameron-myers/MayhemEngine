#pragma once
/*********************************************************************************************************************/
/*!
  \file              Engine.h
  \author            Nick Leben, Cameron Myers
  \par               Email:   nick.leben\@digipen.edu, cameron.myers\@Digipen.edu
  \par               Course: GAM200
  \par               Copyright © 2022 DigiPen, All rights reserved.

  \brief
	interface to add systems to the engine
 */
 /*********************************************************************************************************************/
#include <string>
#include <unordered_map>
#include <Windows.h>


/*#define NDEBUGONLY(func) {
#ifdef _DEBUG 
func
#endif}*/

class MEWindow;

class Engine
{
	// Public Functions:
public:
	Engine(MEWindow* windowptr);
	~Engine();

	// Initialize all systems in the engine.
	void Initialize();

	// Update all systems in the engine.
	void Update(float dt);

	// Render all systems in the engine.
	void Render();

	bool isRunning = true;
	inline static bool isHeadless = false;
	// Public Event Handlers
public:
	// Windows message event handler.
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// Private Functions:

	// Destroy all systems in reverse order from which they were added.
	void DestroySystems();

	static MEWindow* GetWindow();
	inline static bool s_UnitTesting = false;

	/**
	 * \brief Argname to arg value, stored as a char* 
	 */
	inline static std::unordered_map<std::string, const char*> s_TestingArgs;

};
