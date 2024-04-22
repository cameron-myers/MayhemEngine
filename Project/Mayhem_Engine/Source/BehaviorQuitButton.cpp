/*****************************************************************//**
 * \file         BehaviorQuitButton.cpp
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
#include "BehaviorQuitButton.h"
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
#include "Animation.h"


//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

/*!********************************************************************************************************************
	\brief
		Creates a new Button behavior
**********************************************************************************************************************/
QuitButton::QuitButton()
{

}

QuitButton::QuitButton(const QuitButton& rhs)
{

}

QuitButton::~QuitButton()
{
}

BehaviorPtr QuitButton::Clone() const
{
	QuitButton* button = new QuitButton;
	if (button)
	{
		*button = *this;
	}
	return button;
}

void QuitButton::Init()
{
	Button::Init();
}

void QuitButton::Update(float dt)
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

		if(justhover)
		{
			audio->PlaySoundEffect(SOUND(PLAY_BUTTONHOVER_MENU));
			justhover = false;
		}
		Parent()->Has(Animation)->SetRowForMultiAnimation(1);
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
			//MESpaceManager::SetMenuUI();
			click_timer = METimerManager::MakeTimer(pressed_time);
			audio->PlaySoundEffect(SOUND(PLAY_BUTTONCLICK_02));
			MESpaceManager::SetSpaceInclusive("AreUSure");
			MESpaceManager::MoveSpaceToBack("AreUSure");
			SetStateNext(cButtonIdle);
		}
		break;
	case cButtonDestroyed:
		break;
	}
}

void QuitButton::Exit()
{

}
