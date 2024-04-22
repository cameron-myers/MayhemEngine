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

#include "BehaviorUpgradeDButton.h"

#include "Behavior.h"
#include "Audio.h"
#include "Transform.h"
#include "MEInput.h"
#include "Log.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkQueryParameters.h>

#include "BehaviorButton.h"
#include "GameObjectManager.h"
#include "MESpaceManager.h"
#include "MESpace.h"
#include "LaneManager.h"
#include "MEtimer.h"
#include "Wwise_IDs.h"
#include "Stats.h"
#include "BehaviorGoldCount.h"
#include "Animation.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

cZeppelinType UpgradeButton::ship;

int Level1Cost = 60;
int Level2Cost = 75;
int Level3Cost = 120;

int damageLevel = 0;
int healthLevel = 0;
int speedLevel = 0;

GameObject* Zep1;

//cZeppelinType spawnType = cZeppelinTypeS;

/*!********************************************************************************************************************
	\brief
		Creates a new Button behavior
**********************************************************************************************************************/
UpgradeButton::UpgradeButton(cUpgrType _type):type(_type)
{
	//ship = cZeppelinTypeS;
}
/// <summary>
/// param constructor
/// </summary>
/// <param name="_type"></param>
/// <param name="name"></param>
UpgradeButton::UpgradeButton(cUpgrType _type, std::string name):type(_type)
{
	//ship = cZeppelinTypeS;
}
/// <summary>
/// copy constructor
/// </summary>
/// <param name="rhs"></param>
UpgradeButton::UpgradeButton(const UpgradeButton& rhs): type(rhs.type)
{
}
/// <summary>
/// destructor
/// </summary>
UpgradeButton::~UpgradeButton()
{
}
/// <summary>
/// 
/// </summary>
/// <returns>new behavior that is clone</returns>
BehaviorPtr UpgradeButton::Clone() const
{
	UpgradeButton* spawnButton = new UpgradeButton(type);
	if (spawnButton)
	{
		*spawnButton = *this;
	}
	return spawnButton;
}
/// <summary>
/// initialize button
/// </summary>
void UpgradeButton::Init()
{
	Button::Init();
	ship = cZeppelinTypeS;
	
}
/// <summary>
/// update upgradeDButton states
/// </summary>
/// <param name="dt"></param>
void UpgradeButton::Update(float dt)
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
		if (Zep1)
		{
			SetShipType(ship);

			switch (type)
			{
			case cUpgrInvalid:
				break;
			case cUpgrDamage:
				switch (Zep1->Has(Stats)->GetAttackDamageLvlIndex())
				{
				case 0:
					if (GoldCount::GetGold() > Level1Cost)
					{
						Parent()->Has(Animation)->SetRowForMultiAnimation(0);
					}
					else Parent()->Has(Animation)->SetRowForMultiAnimation(3);

					break;
				case 1:
					if (GoldCount::GetGold() > Level2Cost)
					{
						Parent()->Has(Animation)->SetRowForMultiAnimation(0);
					}
					else Parent()->Has(Animation)->SetRowForMultiAnimation(3);

					break;

				default:
					Parent()->Has(Animation)->SetRowForMultiAnimation(3);
					break;
				}
				break;
			case cUpgrHealth:
				switch (Zep1->Has(Stats)->GetMaxHealthLvlIndex())
				{
				case 0:
					if (GoldCount::GetGold() > Level1Cost)
					{
						Parent()->Has(Animation)->SetRowForMultiAnimation(0);
					}
					else Parent()->Has(Animation)->SetRowForMultiAnimation(3);

					break;
				case 1:
					if (GoldCount::GetGold() > Level2Cost)
					{
						Parent()->Has(Animation)->SetRowForMultiAnimation(0);
					}
					else Parent()->Has(Animation)->SetRowForMultiAnimation(3);
					break;

				default:
					Parent()->Has(Animation)->SetRowForMultiAnimation(3);
					break;
				}
				break;
			case cUpgrSpeed:
				switch (Zep1->Has(Stats)->GetMaxSpeedLvlIndex())
				{
				case 0:
					if (GoldCount::GetGold() > Level1Cost)
					{
						Parent()->Has(Animation)->SetRowForMultiAnimation(0);
					}
					else Parent()->Has(Animation)->SetRowForMultiAnimation(3);
					break;
				case 1:
					if (GoldCount::GetGold() > Level2Cost)
					{
						Parent()->Has(Animation)->SetRowForMultiAnimation(0);
					}
					else Parent()->Has(Animation)->SetRowForMultiAnimation(3);

					break;
				default:
					Parent()->Has(Animation)->SetRowForMultiAnimation(3);
					break;
				}
			}
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

		Parent()->Has(Animation)->SetRowForMultiAnimation(1);
		
		if (justhover)
		{
			audio->PlaySoundEffect(SOUND(PLAY_BUTTONHOVER_LEVEL));
			justhover = false;
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
	{
		Parent()->Has(Animation)->SetRowForMultiAnimation(2);

		if (METimerManager::check(click_timer) || click_timer == nullptr)
		{
			click_timer = METimerManager::MakeTimer(pressed_time);
			SetStateNext(cButtonIdle);
			Audio* audio = Parent()->Has(Audio);

				if (type == cUpgrDamage)
				{
					if (Zep1->Has(Stats)->GetAttackDamageLvlIndex() == 0 && GoldCount::GetGold() > Level1Cost)
					{
						GoldCount::ChangeGold(-Level1Cost);
						Zep1->Has(Stats)->UpgradeAttackDamage();
						//to update the bars
						SetShipType(ship);
						audio->PlaySoundEffect(SOUND(PLAY_ZEPPELIN_UPGRADE_DAMAGE));
					}
					else if (Zep1->Has(Stats)->GetAttackDamageLvlIndex() == 1 && GoldCount::GetGold() > Level2Cost)
					{
						GoldCount::ChangeGold(-Level2Cost);
						Zep1->Has(Stats)->UpgradeAttackDamage();
						//to update the bars
						SetShipType(ship);
						audio->PlaySoundEffect(SOUND(PLAY_ZEPPELIN_UPGRADE_DAMAGE));
					}
					/*else if (Zep1->Has(Stats)->GetAttackDamageLvlIndex() == 2 && GoldCount::GetGold() > Level3Cost)
					{
						GoldCount::ChangeGold(-Level3Cost);
						Zep1->Has(Stats)->UpgradeAttackDamage();
						//to update the bars
						SetShipType(ship);
						audio->PlaySoundEffect(SOUND(PLAY_ZEPPELIN_UPGRADE_DAMAGE));
					}*/
				}
				else if (type == cUpgrHealth)
				{
					if (Zep1->Has(Stats)->GetMaxHealthLvlIndex() == 0 && GoldCount::GetGold() > Level1Cost)
					{
						GoldCount::ChangeGold(-Level1Cost);
						Zep1->Has(Stats)->UpgradeMaxHealth();
						//to update the bars
						SetShipType(ship);
						audio->PlaySoundEffect(SOUND(PLAY_ZEPPELIN_UPGRADE_HEALTH));
					}
					else if (Zep1->Has(Stats)->GetMaxHealthLvlIndex() == 1 && GoldCount::GetGold() > Level2Cost)
					{
						GoldCount::ChangeGold(-Level2Cost);
						Zep1->Has(Stats)->UpgradeMaxHealth();
						//to update the bars
						SetShipType(ship);
						audio->PlaySoundEffect(SOUND(PLAY_ZEPPELIN_UPGRADE_HEALTH));
					}
					/*else if (Zep1->Has(Stats)->GetMaxHealthLvlIndex() == 2 && GoldCount::GetGold() > Level3Cost)
					{
						GoldCount::ChangeGold(-Level3Cost);
						Zep1->Has(Stats)->UpgradeMaxHealth();
						//to update the bars
						SetShipType(ship);
						audio->PlaySoundEffect(SOUND(PLAY_ZEPPELIN_UPGRADE_HEALTH));
					}*/

				}
				else if (type == cUpgrSpeed)
				{
					if (Zep1->Has(Stats)->GetMaxSpeedLvlIndex() == 0 && GoldCount::GetGold() > Level1Cost)
					{
						GoldCount::ChangeGold(-Level1Cost);
						Zep1->Has(Stats)->UpgradeMaxSpeed();
						//to update the bars
						SetShipType(ship);
						audio->PlaySoundEffect(SOUND(PLAY_ZEPPELIN_UPGRADE_SPEED));
					}
					else if (Zep1->Has(Stats)->GetMaxSpeedLvlIndex() == 1 && GoldCount::GetGold() > Level2Cost)
					{
						GoldCount::ChangeGold(-Level2Cost);
						Zep1->Has(Stats)->UpgradeMaxSpeed();
						//to update the bars
						SetShipType(ship);
						audio->PlaySoundEffect(SOUND(PLAY_ZEPPELIN_UPGRADE_SPEED));
					}
					/*else if (Zep1->Has(Stats)->GetMaxSpeedLvlIndex() == 2 && GoldCount::GetGold() > Level3Cost)
					{
						GoldCount::ChangeGold(-Level3Cost);
						Zep1->Has(Stats)->UpgradeMaxSpeed();
						//to update the bars
						SetShipType(ship);
						audio->PlaySoundEffect(SOUND(PLAY_ZEPPELIN_UPGRADE_SPEED));
					}*/

				}
		}
		break;
	}
	case cButtonDestroyed:
		break;
	}
}

void UpgradeButton::SetShipType(cZeppelinType type)
{
	ship = type;
	MESpace* sndbox = MESpaceManager::GetSpace("Sandbox");
	GameObjectManager* Actlist = sndbox->GetActive();
	GameObject* hpBar;
	GameObject* dmBar;
	GameObject* spBar;


	if (ship == cZeppelinTypeS)
	{
		Zep1 = Actlist->GetObjectByName("Zeppelin1");
	}
	else if (ship == cZeppelinTypeM)
	{
		Zep1 = Actlist->GetObjectByName("Zeppelin2");
	}
	else if (ship == cZeppelinTypeL)
	{
		Zep1 = Actlist->GetObjectByName("Zeppelin3");
	}
	else
	{
		Zep1 = Actlist->GetObjectByName("Zeppelin1");
	}

	switch (ship)
	{
	case cZeppelinTypeInvalid:
		break;
	case cZeppelinTypeS:
		hpBar = Actlist->GetObjectByName("GremlinHP");
		dmBar = Actlist->GetObjectByName("GremlinDM");
		spBar = Actlist->GetObjectByName("GremlinSP");


		hpBar->Has(Animation)->SetRowForMultiAnimation(hpBar->Has(Animation)->getFrameCount() - 2 + (Zep1->Has(Stats)->GetMaxHealthLvlIndex()));
																								//ifykyk ^
		dmBar->Has(Animation)->SetRowForMultiAnimation(dmBar->Has(Animation)->getFrameCount() - 2 + Zep1->Has(Stats)->GetAttackDamageLvlIndex());
		spBar->Has(Animation)->SetRowForMultiAnimation(spBar->Has(Animation)->getFrameCount() - 2 + Zep1->Has(Stats)->GetMaxSpeedLvlIndex());
		break;
	case cZeppelinTypeM:
		hpBar = Actlist->GetObjectByName("BugbearHP");
		dmBar = Actlist->GetObjectByName("BugbearDM");
		spBar = Actlist->GetObjectByName("BugbearSP");
		hpBar->Has(Animation)->SetRowForMultiAnimation(hpBar->Has(Animation)->getFrameCount() - 2 + Zep1->Has(Stats)->GetMaxHealthLvlIndex());
		dmBar->Has(Animation)->SetRowForMultiAnimation(dmBar->Has(Animation)->getFrameCount() - 2 + Zep1->Has(Stats)->GetAttackDamageLvlIndex());
		spBar->Has(Animation)->SetRowForMultiAnimation(spBar->Has(Animation)->getFrameCount() - 2 + Zep1->Has(Stats)->GetMaxSpeedLvlIndex() );
		break;
	case cZeppelinTypeL:
		hpBar = Actlist->GetObjectByName("TyrantHP");
		dmBar = Actlist->GetObjectByName("TyrantDM");
		spBar = Actlist->GetObjectByName("TyrantSP");
		hpBar->Has(Animation)->SetRowForMultiAnimation(hpBar->Has(Animation)->getFrameCount() - 2 + Zep1->Has(Stats)->GetMaxHealthLvlIndex());
		dmBar->Has(Animation)->SetRowForMultiAnimation(dmBar->Has(Animation)->getFrameCount() - 2 + Zep1->Has(Stats)->GetAttackDamageLvlIndex());
		spBar->Has(Animation)->SetRowForMultiAnimation(spBar->Has(Animation)->getFrameCount() - 2 + Zep1->Has(Stats)->GetMaxSpeedLvlIndex() );
		break;
	default:
		break;
	}
}


/// <summary>
/// does nothing
/// </summary>
void UpgradeButton::Exit()
{
}


