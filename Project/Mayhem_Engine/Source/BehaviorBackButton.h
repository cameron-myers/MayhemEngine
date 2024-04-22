/*****************************************************************//**
 * \file		BehaviorBackButton.h
 * \author		Nicholas Leben	
 * \par          Email: nick.leben@digipen.edu
 * \par          Course: GAM250
 * \copyright    Copyright (c) 2023 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/

#pragma once
#include "Behavior.h"
#include "Transform.h"
#include "BehaviorButton.h"
#include <glm/vec3.hpp>
class Transform;
class Behavior;
class Audio;
typedef Behavior* BehaviorPtr;

class BackButton : public Button
{
public:
	BackButton();
	BackButton(const BackButton& rhs);
	~BackButton() override;
	BehaviorPtr Clone() const override;
	void Init()override;
	void Update(float dt)override;
	void Exit()override;
	void Read(const char* path);
	void CollisionHandler(GameObjectPtr other) {};
};
