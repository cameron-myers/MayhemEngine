/*****************************************************************//**
 * \file         BehaviorNoButton.cpp
 * \author       Ethan Mclaughlin
 * \par          Email: ethan.mclaughlin\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/

 /*********************************************************************************************************************/
 /* Includes */
 /*********************************************************************************************************************/
#include "Behavior.h"
#include "BehaviorNoButton.h"
#include "BehaviorButton.h"
#include "Component.h"
#include "Audio.h"
#include "Transform.h"
#include "Collider.h"
#include "MEInput.h"
#include "Log.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>

#include "LaneManager.h"
#include "MESpaceManager.h"
#include "Wwise_IDs.h"
#include "MESpace.h"
#include "Animation.h"


//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

/*!********************************************************************************************************************
	\brief
		Creates a new Button behavior
**********************************************************************************************************************/
NoButton::NoButton()
{

}

NoButton::NoButton(const NoButton& rhs)
{

}

NoButton::~NoButton()
{
}

BehaviorPtr NoButton::Clone() const
{
	NoButton* button = new NoButton;
	if (button)
	{
		*button = *this;
	}
	return button;
}

void NoButton::Init()
{
	Button::Init();
}

void NoButton::Update(float dt)
{
	MESpace* spac;

	if (MESpaceManager::GetSpace("AreUSure"))
	{
		spac = MESpaceManager::GetSpace("AreUSure");
	}
	else if (MESpaceManager::GetSpace("AreUSureMain"))
	{
		spac = MESpaceManager::GetSpace("AreUSureMain");
	}
	else
	{
		spac = MESpaceManager::GetSpace("MenuUI");
	}

	Button::Update(dt);
	if (!METimerManager::check(click_timer) && click_timer != nullptr)
	{
		stateCurr = cButtonPressed;
	}
	switch (stateCurr)
	{
	case cButtonIdle:
		Parent()->Has(Animation)->SetRowForMultiAnimation(0);
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
		Parent()->Has(Animation)->SetRowForMultiAnimation(1);
		if(justhover)
		{
			audio->PlaySoundEffect(SOUND(PLAY_BUTTONHOVER_MENU));
			justhover = false;
		}
		if (Input::IsMousePressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			audio->PlaySoundEffect(SOUND(PLAY_BUTTONCLICK_01));
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

		Parent()->Has(Animation)->SetRowForMultiAnimation(2);
		if (METimerManager::check(click_timer) || click_timer == nullptr)
		{
			click_timer = METimerManager::MakeTimer(pressed_time);
			spac->SetActive(false);

			if (MESpaceManager::GetSpace("PauseMenu"))
			{
				MESpaceManager::GetSpace("PauseMenu")->SetActive(true);
			}
			else if (MESpaceManager::GetSpace("MenuUI"))
			{
				MESpaceManager::GetSpace("MenuUI")->SetActive(true);
			}
			else
			{
				MESpaceManager::SetSpaceExclusive("MenuUI");
			}

			SetStateNext(cButtonIdle);
		}		
		break;
	case cButtonDestroyed:
		break;
	}
}

void NoButton::Exit()
{

}


