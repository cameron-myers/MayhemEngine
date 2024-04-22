/*****************************************************************//**
 * \file         BehaviorCreditReturnButton.cpp
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
#include "BehaviorCreditReturnButton.h"
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
#include "MESpace.h"
#include "Audio.h"
#include "MEAudio.h"
#include "MEInput.h"


//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

/*!********************************************************************************************************************
	\brief
		Creates a new Button behavior
**********************************************************************************************************************/
CreditReturn::CreditReturn()
{

}

CreditReturn::CreditReturn(const CreditReturn& rhs)
{

}

CreditReturn::~CreditReturn()
{
}

BehaviorPtr CreditReturn::Clone() const
{
	CreditReturn* button = new CreditReturn;
	if (button)
	{
		*button = *this;
	}
	return button;
}

void CreditReturn::Init()
{
	Button::Init();
}

void CreditReturn::Update(float dt)
{
	if (Input::IsKeyPressed(GLFW_KEY_ESCAPE))
	{
		MESpaceManager::GetSpace("CreditMenu")->GetActive()->GetObjectByName("creditMenuBackground")->Has(Audio)->StopAudioAll();
		MESpaceManager::GetSpace("Sandbox")->GetActive()->GetObjectByName("GameBackground")->Has(Audio)->PlayAudio(SOUND(PLAY_LEVEL_MUSIC));
		MESpaceManager::Remove("CreditMenu");
		MESpaceManager::GetSpace("PauseMenu")->SetActive(true);
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

		Parent()->Has(Animation)->SetRowForMultiAnimation(0);

		if (METimerManager::check(click_timer) || click_timer == nullptr)
		{
			//MESpaceManager::SetMenuUI();
			click_timer = METimerManager::MakeTimer(pressed_time);
			MESpaceManager::GetSpace("CreditMenu")->GetActive()->GetObjectByName("creditMenuBackground")->Has(Audio)->StopAudioAll();
			MESpaceManager::GetSpace("Sandbox")->GetActive()->GetObjectByName("GameBackground")->Has(Audio)->PlayAudio(SOUND(PLAY_LEVEL_MUSIC));
			MESpaceManager::Remove("CreditMenu");
			MESpaceManager::GetSpace("PauseMenu")->SetActive(true);
			SetStateNext(cButtonIdle);
		}
		break;
	case cButtonDestroyed:
		break;
	}
}

void CreditReturn::Exit()
{
}
