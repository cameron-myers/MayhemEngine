/*****************************************************************//**
 * \file         BehaviorTimer_Results.h
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

class TimerResult :public Behavior
{
public:
	TimerResult();
	TimerResult(const TimerResult& rhs);
	~TimerResult();
	BehaviorPtr Clone() const override;
	//void TimeSet(float time);
	void Init();
	void Update(float dt);
	void Exit();
	void CollisionHandler(GameObjectPtr other) {};

private:

	float Timeleft;
};

