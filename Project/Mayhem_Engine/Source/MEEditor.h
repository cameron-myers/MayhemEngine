/*****************************************************************//**
 * \file         MEEditor.h
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#pragma once
#include <string>
#include <vector>

#include "MEWindow.h"

class MESpace;
typedef struct tool;
class GameObject;

class MEEditor
{
public:
	static void Initialize(MEWindow* _window);
	static void Update(float dt);
	static void Render();
	static void Shutdown();
	static void SetOpen(bool _isOpen);
	static bool IsOpen();
	static void ResetSelected();
	static void SetSelectedObject(GameObject* obj);
	static void ToggleOpen();
	static GameObject* GetSelectedObject();
	static void ReceiveDrop(const char** paths, int count);
	static void SetWindowSize(glm::vec2 _size);

private:
	static void ToolManager(std::vector<MESpace*> spaceList);
	static void DebugDrawManager(std::vector<MESpace*> spaceList);

};
