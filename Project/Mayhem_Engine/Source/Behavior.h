/*****************************************************************//**
 * \file         Behavior.h
 * \author       Cameron Myers, Nate White, Ethan Mclaughlin
 * \par          Email: cameron.myers\@digipen.edu, nate.white\@digipen.edu, ethan.mclaughlin\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#pragma once

/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/
#include "GameObject.h"
#include "Component.h"
#include <iostream>

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef GameObject* GameObjectPtr;
typedef Behavior* BehaviorPtr;
class Component;
//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class Behavior :public Component
{
public:
	Behavior(int curr, int next, std::string) :Component(cBehavior), Name(), stateCurr(curr), stateNext(next) {};
	Behavior(const Behavior& rhs) :Component(cBehavior) { stateCurr = rhs.stateCurr; stateNext = rhs.stateNext; };
	virtual ~Behavior() { Exit(); };
	virtual Behavior* Clone()const = 0;
	virtual void Init(){};
	virtual void Update(float dt) { UNREFERENCED_PARAMETER(dt); stateCurr = stateNext; };
	virtual void Exit(){};
	virtual void CollisionHandler(GameObjectPtr other)= 0;
	//void Read(const char* path);
	int GetStateCurr();
	int GetStateNext();
	void SetStateCurr(int stateCurr);
	void SetStateNext(int stateNext);

public:
	std::string Name;
	int stateCurr;
	int stateNext;

};
