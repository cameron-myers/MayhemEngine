/*****************************************************************//**
 * \file		BehaviorSettingsButton.cpp
 * \author		Nicholas Leben
 * \par          Email:	nick.leben@gmail.com	
 * \par          Course: GAM250
 * \copyright    Copyright (c) 2023 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/
#include "BehaviorSettingsButton.h"
#include "MEInput.h"
#include "MEAudio.h"
#include "MESpaceManager.h"
#include "Behavior.h"
#include "BehaviorButton.h"
#include "Animation.h"
#include "MEtimer.h"

SettingsButton::SettingsButton()
{

}

SettingsButton::SettingsButton(const SettingsButton& rhs) : Button()
{

}

SettingsButton::~SettingsButton()
{

}

BehaviorPtr SettingsButton::Clone() const
{
	SettingsButton* button = new SettingsButton;
	if (button)
	{
		*button = *this;
	}
	return button;
}

void SettingsButton::Init()
{
	Button::Init();
}

void SettingsButton::Update(float dt)
{
	Button::Update(dt);

	if (!METimerManager::check(click_timer) && click_timer != nullptr)
	{
		stateCurr = cButtonPressed;
	}

	switch (stateCurr)
	{
	case cButtonIdle:
		this->Parent()->Has(Animation)->SetRowForMultiAnimation(0);
		//SetScale(oldTransform.GetScale());
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
			audio->PlaySoundEffect(SOUND(PLAY_BUTTONHOVER_MENU));
			justhover = false;
		}
		//SetScale(oldTransform.GetScale() * 1.05f);

		if (Input::IsMousePressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			audio->PlaySoundEffect(SOUND(PLAY_BUTTONCLICK_01));
		}

		if (Input::IsMouseHeld(GLFW_MOUSE_BUTTON_LEFT))
		{

			//SetScale(0.9f);
		}

		if (Input::IsMouseReleased(GLFW_MOUSE_BUTTON_LEFT))
		{
			SetStateNext(cButtonPressed);
			this->Parent()->Has(Animation)->SetRowForMultiAnimation(1);

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
			click_timer = METimerManager::MakeTimer(pressed_time);
			//MESpaceManager::SetSandbox();
			MESpaceManager::SetSpaceExclusive("PauseMenu");
			SetStateNext(cButtonIdle);
		}
		break;
	case cButtonDestroyed:
		break;
	}
}

void SettingsButton::Exit()
{

}
