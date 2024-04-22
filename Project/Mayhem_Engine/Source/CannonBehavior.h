/*****************************************************************//**
 * \file         CannonBehavior.h
 * \author       Nate White
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



class Cannon :public Behavior
{
public:
	Cannon();
	Cannon(const Cannon& rhs);
	void CollisionHandler(GameObjectPtr other) {};
	~Cannon();
	BehaviorPtr Clone() const override;
	void Init();
	void Update(float dt);
	void Exit();
	void SetDirectionForChildCannon(Transform* transform);

private:
	bool currentlyhurt;

};

