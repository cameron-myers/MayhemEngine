/*****************************************************************//**
 * \file         BehaviorGoldCount.cpp
 * \author       Ethan Mclaughlin
 * \par          Email: ethan.mclaughlin\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
//round timer is going to be the needle on the timer


/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/
#include "BehaviorGoldCount.h"
#include "MESpaceManager.h"
#include "MEtimer.h"
#include "Sprite.h"
#include "MEInput.h"
#include "Component.h"
#include "Log.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
//typedef enum cClockstate { cClockstateInvalid = -1, cClockstateIdle, cClockstateRunning, cClockstateEnded} cClockstate;
enum cCounterState { cCounterStateInvalid = -1, cCounterStateIdle };
int GoldCount::Gold;

//MESpace* space;
/*!********************************************************************************************************************
    \brief
        Creates a new Background behavior
**********************************************************************************************************************/
GoldCounter::GoldCounter() : Behavior(cCounterStateInvalid, cCounterStateIdle, "BIG ERROR"), GoldWait(METimerManager::MakeTimer(1.0))
{
    //Gold = 0;
}
/// <summary>
/// copy constructor
/// </summary>
/// <param name="rhs"></param>
GoldCounter::GoldCounter(const GoldCounter& rhs):Behavior(rhs.stateCurr, rhs.stateNext, "BIG ERROR"), GoldWait(rhs.GoldWait)
{
    //Gold = 0;
}
/// <summary>
/// destructor
/// </summary>
GoldCounter::~GoldCounter()
{
}
/// @brief clones the background behavior
/// @return 
BehaviorPtr GoldCounter::Clone() const
{
    BehaviorPtr GoldCountbhv = new GoldCounter;
    if (GoldCountbhv)
    {
        *GoldCountbhv = *this;
    }
    return GoldCountbhv;
}
/// @brief Init everything for the background
void GoldCounter::Init()
{
    if (stateCurr == cCounterStateInvalid)
    {
        SetStateNext(cCounterStateIdle);

    }

}
/// @brief decide which update function to run
/// @param dt 
void GoldCounter::Update(float dt)
{
    //stateCurr = stateNext;

    //set the uv for the timer and apply some rotation or something
    std::string number(std::to_string(GoldCount::GetGold()));
    this->Parent()->GetComponent<Sprite>(cSprite)->SetText(number);
    
    if (Input::IsKeyReleased(GLFW_KEY_M))
    {
        //GoldCount::ChangeGold(999999);
    }

    if (METimerManager::check(GoldWait))
    {
        GoldCount::ChangeGold(1);
        METimerManager::set(0.2f, GoldWait);
    }
}
/// <summary>
/// does nothing
/// </summary>
void GoldCounter::Exit()
{
}
/// <summary>
/// comparers gold to given value
/// </summary>
/// <param name="value"></param>
/// <returns>if value is less than gold</returns>
bool GoldCount::CompGold(int value)
{
    if (value < Gold)
    {
        ME_CORE_ERROR(Gold);
        return true;
    }
    return false;
}

/*
* use a negative number to remove gold
*/
void GoldCount::ChangeGold(int value)
{
    Gold += value;
}
/// <summary>
/// 
/// </summary>
/// <returns>gold count</returns>
int GoldCount::GetGold()
{
    return Gold;
}

void GoldCount::ResetGold()
{
    Gold = 0;
}
