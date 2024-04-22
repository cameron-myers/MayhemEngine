/*****************************************************************//**
 * \file         BehaviorRoundTimer.cpp
 * \author       Ethan Mclaughlin
 * \par          Email: ethan.mclaughlin\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief  Keeps track of the time remaining in the round, also controlls the needle for the timer on the hud
 *
 * 
 *********************************************************************/

//round timer is going to be the needle on the timer


/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/
#include "Log.h"
#include "Behavior.h"
#include "BehaviorRoundTimer.h"
#include "MESpaceManager.h"
#include "MEtimer.h"
#include "Physics.h"
#include "Sprite.h"
#include "Mesh.h"

#include "Component.h"
#include "LaneManager.h"
#include "MEInput.h"
//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
bool flag = true;
//typedef enum cClockstate { cClockstateInvalid = -1, cClockstateIdle, cClockstateRunning, cClockstateEnded} cClockstate;

//MESpace* space;
/*!********************************************************************************************************************
	\brief
		Creates a new Background behavior
**********************************************************************************************************************/
RoundTimer::RoundTimer():Behavior(cClockstateInvalid, cClockstateIdle, "BIG ERROR"), Roundtime(METimerManager::MakeTimer(RoundLength))
{
	RoundLength = 600.0f;
	METimerManager::set(RoundLength, Roundtime);
}
/// <summary>
/// copy constructor
/// </summary>
/// <param name="rhs"></param>
RoundTimer::RoundTimer(const RoundTimer& rhs):Behavior(rhs.stateCurr, rhs.stateNext, "BIG ERROR"), Roundtime(rhs.Roundtime)
{
}
/// <summary>
/// destructor
/// </summary>
RoundTimer::~RoundTimer()
{
}
/// @brief clones the background behavior
/// @return 
BehaviorPtr RoundTimer::Clone() const
{
	BehaviorPtr roundTimerbhv = new RoundTimer;
	if (roundTimerbhv)
	{
		*roundTimerbhv = *this;
	}
	return roundTimerbhv;
}
/// @brief Init everything for the background
void RoundTimer::Init()
{
	if (stateCurr == cClockstateInvalid)
	{
		SetStateNext(cClockstateIdle);

	}

}
/// @brief decide which update function to run
/// @param dt 
void RoundTimer::Update(float dt)
{
	float scale = (METimerManager::checktime(Roundtime) / RoundLength);


	Parent()->GetComponent<Sprite>(cSprite)->GetMesh()->ScaleX(scale-0.5f);
	/*
	if (Input::IsKeyReleased(GLFW_KEY_P) && flag)
	{
		METimerManager::GlobalPause();
	}
	else if (Input::IsKeyReleased(GLFW_KEY_P) && !flag)
	{
		METimerManager::GlobalUnpause();
	}
	*/

	if (METimerManager::check(Roundtime) && !ended)
	{
		//start the wait until stuff happens on lose
		end_timer = METimerManager::MakeTimer(6.f);
		METimerManager::stop(Roundtime);
		ended = true;

		//MESpaceManager::SetGameLose();
		//LaneManager::KillEverything();

	}

	if(METimerManager::check(end_timer))
	{
		METimerManager::set(RoundLength, Roundtime);
		MESpaceManager::SetSpaceExclusive("GameLose");
	}

}
/// <summary>
/// does nothing
/// </summary>
void RoundTimer::Exit()
{
}
