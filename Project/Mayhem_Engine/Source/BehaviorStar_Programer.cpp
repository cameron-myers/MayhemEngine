/*****************************************************************//**
 * \file         BehaviorStar_Programer.cpp
 * \author       Ethan Mclaughlin
 * \par          Email: ethan.mclaughlin@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 *              Test star
 *********************************************************************/
#include "GameObject.h"
#include "Physics.h"
#include "BehaviorStar_Programmer.h"
#include <glm/gtx/rotate_vector.hpp>
#include "Transform.h"
#include "LaneManager.h"
#include "Animation.h"
#include "Sprite.h"
#include "SpriteSource.h"

#include "Log.h"
//#include "Behavior.h"
#include "MESpaceManager.h"
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

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------



/// <summary>
/// constructor
/// </summary>
/// <param name="_type"></param>
Star::Star(float time) :Behavior(cStarstateInvalid, cStarstateIdle, "BIG ERROR"), StarTimer(METimerManager::MakeTimer(time)), NoTime(true)
{
    StarTime = time;
    METimerManager::set(StarTime, StarTimer);
    //this->Parent()->GetComponent<Animation>(cAnimation)->SetRowForMultiAnimation(0);
}
/// <summary>
/// copy constructor
/// </summary>
/// <param name="rhs"></param>
Star::Star(const Star& rhs) :Behavior(rhs.stateCurr, rhs.stateNext, "BIG ERROR"), StarTimer(rhs.StarTimer), NoTime(true)
{
    //this->Parent()->GetComponent<Animation>(cAnimation)->SetRowForMultiAnimation(0);
}
/// <summary>
/// destructor
/// </summary>
Star::~Star()
{
}
/// <summary>
/// 
/// </summary>
/// <returns>new behavior that is clone</returns>
BehaviorPtr Star::Clone() const
{
    Star* Starc = new Star(this->StarTime);
    if (Starc)
    {
        *Starc = *this;
    }
    return Starc;
}

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


// Initialize the current state of the this component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 this = Pointer to the this component.
void Star::Init()
{



    if (this->stateCurr == cStarstateInvalid)
    {
        this->stateCurr = cStarstateIdle;
    }
    //this->Parent()->GetComponent<Collider>(cCollider)->SetHandler(&CollisionHandler);

}

// Update the current state of the this component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 this = Pointer to the this component.
//	 dt = Change in time (in seconds) since the last game loop.
void Star::Update(float dt)
{
    stateCurr = stateNext;
    AnimationPtr animation = this->Parent()->GetComponent<Animation>(cAnimation);
    //Stats* stats = this->Parent()->GetComponent<Stats>(cStats);

    if (METimerManager::check(StarTimer) && NoTime == true)
    {
        animation->SetRowForMultiAnimation(animation->GetRowOfCurrentIndex() + 1);
        NoTime = false;
    }
    //TeleporterUpdateObject(this->parent);
}

// Exit the current state of the this component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 this = Pointer to the this component.
//	 dt = Change in time (in seconds) since the last game loop.
void Star::Exit()
{

}
