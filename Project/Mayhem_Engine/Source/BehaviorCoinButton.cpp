/*****************************************************************//**
 * \file         BehaviorCoinButton.cpp
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/
#include "Behavior.h"
#include "BehaviorCoinButton.h"
#include "BehaviorButton.h"
#include "Component.h"
#include "Audio.h"
#include "Transform.h"
#include "Collider.h"
#include "MEInput.h"
#include "Log.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkQueryParameters.h>

#include "LaneManager.h"
#include "MESpaceManager.h"
#include "Wwise_IDs.h"


//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

/*!********************************************************************************************************************
	\brief
		Creates a new Button behavior
**********************************************************************************************************************/
CoinButton::CoinButton():Button()
{

}
/// <summary>
/// copy constructor
/// </summary>
/// <param name="rhs"></param>
CoinButton::CoinButton(const CoinButton& rhs):Button()
{

}
/// <summary>
/// destructor
/// </summary>
CoinButton::~CoinButton()
{
}
/// <summary>
/// clone coin button
/// </summary>
/// <returns>new coin button behavior</returns>
BehaviorPtr CoinButton::Clone() const
{
	CoinButton* button = new CoinButton;
	if (button)
	{
		*button = *this;
	}
	return button;
}
/// <summary>
/// initialize the button
/// </summary>
void CoinButton::Init()
{
	Button::Init();
}
/// <summary>
/// updates the coin button state
/// </summary>
/// <param name="dt"></param>
void CoinButton::Update(float dt)
{
	Button::Update(dt);

	GameObject* gold = Parent()->SearchChildByName("GoldText");
	if(gold)
	{
		
	}

	switch (stateCurr)
	{
	case cButtonIdle:
		SetScale(oldTransform.GetScale());
		if (Collision())
		{
			justhover = true;
			SetStateNext(cButtonHovering);
		}
		else
		{
			SetStateNext(cButtonIdle);
		}

		break;
	case cButtonHovering:
		if (justhover)
		{
			audio->PlaySoundEffect(SOUND(PLAY_BUTTONHOVER_LEVEL));
			justhover = false;
		}
		SetScale(oldTransform.GetScale() * 1.05f);
		if (Input::IsMousePressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			audio->PlaySoundEffect(SOUND(PLAY_GOLDPILE));
		}

		if (Input::IsMouseHeld(GLFW_MOUSE_BUTTON_LEFT))
		{

			SetScale(0.9f);
		}

		if (Input::IsMouseReleased(GLFW_MOUSE_BUTTON_LEFT))
		{
			SetStateNext(cButtonPressed);

		}
		else if (Collision())
		{
			SetStateNext(cButtonHovering);
		}
		else
		{
			SetStateNext(cButtonIdle);
		}

		break;
	case cButtonPressed:

		//MESpaceManager::SetSandbox();
		MESpaceManager::SetSpaceExclusive("Sandbox");
		SetStateNext(cButtonIdle);

		break;
	case cButtonDestroyed:
		break;
	}

	
}
/// <summary>
/// this does nothing
/// </summary>
void CoinButton::Exit()
{
}
