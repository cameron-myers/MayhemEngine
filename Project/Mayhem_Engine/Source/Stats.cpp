/*****************************************************************//**
 * \file         Stats.cpp
 * \author       Nate White
 * \par          Email: Nate.white\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#include <glfw/glfw3.h> 
#include "Stats.h"
#include "Log.h"
#include <rapidjson/document.h>
#include "GameObject.h"
#include <stdlib.h>

/// <summary>
/// constructor
/// </summary>
Stats::Stats()
:Component(cStats),
MaxHealth(0),
ReloadTime(0),
RespawnRate(0),
AttackDamage(0),
MaxSpeed(0),
Cost(0),
ListCount(0),
Health(0),
PrevHealth(0),
ReloadTimer(0.0f),
RespawnTimer(-90.0f),
IsHurt(false),
IsAttacking(false),
IsDead(false),
Reload(false),
AttackDamageLvls(0),
MaxHealthLvls(0),
MaxSpeedLvls(0),
ObjectType(""),
Level(0)
{
}
/// <summary>
/// destructor
/// </summary>
Stats::~Stats()
{
	Health = NULL;
	MaxHealth = NULL;
	ReloadTimer = NULL;
	RespawnRate = NULL;
	AttackDamage = NULL;
	MaxSpeed = NULL;
	Cost = NULL;
	ReloadTime = NULL;
	RespawnTimer = NULL;
	IsAttacking = NULL;
	IsHurt = NULL;
	IsDead = NULL;
	Reload = NULL;
	ObjectType = NULL;
	/*delete AttackDamageLvls;
	delete MaxHealthLvls;
	delete MaxSpeedLvls;*/
}
/// <summary>
/// copy constructor
/// </summary>
/// <param name="rhs"></param>
Stats::Stats(const Stats& rhs) :Component(cStats)
{
	MaxHealth = rhs.MaxHealth;
	ReloadTime = rhs.ReloadTime;
	RespawnRate = rhs.RespawnRate;
	AttackDamage = rhs.AttackDamage;
	MaxSpeed = rhs.MaxSpeed;
	Cost = rhs.Cost;
	ListCount = rhs.ListCount;
	Health = rhs.MaxHealth;
	PrevHealth = rhs.MaxHealth;
	ReloadTimer = rhs.ReloadTimer;
	RespawnTimer = rhs.RespawnTimer;
	IsHurt = false;
	IsAttacking = false;
	IsDead = false;
	Reload = false;
	ObjectType = rhs.ObjectType;
	AttackDamageLvls = rhs.AttackDamageLvls;
	MaxHealthLvls = rhs.MaxHealthLvls;
	MaxSpeedLvls = rhs.MaxSpeedLvls;
}
/// <summary>
/// 
/// </summary>
/// <returns>new stats component that is clone</returns>
Component* Stats::Clone() const
{
	Stats* component = new Stats;
	if (component)
	{
		*component = *this;
		component->IsHurt = false;
		component->IsDead = false;
		component->Health = component->MaxHealth;
		component->PrevHealth = component->MaxHealth;
		component->Reload = false;
	}
	return component;
}
/// <summary>
/// read stats from a file
/// </summary>
/// <param name="bufchar"></param>
void Stats::Read(const char* bufchar)
{
	using namespace rapidjson;

	Document d;
	d.Parse(bufchar);

	const rapidjson::Value& ghead = d["GameObject"];
			const Value& head = ghead["Stats"];
			const Value& listcount = head.GetArray()[0]["ListCount"];
			int Count = listcount.GetInt();
			ListCount = Count;
			int MaxHealthCount = 0;
			int MaxSpeedCount = 0;
			int MaxDamageCount = 0;
			for (int i = 0; i < Count; i++)
			{
				const Value& currentStats = head.GetArray()[i];
				if (currentStats.HasMember("MaxHealth"))
				{
					MaxHealthCount += 1;
				}
				if (currentStats.HasMember("MaxSpeed"))
				{
					MaxSpeedCount += 1;
				}
				if (currentStats.HasMember("MaxDamage"))
				{
					MaxDamageCount += 1;
				}
			}
			/*
			MaxHealthLvls = (int*)malloc(sizeof(int) * MaxHealthCount);
			MaxSpeedLvls = (int*)malloc(sizeof(int) * MaxSpeedCount);
			AttackDamageLvls = (int*)malloc(sizeof(int) * MaxDamageCount);
			*/
			for (int i = 0; i < Count; i++)
			{
				const Value& currentStats = head.GetArray()[i];
				if (i == 0)
				{
					if (currentStats.HasMember("MaxHealth"))
					{
						const Value& maxhealth = currentStats["MaxHealth"];
						MaxHealth = maxhealth.GetInt();
						Health = MaxHealth;
						PrevHealth = MaxHealth;
					}
					if (currentStats.HasMember("AttackDamage"))
					{
						const Value& atckdmg = currentStats["AttackDamage"];
						AttackDamage = atckdmg.GetInt();
					}
					if (currentStats.HasMember("RespawnRate"))
					{
						const Value& respawn = currentStats["RespawnRate"];
						RespawnRate = respawn.GetInt();
						RespawnTimer = static_cast<float>(RespawnRate);
					}
					if (currentStats.HasMember("ReloadTime"))
					{
						const Value& reload = currentStats["ReloadTime"];
						ReloadTime = reload.GetInt();
						ReloadTimer = static_cast<float>(ReloadTime);
					}
					if (currentStats.HasMember("Cost"))
					{
						const Value& cost = currentStats["Cost"];
						Cost = cost.GetInt();
					}
					if (currentStats.HasMember("MaxSpeed"))
					{
						const Value& maxSpeed = currentStats["MaxSpeed"];
						MaxSpeed = maxSpeed.GetInt();
					}
					if (currentStats.HasMember("ObjectType"))
					{
						const Value& objecttype = currentStats["ObjectType"];
						ObjectType = objecttype.GetString();
					}
				}
				
				if (currentStats.HasMember("MaxHealth"))
				{
					const Value& maxhealth = currentStats["MaxHealth"];
					MaxHealthLvls.push_back(maxhealth.GetInt());
				}
				
				if (currentStats.HasMember("MaxSpeed"))
				{
					const Value& maxhealth = currentStats["MaxSpeed"];
					MaxSpeedLvls.push_back(maxhealth.GetInt());
				}
				if (currentStats.HasMember("AttackDamage"))
				{
					const Value& maxhealth = currentStats["AttackDamage"];
					AttackDamageLvls.push_back(maxhealth.GetInt());
				}
			}

}
/// <summary>
/// update the stats component
/// </summary>
/// <param name="dt"></param>
void Stats::Update(float dt)
{
	if (this)
	{
		if (this->Reload)
		{
			this->ReloadTimer -=dt;
			if (ReloadTimer < 0.0f)
			{
				ReloadTimer = static_cast<float>(ReloadTime);
				this->Reload = false;
			}
		}
		if (IsDead)
		{
			if (this->RespawnRate != 0)
			{
				RespawnTimer -= dt;
			}
		}
		if (PrevHealth > Health)
		{
			IsHurt = TRUE;
			PrevHealth = Health;
			
		}
		else if (PrevHealth == Health)
		{
			IsHurt = FALSE;
		}
		else if (PrevHealth <= 0 && Health == MaxHealth)
		{
			PrevHealth = Health;
		}

	}
}
/// <summary>
/// upgrade the maximum health
/// </summary>
void Stats::UpgradeMaxHealth()
{
	/*
	if (MaxHealthLvls[GetMaxHealthLvlIndex() + 1] != NULL)
	{
		MaxHealth = MaxHealthLvls[GetMaxHealthLvlIndex() + 1];
	}
	else
	{
		ME_CORE_INFO("Health Is Maximum Level, Cannot Upgrade");
	}
	*/
	try {
		MaxHealth = MaxHealthLvls.at(GetMaxHealthLvlIndex() + 1);
	}
	catch (const std::out_of_range& error) {
		ME_CORE_INFO("Health Is Maximum Level, Cannot Upgrade");
	}
}
/// <summary>
/// upgrade the maximum speed
/// </summary>
void Stats::UpgradeMaxSpeed()
{
	/*
	if (MaxSpeedLvls[GetMaxSpeedLvlIndex() + 1])
	{
		MaxSpeed = MaxSpeedLvls[GetMaxSpeedLvlIndex() + 1];
	}
	else
	{
		ME_CORE_INFO("MaxSpeed Is Maximum Level, Cannot Upgrade");
	}
	*/
	try {
		MaxSpeed = MaxSpeedLvls.at(GetMaxSpeedLvlIndex() + 1);
	}
	catch (const std::out_of_range& error) {
		ME_CORE_INFO("Speed Is Maximum Level, Cannot Upgrade:");
	}
}
/// <summary>
/// upgrade the maximum attack damage
/// </summary>
void Stats::UpgradeAttackDamage()
{
	/*
	if (AttackDamageLvls[GetAttackDamageLvlIndex() + 1])
	{
		AttackDamage = AttackDamageLvls[GetAttackDamageLvlIndex() + 1];
	}
	else
	{
		ME_CORE_INFO("MaxDamage Is Maximum Level, Cannot Upgrade");
	}
	*/
	try {
		AttackDamage = AttackDamageLvls.at(GetAttackDamageLvlIndex() + 1);
	}
	catch (const std::out_of_range& error) {
		ME_CORE_INFO("Damage Is Maximum Level, Cannot Upgrade");
	}
}
//returns the current index of the level
int Stats::GetMaxHealthLvlIndex()
{
	for (int i = 0; MaxHealthLvls.at(i) != NULL; i++)
	{
		if (MaxHealth == MaxHealthLvls.at(i))
		{
			return i;
		}
	}
	ME_CORE_INFO("ERROR max health is having issues. Look at GetMaxHealthLvlIndex");
	return 0;
}
//returns the current index of the level
int Stats::GetMaxSpeedLvlIndex()
{
	for (int i = 0; MaxSpeedLvls.at(i) != NULL; i++)
	{
		if (MaxSpeed == MaxSpeedLvls.at(i))
		{
			return i;
		}
	}
	ME_CORE_INFO("ERROR max Speed is having issues. Look at GetMaxSpeedLvlIndex");
	return 0;
}
//returns the current index of the level
int Stats::GetAttackDamageLvlIndex()
{
	for (int i = 0; AttackDamageLvls.at(i) != NULL; i++)
	{
		if (AttackDamage == AttackDamageLvls.at(i))
		{
			return i;
		}
	}
	ME_CORE_INFO("ERROR Attack Damage is having issues. Look at GetAttackDamageLvlIndex");
	return 0;
}
/// <summary>
/// 
/// </summary>
/// <returns>int of the value of max health</returns>
int Stats::GetMaxHealth()
{
	return MaxHealth;
}
/// <summary>
/// 
/// </summary>
/// <returns>int of the reload time</returns>
int Stats::GetReloadTime()
{
	return ReloadTime;
}
/// <summary>
/// 
/// </summary>
/// <returns>int of the respawn rate</returns>
int Stats::GetRespawnRate()
{
	return RespawnRate;
}
/// <summary>
/// 
/// </summary>
/// <returns>int of the current attack damage</returns>
int Stats::GetAttackDamage()
{
	return AttackDamage;
}
/// <summary>
/// 
/// </summary>
/// <returns>int get the maximum speed</returns>
int Stats::GetMaxSpeed()
{
	return MaxSpeed;
}
/// <summary>
/// 
/// </summary>
/// <returns>int get the cost</returns>
int Stats::GetCost()
{
	return Cost;
}
/// <summary>
/// 
/// </summary>
/// <returns>int get the current health</returns>
int Stats::GetHealth()
{
	return Health;
}

int Stats::GetLevel()
{
	return Level;
}


int Stats::GetMaxAttackDamage()
{
	return AttackDamageLvls.back();
}

/// <summary>
/// 
/// </summary>
/// <returns>const char* get the type of object</returns>
const char* Stats::GetObjectType()
{
	return ObjectType;
}
/// <summary>
/// 
/// </summary>
/// <returns>float get the reload timer</returns>
float Stats::GetReloadTimer()
{
	return ReloadTimer;
}
/// <summary>
/// 
/// </summary>
/// <returns>float get the respawn timer</returns>
float Stats::GetCurrentRespawnTimer()
{
	return RespawnTimer;
}
/// <summary>
/// 
/// </summary>
/// <returns>bool find out if it is hurt or not</returns>
bool Stats::GetIsHurt()
{
	return IsHurt;
}
/// <summary>
/// 
/// </summary>
/// <returns>bool get if it is dead or not</returns>
bool Stats::GetIsDead()
{
	return IsDead;
}
/// <summary>
/// 
/// </summary>
/// <returns>bool get if it is attacking or not</returns>
bool Stats::GetIsAttacking()
{
	return IsAttacking;
}
/// <summary>
/// 
/// </summary>
/// <returns>bool get if it is reloading</returns>
bool Stats::GetIsReloading()
{
	return Reload;
}
/// <summary>
/// set the index of the maximum health level
/// </summary>
/// <param name="indexOfLevel"></param>
void Stats::SetMaxHealthLvlIndex(int indexOfLevel)
{
	MaxHealth = MaxHealthLvls.at(indexOfLevel);
}
/// <summary>
/// set the health
/// </summary>
/// <param name="health"></param>
void Stats::SetHealth(int health)
{
	Health = health;
}
/// <summary>
/// set the maximum speed
/// </summary>
/// <param name="maxSpeed"></param>
void Stats::SetMaxSpeed(int maxSpeed)
{
	MaxSpeed = maxSpeed;
}
/// <summary>
/// set the reload timer
/// </summary>
/// <param name="reloadTimer"></param>
void Stats::SetReloadTimer(float reloadTimer)
{
	ReloadTimer = reloadTimer;
}

void Stats::SetRespawnTimer(float _respawnTimer)
{
	RespawnTimer = _respawnTimer;
}

/// <summary>
/// set the reload status
/// </summary>
/// <param name="status"></param>
void Stats::SetReload(bool status)
{
	Reload = status;
}
//this is handled automatically in update. double check before calling this function.
void Stats::SetIsHurt(bool status)
{
	IsHurt = status;
}

void Stats::SetIsDead(bool status)
{
	IsDead = status;
}

/// <summary>
/// write the stats to json
/// </summary>
/// <param name="writer"></param>
void Stats::Save()
{
	/*writer->Key("ListCount");
	writer->Uint(ListCount);

	if (MaxHealth > 0)
	{
		writer->Key("MaxHealth");
		writer->Uint(MaxHealth);
	}
	//the time between firing
	if (ReloadTime > 0)
	{
		writer->Key("ReloadTime");
		writer->Uint(ReloadTime);
	}
	//time it takes for a dead object to respawn
	if (RespawnRate != 1)
	{
		writer->Key("RespawnRate");
		writer->Uint(RespawnRate);
	}
	//the amount of damage each bullet or attack does
	if (AttackDamage > 0)
	{
		writer->Key("AttackDamage");
		writer->Uint(AttackDamage);
	}
	//the MaxSpeed at which a object moves
	if (MaxSpeed > 0)
	{
		writer->Key("MaxSpeed");
		writer->Uint(MaxSpeed);
	}
	//the cost of gold for each object
	if (Cost > 0)
	{
		writer->Key("Cost");
		writer->Uint(Cost);
	}
*/
}