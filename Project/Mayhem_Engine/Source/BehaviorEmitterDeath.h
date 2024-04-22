/*****************************************************************//**
 * \file         EmitterDeathBehavior.h
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

class EmitterDeath : public EmitterBehavior
{
public:
	EmitterDeath();
	EmitterDeath(const EmitterDeath& rhs);
	~EmitterDeath() override;
	EmitterBehaviorPtr Clone() const override;
	void Init()override;
	void Update(float dt)override;
	void Exit()override;
	void Read(const char* path);
	void CollisionHandler(GameObjectPtr other) {};
private:
	float counter;
	bool CanSpawn = true;
};

