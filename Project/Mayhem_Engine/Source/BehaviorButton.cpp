/*********************************************************************************************************************/
/*!
  \file              BehaviorButton.cpp
  \author            Cameron Myers
  \par               Email: cameron.myers\@digipen.edu
  \par               Course: GAM200
  \par               Copyright © 2022 DigiPen, All rights reserved.

  \brief
	this implements Behaviors for the Button game objects
 */
 /*********************************************************************************************************************/

/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/
#include "Behavior.h"
#include "BehaviorButton.h"
#include "Component.h"
#include "Audio.h"
#include "Transform.h"
#include "Collider.h"
#include "MEInput.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>


//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

/*!********************************************************************************************************************
	\brief
		Creates a new Button behavior
**********************************************************************************************************************/
Button::Button():Behavior(cButtonInvalid, cButtonIdle, "BIG ERROR"), oldTransform(Transform()), audio(nullptr), click_timer(nullptr)
{
}

/// <summary>
/// button copy constructor
/// </summary>
/// <param name="rhs"></param>
Button::Button(const Button& rhs):Behavior(rhs.stateCurr, rhs.stateNext, "BIG ERROR")
{
}
/// <summary>
/// destructor
/// </summary>
Button::~Button()
{
}
/// <summary>
/// button clone
/// </summary>
/// <returns>pointer to new button</returns>
BehaviorPtr Button::Clone() const
{
	Button* button = new Button;
	if (button)
	{
		*button = *this;
	}
	return button;
}
/// <summary>
/// initialize button
/// </summary>
void Button::Init()
{
	if (stateCurr == cButtonInvalid)
	{
		SetStateNext(cButtonIdle);
	}
	oldTransform = *Parent()->Has(Transform);
	audio = Parent()->Has(Audio);
}
/// <summary>
/// update button
/// </summary>
/// <param name="dt"></param>
void Button::Update(float dt)
{
	stateCurr = stateNext;
}



/// <summary>
/// this does nothing
/// </summary>
void Button::Exit()
{
}
/// <summary>
/// set the position of the button
/// </summary>
/// <param name="pos"></param>
void Button::SetPosition(glm::vec3 pos)
{
	auto transform = Parent()->Has(Transform);
	transform->SetTranslation(pos);


}
/// <summary>
/// set the scale of the button using vector
/// </summary>
/// <param name="scale"></param>
void Button::SetScale(glm::vec3 scale)
{
	auto transform = Parent()->Has(Transform);
	
	transform->SetScale(scale);


}
/// <summary>
/// set scale of button using float
/// </summary>
/// <param name="scale"></param>
void Button::SetScale(float scale)
{
	auto transform = Parent()->Has(Transform);
	glm::vec3 temp = transform->GetScale();
	temp *= scale;
	transform->SetScale(temp);


}
/*!********************************************************************************************************************
	\brief
		Checks if the button is being hovered over with the mouse
**********************************************************************************************************************/
bool Button::Collision()
{
	auto collider = Parent()->Has(Collider);
	if (collider->GetType() == TypeBox)
	{
		if (collider->BoxCheck(glm::vec3(Input::GetMouseX(), Input::GetMouseY(), 0.f)))
		{
			return true;
		}
		

		return false;
	}
	if (collider->GetType() == TypeArea)
	{

	}
	return false;
}

