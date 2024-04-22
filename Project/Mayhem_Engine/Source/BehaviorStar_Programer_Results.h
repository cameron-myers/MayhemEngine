/*****************************************************************//**
 * \file         BehaviorStar_programer_Results.h
 * \author       Ethan Mclaughlin
 * \par          Email: Ethan.Mclaughlin@digipen.edu
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

enum cStarResultstate { cStarResultstateInvalid = -1, cStarResultstateIdle, cStarResultstateRunning, cStarResultstateEnded };

class StarResult :public Behavior
{
public:
	StarResult();
	StarResult(const StarResult& rhs);
	~StarResult();
	BehaviorPtr Clone() const override;
	void Init();
	void Update(float dt);
	void Exit();
	void CollisionHandler(GameObjectPtr other) {};

	//Timer* GetTimer() { return StarTimer; };

private:

	int stardata;
};

