/*****************************************************************//**
 * \file         BehaviorYesButton.h
 * \author       Nicholas Leben
 * \par          Email: nick.leben@digipen.edu
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


class YesButton : public Button
{
public:
	YesButton();
	YesButton(const YesButton& rhs);
	~YesButton() override;
	BehaviorPtr Clone() const override;
	void Init()override;
	void Update(float dt)override;
	void Exit()override;
	void Read(const char* path) {};
	void CollisionHandler(GameObjectPtr other) {};
private:

};

