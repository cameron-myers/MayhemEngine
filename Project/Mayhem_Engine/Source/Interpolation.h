/*****************************************************************//**
 * \file		Interpolation.h
 * \author		Nicholas Leben
 * \par          Email:	nick.leben@digipen.edu
 * \par          Course: GAM250
 * \copyright    Copyright (c) 2023 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/
#include "MEtimer.h"
#include "Transform.h"

#pragma once

typedef enum InterpolType
{
	linear,
	exponential,
	sinWave
}InterpolType;

class Interpolation
{
public:

	Interpolation():finalVal(0), timer(nullptr), factor(0), Val(0), type(linear), sinGoUp(true),initialVal(0), shouldUpdate(true){}
	
	Interpolation(float valFinal, float time, float input, InterpolType intype);
	
	~Interpolation()
	{
		delete timer;
		timer = NULL;
	}

	void Update(float dt);
	float getInval() { return Val; };
	bool getPause() { return shouldUpdate; };
	void Pause() { shouldUpdate = false; };

private:
	float finalVal;
	Timer* timer;
	float factor;
	float Val;
	InterpolType type;
	bool sinGoUp;
	float initialVal;
	bool shouldUpdate;
};


