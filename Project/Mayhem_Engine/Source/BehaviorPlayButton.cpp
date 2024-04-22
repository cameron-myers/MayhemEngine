/*****************************************************************//**
 * \file         BehaviorPlayButton.cpp
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
#include "BehaviorPlayButton.h"
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
#include "Sprite.h"
#include "BehaviorBlackSquare.h"
#include "Animation.h"
#include "BehaviorGoldCount.h"


//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

/*!********************************************************************************************************************
	\brief
		Creates a new Button behavior
**********************************************************************************************************************/
PlayButton::PlayButton()
{

}
/// <summary>
/// copy constructor
/// </summary>
/// <param name="rhs"></param>
PlayButton::PlayButton(const PlayButton& rhs)
{

}
/// <summary>
/// destructor
/// </summary>
PlayButton::~PlayButton()
{
}
/// <summary>
/// 
/// </summary>
/// <returns>new behavior that is clone</returns>
BehaviorPtr PlayButton::Clone() const
{
	PlayButton* button = new PlayButton;
	if (button)
	{
		*button = *this;
	}
	return button;
}
/// <summary>
/// initialize button
/// </summary>
void PlayButton::Init()
{
	Button::Init();
	flag = false;
}
/// <summary>
/// update button
/// </summary>
/// <param name="dt"></param>
void PlayButton::Update(float dt)
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
		SetScale(oldTransform.GetScale() * 1.05f);

		if (Input::IsMousePressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			audio->PlaySoundEffect(SOUND(PLAY_BUTTONCLICK_01));
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
		if (METimerManager::check(click_timer) || click_timer == nullptr)
		{
			Parent()->Has(Animation)->SetRowForMultiAnimation(2);

			interpol = InterpolManager::MakeInterpolation(1.0f, 1.0f, 0.0f, linear);

			if (MESpaceManager::GetSpace("MenuUI"))
			{
				click_timer = METimerManager::MakeTimer(pressed_time);
				interpol = InterpolManager::MakeInterpolation(1.0f, 1.0f, 0.0f, linear);

				if (MESpaceManager::GetSpace("MenuUI"))
				{
					MESpace* spac = MESpaceManager::GetSpace("MenuUI");

					if (spac->GetActive())
					{
						if (spac->GetActive()->GetObjectByName("BlackSquare"))
						{
							if (interpol)
							{
								GameObject* obj = spac->GetActive()->GetObjectByName("BlackSquare");
								BehaviorBlackSquare* behav = static_cast<BehaviorBlackSquare*>(obj->Has(Behavior));
								behav->PauseInterpol();
							}
						}
					}
				}

				SetStateNext(cButtonIdle);
			}
		}
		break;
	case cButtonDestroyed:
		break;
	}

	if (flag)
	{
		MESpaceManager::GetSpace("MenuUI")->GetActive()->GetObjectByName("MainMenuBackground")->Has(Audio)->StopAudioAll();
		GoldCount::ResetGold();
		MESpaceManager::SetSpaceExclusive("Sandbox");
	}

	if (MESpaceManager::GetSpace("MenuUI"))
	{
		MESpace* spac = MESpaceManager::GetSpace("MenuUI");

		if (spac->GetActive())
		{
			if (spac->GetActive()->GetObjectByName("BlackSquare"))
			{
				if (interpol)
				{
					GameObject* obj = spac->GetActive()->GetObjectByName("BlackSquare");
					obj->Has(Sprite)->SetAlpha(interpol->getInval());

					if (interpol->getInval() >= 1.0f)
					{
						MESpaceManager::SetSpaceInclusive("Loading1");
						flag = true;
					}
				}
			}
		}
	}
	
}
/// <summary>
/// does nothing
/// </summary>
void PlayButton::Exit()
{
}
