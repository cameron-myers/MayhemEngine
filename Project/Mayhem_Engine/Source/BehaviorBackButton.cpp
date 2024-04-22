/*****************************************************************//**
 * \file         BehaviorBackButton.cpp
 * \author       Nicholas Leben
 * \par          Email: nick.leben@digipen.edu
 * \par          Course: GAM250
 * \copyright    Copyright (c) 2023 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/
#include "BehaviorBackButton.h"
#include "MEInput.h"
#include "MEAudio.h"
#include "MESpaceManager.h"
#include "Behavior.h"
#include "BehaviorButton.h"
#include "Animation.h"
#include "MEtimer.h"

BackButton::BackButton()
{

}

BackButton::BackButton(const BackButton& rhs)
{

}

BackButton::~BackButton()
{

}

BehaviorPtr BackButton::Clone() const
{
	BackButton* button = new BackButton;
	if (button)
	{
		*button = *this;
	}
	return button;
}

void BackButton::Init()
{
	Button::Init();
}

void BackButton::Update(float dt)
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
		//hover sound
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
		Parent()->Has(Animation)->SetRowForMultiAnimation(2);
if (METimerManager::check(click_timer) || click_timer == nullptr)
		{
			//MESpaceManager::SetSandbox();
			click_timer = METimerManager::MakeTimer(pressed_time);
			MESpaceManager::SetSpacePrevExclusive();
			SetStateNext(cButtonIdle);
		}		break;
	case cButtonDestroyed:
		break;
	}
}

void BackButton::Exit()
{

}

