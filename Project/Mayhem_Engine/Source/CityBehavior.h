/*****************************************************************//**
 * \file         CityBehavior.h
 * \author       Nate WHite
 * \par          Email: nate.white\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#pragma once

#include "Behavior.h"
class Behavior;
typedef Behavior* BehaviorPtr;
class Timer;



class City :public Behavior
{
public:
	City();
	City(const City& rhs);

	~City();
	BehaviorPtr Clone() const override;
	void Init();
	void Update(float dt);
	void Exit();
	void Take_Damage(float damage);
	void Stun(float time);
	void CollisionHandler(GameObjectPtr other);

private:
	void idle();
	void do_attack(GameObjectPtr target);
	void do_dead();
	void play_explosion();

	bool JustDead;
	Timer* city_dead_timer = nullptr;

};

