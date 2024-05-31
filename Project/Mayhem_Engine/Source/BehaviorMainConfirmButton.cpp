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
#include "BehaviorMainConfirmButton.h"
#include "BehaviorButton.h"
#include "Component.h"
#include "Audio.h"
#include "Collider.h"
#include "MEInput.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>

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
MainConfirm::MainConfirm()
{

}

MainConfirm::MainConfirm(const MainConfirm& rhs)
{

}

MainConfirm::~MainConfirm()
{
}

BehaviorPtr MainConfirm::Clone() const
{
	MainConfirm* button = new MainConfirm;
	if (button)
	{
		*button = *this;
	}
	return button;
}

void MainConfirm::Init()
{
	Button::Init();
}

void MainConfirm::Update(float dt)
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
			MESpaceManager::SetSpaceInclusive("AreUSureMain");
			MESpaceManager::MoveSpaceToBack("AreUSureMain");
			SetStateNext(cButtonIdle);
		}
		break;
	case cButtonDestroyed:
		break;
	}
}

void MainConfirm::Exit()
{

}
