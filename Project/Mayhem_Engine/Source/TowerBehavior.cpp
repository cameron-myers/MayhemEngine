/*****************************************************************//**
 * \file         TowerBehavior.cpp
 * \author       Nate White
 * \par          Email: nate.white\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#include "GameObject.h"
#include "Physics.h"
#include "TowerBehavior.h"
#include "MEInput.h"
#include "MERand.h"
#include <glm/gtx/rotate_vector.hpp>
#include "Transform.h"
#include "Stats.h"
#include "Collider.h"
#include "LaneManager.h"
#include "Animation.h"
#include "Audio.h"
#include "BehaviorGoldCount.h"
#include "BehaviorHealthBar.h"
#include "Collider.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "CannonBehavior.h"
#include "ZeppelinBehavior.h"
#include "Mesh.h"
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------


typedef enum cTower { cTowerInvalid = -1, cTowerIdle, cTowerAttack, cTowerDamage, cTowerStun, cTowerDead } TowerStates;
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------



/// <summary>
/// constructor
/// </summary>
/// <param name="_type"></param>
Tower::Tower(cTowerType _type) :Behavior(cTowerInvalid, cTowerIdle, "BIG ERROR"), type(_type)
{
}
/// <summary>
/// copy constructor
/// </summary>
/// <param name="rhs"></param>
Tower::Tower(const Tower& rhs):Behavior(rhs.stateCurr, rhs.stateNext, "BIG ERROR"), type(rhs.type)
{

}
/// <summary>
/// destructor
/// </summary>
Tower::~Tower()
{
}
/// <summary>
/// 
/// </summary>
/// <returns>new behavior that is clone</returns>
BehaviorPtr Tower::Clone() const
{
    Tower* tower = new Tower(type);
    if (tower)
    {
        *tower = *this;
    }
    return tower;
}
/// <summary>
/// handle the collision of a tower
/// </summary>
/// <param name="other"></param>
void Tower::CollisionHandler(GameObjectPtr other)
{
    //if colliding with zeppelin
    if (strstr(other->GetName().c_str(), "Zeppelin"))
    {
        Stats* stats = other->Has(Stats);
        if (stats->GetIsDead() == true || Parent()->Has(Stats)->GetIsDead())
        {
            return;
        }
        else
        {
            aim_cannon(other);
            do_attack(other);
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
void Tower::Init()
{
    if (this->stateCurr == cTowerInvalid)
    {
        this->stateCurr = cTowerIdle;
    }
    //this->Parent()->GetComponent<Collider>(cCollider)->SetHandler(&CollisionHandler);

}

// Update the current state of the this component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 this = Pointer to the this component.
//	 dt = Change in time (in seconds) since the last game loop.
void Tower::Update(float dt)
{
    stateCurr = stateNext;
    if (Parent()->Has(Stats)->GetIsDead()) stateCurr = cTowerDead;

    switch (this->stateCurr)
    {
        case cTowerIdle:
        {
            idle();
            break;
        }
        //attacking
        case cTowerAttack:
        {
            this->stateNext = cTowerIdle;
            break;
        }

        //animation set col
        case cTowerStun:
        {
            Stun(3);
            break;
        }
        case cTowerDead:
        {
           do_dead();
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
void Tower::Exit()
{

}
/// <summary>
/// get the type of tower
/// </summary>
/// <returns>cTowerType: type of tower</returns>
const cTowerType Tower::GetType()
{
    return type;
}

void Tower::play_explosion()
{
    //gets explosion child
    GameObjectPtr explosion = this->Parent()->GetChildByName("WoodTowerExplosion");
    //gets animation
    Animation* anim = explosion->Has(Animation);
    //plays  explosion
    anim->AnimationPlay(anim->getFrameCount(), anim->getFrameDuration(), false);

    switch (type)
    {
    case cTowerTypeWood:
        Parent()->Has(Audio)->PlaySoundEffect(SOUND(PLAY_WOODTOWER_DESTRUCTION));

        break;

    case cTowerTypeStone:
        Parent()->Has(Audio)->PlaySoundEffect(SOUND(PLAY_STONETOWER_DESTRUCTION));

        break;
    }
    //play the audio clip
}

void Tower::idle()
{
    Stats* stats = this->Parent()->Has(Stats);

    if (stats->GetIsDead() == true)
    {
        this->stateNext = cTowerDead;
        return;
    }
    //stats->SetHealth(stats->GetHealth() - 1);
    if (Input::IsKeyReleased(GLFW_KEY_T))
    {
       // this->stateNext = cTowerDamage;
    }

    else if (stats->GetIsHurt())
    {
        this->stateNext = cTowerDamage;
    }
}

void Tower::Take_Damage(float damage)
{
    //if dead - cant take damage
    Stats* stats = this->Parent()->Has(Stats);
    if (stats->GetHealth() % 50 == 0 && stats->GetHealth() != stats->GetMaxHealth())//checks if the value is 25,50,75
    {

        //set the frame index to destroyed
    }
    if (stats->GetHealth() <= 0.f)
    {
        JustDead = true;
        stats->SetIsDead(true);
        this->stateNext = cTowerDead;
        return;
    }
    else
    {
        stats->SetHealth(stats->GetHealth() - (int)damage);
        switch (type)
        {
        case cTowerTypeWood:
	        {
            Parent()->Has(Audio)->PlaySoundEffect(SOUND(PLAY_WOODTOWER_DAMAGED));
            break;
	        }
        case cTowerTypeStone:
	        {
            Parent()->Has(Audio)->PlaySoundEffect(SOUND(PLAY_STONETOWER_DAMAGED));
            break;
	        }
        }
    }
    //Parent()->Has(Audio)->PlaySoundEffect(SOUND(PLAY_WOODTOWER_DAMAGED));
    this->stateNext = cTowerIdle;

}

void Tower::Stun(float time)
{
    Stats* stats = this->Parent()->Has(Stats);
    stats->SetReloadTimer(stats->GetReloadTimer() + time);//hardcoded stun value there now
    stats->SetReload(true);
    this->stateNext = cTowerIdle;
}

void Tower::do_attack(GameObjectPtr target)
{
    AnimationPtr animation = this->Parent()->Has(Animation);
    Stats* stats = this->Parent()->Has(Stats);

    //checks for reloading
    if (Parent()->Has(Stats)->GetIsReloading() == false && target->Has(Stats)->GetIsDead() == false)
    {
        //get attack and health
        int attackDamage = Parent()->GetComponent<Stats>(cStats)->GetAttackDamage();
        //do damage
        target->GetComponent<Zeppelin>(cBehavior)->Take_Damage(attackDamage);
        //set zeppelin to damage state
        Parent()->GetComponent<Stats>(cStats)->SetReload(true);

        SetStateNext(cTowerAttack);

         //checks if row is correct
        if (animation->GetRowOfCurrentIndex() != Parent()->Has(Sprite)->GetSpriteSource()->GetNumRows() - 1)
        {
            animation->SetIsRunning(true);
        }

        //play zeppelin damaged
        switch (type)
        {
        case cTowerTypeWood:
            Parent()->Has(Audio)->PlayAudio(SOUND(PLAY_WOODTOWER_ATTACK));
            break;
        case cTowerTypeStone:
            Parent()->Has(Audio)->PlayAudio(SOUND(PLAY_STONETOWER_ATTACK));
            break;
        }
    }
}

void Tower::do_dead()
{
    AnimationPtr animation = this->Parent()->Has(Animation);
    Stats* stats = this->Parent()->Has(Stats);
    Audio* audio = Parent()->Has(Audio);

    if (JustDead)
    {
        play_explosion();

        //Serialize this 
        if (this->type == cTowerTypeWood)
        {
            GoldCount::ChangeGold(50);
        }
        else
        {
            GoldCount::ChangeGold(100);
        }

        //@todo might need to be updated
        audio->PlaySoundEffect(SOUND(PLAY_COINS_FALLING));

        animation->SetRowForMultiAnimation(animation->GetRowOfCurrentIndex() + 1);
    }
    //Parent()->Has(Audio)->PlaySoundEffect(SOUND(PLAY_STONETOWER_DESTRUCTION));
    if (stats->GetIsDead() == false)
    {
        animation->SetRowForMultiAnimation(0);
        stats->SetRespawnTimer(stats->GetRespawnRate());
        stats->SetHealth(stats->GetMaxHealth());
        stats->SetIsDead(false);
        //@todo get the respawn sound effect
        this->stateNext = cTowerIdle;

    }//is still dead
    else
    {
        //do respawn stuff
        if (stats->GetCurrentRespawnTimer() < 0.0f && stats->GetCurrentRespawnTimer() > -89.0f)
        {
            stats->SetIsDead(false);
            animation->SetRowForMultiAnimation(0);
            stats->SetHealth(stats->GetMaxHealth());
        }
    }
    JustDead = false;
}

void Tower::aim_cannon(GameObjectPtr target)
{
    AnimationPtr animation = this->Parent()->Has(Animation);
    Stats* stats = this->Parent()->Has(Stats);

	switch (type)
	{
    //DONT CARE TALK TO BEN
	case cTowerTypeWood:
		{
        //stone tower direct cannon
        if (Parent()->GetChildByName("TowerBallista"))
        {
            Parent()->GetChildByName("TowerBallista")->GetComponent<Cannon>(cBehavior)->SetDirectionForChildCannon(target->Has(Transform));
            if(stats->GetIsReloading() == true)
            {
                Parent()->GetChildByName("TowerBallista")->Has(Animation)->SetIsRunning(true);

            }
            //going to shoot
            if(stats->GetIsReloading() == false)
            {
                animation->AnimationAdvanceFrame();
            }
        }
        break;
		}
	case cTowerTypeStone:
		{
        //stone tower direct cannon
        if (Parent()->GetChildByName("TowerCannon"))
        {
            Parent()->GetChildByName("TowerCannon")->GetComponent<Cannon>(cBehavior)->SetDirectionForChildCannon(target->Has(Transform));
            Parent()->GetChildByName("TowerCannon")->Has(Animation)->SetIsRunning(true);
        }
        break;
		}
	}

}
