/*****************************************************************//**
 * \file         BehaviorZeppelinSelectButton.h
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
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
#include "Transform.h"
#include "BehaviorButton.h"
#include <glm/vec3.hpp>
#include "LaneManager.h"

class SpriteSource;
class Transform;
class Behavior;
class Audio;
typedef Behavior* BehaviorPtr;

enum frames
{
	Idle = 0,
	Hover,
	Pressed

};


class ZeppelinSelectButton : public Button
{
public:
	ZeppelinSelectButton(cZeppelinType type);
	ZeppelinSelectButton(const ZeppelinSelectButton& rhs);
	~ZeppelinSelectButton() override;
	BehaviorPtr Clone() const override;
	void Init()override;
	void Update(float dt)override;
	void Exit()override;
	void Read(const char* path);
	void CollisionHandler(GameObjectPtr other) {};

private:
	void SetSelectedIcon();
	void SetStatBar();
	void SetCostNumber();
	void PopStatBars();

	static cZeppelinType old_type;

	cZeppelinType type;

	SpriteSource* s_health_bar;
	SpriteSource* m_health_bar;
	SpriteSource* l_health_bar;

	SpriteSource* s_speed_bar;
	SpriteSource* m_speed_bar;
	SpriteSource* l_speed_bar;

	SpriteSource* s_damage_bar;
	SpriteSource* m_damage_bar;
	SpriteSource* l_damage_bar;

};

