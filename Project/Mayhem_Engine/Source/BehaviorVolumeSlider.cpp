/*****************************************************************//**
 * \file		BehaviorVolumeSlider.cpp
 * \author		Nicholas Leben
 * \par          Email:	nick.leben@digipen.edu
 * \par          Course: GAM250
 * \copyright    Copyright (c) 2023 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/
#include "BehaviorVolumeSlider.h"
#include "MEInput.h"
#include "MEAudio.h"
#include "MESpaceManager.h"
#include "Behavior.h"
#include "BehaviorButton.h"
#include "Sprite.h"

VolumeSlider::VolumeSlider()
{

}

VolumeSlider::VolumeSlider(const VolumeSlider& rhs) : Button()
{

}

VolumeSlider::~VolumeSlider()
{

}

BehaviorPtr VolumeSlider::Clone() const
{
	VolumeSlider* button = new VolumeSlider;
	if (button)
	{
		*button = *this;
	}
	return button;
}

void VolumeSlider::Init()
{
	Button::Init();
}

void VolumeSlider::Update(float dt)
{
	Button::Update(dt);
	switch (stateCurr)
	{
	case cButtonIdle:
		if (Collision())
		{
			SetStateNext(cButtonHovering);
		}
		else
		{
			SetStateNext(cButtonIdle);
		}

		break;
	case cButtonHovering:
			if ((Input::IsMouseHeld(GLFW_MOUSE_BUTTON_LEFT)))
			{
				if ((Input::GetMouseY() < Parent()->GetParent()->Has(Transform)->GetTranslation().y + (Parent()->GetParent()->Has(Transform)->GetScale().y*.4))
					&&(Input::GetMouseY() > Parent()->GetParent()->Has(Transform)->GetTranslation().y - (Parent()->GetParent()->Has(Transform)->GetScale().y / 6)))
				{
					Parent()->Has(Transform)->SetTranslation(glm::vec3(Parent()->Has(Transform)->GetRelativeTran().x, Input::GetMouseY(), 0));
				}

				MEAudio::SetMusicVolume((Parent()->Has(Transform)->GetRelativeTran().y/ 294)*100);
				MEAudio::SetSFXVolume((Parent()->Has(Transform)->GetRelativeTran().y / 294) * 100);
			}
			if (Collision())
			{
				SetStateNext(cButtonHovering);
			}
			else
			{
				SetStateNext(cButtonIdle);
			}

		break;
	case cButtonPressed:
		MEAudio::ToggleMute();
		break;
	case cButtonDestroyed:
		break;
	}
}

void VolumeSlider::Exit()
{

}
