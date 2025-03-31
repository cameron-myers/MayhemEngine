/*****************************************************************//**
 * \file         Stats.h
 * \author       Nate White
 * \par          Email: nate.white\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#pragma once

#include "Component.h"
#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"

class Animation;
class Sprite;
class GameObject;

typedef GameObject* GameObjectPtr;
typedef Animation* AnimationPtr;
typedef Sprite* SpritePtr;


class Component;
class Stats : public Component
{
public:
	Stats();
	~Stats();
	Stats(const Stats& rhs);
	Component* Clone() const;
	void Read(const char* bufchar);
	void Save();
	void Update(float dt);
	//getters
	int GetMaxHealth();
	int GetReloadTime();
	int GetRespawnRate();
	int GetAttackDamage();
	int GetMaxSpeed();
	int GetCost();
	int GetHealth();
	int GetLevel();
	int GetMaxAttackDamage();
	

	int GetID();
	const char* GetObjectType();
	float GetReloadTimer();
	float GetCurrentRespawnTimer();
	bool GetIsHurt();
	bool GetIsDead();
	bool GetIsAttacking();
	bool GetIsReloading();
	int GetMaxHealthLvlIndex();
	int GetMaxSpeedLvlIndex();
	int GetAttackDamageLvlIndex();
	//setters
	void SetHealth(int health);
	void SetMaxSpeed(int MaxSpeed);
	void SetReloadTimer(float reloadTimer);
	void SetRespawnTimer(float _respawnTimer);
	void SetIsHurt(bool status);
	void SetIsDead(bool status);
	void SetReload(bool status);
	void SetMaxHealthLvlIndex(int indexOfLevel);
	void SetMaxMaxSpeedLvlIndex(int indexOfLevel);
	void SetAttackDamageLvlIndex(int indexOfLevel);
	//upgrades
	void UpgradeMaxHealth();
	void UpgradeMaxSpeed();
	void UpgradeAttackDamage();


private:
		/*searialized*/
		//total health a object can have
		int MaxHealth;
		//the time between firing
		int ReloadTime;
		//time it takes for a dead object to respawn
		int RespawnRate;
		//the amount of damage each bullet or attack does
		int AttackDamage;
		//the MaxSpeed at which a object moves
		int MaxSpeed;
		//the cost of gold for each object
		int Cost;
		int ListCount;

		/*non searialized*/
		//current health status
		int Health;
		//health tracking to set is hurt bool
		int PrevHealth;
		//how much time is left untill reloaded
		float ReloadTimer;
		//how much time is left untill respawned
		float RespawnTimer;
		//true if the object is activley getting hurt
		bool IsHurt;
		//true if the object is currently attacking another object
		bool IsAttacking;
		//true if the object is declared dead(not destroyed)
		bool IsDead;
		//true if the object is reloading
		bool Reload;
		//array of all the max health stats for different levels
		std::vector<int> MaxHealthLvls;
		//array of all the MaxSpeeds stats for different levels
		std::vector<int> MaxSpeedLvls;
		//array of all the attack damage stats for different levels
		std::vector<int> AttackDamageLvls;
		//the type of object, ex zeppelin,tower,etc.
		const char* ObjectType;
		//upgrade level
		int Level = 0;
		
};
