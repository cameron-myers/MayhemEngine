/*****************************************************************//**
 * \file		BehaviorEmitterKey.cpp
 * \author		Nate White
 * \par          Email: nate.white@digipen.edu
 * \par          Course: GAM250
 * \copyright    Copyright (c) 2023 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/
#include "BehaviorEmitterKey.h"
#include "MEInput.h"
#include "MEAudio.h"
#include "MESpaceManager.h"
#include "Behavior.h"
#include "Component.h"
#include "ParticleSystem.h"
#include "GameObject.h"
#include "EmitterBehavior.h"
#include "MEInput.h"
#include <random>
typedef enum cEmitterState { cEmitterInvalid = -1, cEmitterIdle,cEmitterSpawn } EmitterStates;
EmitterKey::EmitterKey():EmitterBehavior(cEmitterInvalid, cEmitterIdle, "BIG ERROR")
{
}

EmitterKey::EmitterKey(const EmitterKey& rhs):EmitterBehavior(rhs.stateCurr, rhs.stateNext, "BIG ERROR")
{

}

EmitterKey::~EmitterKey()
{

}
EmitterBehaviorPtr EmitterKey::Clone() const
{
	EmitterKey* emitterTest = new EmitterKey;
	if (emitterTest)
	{
		*emitterTest = *this;
		emitterTest->stateCurr = cEmitterInvalid;
		emitterTest->stateNext = cEmitterIdle;
	}
	return emitterTest;
}
inline int randomRange(int min, int max)
{
	// Seed the random number generator with a random device
	std::random_device rd;
	std::mt19937 gen(rd());

	// Generate a random number between 1 and 10
	std::uniform_int_distribution<int> dis(min, max);
	return dis(gen);
}

void EmitterKey::Init()
{

}

void EmitterKey::Update(float dt)
{
	if (Input::IsKeyHeld(GLFW_KEY_P))
	{
		Emitter* emitter = Parent()->Has(Emitter);
		if (counter >= 1.0f / emitter->GetSpawnRate())
		{
			if (1.0f / emitter->GetSpawnRate() < dt)
			{
				for (float i = 1.0f / emitter->GetSpawnRate(); i <= dt && !emitter->IsFull(); i += (1.0f / emitter->GetSpawnRate()))
				{
					emitter->SetAcceleration(emitter->RandomAcceleration());
					emitter->spawn(1);
				}
			}
			else
			{
				emitter->SetAcceleration(emitter->RandomAcceleration());
				emitter->spawn(1);
				counter = 0;
			}

		}
		else
		{
			counter += dt;
		}
	}
	 //spawn a shit ton of particles in a circle
	

//DO NOT CALL EMITTER UPDATE AS IT IS ALREADY CALLED AS A OVERLOADED COMPONENT FUNCTION
}

void EmitterKey::Exit()
{

}

