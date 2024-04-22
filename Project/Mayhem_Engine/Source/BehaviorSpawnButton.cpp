/*****************************************************************//**
 * \file         BehaviorSpawnButton.cpp
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
#pragma once

#include "BehaviorSpawnButton.h"

#include "Behavior.h"
#include "Audio.h"
#include "Transform.h"
#include "MEInput.h"
#include "Log.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkQueryParameters.h>

#include "Animation.h"
#include "BehaviorButton.h"
#include "BehaviorGoldCount.h"
#include "BehaviorRoundTimer.h"
#include "Collider.h"
#include "GameObjectManager.h"
#include "LaneManager.h"
#include "MESpace.h"
#include "MESpaceManager.h"
#include "MEtimer.h"
#include "Sprite.h"
#include "Stats.h"
#include "Wwise_IDs.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

cZeppelinType spawnType = cZeppelinTypeS;

/*!********************************************************************************************************************
	\brief
		Creates a new Button behavior
**********************************************************************************************************************/
SpawnButton::SpawnButton(cSpawnLoc _type):type(_type)
{
}
/// <summary>
/// param constructor?
/// </summary>
/// <param name="_type"></param>
/// <param name="name"></param>
SpawnButton::SpawnButton(cSpawnLoc _type, std::string name) : type(_type)
{
}
/// <summary>
/// copy constructor
/// </summary>
/// <param name="rhs"></param>
SpawnButton::SpawnButton(const SpawnButton& rhs): type(rhs.type)
{
}
/// <summary>
/// destructor
/// </summary>
SpawnButton::~SpawnButton()
{
}
/// <summary>
/// 
/// </summary>
/// <returns>new behavior that is clone</returns>
BehaviorPtr SpawnButton::Clone() const
{
	SpawnButton* spawnButton = new SpawnButton(type);
	if (spawnButton)
	{
		*spawnButton = *this;
	}
	return spawnButton;
}
/// <summary>
/// initialize button
/// </summary>
void SpawnButton::Init()
{
	Button::Init();
	spawn_cooldown = METimerManager::MakeTimer(1.0f);
}
/// <summary>
/// update the button
/// </summary>
/// <param name="dt"></param>
void SpawnButton::Update(float dt)
{
	Button::Update(dt);
	if(METimerManager::check(MESpaceManager::GetSpace("Sandbox")->GetActive()->GetObjectByName("FuelGaugeFill")->GetComponent<RoundTimer>(cBehavior)->GetTimer()))
	{
		stateCurr = cButtonIdle;
	}
	if (!METimerManager::check(click_timer) && click_timer != nullptr)
	{
		stateCurr = cButtonPressed;
	}
	switch (stateCurr)
	{
	case cButtonIdle:

		Parent()->Has(Sprite)->SetAlpha(0.f);

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
		Parent()->Has(Sprite)->SetAlpha(1.0f);

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
			if (METimerManager::check(click_timer) || click_timer == nullptr)
			{
				SetStateNext(cButtonIdle);
				Stats* stats = MESpaceManager::GetSpace("Sandbox")->GetActive()->GetObjectByName("Zeppelin" + std::to_string(spawnType))->Has(Stats);

				if (METimerManager::check(spawn_cooldown))
				{
					switch (type)
					{
					case cSpawnTop:
						if (GoldCount::CompGold(stats->GetCost()))
						{
							PlayZeppelinSpawn();
							LaneManager::AddTop(spawnType);
							GoldCount::ChangeGold(-stats->GetCost());
							spawn_cooldown = METimerManager::MakeTimer(1.0f);
						}
						SetStateNext(cButtonIdle);

						break;
					case cSpawnMid:
						if (GoldCount::CompGold(stats->GetCost()))
						{
							PlayZeppelinSpawn();
							LaneManager::AddMid(spawnType);
							GoldCount::ChangeGold(-stats->GetCost());
							spawn_cooldown = METimerManager::MakeTimer(1.0f);
						}
						SetStateNext(cButtonIdle);

						break;
					case cSpawnBot:
						if (GoldCount::CompGold(stats->GetCost()))
						{
							PlayZeppelinSpawn();
							LaneManager::AddBot(spawnType);
							GoldCount::ChangeGold(-stats->GetCost());
							spawn_cooldown = METimerManager::MakeTimer(1.0f);
						}
						SetStateNext(cButtonIdle);
						break;
					}
				}
			}
			break;
		}
		
	case cButtonDestroyed:
		break;
	}
	
}
/// <summary>
/// does nothing
/// </summary>
void SpawnButton::Exit()
{
}


void SpawnButton::SetSpawnType(cZeppelinType type)
{
	spawnType = type;
}

/**
 * \brief Ovveride of Button Collision to use a child as the collider
 * \return true or false collision
 */
/*bool SpawnButton::Collision()
{

	//add the postfix for the object name
	std::string loc;
	switch (type)
	{
		case cSpawnBot: loc = "B";
			break;
		case cSpawnMid: loc = "M";
			break;

		case cSpawnTop: loc = "T";
			break;

	}
	//do actual collider stuff
	auto collider = Parent()->GetChildByName("LaneHover" + loc)->Has(Collider);
	if (collider->GetType() == TypeBox)
	{
		if (collider->BoxCheck(glm::vec3(Input::GetMouseX(), Input::GetMouseY(), 0.f)))
		{
			return true;
		}
		return false;
	}
	return false;
}*/


void SpawnButton::PlayZeppelinSpawn()
{
	switch(spawnType)
	{
	case cZeppelinTypeS:
		audio->PlaySoundEffect(SOUND(PLAY_ZEPPELIN_SPAWN_GREMLIN));
		break;
	case cZeppelinTypeM:
		audio->PlaySoundEffect(SOUND(PLAY_ZEPPELIN_SPAWN_BUGBEAR));
		break;
	case cZeppelinTypeL:
		audio->PlaySoundEffect(SOUND(PLAY_ZEPPELIN_SPAWN_TYRANT));
		break;
	}
}
