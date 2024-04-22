/*****************************************************************//**
 * \file		BehaviorVolumeToggle.cpp
 * \author		Cameron Myers
 * \par          Email:	cameron.myers@digipen.edu
 * \par          Course: GAM250
 * \copyright    Copyright (c) 2023 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/
#include "BehaviorVolumeToggle.h"
#include "MEInput.h"
#include "MEAudio.h"
#include "MESpaceManager.h"
#include "Behavior.h"
#include "BehaviorButton.h"
#include "MEtimer.h"
#include "Sprite.h"

VolumeToggle::VolumeToggle()
{

}

VolumeToggle::VolumeToggle(const VolumeToggle& rhs) : Button()
{

}

VolumeToggle::~VolumeToggle()
{

}

BehaviorPtr VolumeToggle::Clone() const
{
	VolumeToggle* button = new VolumeToggle;
	if (button)
	{
		*button = *this;
	}
	return button;
}

void VolumeToggle::Init()
{
	Button::Init();
}

void VolumeToggle::Update(float dt)
{
	Button::Update(dt);
	if (!METimerManager::check(click_timer) && click_timer != nullptr)
	{
		stateCurr = cButtonPressed;
	}
	switch (stateCurr)
	{
	case cButtonIdle:
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
			/*if ((Input::IsMouseHeld(GLFW_MOUSE_BUTTON_LEFT)))
			{
				if ((Input::GetMouseY() < Parent()->GetParent()->Has(Transform)->GetTranslation().y + (Parent()->GetParent()->Has(Transform)->GetScale().y/2))
					&&(Input::GetMouseY() > Parent()->GetParent()->Has(Transform)->GetTranslation().y))
				{
					Parent()->Has(Transform)->SetTranslation(glm::vec3(Parent()->Has(Transform)->GetRelativeTran().x, Input::GetMouseY(), 0));
				}

				MEAudio::SetMusicVolume(Parent()->Has(Transform)->GetRelativeTran().y);
			}*/
			if (justhover)
			{
				audio->PlaySoundEffect(SOUND(PLAY_BUTTONHOVER_MENU));
				justhover = false;
			}
			SetScale(oldTransform.GetScale() * 1.05f);

			if (Input::IsMouseHeld(GLFW_MOUSE_BUTTON_LEFT))
			{

				SetScale(0.9f);
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
			Parent()->Has(Sprite)->SetFrame(!MEAudio::ToggleMute());
		}
		SetStateNext(cButtonIdle);
		break;
	case cButtonDestroyed:
		break;
	}
}

void VolumeToggle::Exit()
{

}
