/*****************************************************************//**
 * \file		BehaviorEmitterExplosion.cpp
 * \author		Nate White
 * \par          Email: nate.white@digipen.edu
 * \par          Course: GAM250
 * \copyright    Copyright (c) 2023 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/
#include "BehaviorEmitterExplosion.h"
#include "MEInput.h"
#include "MEAudio.h"
#include "MESpaceManager.h"
#include "Behavior.h"
#include "Component.h"
#include "ParticleSystem.h"
#include "GameObject.h"
#include "EmitterBehavior.h"
#include "Stats.h"
#include <random>
typedef enum cEmitterState { cEmitterInvalid = -1, cEmitterIdle,cEmitterSpawn } EmitterStates;
EmitterExplosion::EmitterExplosion():EmitterBehavior(cEmitterInvalid, cEmitterIdle, "BIG ERROR")
{
}

EmitterExplosion::EmitterExplosion(const EmitterExplosion& rhs):EmitterBehavior(rhs.stateCurr, rhs.stateNext, "BIG ERROR")
{

}

EmitterExplosion::~EmitterExplosion()
{

}
EmitterBehaviorPtr EmitterExplosion::Clone() const
{
	EmitterExplosion* emitterExplosion = new EmitterExplosion;
	if (emitterExplosion)
	{
		*emitterExplosion = *this;
		emitterExplosion->stateCurr = cEmitterInvalid;
		emitterExplosion->stateNext = cEmitterIdle;
	}
	return emitterExplosion;
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

void EmitterExplosion::Init()
{

}

void EmitterExplosion::Update(float dt)
{
	Emitter* emitter = Parent()->Has(Emitter);

	if(this->Parent()->HasParent())
	{
		if(this->Parent()->GetParent()->GetComponent<Stats>(cStats)->GetIsHurt())
		{
			for(int i = 0; i<1;++i)
			{
				emitter->SetAcceleration(emitter->RandomAcceleration());
				emitter->spawn(1);
			}
		}
	}
	 //spawn a shit ton of particles in a circle
	

//DO NOT CALL EMITTER UPDATE AS IT IS ALREADY CALLED AS A OVERLOADED COMPONENT FUNCTION
}

void EmitterExplosion::Exit()
{

}

