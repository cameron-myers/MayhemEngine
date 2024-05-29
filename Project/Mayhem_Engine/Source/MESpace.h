/*****************************************************************//**
 * \file         MESpace.h
 * \author       Cameron Myers,Nick.leben
 * \par          Email: cameron.myers\@digipen.edu,Nick.leben\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#pragma once

#define UNREFERENCED_PARAMETER(P) (P)

#include <list>

#include "GameObjectManager.h"

enum class State
{
	SQuit = -3,
	SRestart = -2,
	SInvalid = -1,

	SSandbox,

	SNum,

	SLast = SNum - 1,

	SInitial = SSandbox,

	SZero = 0
};

typedef struct
{
	State prev;
	State curr;
	State next;
}GameState;


class MESpace
{
public:

	MESpace(std::string const& filename);

	MESpace(GameState s) :active(true)
	{

	}

	~MESpace()
	{
		active = NULL;

		name.clear();
	}
	
	void Load();

	void Init() {}

	void Update(float dt) { if (this->active) { activeList.CheckCollisions(); activeList.Update(dt); } }

	void Shutdown() { activeList.Shutdown(); InactiveList.Shutdown(); }

	void Unload() {}

	void Draw() { activeList.Draw(); }

	GameObjectManager* GetActive() { return &activeList; }

	GameObjectManager* GetInactive() { return &InactiveList; }

	bool IsActive() { return active; }

	std::string* GetName() { return &name; }

	void SetActive(bool input) { active = input; }

	void PutInInactive(GameObject* gamObj);
	void PutInActive(GameObject* gamObj);

	static std::vector<std::string> GetActiveListFromFile(std::string const& filename);

private:
	
	bool active;

	std::string name;

	GameObjectManager activeList;
	GameObjectManager InactiveList;
};

