/*****************************************************************//**
 * \file         ZeppelinBehavior.cpp
 * \authors       Nate White,Cameron Myers, Ethan Mclaughlin,Nick Leben
 * \par          Email: Nate.white\@digipen.edu,cameron.myers\@digipen.edu, ethan.mclaughlin\@digipen.edu, nick.leben\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Behavior.h"
#include "GameObject.h"
#include "Physics.h"
#include "ZeppelinBehavior.h"
#include "MEInput.h"
#include "MERand.h"
#include <glm/gtx/rotate_vector.hpp>
#include "Transform.h"
#include "Stats.h"
#include "Collider.h"
#include "Animation.h"
#include "Audio.h"
#include "Sprite.h"
#include "MESpaceManager.h"
#include "MESpace.h"
#include "CannonBehavior.h"
#include "TowerBehavior.h"
#include "MEtimer.h"
#include <typeinfo>

#include "BehaviorEmitterTest.h"
#include "BehaviorHealthBar.h"
#include "CityBehavior.h"
#include "Mesh.h"
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

class EmitterTest;
static const float ZeppelinSpeedMax = 100.f;


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

//static void ZeppelinUpdateVelocity(float dt);
/**
 * .
 * 
 * \param first
 *      a game object that is colliding
 * \param second
 *      a game object that is colliding
 */



//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

/**
 * /brief   
 *      default constructor.
 * 
 */
Zeppelin::Zeppelin(cZeppelinType _type):Behavior(cZeppelinInvalid, cZeppelinMove, "BIG ERROR"), type(_type)
{
}
/**
 * /brief 
 *      copy constructor.
 * 
 * \param rhs
 *      behavior to copy from
 */
Zeppelin::Zeppelin(const Zeppelin& rhs):Behavior(rhs.stateCurr, rhs.stateNext, "BIG ERROR"), type(rhs.type)
{
}
/**
 * /brief   
 *      default destructor.
 * 
 */
Zeppelin::~Zeppelin()
{
    Name.clear();
}
/**
 * /brief 
 *      funtion to clone a Zeppelin behavior.
 * 
 * \return 
 *      Pointer to new Zeppelin behavior
 */
BehaviorPtr Zeppelin::Clone()const
{
    Zeppelin* zeppelin = new Zeppelin(type);
    if (zeppelin)
    {
        *zeppelin = *this;
    }
    return zeppelin;
}

// Initialize the current state of the this component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 this = Pointer to the this component.
void Zeppelin::Init()
{

        /*if ( this->stateCurr == cSpaceshipIdle )
        {
            ColliderPtr collider = GameObjectGetCollider( this->parent );
            if ( collider )
            {
                ColliderSetCollisionHandler( collider, thisSpaceshipCollisionHandler );
            }
        }
        */

    //timer for turning off the spawn smoke cloud particle effect in update -ian
    spawn_timer = METimerManager::MakeTimer(0.1f);

        if ( this->stateCurr == cZeppelinInvalid )
        {
            SetStateCurr(cZeppelinIdle);
            if (this->Parent()->GetChildByName("Cannon"))
            {
                this->Parent()->GetChildByName("Cannon")->Has(Animation)->SetRowForMultiAnimation(5);
            }
            else if (this->Parent()->GetChildByName("Cannon2"))
            {
                this->Parent()->GetChildByName("Cannon2")->Has(Animation)->SetRowForMultiAnimation(5);
            }
            else if (this->Parent()->GetChildByName("TyrantCannon"))
            {
                this->Parent()->GetChildByName("TyrantCannon")->Has(Animation)->SetRowForMultiAnimation(5);
            }
            //init the health bar to max health
        }
}

// Update the current state of the this component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 this = Pointer to the this component.
//	 dt = Change in time (in seconds) since the last game loop.
void Zeppelin::Update( float dt)
{
    stateCurr = stateNext;
    Stats* stats = this->Parent()->GetComponent<Stats>(cStats);

    //turn off the spawn smoke cloud after it spawns on screen -ian
    if (METimerManager::check(spawn_timer))
    {
        if (Parent()->GetChildByName("Particle_SpawnCloud1"))
        {
            Parent()->GetChildByName("Particle_SpawnCloud1")->Has(Emitter)->SetSpawnRate(0.f);
        }
        if (Parent()->GetChildByName("Particle_SpawnCloud2"))
        {
            Parent()->GetChildByName("Particle_SpawnCloud2")->Has(Emitter)->SetSpawnRate(0.f);
        }
        if (Parent()->GetChildByName("Particle_SpawnCloud3"))
        {
            Parent()->GetChildByName("Particle_SpawnCloud3")->Has(Emitter)->SetSpawnRate(0.f);
        }
        if (Parent()->GetChildByName("Particle_SpawnCloud4"))
        {
            Parent()->GetChildByName("Particle_SpawnCloud4")->Has(Emitter)->SetSpawnRate(0.f);
        }
        if (Parent()->GetChildByName("Particle_SpawnCloud5"))
        {
            Parent()->GetChildByName("Particle_SpawnCloud5")->Has(Emitter)->SetSpawnRate(0.f);
        }
        if (Parent()->GetChildByName("Particle_SpawnCloud6"))
        {
            Parent()->GetChildByName("Particle_SpawnCloud6")->Has(Emitter)->SetSpawnRate(0.f);
        }
    }

    if (stats->GetIsDead())
    {
        this->stateCurr = cZeppelinDead;
    }
    switch (this->stateCurr)
    {
    case cZeppelinIdle:
    {
        idle();
        break;
    }
    case cZeppelinMove:
    {
        Parent()->Has(Audio)->PlaySoundEffect(SOUND(PLAY_ZEPPELINHOVER));
        SetVelocity(static_cast<float>(stats->GetMaxSpeed()));
        auto* transform = this->Parent()->GetComponent<Transform>(cTransform);
        if (transform->GetTranslation().x>750.0f)
        {
            this->stateNext = cZeppelinIdle;
            this->SetVelocity(0.0f);
        }
        break;
    }
        //zeppelin is attacking
    case cZeppelinAttack:
    {
        if (stats->GetIsDead() == true) break;
        auto* transform = this->Parent()->GetComponent<Transform>(cTransform);

    		//this needs to be based on zeppelin type

        ///*****************************************
		//if at edge of screen
        if (transform->GetTranslation().x > 750.0f)
        {
            this->stateNext = cZeppelinIdle;
            this->SetVelocity(0.0f);
            // glm::vec3 pos = transform->GetTranslation();
              //pos.x = 0;
            //  transform->SetTranslation(pos);
        }
        else
        {
            this->stateNext = cZeppelinMove;
        }

        break;
    }
    case cZeppelinDrag:
        if(Input::IsMousePressed(GLFW_MOUSE_BUTTON_RIGHT))
        {
            MouseFollow();
        }
        else
        {
            this->stateNext = cZeppelinIdle;
        }
        break;
    case cZeppelinDead:
        do_dead();
        break;
    }
    //TeleporterUpdateObject(this->parent);
}

// Exit the current state of the this component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 this = Pointer to the this component.
//	 dt = Change in time (in seconds) since the last game loop.
void Zeppelin::Exit( )
{

}

/**
 * \brief collision handler for the zeppelins
 * \param other 
 */
void Zeppelin::CollisionHandler(GameObjectPtr other)
{
    //check if colliding with something thats dead
    //check if colliding with a tower
    if (strstr(other->GetName().c_str(), "Tower"))
    {

        if (other->Has(Stats)->GetIsDead() == true)
        {
            //this makes sure the zeppelin stays moving if the target is dead
            this->stateNext = cZeppelinMove;
            return;
        }
        //if not reloading

        /// <summary>
        /// attack sequence
        /// </summary>
        if (Parent()->GetComponent<Stats>(cStats)->GetIsReloading() == false)
        {
            aim_cannon(other);
            do_attack<Tower>(other);
        }

    }
    //check if colliding with the city
    else if (strstr(other->GetName().c_str(), "DwarfCity"))
    {

        if (other->Has(Stats)->GetIsDead() == true)
        {
            //this makes sure the zeppelin stays moving if the target is dead
            //also make the zeppelins move again when the city is dead
            this->stateNext = cZeppelinMove;
            return;
        }

        aim_cannon(other);
        do_attack<City>(other);


        //get physics
    }
}


//Private Functions
/**
 * Set the Velocity for the Zeppelin.
 * 
 * \param speed
 *      new velocity of Zeppelin
 */
void Zeppelin::SetVelocity( float speed )
{

    auto physics = this->Parent()->GetComponent<Physics>(cPhysics);
	auto transform = this->Parent()->GetComponent<Transform>(cTransform);

    glm::vec3 velocity = physics->GetVelocity();
	glm::vec3 angle = {0, 0, 0};
    angle.x = cosf(transform->GetRotation());
    angle.y = sinf(transform->GetRotation());


    velocity = angle * speed;
    physics->SetVelocity(velocity);
    //ME_CORE_INFO(velocity.x);

}
/**
 * Set the Zeppelin to follow the mouse.
 * 
 */
void Zeppelin::MouseFollow()
{
    auto physics = this->Parent()->GetComponent<Physics>(cPhysics);
    auto transform = this->Parent()->GetComponent<Transform>(cTransform);

    glm::vec3 velocity = { 0, 0, 0 };

    physics->SetVelocity(velocity);

    transform->SetTranslation(glm::vec3(Input::GetMouseX(), Input::GetMouseY(), 1.f));
}

void Zeppelin::play_explosion()
{
    //gets explosion child
    GameObjectPtr explosion = this->Parent()->GetChildByName("ZeppelinExplosion");
    //gets animation
    Animation* anim = explosion->Has(Animation);
    //plays  explosion
    anim->AnimationPlay(anim->getFrameCount(), anim->getFrameDuration(), false);

    //play the audio clip
    //this->Parent()->Has(Audio)->PlaySoundEffect(SOUND(PLAY_ZEPPELIN_DESTROYED));


}

void Zeppelin::Take_Damage(int damage)
{
    Stats* stats = this->Parent()->Has(Stats);
    if (stats->GetIsDead()) return;

    //check if the zeppelin just died
    if (stats->GetHealth() <= 0.5f)
    {
        stats->SetIsDead(true);
        JustDead = true;
        this->stateNext = cZeppelinDead;
        return;
    }
    //zeppelin is taking damage
    Parent()->Has(Audio)->PlaySoundEffect(SOUND(PLAY_ZEPPELIN_DAMAGED));
    stats->SetHealth(stats->GetHealth() - damage);
    this->stateNext = cZeppelinIdle;
}

void Zeppelin::idle()
{
    Stats* stats = Parent()->Has(Stats);

    if (stats->GetIsDead() == true)
    {
        this->stateNext = cZeppelinDead;
        return;
    }
   // else this->stateNext = cZeppelinMove;

}

void Zeppelin::do_dead()
{
    AnimationPtr animation = this->Parent()->GetComponent<Animation>(cAnimation);

    SetVelocity(0.0f);
    
    Parent()->Has(Sprite)->SetAlpha(0.0f);

    if (JustDead == true)
    {
        Parent()->Has(Audio)->PlaySoundEffect(SOUND(PLAY_ZEPPELIN_DESTROYED));


        dead_timer = METimerManager::MakeTimer(2.5f);
        play_explosion();
        animation->SetRowForMultiAnimation(animation->GetRowOfCurrentIndex() + 1);

        //Remove the zeppelin from the lane
        //LaneManager::Remove(LaneManager::FindLane(Parent()->GetName()), Parent()->GetName());

                //stop flapping the wings
        if (Parent()->GetChildByName("GremlinWing"))
        {
            Parent()->GetChildByName("GremlinWing")->Has(Animation)->SetIsRunning(false);
        }
        else if (Parent()->GetChildByName("TyrantWing"))
        {
            Parent()->GetChildByName("TyrantWing")->Has(Animation)->SetIsRunning(false);
        }
        if (Parent()->GetChildByName("Particle_Smoke"))
        {
            //stop spawning smoke particles
            Parent()->GetChildByName("Particle_Smoke")->Has(Emitter)->SetSpawnRate(0.f);
   
        }
        else if (Parent()->GetChildByName("Big_Smoke"))
        {
            //stop spawning smoke particles
            Parent()->GetChildByName("Big_Smoke")->Has(Emitter)->SetSpawnRate(0.f);
        }

    }

    else if (METimerManager::check(dead_timer))
    {
        MESpace* sandbox = MESpaceManager::GetSpace("Sandbox");

        if (sandbox)
        {
            sandbox->PutInInactive(this->Parent());
        }
    }
    //stop cannon sounds
    switch (type)
    {
    case cZeppelinTypeS:
        Parent()->Has(Audio)->PlayAudio(SOUND(STOP_CANNON_FIRE_GREMLIN));
        break;
    case cZeppelinTypeM:
        Parent()->Has(Audio)->PlayAudio(SOUND(STOP_CANNON_FIRE_BUGBEAR));
        break;
    case cZeppelinTypeL:
        Parent()->Has(Audio)->PlayAudio(SOUND(STOP_CANNON_FIRE_TYRANT));
        break;
    }

    JustDead = false;

}
template <class T>
void Zeppelin::do_attack(GameObjectPtr target)
{
    Stats* stats = Parent()->Has(Stats);

    //make sure the zeppelin isnt firing while reloading or dead
    if (stats->GetIsReloading() == false && stats->GetIsDead() == false)
    {
    	//get attack damage
        int attackDamage = stats->GetAttackDamage();
        //apply attack damage
        //NOTE: Templated to allow zeppelin to do damage to any class derived from Behavior
        //      Objects are guaranteed to only have one behavior, therefore this is possible
        //      an event system is better, but too much to implement last minute for one feature.
        target->GetComponent<T>(cBehavior)->Take_Damage(attackDamage);
        //enter reload state
        stats->SetReload(true);
        //post respective cannon fire Wwise event
        switch (type)
        {
        case cZeppelinTypeS:
            Parent()->Has(Audio)->PlayAudio(SOUND(CANNON_FIRE_GREMLIN));
            break;
        case cZeppelinTypeM:
            Parent()->Has(Audio)->PlayAudio(SOUND(CANNON_FIRE_BUGBEAR));
            break;
        case cZeppelinTypeL:
            Parent()->Has(Audio)->PlayAudio(SOUND(CANNON_FIRE_TYRANT));
            break;
        default:
            break;
        }
        //set state to attack
        this->stateNext = cZeppelinAttack;
	    
    }
}

void Zeppelin::aim_cannon(GameObjectPtr target)
{
    Stats* stats = Parent()->Has(Stats);
    if (stats->GetIsReloading() == true) return;

    //aim the cannon
    if (Parent()->GetChildByName("Cannon"))
    {
        Parent()->GetChildByName("Cannon")->GetComponent<Cannon>(cBehavior)->SetDirectionForChildCannon(target->Has(Transform));
        Parent()->GetChildByName("Cannon")->Has(Animation)->SetIsRunning(true);
    }
    else if (Parent()->GetChildByName("Cannon2"))
    {
        Parent()->GetChildByName("Cannon2")->GetComponent<Cannon>(cBehavior)->SetDirectionForChildCannon(target->Has(Transform));
        Parent()->GetChildByName("Cannon2")->Has(Animation)->SetIsRunning(true);
    }
    else if (Parent()->GetChildByName("TyrantCannon"))
    {
        Parent()->GetChildByName("TyrantCannon")->GetComponent<Cannon>(cBehavior)->SetDirectionForChildCannon(target->Has(Transform));
        Parent()->GetChildByName("TyrantCannon")->Has(Animation)->SetIsRunning(true);
    }

}
