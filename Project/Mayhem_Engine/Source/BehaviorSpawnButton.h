#pragma once
/*****************************************************************//**
 * \file         BehaviorSpawnButton.h
 * \author       Cameron Myers, Ethan Mclaughlin
 * \par          Email: cameron.myers\@digipen.edu,ethan.mclaughlin\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/

 /*********************************************************************************************************************/
 /* Includes */
 /*********************************************************************************************************************/
#pragma once
#include "Behavior.h"
#include "BehaviorButton.h"
#include "LaneManager.h"
#include "Transform.h"

class Transform;
class Behavior;
class Audio;
typedef Behavior* BehaviorPtr;

enum cSpawnLoc { cSpawnInvalid = -1, cSpawnTop, cSpawnMid, cSpawnBot };


class SpawnButton : public Button
{
public:
	SpawnButton(cSpawnLoc _type);
	SpawnButton(cSpawnLoc _type, std::string name);
	SpawnButton(const SpawnButton& rhs);
	~SpawnButton();
	void CollisionHandler(GameObjectPtr other) {};
	//bool Collision();
	BehaviorPtr Clone() const override;
	void Init()override;
	void Update(float dt)override;
	void Exit()override;
	void Read(const char* path);
	static void SetSpawnType(cZeppelinType type);
	void PlayZeppelinSpawn();



private:

	cSpawnLoc type;
	Timer* spawn_cooldown = nullptr;
	
};

