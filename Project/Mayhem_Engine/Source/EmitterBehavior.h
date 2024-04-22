/*****************************************************************//**
 * \file         EmitterBehavior.h
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

class EmitterBehavior;
//class EmitterBehavior;
typedef GameObject* GameObjectPtr;
typedef EmitterBehavior* EmitterBehaviorPtr;
class Component;
//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class EmitterBehavior :public Component
{
public:
	EmitterBehavior(int curr, int next, std::string) :Component(cEmitterBehavior), Name(), stateCurr(curr), stateNext(next) {};
	EmitterBehavior(const EmitterBehavior& rhs) :Component(cEmitterBehavior) { stateCurr = rhs.stateCurr; stateNext = rhs.stateNext; };
	virtual ~EmitterBehavior() { Exit(); };
	virtual EmitterBehavior* Clone()const = 0;
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
