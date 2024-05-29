/*****************************************************************//**
 * \file         BehaviorPlayButton.cpp
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
#include "BehaviorCreditsButton.h"
#include "BehaviorButton.h"
#include "Component.h"
#include "Audio.h"
#include "Transform.h"
#include "Collider.h"
#include "MEInput.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include "MESpaceManager.h"
#include "Wwise_IDs.h"
#include "Animation.h"
#include "MEAudio.h"
#include "GameObjectManager.h"
#include "MESpace.h"


//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

/*!********************************************************************************************************************
	\brief
		Creates a new Button behavior
**********************************************************************************************************************/
CreditButton::CreditButton()
{

}

CreditButton::CreditButton(const CreditButton& rhs)
{

}

CreditButton::~CreditButton()
{
}

BehaviorPtr CreditButton::Clone() const
{
	CreditButton* button = new CreditButton;
	if (button)
	{
		*button = *this;
	}
	return button;
}

void CreditButton::Init()
{
	Button::Init();
}

void CreditButton::Update(float dt)
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
		{
			Parent()->Has(Animation)->SetRowForMultiAnimation(2);
			if (METimerManager::check(click_timer) || click_timer == nullptr)
			{
				//MESpaceManager::SetMenuUI();
				click_timer = METimerManager::MakeTimer(pressed_time);

				MESpaceManager::GetSpace("Sandbox")->GetActive()->GetObjectByName("GameBackground")->Has(Audio)->StopAudioAll();
				MESpaceManager::SetSpaceInclusive("CreditMenu");
			}
			SetStateNext(cButtonIdle);

			break;
		}
		
	case cButtonDestroyed:
		break;
	}


}


void CreditButton::Exit()
{
}
