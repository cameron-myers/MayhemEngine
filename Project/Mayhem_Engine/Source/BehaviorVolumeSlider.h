/*****************************************************************//**
 * \file		BehaviorVolumeSlider.h
 * \author		Nicholas Leben
 * \par          Email:	nick.leben@gmail.com
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

class VolumeSlider : public Button
{
public:
	VolumeSlider();
	VolumeSlider(const VolumeSlider& rhs);
	~VolumeSlider() override;
	BehaviorPtr Clone() const override;
	void Init()override;
	void Update(float dt)override;
	void Exit()override;
	void Read(const char* path);
	void CollisionHandler(GameObjectPtr other) {};
};

