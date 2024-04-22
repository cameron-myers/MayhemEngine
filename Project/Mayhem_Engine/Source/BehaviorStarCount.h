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


class StarCount : public Behavior
{
public:
	StarCount();
	StarCount(const StarCount& rhs);
	~StarCount();
	void CollisionHandler(GameObjectPtr other) {};
	BehaviorPtr Clone() const override;
	void Init();
	void Update(float dt);
	void Exit();

private:
	int Stars;
};