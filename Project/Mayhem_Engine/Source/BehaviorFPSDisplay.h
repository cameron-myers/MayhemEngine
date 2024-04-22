/*****************************************************************//**
 * \file         BehaviorFPSDisplay.h
 * \author       Nicholas Leben
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


class FPSDisplay : public Behavior
{
public:
	FPSDisplay();
	FPSDisplay(const FPSDisplay& rhs);
	~FPSDisplay();
	Behavior* Clone()const override;
	void CollisionHandler(GameObjectPtr other) {};
	void Init();
	void Update(float dt);
	void Exit();

private:
	bool displayFlag;
};
