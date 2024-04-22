/*****************************************************************//**
 * \file         BehaviorStar_Programer_Results.cpp
 * \author       Ethan Mclaughlin
 * \par          Email: ethan.mclaughlin@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 *              Test star
 *********************************************************************/
#include "GameObject.h"
#include "Physics.h"
#include "BehaviorTimer_Results.h"
#include <glm/gtx/rotate_vector.hpp>
#include "Transform.h"
#include "LaneManager.h"
#include "Animation.h"
#include "Sprite.h"
#include "SpriteSource.h"

#include "Log.h"
#include "MESpace.h"
//#include "Behavior.h"
#include "MESpaceManager.h"
#include "GameObjectManager.h"
#include "BackgroundBehavior.h"
//#include "MEtimer.h"
#include "Physics.h"
#include "Component.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
enum cTimerState { cTimerStateInvalid = -1, cTimerStateIdle };
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------



/// <summary>
/// constructor
/// </summary>
/// <param name="_type"></param>
TimerResult::TimerResult() :Behavior(cTimerStateInvalid, cTimerStateIdle, "BIG ERROR"), Timeleft(0)
{
    //this->Parent()->GetComponent<Animation>(cAnimation)->SetRowForMultiAnimation(0);
}
/// <summary>
/// copy constructor
/// </summary>
/// <param name="rhs"></param>
TimerResult::TimerResult(const TimerResult& rhs) :Behavior(rhs.stateCurr, rhs.stateNext, "BIG ERROR"), Timeleft(0)
{
    //this->Parent()->GetComponent<Animation>(cAnimation)->SetRowForMultiAnimation(0);
}
/// <summary>
/// destructor
/// </summary>
TimerResult::~TimerResult()
{
}
/// <summary>
/// 
/// </summary>
/// <returns>new behavior that is clone</returns>
BehaviorPtr TimerResult::Clone() const
{
    TimerResult* Timerc = new TimerResult();
    if (Timerc)
    {
        *Timerc = *this;
    }
    return Timerc;
}

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


// Initialize the current state of the this component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 this = Pointer to the this component.
void TimerResult::Init()
{
    if (this->stateCurr == cTimerStateInvalid)
    {
        this->stateCurr = cTimerStateIdle;
    }

    if (MESpaceManager::GetSpace("Sandbox"))
    {
        MESpace* sndbox = MESpaceManager::GetSpace("Sandbox");
        GameObjectManager* Actlist = sndbox->GetActive();
        GameObject* bg = Actlist->GetObjectByName("GameBackground");
        float time = static_cast<Background*>(bg->Has(Behavior))->GetTimeData();
        Timeleft = time;
    }
}

// Update the current state of the this component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 this = Pointer to the this component.
//	 dt = Change in time (in seconds) since the last game loop.
void TimerResult::Update(float dt)
{
    stateCurr = stateNext;

    std::string number(std::to_string(Timeleft));
    this->Parent()->GetComponent<Sprite>(cSprite)->SetText(number);
}

// Exit the current state of the this component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 this = Pointer to the this component.
//	 dt = Change in time (in seconds) since the last game loop.
void TimerResult::Exit()
{

}
