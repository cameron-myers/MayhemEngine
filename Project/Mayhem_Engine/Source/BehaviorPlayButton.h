/*****************************************************************//**
 * \file         BehaviorPlayButton.h
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
#include "InterpolManager.h"
#include <glm/vec3.hpp>
class Transform;
class Behavior;
class Audio;
typedef Behavior* BehaviorPtr;


class PlayButton : public Button
{
public:
	PlayButton();
	PlayButton(const PlayButton& rhs);
	~PlayButton() override;
	BehaviorPtr Clone() const override;
	void Init()override;
	void Update(float dt)override;
	void Exit()override;
	void Read(const char* path);
	void CollisionHandler(GameObjectPtr other) {};
private:
	bool flag;
	Interpolation* interpol;
};

