/*****************************************************************//**
 * \file		BehaviorBlackSquare.h
 * \author		Nicholas Leben
 * \par          Email: nick.leben@digipen.edu
 * \par          Course: GAM250
 * \copyright    Copyright (c) 2023 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/


#pragma once
#include "Behavior.h"
#include "MEtimer.h"
#include "InterpolManager.h"

class Behavior;

#pragma once
class BehaviorBlackSquare : public Behavior
{
public:
	BehaviorBlackSquare();
	BehaviorBlackSquare(const BehaviorBlackSquare& rhs);
	~BehaviorBlackSquare();
	BehaviorPtr Clone() const override;
	void Init();
	void Update(float dt);
	void Exit();
	void CollisionHandler(GameObjectPtr other)override {};
	void PauseInterpol();

private:
	Interpolation* interpol;
};

