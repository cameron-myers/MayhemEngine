/*****************************************************************//**
 * \file   LaneManager.h
 * \author       Nick Leben
 * \par          Email: Nick.Leben\@digipen.edu
 * \date   November 2022
 *********************************************************************/
#pragma once
#include <vector>
#include "GameObject.h"

class GameObject;

enum cZeppelinType { cZeppelinTypeInvalid = 0, cZeppelinTypeS, cZeppelinTypeM, cZeppelinTypeL };
enum cTowerType { cTowerTypeInvalid = 0, cTowerTypeWood, cTowerTypeStone };


class LaneManager
{
public:

	static bool AddTop(cZeppelinType type);
	static bool AddMid(cZeppelinType type);
	static bool AddBot(cZeppelinType type);
	static std::vector<GameObjectPtr>* FindLane(std::string obj_name);
	static void Remove(std::vector<GameObjectPtr>* lane, std::string obj_name);
	static std::vector<GameObjectPtr>* GetTop();
	static std::vector<GameObjectPtr>* GetMid();
	static std::vector<GameObjectPtr>* GetBot();
	static void Update(float dt);
	static void KillEverything();
private:

};

