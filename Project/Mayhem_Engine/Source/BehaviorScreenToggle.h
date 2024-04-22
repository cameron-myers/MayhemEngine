/*****************************************************************//**
 * \file		BehaviorScreenToggle.h
 * \author		Cameron Myers
 * \par          Email:	cameron.myers@digipen.edu
 * \par          Course: GAM250
 * \copyright    Copyright (c) 2023 DigiPen, All rights reserved
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
class Transform;
class Behavior;
class Audio;
typedef Behavior* BehaviorPtr;

class ScreenToggle : public Button
{
public:
	ScreenToggle();
	ScreenToggle(const ScreenToggle& rhs);
	~ScreenToggle() override;
	BehaviorPtr Clone() const override;
	void Init()override;
	void Update(float dt)override;
	void Exit()override;
	void Read(const char* path);
	void CollisionHandler(GameObjectPtr other) {};

private:
	bool checked;
};

