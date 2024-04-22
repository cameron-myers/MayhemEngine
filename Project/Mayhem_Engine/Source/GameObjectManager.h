//------------------------------------------------------------------------------
//
// File Name:	GameObjectManager.h
// Author(s):	Nick Leben, Cameron Myers
// Email:		nick.leben\@digipen.edu,cameron.myers\@digipen.edu
// Project:		Project 4
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

#include "GameObject.h"
#include <vector>
#include <deque>
#include "glm/vec3.hpp"

enum cZeppelinType;

class GameObject;

class GameObjectManager
{
public:
	GameObjectManager();
	GameObjectManager(std::deque<GameObject*> _objects);
	~GameObjectManager();
	void Init();
	void Update(float dt);
	//check collisions one day maybe?
	//YES!!!
	void CheckCollisions();
	void Draw();
	void Shutdown();
	void Add(GameObjectPtr gameObject);
	GameObjectPtr GetObjectByName(std::string name);
	GameObjectPtr GetObjectByID(int _id);
	std::vector<std::string> GetObjectNames();
	std::deque<int> GetObjectIDs();
	std::vector<GameObject*> GetObjects();
	int GetObjectCount();
	GameObjectPtr FactoryBuild(const char* objectName);
	bool Remove(GameObjectPtr gamObj);
	GameObjectPtr ZeppelinBuild(glm::vec3 startPos, cZeppelinType type);
	std::deque<GameObjectPtr>* GetList();
	GameObject* GetChildByName(std::string name);
	void pop(GameObject* gamObj);
	void push(GameObject* gamObj);

	GameObject* operator[] (int i) { return objectList[i]; }
	
private:

	static unsigned int objectCount;
	static unsigned int objectMax;
	std::deque<GameObjectPtr> objectList;
};
