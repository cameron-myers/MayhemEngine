/*****************************************************************//**
 * \file		BehaviorRetryButton.cpp
 * \author		Nicholas Leben
 * \par          Email:	nick.leben@digipen.edu
 * \par          Course: GAM250
 * \copyright    Copyright (c) 2023 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/
#include "BehaviorRetryButton.h"
#include "MEInput.h"
#include "MEAudio.h"
#include "MESpaceManager.h"
#include "Behavior.h"
#include "BehaviorButton.h"
#include "MEtimer.h"
#include "Animation.h"
#include "BehaviorGoldCount.h"

RetryButton::RetryButton()
{

}

RetryButton::RetryButton(const RetryButton& rhs) : Button()
{

}

RetryButton::~RetryButton()
{

}

BehaviorPtr RetryButton::Clone() const
{
	RetryButton* button = new RetryButton;
	if (button)
	{
		*button = *this;
	}
	return button;
}

void RetryButton::Init()
{
	Button::Init();
}

void RetryButton::Update(float dt)
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
			GoldCount::ResetGold();
			MESpaceManager::SetSpaceExclusive("Sandbox");
			SetStateNext(cButtonIdle);
		}		break;
	case cButtonDestroyed:
		break;
	}
}

void RetryButton::Exit()
{

}
