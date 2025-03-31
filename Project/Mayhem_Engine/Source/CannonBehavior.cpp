/*****************************************************************//**
 * \file         CannonBehavior.cpp
 * \author       Nate White
 * \par          Email: nate.white\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#include "GameObject.h"
#include "Physics.h"
#include "CannonBehavior.h"
#include "MEInput.h"
#include "MERand.h"
#include <glm/gtx/rotate_vector.hpp>
#include "Transform.h"
#include "Stats.h"
#include "Collider.h"
#include "LaneManager.h"
#include "Animation.h"
#include "Collider.h"
#include "Sprite.h"
#include "SpriteSource.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------


typedef enum cCannon { cCannonInvalid = -1, cCannonIdle, cCannonAngleChange, cCannonFire,cCannonDead } CannonStates;
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

/// <summary>
/// constructor
/// </summary>
Cannon::Cannon() :Behavior(cCannonInvalid, cCannonIdle, "BIG ERROR")
{
}
/// <summary>
/// copy constructor
/// </summary>
/// <param name="rhs"></param>
Cannon::Cannon(const Cannon& rhs) : Behavior(rhs.stateCurr, rhs.stateNext, "BIG ERROR")
{

}
/// <summary>
/// destructor
/// </summary>
Cannon::~Cannon()
{
}
/// <summary>
/// 
/// </summary>
/// <returns>new behavior that is clone</returns>
BehaviorPtr Cannon::Clone() const
{
    Cannon* cannon = new Cannon;
    if (cannon)
    {
        *cannon = *this;
    }
    return cannon;
}
/// <summary>
/// set direction for the cannon that is a child
/// </summary>
/// <param name="transform"></param>
void Cannon::SetDirectionForChildCannon(Transform* transform)
{
    
    
    Animation* animation = this->Parent()->Has(Animation);
    Stats* stats = this->Parent()-> GetParent()->Has(Stats);
    if (stats != NULL && stats->GetIsReloading() == false)
    {
        if (strstr(Parent()->GetParent()->GetName().c_str(), "Zeppelin"))
        {
            if (Parent()->GetParent()->Has(Transform)->GetTranslation().x > 700)
            {
                if (transform->Parent()->GetName() == "DwarfCity")
                {
                    if (animation->GetRowOfCurrentIndex() != 0)
                    {
                        animation->SetRowForMultiAnimation(0);
                    }
                    return;
                }
            }
        }
        glm::vec3 AttackingPos = transform->GetTranslation();
        glm::vec3 scale = transform->GetScale();
        glm::vec3 CannonPos = this->Parent()->GetParent()->Has(Transform)->GetTranslation();
        if (CannonPos.x > AttackingPos.x - scale.x / 2 && CannonPos.x < AttackingPos.x + scale.x / 2)
        {
            if (CannonPos.y < AttackingPos.y)
            {
                if (strstr(Parent()->GetParent()->GetName().c_str(), "Tower"))
                {
                    if (animation->GetRowOfCurrentIndex() != 4)
                    {
                        animation->SetRowForMultiAnimation(4);
                    }
                }
                else if (strstr(Parent()->GetParent()->GetName().c_str(), "Zeppelin"))
                {
                    if (animation->GetRowOfCurrentIndex() != 3)
                    {
                        animation->SetRowForMultiAnimation(3);
                    }
                }
            }
            else if (CannonPos.y > AttackingPos.y)
            {
                if (strstr(Parent()->GetParent()->GetName().c_str(), "Tower"))
                {
                    if (animation->GetRowOfCurrentIndex() != 0)
                    {
                        animation->SetRowForMultiAnimation(0);
                    }
                }
                else if (strstr(Parent()->GetParent()->GetName().c_str(), "Zeppelin"))
                {
                    if (animation->GetRowOfCurrentIndex() != 6)
                    {
                        animation->SetRowForMultiAnimation(6);
                    }
                }
            }
        }
        else if (CannonPos.x < AttackingPos.x)
        {

            if (CannonPos.y < AttackingPos.y)
            {
                if (strstr(Parent()->GetParent()->GetName().c_str(), "Tower"))
                {
                    if (animation->GetRowOfCurrentIndex() != 3)
                    {
                        animation->SetRowForMultiAnimation(3);
                    }
                }
                else if (strstr(Parent()->GetParent()->GetName().c_str(), "Zeppelin"))
                {
                    if (animation->GetRowOfCurrentIndex() != 2)
                    {
                        animation->SetRowForMultiAnimation(2);
                    }
                }
            }
            else if (CannonPos.y > AttackingPos.y)
            {
                if (strstr(Parent()->GetParent()->GetName().c_str(), "Tower"))
                {
                    if (animation->GetRowOfCurrentIndex() != 1)
                    {
                        animation->SetRowForMultiAnimation(1);
                    }
                }
                else if (animation->GetRowOfCurrentIndex() != 1)
                {
                    if (strstr(Parent()->GetParent()->GetName().c_str(), "Zeppelin"))
                    {
                        animation->SetRowForMultiAnimation(1);
                    }
                }
            }
        }
        else if (CannonPos.x > AttackingPos.x)
        {
            if (CannonPos.y < AttackingPos.y)
            {
                if (strstr(Parent()->GetParent()->GetName().c_str(), "Tower"))
                {
                    if (animation->GetRowOfCurrentIndex() != 5)
                    {
                        animation->SetRowForMultiAnimation(5);
                    }
                }
                else if (strstr(Parent()->GetParent()->GetName().c_str(), "Zeppelin"))
                {
                    if (animation->GetRowOfCurrentIndex() != 4)
                    {
                        animation->SetRowForMultiAnimation(4);
                    }
                }
            }
            else if (CannonPos.y > AttackingPos.y)
            {
                if (strstr(Parent()->GetParent()->GetName().c_str(), "Tower"))
                {
                    if (animation->GetRowOfCurrentIndex() != 7)
                    {
                        animation->SetRowForMultiAnimation(7);
                    }
                }
                else if (strstr(Parent()->GetParent()->GetName().c_str(), "Zeppelin"))
                {
                    if (animation->GetRowOfCurrentIndex() != 5)
                    {
                        animation->SetRowForMultiAnimation(5);
                    }
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


// Initialize the current state of the this component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 this = Pointer to the this component.
void Cannon::Init()
{
    if (this->stateCurr == cCannonInvalid)
    {
        this->stateCurr = cCannonIdle;
    }

}

// Update the current state of the this component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 this = Pointer to the this component.
//	 dt = Change in time (in seconds) since the last game loop.
void Cannon::Update(float dt)
{
    stateCurr = stateNext;
    AnimationPtr animation = this->Parent()->GetComponent<Animation>(cAnimation);
    Stats* stats = this->Parent()->GetParent()->GetComponent<Stats>(cStats);
    switch (this->stateCurr)
    {
    case cCannonIdle:
    {

      
        if(stats->GetReloadTime() == stats->GetReloadTimer() && stats->GetIsReloading()== false)
        {
            this->stateNext = cCannonAngleChange;
        }
        
        if (stats->GetIsDead())
        {
            this->stateNext = cCannonDead;
        }
        //this->stateNext = cCannonAttack;
        break;
    }
    case cCannonAngleChange:
    {
        //advancing the Cannon attack animation
        if (animation->GetRowOfCurrentIndex() != this->Parent()->GetComponent<Sprite>(cSprite)->GetSpriteSource()->GetNumRows() - 1 )
        {
            //animation->SetIsRunning(true);
            // animation->AnimationPlay();
        }
        //stats->SetReload(true);
        this->stateNext = cCannonIdle;
        break;
    }
    case cCannonDead:
    {
        if (stats->GetCurrentRespawnTimer() <= 0.0f)
        {
            if (animation->GetRowOfCurrentIndex() != this->Parent()->GetComponent<Sprite>(cSprite)->GetSpriteSource()->GetNumRows() - 1)
            {
                animation->SetRowForMultiAnimation(0);
            }
            Parent()->Has(Sprite)->SetAlpha(1);
            this->stateNext = cCannonIdle;
        }
        else if(stats->GetIsDead())
        {
            Parent()->Has(Sprite)->SetAlpha(0);
        }
        break;
    }
    }
    //TeleporterUpdateObject(this->parent);
}

// Exit the current state of the this component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 this = Pointer to the this component.
//	 dt = Change in time (in seconds) since the last game loop.
void Cannon::Exit()
{

}

