/*****************************************************************//**
 * \file         BehaviorHealthBar.cpp
 * \author       Nate WHite
 * \par          Email: Nate.white\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#include "Behavior.h"
#include "BehaviorHealthBar.h"
#include "Sprite.h"
#include "Mesh.h"
#include "MEWindow.h"
#include "Stats.h"
#include "Transform.h"


//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------


typedef enum cHealthBar { cHealthBarInvalid = -1, cHealthBarIdle, cHealthBarDamage } HealthBarStates;
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

/// <summary>
/// constructor
/// </summary>
HealthBar::HealthBar() :Behavior(cHealthBarInvalid, cHealthBarIdle, "BIG ERROR")
{
}
/// <summary>
/// copy constructor
/// </summary>
/// <param name="rhs"></param>
HealthBar::HealthBar(const HealthBar& rhs) : Behavior(rhs.stateCurr, rhs.stateNext, "BIG ERROR")
{
}
/// <summary>
/// destructor
/// </summary>
HealthBar::~HealthBar()
{
}
/// <summary>
/// clone
/// </summary>
/// <returns>new behavior that is clone</returns>
BehaviorPtr HealthBar::Clone() const
{
    HealthBar* bar = new HealthBar;
    if (bar)
    {
        *bar = *this;
    }
    return bar;
}

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


// Initialize the current state of the this component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 this = Pointer to the this component.
void HealthBar::Init()
{
    if (this->stateCurr == cHealthBarInvalid)
    {
        this->stateCurr = cHealthBarIdle;
    }
    //init the bar size
   // Parent()->Has(Sprite)->GetMesh()->ScaleX(Parent()->GetParent()->Has(Stats)->GetHealth());

}

// Update the current state of the this component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 this = Pointer to the this component.
//	 dt = Change in time (in seconds) since the last game loop.
void HealthBar::Update(float dt)
{
    Stats* stats = Parent()->GetParent()->Has(Stats);
    //prevent bar from over scaling
    if(stats->GetHealth() <= 0.0f || stats->GetIsDead())
    {
        Parent()->Has(Sprite)->GetMesh()->ScaleX(0.0f);
    }
    else
    {
        Parent()->Has(Sprite)->GetMesh()->ScaleX((static_cast<float>(stats->GetHealth()) / static_cast<float>(stats->GetMaxHealth())) * 100.f);
    }
}

// Exit the current state of the this component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 this = Pointer to the this component.
//	 dt = Change in time (in seconds) since the last game loop.
void HealthBar::Exit()
{

}

void HealthBar::ScaleToHealth(float health)
{
    const auto max_health = static_cast<float>(Parent()->GetParent()->Has(Stats)->GetMaxHealth());
    Parent()->Has(Sprite)->GetMesh()->ScaleX(max_health/health);

}
