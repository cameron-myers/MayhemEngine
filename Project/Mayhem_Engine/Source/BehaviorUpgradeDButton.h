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

enum cUpgrType { cUpgrInvalid = -1, cUpgrDamage, cUpgrHealth, cUpgrSpeed };

//NEEDS NAME CHANGE
class UpgradeButton : public Button
{
public:
	UpgradeButton(cUpgrType _type);
	UpgradeButton(cUpgrType _type, std::string name);
	UpgradeButton(const UpgradeButton& rhs);
	~UpgradeButton();
	void CollisionHandler(GameObjectPtr other) {};
	BehaviorPtr Clone() const override;
	void Init()override;
	void Update(float dt)override;
	void Exit()override;
	void Read(const char* path);
	static void SetShipType(cZeppelinType type);


private:

	cUpgrType type;
	static cZeppelinType ship;
	
};

