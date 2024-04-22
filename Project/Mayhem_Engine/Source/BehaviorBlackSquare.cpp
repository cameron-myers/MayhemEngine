/*****************************************************************//**
 * \file		BehaviorBlackSquare.cpp
 * \author		Nicholas Leben
 * \par          Email: nick.leben@digipen.edu
 * \par          Course: GAM250
 * \copyright    Copyright (c) 2023 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/
#include "BehaviorBlackSquare.h"
#include "Sprite.h"
#include "Behavior.h"
#include "Component.h"

BehaviorBlackSquare::BehaviorBlackSquare():Behavior(-1, 0, "BIG ERROR")
{

}

BehaviorBlackSquare::BehaviorBlackSquare(const BehaviorBlackSquare& rhs) :Behavior(-1, 0, "BIG ERROR")
{

}

BehaviorBlackSquare::~BehaviorBlackSquare()
{

}

BehaviorPtr BehaviorBlackSquare::Clone() const
{
	BehaviorPtr behav = new BehaviorBlackSquare;
	if (behav)
	{
		*behav = *this;
	}
	return behav;
}

void BehaviorBlackSquare::Init()
{
	if (Parent()->Has(Sprite)->GetAlpha() == 0.0f)
	{
		interpol = InterpolManager::MakeInterpolation(1.0f, 1.0f, Parent()->Has(Sprite)->GetAlpha(), linear);
	}
	else
	{
		interpol = InterpolManager::MakeInterpolation(0.0f, 1.0f, Parent()->Has(Sprite)->GetAlpha(), linear);
	}
}

void BehaviorBlackSquare::Update(float dt)
{
	if (interpol->getPause())
	{
		Parent()->Has(Sprite)->SetAlpha(interpol->getInval());
	}
}

void BehaviorBlackSquare::Exit()
{

}

void BehaviorBlackSquare::PauseInterpol()
{
	interpol->Pause();
}
