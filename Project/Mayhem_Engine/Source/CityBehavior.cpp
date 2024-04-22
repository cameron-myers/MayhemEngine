/*****************************************************************//**
 * \file         CityBehavior.cpp
 * \author       Nate WHite
 * \par          Email: nate.white\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#include "Behavior.h"
#include "GameObject.h"
#include "Physics.h"
#include "CityBehavior.h"
#include "Audio.h"
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
#include "glm/glm.hpp"
#include <vector>
#include "CannonBehavior.h"
#include "BehaviorGoldCount.h"
#include "MESpaceManager.h"
#include "MEtimer.h"
#include "MESpace.h"
#include "BackgroundBehavior.h"
#include "BehaviorStar_Programmer.h"
#include "BehaviorRoundTimer.h"
#include "ZeppelinBehavior.h"
#include "MEtimer.h"
#include "MEInput.h"
//#include "LaneManager.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------



typedef enum cCity { cCityInvalid = -1, cCityIdle, cCityAttack, cCityDamage, cCityDead } CityStates;
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

/// <summary>
/// constructor
/// </summary>
City::City():Behavior(cCityInvalid, cCityIdle, "BIG ERROR")
{
}
/// <summary>
/// copy constructor
/// </summary>
/// <param name="rhs"></param>
City::City(const City& rhs):Behavior(rhs.stateCurr, rhs.stateNext, "BIG ERROR")
{

}
/// <summary>
/// destructor
/// </summary>
City::~City()
{
}
/// <summary>
/// 
/// </summary>
/// <returns>new behavior that is clone</returns>
BehaviorPtr City::Clone() const
{
    City* city = new City;
    if (city)
    {
        *city = *this;
    }
    return city;
}
/// <summary>
/// handles the cities collision
/// </summary>
/// <param name="other"></param>
void City::CollisionHandler(GameObjectPtr other)
{
    if (strstr(other->GetName().c_str(), "Zeppelin"))
    {
        do_attack(other);
    }    

}

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


// Initialize the current state of the this component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 this = Pointer to the this component.
void City::Init()
{



    if (this->stateCurr == cCityInvalid)
    {
        this->stateCurr = cCityIdle;
    }
   //this->Parent()->GetComponent<Collider>(cCollider)->SetHandler(&CollisionHandler);

}

// Update the current state of the this component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 this = Pointer to the this component.
//	 dt = Change in time (in seconds) since the last game loop.
void City::Update(float dt)
{
    /*
    if (Input::IsKeyReleased(GLFW_KEY_K))
    {
        stateNext = cCityDead;
        JustDead = true;
    }

    if (Input::IsKeyReleased(GLFW_KEY_N))
    {
        play_explosion();
    }
    */
    stateCurr = stateNext;
    //AnimationPtr animation = this->Parent()->GetComponent<Animation>(cAnimation);
    Stats* stats = this->Parent()->GetComponent<Stats>(cStats);

    //Parent()->Has(Audio)->PlaySoundEffect(SOUND(PLAY_CITY_DESTROYED));

    switch (this->stateCurr)
    {
        case cCityIdle:
        { 
            idle();
            break;
        }
        
        case cCityDead:
        {
            
            this->Parent()->Has(Animation)->SetRowForMultiAnimation(2);
            //MESpaceManager::SetGameWin();

            //In this spot, grab the data from the stars and timer to put into the background, so it can be retrieved in the win state
            MESpace* sndbox = MESpaceManager::GetSpace("Sandbox");
            GameObjectManager* Actlist = sndbox->GetActive();
            GameObject* star1 = Actlist->GetObjectByName("StarTest");
            bool star1time = METimerManager::check(static_cast<Star*>(star1->Has(Behavior))->GetTimer());

            GameObject* star2 = Actlist->GetObjectByName("StarTest1");
            bool star2time = METimerManager::check(static_cast<Star*>(star2->Has(Behavior))->GetTimer());

            GameObject* star3 = Actlist->GetObjectByName("StarTest2");
            bool star3time = METimerManager::check(static_cast<Star*>(star3->Has(Behavior))->GetTimer());
            //repeat process for 2nd star, dont have it implemented yet though

            GameObject* RoundTimer1 = Actlist->GetObjectByName("FuelGaugeFill"); //name is most likely way off for this, need to fix this later!!!!!! fixed it for you -ian
            float Timeleft = METimerManager::checktime(static_cast<RoundTimer*>(RoundTimer1->Has(Behavior))->GetTimer());

            GameObject* bg = Actlist->GetObjectByName("GameBackground"); //Wrong name for background most likely, need to check
            static_cast<Background*>(bg->Has(Behavior))->SetTimeData(Timeleft);
            static_cast<Background*>(bg->Has(Behavior))->SetStarData(3 - static_cast<int>(star1time) - static_cast<int>(star2time) - static_cast<int>(star3time)); //add the other star to this, add one as there will always be 1 star for completeing the level.

            //go to win state
            GoldCount::ResetGold();
            do_dead();
            
            break;
        }
    }
    if (city_dead_timer)
    {
        if (METimerManager::check(city_dead_timer))
        {
            MESpaceManager::SetSpaceExclusive("GameWin");
        }
    }
    //TeleporterUpdateObject(this->parent);
}

// Exit the current state of the this component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 this = Pointer to the this component.
//	 dt = Change in time (in seconds) since the last game loop.
void City::Exit()
{

}

void City::Take_Damage(float damage)
{
    Stats* stats = this->Parent()->GetComponent<Stats>(cStats);
    stats->SetHealth(stats->GetHealth() - damage);
    stats->SetIsHurt(true);

    if (stats->GetHealth() <= stats->GetMaxHealth() / 2)
    {
        this->Parent()->Has(Animation)->SetRowForMultiAnimation(1);
    }

    //check if dead
    if (stats->GetHealth() <= 0.f)
    {
        //JustDead = true;
        this->Parent()->Has(Animation)->SetRowForMultiAnimation(2);
        stats->SetIsDead(true);
        this->stateNext = cCityDead;
        JustDead = true;
        return;
    }

    if (stateCurr != cCityDead)
    {
        this->stateNext = cCityIdle;
        Parent()->Has(Audio)->PlaySoundEffect(SOUND(PLAY_CITY_DAMAGED));
    }

}

void City::Stun(float time)
{
}

void City::idle()
{
    Stats* stats = this->Parent()->GetComponent<Stats>(cStats);

    if (stats->GetIsDead())
    {
        this->stateNext = cCityDead;
    }
    else if (stats->GetIsHurt())
    {
        this->stateNext = cCityDamage;
    }
}

void City::do_attack(GameObjectPtr target)
{
    Stats* stats = this->Parent()->GetComponent<Stats>(cStats);


    if (Parent()->GetComponent<Stats>(cStats)->GetIsReloading() == false)
    {
        //get attack and health
        int attackDamage = Parent()->GetComponent<Stats>(cStats)->GetAttackDamage();
        //do damage
        target->GetComponent<Zeppelin>(cBehavior)->Take_Damage(attackDamage);

        //set reload
        Parent()->GetComponent<Stats>(cStats)->SetReload(true);
        Parent()->GetComponent<Behavior>(cBehavior)->SetStateNext(cCityAttack);
        Parent()->Has(Audio)->PlaySoundEffect(SOUND(PLAY_STONETOWER_ATTACK));
        stats->SetReload(true);

        for (auto child : Parent()->GetChildren())
        {
            
	        if(child->GetComponent<Cannon>(cBehavior))
	        {
                child->Has(Animation)->SetIsRunning(true);
	        }
        }

    }

}

void City::do_dead()
{
    if (JustDead == true)
    {

        Parent()->Has(Audio)->PlaySoundEffect(SOUND(PLAY_CITY_DESTROYED));
        play_explosion();

        //MESpaceManager::SetGameWin();

        //In this spot, grab the data from the stars and timer to put into the background, so it can be retrieved in the win state
        MESpace* sndbox = MESpaceManager::GetSpace("Sandbox");
        GameObjectManager* Actlist = sndbox->GetActive();
        GameObject* star1 = Actlist->GetObjectByName("StarTest");
        bool star1time = METimerManager::checktime(static_cast<Star*>(star1->Has(Behavior))->GetTimer());
        //repeat process for 2nd star, dont have it implemented yet though

        GameObject* RoundTimer1 = Actlist->GetObjectByName("FuelGaugeFill"); //name is most likely way off for this, need to fix this later!!!!!!
        float Timeleft = METimerManager::checktime(static_cast<RoundTimer*>(RoundTimer1->Has(Behavior))->GetTimer());

        GameObject* bg = Actlist->GetObjectByName("GameBackground"); //Wrong name for background most likely, need to check
        static_cast<Background*>(bg->Has(Behavior))->SetTimeData(Timeleft);
        static_cast<Background*>(bg->Has(Behavior))->SetStarData(static_cast<int>(star1time) + 1); //add the other star to this, add one as there will always be 1 star for completeing the level.

        METimerManager::stop(static_cast<RoundTimer*>(RoundTimer1->Has(Behavior))->GetTimer());

        //Timer for Win state
    	city_dead_timer = METimerManager::MakeTimer(5.0f);

    }
    else if (METimerManager::check(city_dead_timer))
    {
        MESpaceManager::SetSpaceExclusive("GameWin");
    }

    JustDead = false;

    //go to win state
    GoldCount::ResetGold();
}

void City::play_explosion()
{
   std::deque<GameObject*> children =  Parent()->GetChildren();

   for (GameObject* child : children)
   {
       if (child->Has(Animation))
       {
           //gets animation
           Animation* anim = child->Has(Animation);
           //plays  explosion
           anim->AnimationPlay(anim->getFrameCount(), anim->getFrameDuration(), false);
       }
   }
}
