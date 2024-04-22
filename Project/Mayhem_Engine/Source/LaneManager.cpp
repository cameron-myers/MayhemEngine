/*****************************************************************//**
 * \file         LaneManager.cpp
 * \author       Nick Leben
 * \par          Email: Nick.Leben\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#include "LaneManager.h"
#include "Log.h"
#include "MEInput.h"
#include "MESpaceManager.h"
#include "MESpace.h"
#include "BehaviorGoldCount.h"
#include "ZeppelinBehavior.h"

static std::vector<GameObjectPtr> TopLane;
static std::vector<GameObjectPtr> MidLane;
static std::vector<GameObjectPtr> BotLane;
/**
 * creates new zeppelin in the top lane.
 * 
 */
bool LaneManager::AddTop(cZeppelinType type)
{
	glm::vec3 Top{ -546,270,1 };
	MESpace* sandbox = MESpaceManager::GetSpace("Sandbox");
	GameObjectManager* activeList = sandbox->GetActive();
	GameObjectPtr gamObj = activeList->ZeppelinBuild(Top, type);
	TopLane.push_back(gamObj);
	return true;
}
/**
 * creates new zeppelin in the middle lane.
 * 
 */
bool LaneManager::AddMid(cZeppelinType type)
{
	glm::vec3 Mid{ -600,150,1 };
	MESpace* sandbox = MESpaceManager::GetSpace("Sandbox");
	GameObjectManager* activeList = sandbox->GetActive();
	GameObjectPtr gamObj = activeList->ZeppelinBuild(Mid, type);
	MidLane.push_back(gamObj);
	return true;
}
/**
 * creates new zeppelin in the bottom lane.
 * 
 */
bool LaneManager::AddBot(cZeppelinType type)
{

	glm::vec3 Bot{ -695,0,1 };
	MESpace* sandbox = MESpaceManager::GetSpace("Sandbox");
	GameObjectManager* activeList = sandbox->GetActive();
	GameObjectPtr gamObj = activeList->ZeppelinBuild(Bot, type);
	BotLane.push_back(gamObj);
	return true;

}
/**
 * gets the top lane vector.
 * 
 * \return 
 *		the top lane vector
 */
std::vector<GameObjectPtr>* LaneManager::GetTop()
{
	return &TopLane;
}
/**
 * gets the middle lane vector.
 * 
 * \return 
 *		the middle lane vector
 */
std::vector<GameObjectPtr>* LaneManager::GetMid()
{
	return &MidLane;
}
/**
 * gets the bottom lane vector.
 * 
 * \return 
 *		the bottom lane vector
 */
std::vector<GameObjectPtr>* LaneManager::GetBot()
{
	return &BotLane;
}

/**
 * \brief Find the lane an object is in
 * \param obj_name 
 * \return pointer to the lane, or null if not found
 */
std::vector<GameObjectPtr>* LaneManager::FindLane(std::string obj_name)
{
	for (auto obj : TopLane)
	{
		if (obj->GetName() == obj_name) return &TopLane;
	}

	for (auto obj : MidLane)
	{
		if (obj->GetName() == obj_name) return &MidLane;
	}

	for (auto obj : BotLane)
	{
		if (obj->GetName() == obj_name) return &BotLane;
	}

	return nullptr;
}

void LaneManager::Remove(std::vector<GameObjectPtr>* lane, std::string obj_name)
{
	if(lane == nullptr) return;
	for (auto i = lane->begin(); i != lane->end(); i++)
	{
		if ((*i)->GetName() == obj_name)
		{
			lane->erase(i);
			break;
		}

	}
}

/**
 * update loop for lane manager.
 * 
 * \param dt
 */
void LaneManager::Update(float dt)
{
	/*
	if (Input::IsKeyPressed(GLFW_KEY_A))
	{
		AddTop(cZeppelinTypeS);
	}
	else if (Input::IsKeyPressed(GLFW_KEY_S))
	{

		AddMid(cZeppelinTypeM);
	}
	else if (Input::IsKeyPressed(GLFW_KEY_D))
	{
	
		AddBot(cZeppelinTypeL);
	}
	*/
}

void LaneManager::KillEverything()
{
	for (auto zep : TopLane)
	{
		zep->GetComponent<Zeppelin>(cBehavior)->SetStateNext(cZeppelinDead);
	}
	for (auto zep : MidLane)
	{
		zep->GetComponent<Zeppelin>(cBehavior)->SetStateNext(cZeppelinDead);
	}
	for (auto zep : BotLane)
	{
		zep->GetComponent<Zeppelin>(cBehavior)->SetStateNext(cZeppelinDead);
	}
}