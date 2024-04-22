/*****************************************************************//**
 * \file         BehaviorHealthBar.h
 * \author       Nate White
 * \par          Email: nate.white\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#pragma once

#include "Behavior.h"
class Interpolation;
class Behavior;
typedef Behavior* BehaviorPtr;



class HealthBar :public Behavior
{
public:
	HealthBar();
	HealthBar(const HealthBar& rhs);

	~HealthBar();
	BehaviorPtr Clone() const override;
	void Init();
	void Update(float dt);
	void Exit();
	void CollisionHandler(GameObjectPtr other) {};
	void ScaleToHealth(float health);

private:
	
	float bar_scale = 0.00001f;

};

