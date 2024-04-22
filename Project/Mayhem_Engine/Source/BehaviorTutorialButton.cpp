/*****************************************************************//**
 * \file         BehaviorTutorialButton.cpp
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
#include "BehaviorTutorialButton.h"
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
#include "MEtimer.h"
#include "Wwise_IDs.h"
#include "Animation.h"


//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

/*!********************************************************************************************************************
	\brief
		Creates a new Button behavior
**********************************************************************************************************************/
TutorialButton::TutorialButton():Button()
{

}
/// <summary>
/// copy constructor (DO NOT USE)
/// </summary>
/// <param name="rhs"></param>
TutorialButton::TutorialButton(const TutorialButton& rhs):Button()
{

}
/// <summary>
/// destructor
/// </summary>
TutorialButton::~TutorialButton()
{
}
/// <summary>
/// 
/// </summary>
/// <returns>returns new behavior that is clone</returns>
BehaviorPtr TutorialButton::Clone() const
{
	TutorialButton* button = new TutorialButton;
	if (button)
	{
		*button = *this;
	}
	return button;
}
/// <summary>
/// initialize button
/// </summary>
void TutorialButton::Init()
{
	Button::Init();
}
/// <summary>
/// update tutorial button states
/// </summary>
/// <param name="dt"></param>
void TutorialButton::Update(float dt)
{

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
		if (justhover)
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


		Parent()->Has(Animation)->SetRowForMultiAnimation(2);if (METimerManager::check(click_timer) || click_timer == nullptr)
		{
			click_timer = METimerManager::MakeTimer(pressed_time);
			//MESpaceManager::SetSandbox();
			MESpaceManager::SetSpaceInclusive("Tutorial");
			SetStateNext(cButtonIdle);
		}		break;
	case cButtonDestroyed:
		break;
	}
}
/// <summary>
/// this does nothing
/// </summary>
void TutorialButton::Exit()
{
}
