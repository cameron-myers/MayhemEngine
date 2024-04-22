/*****************************************************************//**
 * \file         GameObject.h
* \authors       Nate White,Ethan Mclaughlin,Nick Leben
 * \par          Email: Nate.white\@digipen.edu,EthanMclaughlin\@digipen.edu, nick.leben\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/

/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/
#pragma once

#include <cstdio>
#include "Component.h"
#include <string>
#include <deque>
#include <vector>

#define Has(type) GetComponent<type>(c##type)

class Component;
class Behavior;
class Transform;
class Audio;
class GameObject;
class ChildObject;
typedef GameObject* GameObjectPtr;
typedef Component* ComponentPtr;
typedef FILE* Stream;
//typedef Behavior* BehaviorPtr;
typedef Transform* TransformPtr;

class GameObject
{
public:
	GameObject();
	~GameObject();
	GameObject(const GameObjectPtr& cpy);
	GameObjectPtr Clone();
	void Add(ComponentPtr component);
	ComponentPtr Find(ComponentPtr component);
	void Update(float dt);
	void Draw();
	ComponentPtr Get(TypeEnum type);
	
	template<typename type>
	inline type* GetComponent(TypeEnum typeId)
	{
		//*****DO NOt USE**** use Has macro instead!
		
		return static_cast<type*>(Get(typeId));
	};
	bool HasComponent(TypeEnum type);
	bool Destroyed();
	void Destroy();
	void Read(const char* path);
	void Save();
	void Write();
	std::string GetName();
	int	 GetID();
	bool IsNamed(std::string input);
	void SetName(std::string input);
	void SetID(int _id);
	GameObject* ChildBuild(const char* objectName);
	GameObject* GetChildByName(std::string name);
	GameObject* SearchChildByName(std::string name);
	GameObject* GetParent();
	std::deque<GameObject*>& GetChildren();
	void SetParent(GameObject* parent);
	bool HasParent();
	bool HasChildren();
	bool IsParent();
	bool IsTimeTraveler();
	void AddChild(GameObject* child);
	int	 GetChildCount();
	std::vector<std::string> GetChildNames();
	void SetHidden(bool hide);

	
	//void load();

private:
	std::string name;
	int id;
	bool isDestroyed;
	bool hidden;
	ComponentPtr components[cCount];
	std::deque<GameObject*> children;
	GameObject* Parent;
};
