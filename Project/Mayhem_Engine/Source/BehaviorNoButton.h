/*****************************************************************//**
 * \file         BehaviorNoButton.h
 * \author       Nicholas Leben
 * \par          Email: ethan.mclaughlin\@digipen.edu
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
#include "InterpolManager.h"

class Transform;
class Behavior;
class Audio;
typedef Behavior* BehaviorPtr;


class NoButton : public Button
{
public:
	NoButton();
	NoButton(const NoButton& rhs);
	~NoButton() override;
	BehaviorPtr Clone() const override;
	void Init()override;
	void Update(float dt)override;
	void Exit()override;
	void Read(const char* path) {};
	void CollisionHandler(GameObjectPtr other) {};
private:

};

