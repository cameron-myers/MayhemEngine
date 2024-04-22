/*****************************************************************//**
 * \file         BehaviorYesButton.cpp
 * \author       Nicholas Leben
 * \par          Email: nick.leben@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/

 /*********************************************************************************************************************/
 /* Includes */
 /*********************************************************************************************************************/
#include "Behavior.h"
#include "BehaviorYesButton.h"
#include "BehaviorButton.h"
#include "Component.h"
#include "Audio.h"
#include "Transform.h"
#include "Collider.h"
#include "MEInput.h"
#include "Log.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include "MEWindow.h"
#include "Engine.h"
#include "LaneManager.h"
#include "MESpaceManager.h"
#include "Wwise_IDs.h"
#include "MEtimer.h"
#include "Animation.h"



//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
MEWindow* window2;
/*!********************************************************************************************************************
	\brief
		Creates a new Button behavior
**********************************************************************************************************************/
YesButton::YesButton()
{
	window2 = Engine::GetWindow();
}

YesButton::YesButton(const YesButton& rhs)
{

}

YesButton::~YesButton()
{
}

BehaviorPtr YesButton::Clone() const
{
	YesButton* button = new YesButton;
	if (button)
	{
		*button = *this;
	}
	return button;
}

void YesButton::Init()
{
	Button::Init();
}

void YesButton::Update(float dt)
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
			//MESpaceManager::SetMenuUI();
			glfwSetWindowShouldClose(window2->GetWindow(), TRUE);
			SetStateNext(cButtonIdle);
		}		break;
	case cButtonDestroyed:
		break;
	}
}

void YesButton::Exit()
{

}
