/*****************************************************************//**
 * \file         ZeppelinBehavior.h
 * \authors       Nate White,Cameron Myers, Ethan Mclaughlin,Nick Leben
 * \par          Email: Nate.white\@digipen.edu,cameron.myers\@digipen.edu, EthanMclaughlin\@digipen.edu, nick.leben\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------
#include "Behavior.h"
#include "LaneManager.h"
class Timer;
class Behavior;
typedef Behavior* BehaviorPtr;
typedef enum cZeppelin { cZeppelinInvalid = -1, cZeppelinIdle, cZeppelinMove, cZeppelinDrag, cZeppelinAttack, cZeppelinDamage, cZeppelinDead } ZeppelinStates;

class Zeppelin : public Behavior
{
public:
	Zeppelin(cZeppelinType _type);
	Zeppelin(const Zeppelin& rhs);

	~Zeppelin();
	Behavior* Clone() const override;
	void Init();
	void Update(float dt);
	void Exit();
	void CollisionHandler(GameObjectPtr other);
	void Take_Damage(int damage);
private:


	void play_explosion();
	void idle();
	void do_dead();
	template <class T>
	void do_attack(GameObjectPtr target);
	void aim_cannon(GameObjectPtr target);

	void SetVelocity(float speed);
	void MouseFollow();
	cZeppelinType type;
	bool JustDead = false;
	Timer* dead_timer = nullptr;
	Timer* spawn_timer = nullptr;



};
