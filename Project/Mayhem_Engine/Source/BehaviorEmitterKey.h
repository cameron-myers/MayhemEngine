/*****************************************************************//**
 * \file         EmitterKeyBehavior.h
 * \author       Nate White
 * \par          Email: nate.white@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/
#pragma once

#pragma once
#include "Behavior.h"
#include "Transform.h"
#include "BehaviorButton.h"
#include <glm/vec3.hpp>
#include "ParticleSystem.h"
#include "EmitterBehavior.h"
class Transform;
class Behavior;
class Audio;
typedef EmitterBehavior* EmitterBehaviorPtr;

class EmitterKey : public EmitterBehavior
{
public:
	EmitterKey();
	EmitterKey(const EmitterKey& rhs);
	~EmitterKey() override;
	EmitterBehaviorPtr Clone() const override;
	void Init()override;
	void Update(float dt)override;
	void Exit()override;
	void Read(const char* path);
	void CollisionHandler(GameObjectPtr other) {};
private:
	float counter;
};

