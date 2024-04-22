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
#include "BehaviorStar_Programer_Results.h"
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

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------



/// <summary>
/// constructor
/// </summary>
/// <param name="_type"></param>
StarResult::StarResult() :Behavior(cStarResultstateInvalid, cStarResultstateIdle, "BIG ERROR"), stardata(0)
{
    //this->Parent()->GetComponent<Animation>(cAnimation)->SetRowForMultiAnimation(0);
}
/// <summary>
/// copy constructor
/// </summary>
/// <param name="rhs"></param>
StarResult::StarResult(const StarResult& rhs) :Behavior(rhs.stateCurr, rhs.stateNext, "BIG ERROR"), stardata(0)
{
    //this->Parent()->GetComponent<Animation>(cAnimation)->SetRowForMultiAnimation(0);
}
/// <summary>
/// destructor
/// </summary>
StarResult::~StarResult()
{
}
/// <summary>
/// 
/// </summary>
/// <returns>new behavior that is clone</returns>
BehaviorPtr StarResult::Clone() const
{
    StarResult* Starc = new StarResult();
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
void StarResult::Init()
{
    if (this->stateCurr == cStarResultstateInvalid)
    {
        this->stateCurr = cStarResultstateIdle;
    }

    if (MESpaceManager::GetSpace("Sandbox"))
    {
        MESpace* sndbox = MESpaceManager::GetSpace("Sandbox");

        if (sndbox->GetActive())
        {
            GameObjectManager* Actlist = sndbox->GetActive();
            if (Actlist->GetObjectByName("GameBackground"))
            {
                GameObject* bg = Actlist->GetObjectByName("GameBackground");
                int stars = static_cast<Background*>(bg->Has(Behavior))->GetStarData();
                AnimationPtr animation = this->Parent()->GetComponent<Animation>(cAnimation);

                GameObjectPtr star = this->Parent();
                if (star->GetName() == "StarTest_Results" && stars < 1)
                {
                    animation->SetRowForMultiAnimation(animation->GetRowOfCurrentIndex() + 1);
                }
                else if (star->GetName() == "StarTest_Results1" && stars < 2)
                {
                    animation->SetRowForMultiAnimation(animation->GetRowOfCurrentIndex() + 1);
                }
                else if (star->GetName() == "StarTest_Results2" && stars < 3)
                {
                    animation->SetRowForMultiAnimation(animation->GetRowOfCurrentIndex() + 1);
                }
            }
        }
    }
   

    //Check results from background to set animation row
    

    //this->Parent()->GetComponent<Collider>(cCollider)->SetHandler(&CollisionHandler);

}

// Update the current state of the this component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 this = Pointer to the this component.
//	 dt = Change in time (in seconds) since the last game loop.
void StarResult::Update(float dt)
{
    stateCurr = stateNext;
    //AnimationPtr animation = this->Parent()->GetComponent<Animation>(cAnimation);
    //Stats* stats = this->Parent()->GetComponent<Stats>(cStats);

    //animation->SetRowForMultiAnimation(animation->GetRowOfCurrentIndex() + 1);
    //NoTime = false;
    //TeleporterUpdateObject(this->parent);
}

// Exit the current state of the this component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 this = Pointer to the this component.
//	 dt = Change in time (in seconds) since the last game loop.
void StarResult::Exit()
{

}
