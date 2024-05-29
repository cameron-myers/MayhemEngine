/*****************************************************************//**
 * \file         BackgroundBehavior.cpp
 * \authors       Cameron Myers, Ethan Mclaughlin
 * \par          Email: cameron.myers\@digipen.edu, Ethan.mclaughlin\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/

/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/
#include "BackgroundBehavior.h"

#include "Audio.h"
#include "Component.h"
#include <Wwise_IDs.h>
#include <SoundEngine/Common/AkSoundEngine.h>

#include "MESpace.h"
#include "MESpaceManager.h"
#include "Transform.h"
//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
typedef enum cBackground { cBackgroundInvalid = -1, cBackgroundIdle, cBackgroundDestroyed } BackgroundStates;

//MESpace* space;
/*!********************************************************************************************************************
	\brief
		Creates a new Background behavior
**********************************************************************************************************************/
Background::Background():Behavior(cBackgroundInvalid, cBackgroundIdle, "BIG ERROR"), audio(nullptr),StarData(0)
{
	
}
/// @brief Background behavior ctor
/// @param _type Type of background
/// @param name name of the behavior
Background::Background(cBackgroundType _type, std::string name) :Behavior(cBackgroundInvalid, cBackgroundIdle, name), type(_type)
{
}

/// <summary>
/// background behavior copy constructor
/// </summary>
/// <param name="rhs"></param>
Background::Background(const Background& rhs):Behavior(rhs.stateCurr, rhs.stateNext, "BIG ERROR")
{
}

/// <summary>
/// destructor
/// </summary>
Background::~Background()
{
}
/// @brief clones the background behavior
/// @return 
BehaviorPtr Background::Clone() const
{
	BehaviorPtr background = new Background;
	if (background)
	{
		*background = *this;
	}
	return background;
}
/// @brief Init everything for the background
void Background::Init()
{
	if (stateCurr == cBackgroundInvalid)
	{
		SetStateNext(cBackgroundIdle);

	}
	auto audio = Parent()->Has(Audio);
	oldTransform = *Parent()->Has(Transform);
	//space = Par
	//which event to post for the background music
	/// @todo change this to an actual background music call
	switch (type)
	{
	case cMainMenuBackground:
		audio->PlayAudio(SOUND(PLAY_MENU_MUSIC));
		break;

	case cPauseMenuBackground:
		audio->PlayAudio(SOUND(PLAY_MENU_MUSIC));

		break;

	case cGameBackground:
		audio->PlayAudio(SOUND(PLAY_LEVEL_MUSIC));

		break;
	case cWinMenuBackground:
		audio->PlayAudio(SOUND(PLAY_LEVEL_WIN));

		break;
	case cLoseMenuBackground:
		audio->PlayAudio(SOUND(PLAY_LEVEL_LOSE));

		break;

	case cCreditMenuBackground:
		audio->PlayAudio(SOUND(PLAY_CREDIT_MUSIC));
		break;

	case cDigiBackground:
		audio->PlayAudio(SOUND(PLAY_SPLASH_SCREEN_DIGI));
		break;
	}
}
/// @brief decide which update function to run
/// @param dt 
void Background::Update(float dt)
{
	stateCurr = stateNext;

	switch (stateCurr)
	{
	case cMainMenuBackground:
		MainMenuUpdate(dt);
		break;
	case cGameBackground:
		GameBackUpdate(dt);
		break;
	case cPauseMenuBackground:
		PauseMenuUpdate(dt);
		break;
	case cWinMenuBackground:
		WinMenuUpdate(dt);
		break;
	case cLoseMenuBackground:
		LoseMenuUpdate(dt);
		break;
	case cCreditMenuBackground:
		CreditMenuUpdate(dt);
		break;
	case cDigiBackground:
		DigiUpdate(dt);
		break;
	}
}
/// @brief update for the main menu
/// @param dt 
void Background::MainMenuUpdate(float dt)
{
	if (!MESpaceManager::IsActive(Parent()))
	{
		AK::SoundEngine::StopPlayingID(audio->GetPlayingID(AK::EVENTS::PLAY_MENU_MUSIC));
	}
}

/// @brief update for the pause menu
/// @param dt 
void Background::PauseMenuUpdate(float dt)
{
	if (!MESpaceManager::IsActive(Parent()))
	{
		AK::SoundEngine::StopPlayingID(audio->GetPlayingID(AK::EVENTS::PLAY_MENU_MUSIC));
	}
}
/// @brief update for the main gamestate
/// @param dt 
void Background::GameBackUpdate(float dt)
{
	if (!MESpaceManager::IsActive(Parent()))
	{
		AK::SoundEngine::StopPlayingID(audio->GetPlayingID(AK::EVENTS::PLAY_LEVEL_MUSIC));
	}
}
/// <summary>
/// update lose menu sound
/// </summary>
/// <param name="dt"></param>
void Background::LoseMenuUpdate(float dt)
{
	if (!MESpaceManager::IsActive(Parent()))
	{
		AK::SoundEngine::StopPlayingID(audio->GetPlayingID(AK::EVENTS::PLAY_LEVEL_WIN));
	}
}
/// <summary>
/// update win menu sound
/// </summary>
/// <param name="dt"></param>
void Background::WinMenuUpdate(float dt)
{
	if (!MESpaceManager::IsActive(Parent()))
	{
		AK::SoundEngine::StopPlayingID(audio->GetPlayingID(AK::EVENTS::PLAY_LEVEL_LOSE));
	}
}

void Background::CreditMenuUpdate(float dt)
{
	if (!MESpaceManager::IsActive(Parent()))
	{
		AK::SoundEngine::StopPlayingID(audio->GetPlayingID(AK::EVENTS::PLAY_CREDIT_MUSIC));
	}
}

void Background::DigiUpdate(float dt)
{
	if (!MESpaceManager::IsActive(Parent()))
	{
		AK::SoundEngine::StopPlayingID(audio->GetPlayingID(AK::EVENTS::PLAY_SPLASH_SCREEN_DIGI));
	}
}

/// /// <summary>
/// stop background audio
/// set old transform
/// </summary>
void Background::Exit()
{
	auto audio = Parent()->Has(Audio);
	oldTransform = *Parent()->Has(Transform);
	//space = Par
	//which event to post for the background music
	/// @todo change this to an actual background music call
		audio->StopAudioAll();
}

//get star data
int Background::GetStarData()
{
	return StarData;
}

//set star data
void Background::SetStarData(int val)
{
	StarData = val;
}

//get the time score
float Background::GetTimeData()
{
	return TimeRemaining;
}

//set the stored time score
void Background::SetTimeData(float var)
{
	TimeRemaining = var;
}
