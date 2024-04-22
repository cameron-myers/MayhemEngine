/*****************************************************************//**
 * \file		BehaviorEmitterTest.cpp
 * \author		Nate White
 * \par          Email:	nate.white@digpen.edu
 * \par          Course: GAM250
 * \copyright    Copyright (c) 2023 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/
#include "BehaviorEmitterTest.h"
#include "MEInput.h"
#include "MEAudio.h"
#include "MESpaceManager.h"
#include "Behavior.h"
#include "Component.h"
#include "ParticleSystem.h"
#include "GameObject.h"
#include "EmitterBehavior.h"
#include <random>
typedef enum cEmitterState { cEmitterInvalid = -1, cEmitterIdle,cEmitterSpawn } EmitterStates;
EmitterTest::EmitterTest():EmitterBehavior(cEmitterInvalid, cEmitterIdle, "BIG ERROR")
{
}

EmitterTest::EmitterTest(const EmitterTest& rhs):EmitterBehavior(rhs.stateCurr, rhs.stateNext, "BIG ERROR")
{

}

EmitterTest::~EmitterTest()
{

}
EmitterBehaviorPtr EmitterTest::Clone() const
{
	EmitterTest* emitterTest = new EmitterTest;
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

void EmitterTest::Init()
{

}

void EmitterTest::Update(float dt)
{
	Emitter* emitter = Parent()->Has(Emitter);
	if (counter >= 1.0f/emitter->GetSpawnRate())
	{
		if(1.0f/ emitter->GetSpawnRate()<dt)
		{
			for(float i = 1.0f / emitter->GetSpawnRate(); i<=dt && !emitter->IsFull();i+= (1.0f / emitter->GetSpawnRate()))
			{
				emitter->SetAcceleration(emitter->RandomAcceleration());
				emitter->spawn(1);
				if (emitter->GetPatternType() == "Rotate")
				{
					emitter->Parent()->Has(Transform)->SetRotation(emitter->Parent()->Has(Transform)->GetRotation() + 5);
				}
			}
		}
		else
		{
			emitter->SetAcceleration(emitter->RandomAcceleration());
			emitter->spawn(1);
			counter = 0;
			if (emitter->GetPatternType() == "Rotate")
			{
				emitter->Parent()->Has(Transform)->SetRotation(emitter->Parent()->Has(Transform)->GetRotation() + 5);
			}
		}
		
	}
	else 
	{
		counter += dt;
	}
	
}

void EmitterTest::Exit()
{

}

