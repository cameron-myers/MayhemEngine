/*****************************************************************//**
 * \file         BehaviorStar_Programmer.h
 * \author       Ethan Mclauhglin
 * \par          Email: ethan.mclauhglin\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#pragma once

#include "Behavior.h"
#include "MEtimer.h"
class Behavior;
typedef Behavior* BehaviorPtr;

enum cStarstate { cStarstateInvalid = -1, cStarstateIdle, cStarstateRunning, cStarstateEnded };

class Star :public Behavior
{
public:
	Star(float time);
	Star(const Star& rhs);
	~Star();
	BehaviorPtr Clone() const override;
	void Init();
	void Update(float dt);
	void Exit();
	void CollisionHandler(GameObjectPtr other) {};

	Timer* GetTimer() { return StarTimer; };

private:

	Timer* StarTimer;
	float StarTime = 60.0f;
	bool NoTime;
};

