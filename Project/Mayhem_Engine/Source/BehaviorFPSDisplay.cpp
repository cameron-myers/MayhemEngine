/*****************************************************************//**
 * \file        BehaviorFPSDisplay.cpp
 * \author      Nicholas Leben
 * \par          Email: nick.leben@digipen.edu
 * \par          Course: GAM250
 * \copyright    Copyright (c) 2023 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/
#include "BehaviorFPSDisplay.h"
#include "MEFramerateController.h"
#include "Component.h"
#include "Sprite.h"
#include "MEInput.h"

enum cFPSState { cFPSStateInvalid = -1, cFPSStateIdle };

FPSDisplay::FPSDisplay() : Behavior(cFPSStateInvalid, cFPSStateIdle, "BIG ERROR"), displayFlag(false)
{

}

FPSDisplay::FPSDisplay(const FPSDisplay& rhs) : Behavior(rhs.stateCurr, rhs.stateNext, "BIG ERROR")
{

}

BehaviorPtr FPSDisplay::Clone() const
{
    BehaviorPtr Display = new FPSDisplay;
    if (Display)
    {
        *Display = *this;
    }
    return Display;
}

FPSDisplay::~FPSDisplay()
{

}

void FPSDisplay::Init()
{
    displayFlag = true;
}

void FPSDisplay::Update(float dt)
{
    if (Input::IsKeyReleased(GLFW_KEY_F))
    {
        if (displayFlag)
        {
            displayFlag = false;
        }
        else
        {
            displayFlag = true;
        }
    }

    if (displayFlag)
    {
        Parent()->Has(Sprite)->SetAlpha(0.0f);
    }
    else
    {
        Parent()->Has(Sprite)->SetAlpha(1.0f);
    }

	std::string number(std::to_string(static_cast<int>(MEFramerateController::GetFPS())));
	this->Parent()->GetComponent<Sprite>(cSprite)->SetText(number);

}

void FPSDisplay::Exit()
{

}
