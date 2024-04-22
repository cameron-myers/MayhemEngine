/*****************************************************************//**
 * \file         MESpaceManager.h
 * \author       Nick Leben, Ethan mclaughlin
 * \par          Email: Nick.leben\@digipen.edu,ethan.mclaughlin\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#pragma once

#include "GameObject.h"
#include <string>
#include <vector>

class MESpace;

class MESpaceManager
{
public:
	MESpaceManager();
	~MESpaceManager();

	static void Init();

	static void Update(float dt);

	static void Shutdown();

	static void Render();

	static MESpace* GetSpace(std::string name);

	static bool HasSpace(std::string name);

	static bool IsActive(GameObjectPtr gamObj);

	static std::vector<std::string> GetSpaceNames();

	static void RefreshAll();

	static void SetSpaceInclusive(std::string spaceName);

	static void SetSpaceExclusive(std::string spaceName);

	static void SetSpacePrevInclusive();

	static void SetSpacePrevExclusive();

	static void MoveSpaceToBack(std::string spaceName);

	static void Remove(std::string spacename);

	/*
	static void SetPause();

	static void SetSandbox();

	static void SetMenuUI();

	static void SetGameLose();

	static void SetGameWin();
	*/

private:
	
};
