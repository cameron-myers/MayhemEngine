/*****************************************************************//**
 * \file		InterpolManager.cpp
 * \author		Nicholas Leben
 * \par          Email:	nick.leben@digipen.edu
 * \par          Course: GAM250
 * \copyright    Copyright (c) 2023 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/
#include "InterpolManager.h"
#include "Log.h"

static std::deque<Interpolation*> interpolList;

Interpolation* InterpolManager::MakeInterpolation(float valFinal, float time, float input, InterpolType type)
{
	Interpolation* inter = new Interpolation(valFinal, time, input, type);
	Push(inter);
	return inter;
}

void InterpolManager::Init()
{

}

void InterpolManager::Update(float dt)
{
	for (Interpolation* inter : interpolList)
	{
		inter->Update(dt);
	}
}

void InterpolManager::Push(Interpolation* inter)
{
	interpolList.push_back(inter);
}
