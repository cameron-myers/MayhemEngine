/*****************************************************************//**
 * \file         TowerBehavior.h
 * \author       Nate White
 * \par          Email: nate.white\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#pragma once

#include "Behavior.h"
#include "LaneManager.h"
class Behavior;
typedef Behavior* BehaviorPtr;



class Tower :public Behavior
{
public:
	Tower(cTowerType _type);
	Tower(const Tower& rhs);

	~Tower();
	BehaviorPtr Clone() const override;
	void Init();
	void Update(float dt);
	void Exit();
	void CollisionHandler(GameObjectPtr other);
	void Take_Damage(float damage);
	void Stun(float time);
	const cTowerType GetType();

private:

	void play_explosion();
	void idle();
	void do_attack(GameObjectPtr target);
	void do_dead();
	void aim_cannon(GameObjectPtr target);

	bool currently_hurt;
	bool JustDead;
	cTowerType type;
};

