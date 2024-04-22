/*****************************************************************//**
 * \file		BehaviorResumeButton.cpp
 * \author		Nicholas Leben
 * \par          Email:	nick.leben@gmail.com
 * \par          Course: GAM250
 * \copyright    Copyright (c) 2023 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/
#include "BehaviorResumeButton.h"
#include "MEInput.h"
#include "MEAudio.h"
#include "MESpaceManager.h"
#include "Behavior.h"
#include "BehaviorButton.h"
#include "MEtimer.h"
#include "Animation.h"
#include "MESpace.h"

ResumeButton::ResumeButton()
{

}

ResumeButton::ResumeButton(const ResumeButton& rhs): Button()
{
	
}

ResumeButton::~ResumeButton()
{

}

BehaviorPtr ResumeButton::Clone() const
{
	ResumeButton* button = new ResumeButton;
	if (button)
	{
		*button = *this;
	}
	return button;
}

void ResumeButton::Init()
{
	Button::Init();
}

void ResumeButton::Update(float dt)
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
		Parent()->Has(Animation)->SetRowForMultiAnimation(2);
		if (METimerManager::check(click_timer) || click_timer == nullptr)
		{
			click_timer = METimerManager::MakeTimer(pressed_time);
			METimerManager::GlobalUnpause();
			//MESpaceManager::SetSandbox();
			//MESpaceManager::GetSpace("Sandbox")->GetActive()->GetObjectByName("GameBackground")->Has(Audio)->PlayAudio(SOUND(PLAY_LEVEL_MUSIC));
			MESpaceManager::SetSpaceExclusive("Sandbox");
			SetStateNext(cButtonIdle);
		}		break;
	case cButtonDestroyed:
		break;
	}
}

void ResumeButton::Exit()
{

}
