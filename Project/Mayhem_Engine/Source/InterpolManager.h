/*****************************************************************//**
 * \file		InterpolManager.h
 * \author		Nicholas Leben
 * \par          Email:	nick.leben@digipen.edu
 * \par          Course: GAM250
 * \copyright    Copyright (c) 2023 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/
#include "Interpolation.h"
#include "Transform.h"

#pragma once

class InterpolManager
{
public:
	static Interpolation* MakeInterpolation(float valFinal, float time, float input, InterpolType type);
	static void Init();
	static void Update(float dt);
	static void Push(Interpolation* inter);
};
