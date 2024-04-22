/*****************************************************************//**
 * \file         Component.h
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/

#pragma once

#define UNREFERENCED_PARAMETER(P) (P)

class GameObject;
typedef GameObject* GameObjectPtr;

typedef enum TypeEnum
{
	cTypeInvalid = -1,
	cTransform,
	cSprite,
	cAnimation,
	cPhysics,
	cCollider,
	cAudio,
	cBehavior,
	cStats,
	cEmitter,
	cEmitterBehavior,
	cCount,
}TypeEnum;

class Component
{
public:
	
	Component() :mType(cTypeInvalid), mParent(nullptr)
	{

	}

	Component(TypeEnum type) :mType(type), mParent(nullptr)
	{

	}
	Component(const Component& rhs):mType(rhs.mType), mParent(nullptr)
	{
		
	}
	virtual ~Component() {}

	virtual void Init() {};

	_inline TypeEnum Type() const { return mType; }

	_inline void Parent(GameObjectPtr parent) { mParent = parent; }

	GameObjectPtr Parent() const { return mParent; }

	virtual Component* Clone() const { return nullptr;  };

	virtual void Update(float dt) { UNREFERENCED_PARAMETER(dt); }

	virtual void Draw() const {};

	virtual void Read() const {};

	virtual void Save() const {};


private:
	TypeEnum mType;
	GameObjectPtr mParent;
};
