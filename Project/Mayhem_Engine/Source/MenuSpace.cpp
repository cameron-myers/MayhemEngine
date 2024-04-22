/*****************************************************************//**
 * \file         UIManager.cpp
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/

#include "MenuSpace.h"

#include <rapidjson/document.h>

#include "GameObjectFactory.h"
#include "MESerializer.h"

void MenuSpace::Load()
{
	Build("MenuUI");
}

void MenuSpace::Init()
{
	GameObjectManager* activeList = this->GetActive();
	GameObjectManager* InactiveList = this->GetInactive();
	activeList->Init();
	InactiveList->Init();
}

void MenuSpace::Update(float dt)
{
	this->GetActive()->Update(dt);
}

void MenuSpace::Draw()
{
	this->GetActive()->Draw();
}

void MenuSpace::Shutdown()
{
	this->GetActive()->Shutdown();
	this->GetInactive()->Shutdown();
}

void MenuSpace::Unload()
{
}

void MenuSpace::Build(std::string const& filename)
{
	std::string prefab = filename;

	prefab.insert(0, "../Assets/");
	prefab.append(".json");
	
	//open the prefab json
	std::string json;
	std::string clearData(json);
	json = MESerializer::OpenFileRead(prefab.c_str());
	const char* bufchar = json.c_str();
	rapidjson::Document d;
	d.Parse(bufchar);

	const rapidjson::Value& head = d["UIElements"];
	std::vector<std::string> result(head.Size());
	for(rapidjson::SizeType i = 0; i < head.Size(); ++i)
	{
		result[i] = head[i].GetString();
	}

	for(const std::string obj : result)
	{
		GameObject* newObj = GetActive()->FactoryBuild(obj.c_str());
	}
}
