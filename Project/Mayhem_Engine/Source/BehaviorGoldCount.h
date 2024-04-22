/*****************************************************************//**
 * \file         BehaviorGoldCount.h
 * \author       Ethan Mclaughlin
 * \par          Email: ethan.mclaughlin\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#pragma once
#include "Behavior.h"
typedef Behavior* BehaviorPtr;
class Timer;


class GoldCounter : public Behavior
{
public:
	GoldCounter();
	GoldCounter(const GoldCounter& rhs);
	~GoldCounter();
	void CollisionHandler(GameObjectPtr other) {};
	BehaviorPtr Clone() const override;
	void Init();
	void Update(float dt);
	void Exit();
	static bool CompGold(int value); //compares values
	static void ChangeGold(int value); //removes gold


private:
	Timer* GoldWait;
};

class GoldCount : public Behavior
{
public:
	static bool CompGold(int value); //compares values
	static void ChangeGold(int value); //removes gold
	static int GetGold(); //gets gold
	static void ResetGold();

private:
	static int Gold;
	Timer* GoldWait;
};
