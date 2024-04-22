/*****************************************************************//**
 * \file		Interpolation.cpp
 * \author		Nicholas Leben
 * \par          Email:	nick.leben@digipen.edu
 * \par          Course: GAM250
 * \copyright    Copyright (c) 2023 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/
#include "Interpolation.h"


Interpolation::Interpolation(float val, float time, float input, InterpolType intype): finalVal(val), factor(val/time), Val(input), type(intype), initialVal(input), shouldUpdate(true)
{
	timer = METimerManager::MakeTimer(time);
	if (val == 0.0f)
	{
		factor = 0.4f;
	}
}

void Interpolation::Update(float dt)
{
	if (shouldUpdate)
	{
		if (Val < finalVal)
		{
			if (Val < initialVal)
			{
				sinGoUp = true;
			}

			switch (type)
			{
			case linear:
				Val += factor * dt;
				break;
			case exponential:
				Val *= factor * dt;
				break;
			case sinWave:
				if (sinGoUp)
				{
					Val += factor * dt;
				}
				else
				{
					Val -= factor * dt;
				}
				break;
			default:
				break;
			}
		}
		else if (Val >= finalVal)
		{
			switch (type)
			{
			case linear:
				Val -= factor * dt;
				break;
			case exponential:
				Val /= factor * dt;
				break;
			case sinWave:
				sinGoUp = false;
				Val -= factor * dt;
				break;
			default:
				break;
			}
		}
	}

}
 