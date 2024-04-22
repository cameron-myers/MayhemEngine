/*****************************************************************//**
 * \file         BehaviorRoundTimer.h
 * \author       Ethan Mclaughlin
 * \par          Email: ethan.mclaughlin\@digipen.edu
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
class Timer;

enum cClockstate { cClockstateInvalid = -1, cClockstateIdle, cClockstateRunning, cClockstateEnded };

class RoundTimer : public Behavior
{
public:
	RoundTimer();
	RoundTimer(const RoundTimer& rhs);
	~RoundTimer();
	void CollisionHandler(GameObjectPtr other) {};
	BehaviorPtr Clone() const override;
	void Init();
	void Update(float dt);
	void Exit();

	Timer* GetTimer() { return Roundtime; };

private:
	Timer* Roundtime = nullptr;
	Timer* end_timer = nullptr;
	float RoundLength = 60.0f;
	bool ended = false;
	//Audio* audio;
};

