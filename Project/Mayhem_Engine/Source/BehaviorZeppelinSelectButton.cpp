/*****************************************************************//**
 * \file         BehaviorZeppelinSelectButton.cpp
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
#include "BehaviorZeppelinSelectButton.h"
#include "BehaviorButton.h"
#include "Component.h"
#include "Audio.h"
#include "Transform.h"
#include "Collider.h"
#include "MEInput.h"
#include "Log.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkQueryParameters.h>
#include "BehaviorSpawnButton.h"
#include "LaneManager.h"
#include "MESpaceManager.h"
#include "BehaviorUpgradeDButton.h"
#include "MESpace.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "SpriteSourceManager.h"
#include "Wwise_IDs.h"
#include "Animation.h"
#include "MEtimer.h"
#include "Stats.h"


//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
cZeppelinType ZeppelinSelectButton::old_type = cZeppelinTypeInvalid;

/// <summary>
/// constructor
/// </summary>
/// <param name="_type"></param>
ZeppelinSelectButton::ZeppelinSelectButton(cZeppelinType _type):Button(), type(_type)
{
	
}
/// <summary>
/// copy constructor
/// </summary>
/// <param name="rhs"></param>
ZeppelinSelectButton::ZeppelinSelectButton(const ZeppelinSelectButton& rhs):Button(), type(rhs.type)
{

}
/// <summary>
/// destuctor
/// </summary>
ZeppelinSelectButton::~ZeppelinSelectButton()
{

}
/// <summary>
/// 
/// </summary>
/// <returns>new behavior that is clone</returns>
BehaviorPtr ZeppelinSelectButton::Clone() const
{
	ZeppelinSelectButton* button = new ZeppelinSelectButton(type);
	if (button)
	{
		*button = *this;
	}
	return button;
}
/// <summary>
/// initialize button
/// </summary>
void ZeppelinSelectButton::Init()
{
	Button::Init();

	//put everything but gremlin in inactive
	old_type = cZeppelinTypeInvalid;

}
/// <summary>
/// update the state of the button
/// </summary>
/// <param name="dt"></param>
void ZeppelinSelectButton::Update(float dt)
{
	Button::Update(dt);

	if(old_type == cZeppelinTypeInvalid)
	{
		MESpace* space = MESpaceManager::GetSpace("Sandbox");
		GameObjectManager* active_list = space->GetActive();

		//bugbear
		space->PutInInactive(active_list->GetObjectByName("BugbearHP"));
		space->PutInInactive(active_list->GetObjectByName("BugbearSP"));
		space->PutInInactive(active_list->GetObjectByName("BugbearDM"));

		//tyrant
		space->PutInInactive(active_list->GetObjectByName("TyrantHP"));
		space->PutInInactive(active_list->GetObjectByName("TyrantSP"));
		space->PutInInactive(active_list->GetObjectByName("TyrantDM"));

		SetCostNumber();
		UpgradeButton::SetShipType(type);

		old_type = cZeppelinTypeS;

	}

	if(!METimerManager::check(click_timer) && click_timer != nullptr)
	{
		stateCurr = cButtonPressed;
	}

	switch (stateCurr)
	{
	case cButtonIdle:

		Parent()->Has(Animation)->SetRowForMultiAnimation(Idle);

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
			audio->PlaySoundEffect(SOUND(PLAY_BUTTONHOVER_LEVEL));
			justhover = false;
		}

		Parent()->Has(Animation)->SetRowForMultiAnimation(Hover);

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
			//if the timer isnt active do stuff
			if (METimerManager::check(click_timer) || click_timer == nullptr)
			{
				click_timer = METimerManager::MakeTimer(pressed_time);
				audio->PlaySoundEffect(SOUND(PLAY_BUTTONCLICK_03));
				Parent()->Has(Animation)->SetRowForMultiAnimation(Pressed);

				//set the type of zeppelin that buttons spawn
				SpawnButton::SetSpawnType(type);
				SetSelectedIcon();
				SetStatBar();
				UpgradeButton::SetShipType(type);
				SetCostNumber();
				SetStateNext(cButtonIdle);
				old_type = type;
			}
		}
		break;
	case cButtonDestroyed:

		break;
	}
}
/// <summary>
/// does nothing
/// </summary>
void ZeppelinSelectButton::Exit()
{
}

/**
 * \brief Sets the icon of the zep selection box to type
 * \param type type of the zeppelin
 */
void ZeppelinSelectButton::SetSelectedIcon()
{
	GameObjectPtr obj =  MESpaceManager::GetSpace("Sandbox")->GetActive()->GetObjectByName("ZepSelected")->GetChildByName("ZepSelectedIcon");
	if (!obj) ME_CORE_ERROR("Could not find Zeppelin Select Child");
	//set the sprite source
	switch (type)
	{
	case cZeppelinTypeS:
		{
			obj->Has(Sprite)->SetSpriteSource(SpriteSourceManager::Find("Gremlin"));
			break;
		}
	case cZeppelinTypeM:
		{
			obj->Has(Sprite)->SetSpriteSource(SpriteSourceManager::Find("Bugbear"));
			break;
		}
	case cZeppelinTypeL:
		{
			obj->Has(Sprite)->SetSpriteSource(SpriteSourceManager::Find("Tyrant"));
			break;
		}
		
	}

}

/**
 * \brief changes which sprite sources render for the stat bars
 */
void ZeppelinSelectButton::SetStatBar()
{

	MESpace* space = MESpaceManager::GetSpace("Sandbox");
	GameObjectManager* active_list = space->GetActive();
	GameObjectManager* inactive_list = space->GetInactive();


	//Set the sprite sources for the respective zeppelin
	switch (type)
	{
		case cZeppelinTypeS:
		{
			//Get rid of current stat bars
			PopStatBars();
			//load new bars
			space->PutInActive(inactive_list->GetObjectByName("GremlinHP"));
			space->PutInActive(inactive_list->GetObjectByName("GremlinSP"));
			space->PutInActive(inactive_list->GetObjectByName("GremlinDM"));

			break;
		}
		case cZeppelinTypeM:
		{
			//Get rid of current stat bars
			PopStatBars();
			//load new bars
			space->PutInActive(inactive_list->GetObjectByName("BugbearHP"));
			space->PutInActive(inactive_list->GetObjectByName("BugbearSP"));
			space->PutInActive(inactive_list->GetObjectByName("BugbearDM"));
			break;
		}
		case cZeppelinTypeL:
		{
			//Get rid of current stat bars
			PopStatBars();
			//load new bars
			space->PutInActive(inactive_list->GetObjectByName("TyrantHP"));
			space->PutInActive(inactive_list->GetObjectByName("TyrantSP"));
			space->PutInActive(inactive_list->GetObjectByName("TyrantDM"));

			break;
		}
	}
}

/**
 * \brief Gets rid of the current stat bars
 */
void ZeppelinSelectButton::PopStatBars()
{
	MESpace* space = MESpaceManager::GetSpace("Sandbox");
	GameObjectManager* active_list = space->GetActive();

	switch (old_type)
	{
		case cZeppelinTypeS:
			{
				space->PutInInactive(active_list->GetObjectByName("GremlinHP"));
				space->PutInInactive(active_list->GetObjectByName("GremlinSP"));
				space->PutInInactive(active_list->GetObjectByName("GremlinDM"));
				break;
			}
		case cZeppelinTypeM:
			{
				space->PutInInactive(active_list->GetObjectByName("BugbearHP"));
				space->PutInInactive(active_list->GetObjectByName("BugbearSP"));
				space->PutInInactive(active_list->GetObjectByName("BugbearDM"));
				break;
			}
		case cZeppelinTypeL:
			{
				space->PutInInactive(active_list->GetObjectByName("TyrantHP"));
				space->PutInInactive(active_list->GetObjectByName("TyrantSP"));
				space->PutInInactive(active_list->GetObjectByName("TyrantDM"));
				break;
			}
	}
}

void ZeppelinSelectButton::SetCostNumber()
{
	MESpace* space = MESpaceManager::GetSpace("Sandbox");
	GameObjectManager* active_list = space->GetActive();

	active_list->GetObjectByName("DeployCostNumber")->Has(Sprite)->SetText(
		std::to_string(active_list->GetObjectByName("Zeppelin" + std::to_string(type))->Has(Stats)->GetCost())
	);

	
}
