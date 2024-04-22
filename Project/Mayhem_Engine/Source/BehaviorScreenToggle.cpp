/*****************************************************************//**
 * \file		BehaviorScreenToggle.cpp
 * \author		Cameron Myers
 * \par          Email:	cameron.myers@digipen.edu
 * \par          Course: GAM250
 * \copyright    Copyright (c) 2023 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/
#include "BehaviorScreenToggle.h"
#include "MEInput.h"
#include "MEAudio.h"
#include "MESpaceManager.h"
#include "Behavior.h"
#include "BehaviorButton.h"
#include "MEtimer.h"
#include "Sprite.h"

enum check_box_states
{
	cIdleState = 0,
	cHoverState,
	cClickedState,
	cCheckedState

};


ScreenToggle::ScreenToggle()
{

}

ScreenToggle::ScreenToggle(const ScreenToggle& rhs) : Button()
{

}

ScreenToggle::~ScreenToggle()
{

}

BehaviorPtr ScreenToggle::Clone() const
{
	ScreenToggle* button = new ScreenToggle;
	if (button)
	{
		*button = *this;
	}
	return button;
}

void ScreenToggle::Init()
{
	Button::Init();
}

void ScreenToggle::Update(float dt)
{
	checked = !MEWindow::IsFullscreen();
	Button::Update(dt);
	if (!METimerManager::check(click_timer) && click_timer != nullptr)
	{
		stateCurr = cButtonPressed;
	}

	if (checked)
	{
		Parent()->Has(Sprite)->SetFrame(cCheckedState);
	}
	else
	{
		Parent()->Has(Sprite)->SetFrame(cIdleState);

	}

	switch (stateCurr)
	{
	case cButtonIdle:
		SetScale(oldTransform.GetScale());
		if (checked)
		{
			Parent()->Has(Sprite)->SetFrame(cCheckedState);
		}

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
		
			if(!checked) Parent()->Has(Sprite)->SetFrame(cHoverState);
			if (justhover)
			{
				audio->PlaySoundEffect(SOUND(PLAY_BUTTONHOVER_MENU));

				justhover = false;
			}
			if(checked)
			{
				SetScale(oldTransform.GetScale() * 1.05f);
			}

			if (Input::IsMouseHeld(GLFW_MOUSE_BUTTON_LEFT))
			{

				SetScale(0.95f);
			}
			if(Input::IsMousePressed(GLFW_MOUSE_BUTTON_LEFT))
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
			click_timer = METimerManager::MakeTimer(pressed_time-0.1f);
			Parent()->Has(Sprite)->SetFrame(cClickedState);
			MEWindow::ToggleFullscreen();
		}
		SetStateNext(cButtonIdle);
		break;
	case cButtonDestroyed:
		break;
	}
}

void ScreenToggle::Exit()
{

}
