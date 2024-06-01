/*****************************************************************//**
 * \file         MESpace.cpp
 * \author       Cameron Myers, Nick Leben
 * \par          Email: cameron.myers\@digipen.edu, Nick.leben\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#include "MESpace.h"
#include "GameObjectManager.h"
#include "Log.h"
#include "Physics.h"
#include "Transform.h"
#include "MESerializer.h"
#include <rapidjson/document.h>
#include "Behavior.h"

MESpace::MESpace(const char* _name) :active(false), name(_name)
{
	
}



/// <summary>
/// space constructor
/// </summary>
/// <param name="filename"></param>
MESpace::MESpace(std::string const& filename)
	:active(false),
	activeList(),
	InactiveList()
{
	this->name = filename;
	std::string prefab = filename;

#ifdef _DEBUG
	prefab.insert(0, "../Assets/Spaces/");
#endif // _DEBUG

#ifdef _DISTRIBUTE
	prefab.insert(0, "./Assets/Spaces/");
#endif // _DISTRIBUTE


	prefab.append(".json");

	//open the prefab json
	std::string json;
	std::string clearData(json);
	json = MESerializer::OpenFileRead(prefab.c_str());
	const char* bufchar = json.c_str();
	rapidjson::Document d;
	d.Parse(bufchar);

	const rapidjson::Value& head = d["Objects"];
	std::vector<std::string> result(head.Size());
	for (rapidjson::SizeType i = 0; i < head.Size(); ++i)
	{
		result[i] = head[i].GetString();
	}

	for (const std::string obj : result)
	{
		GameObject* newObj = activeList.FactoryBuild(obj.c_str());
	}

	//have the behaviors init
	activeList.Init();
	InactiveList.Init();
}
/// <summary>
/// moves an object from the active list to the inactive list
/// </summary>
/// <param name="gamObj"></param>
void MESpace::PutInInactive(GameObject* gamObj)
{
	activeList.pop(gamObj);
	InactiveList.push(gamObj);
}

/**
 * \brief moves an object from the inactive list to the active list
 * \param gamObj 
 */
void MESpace::PutInActive(GameObject* gamObj)
{
	InactiveList.pop(gamObj);
	activeList.push(gamObj);
}

/// <summary>
/// load the space
/// </summary>
void MESpace::Load() 
{
	if (*GetName() == "Sandbox")
	{
		GameObject* Obj1 = activeList.FactoryBuild("Zeppelin1");
		//sets zeppelin to idle
		Obj1->Has(Behavior)->SetStateNext(0);
		Obj1->Has(Transform)->SetTranslation(glm::vec3{ 0,1000,0 });

		GameObject* Obj2 = activeList.FactoryBuild("Zeppelin2");
		Obj2->Has(Behavior)->SetStateNext(0);
		Obj2->Has(Transform)->SetTranslation(glm::vec3{ 0,1000,0 });

		GameObject* Obj3 = activeList.FactoryBuild("Zeppelin3");
		Obj3->Has(Behavior)->SetStateNext(0);
		Obj3->Has(Transform)->SetTranslation(glm::vec3{ 0,1000,0 });
	}
}

std::vector<std::string> MESpace::GetActiveListFromFile(std::string const& filename)
{
	std::string prefab = filename;

#ifdef _DEBUG
	prefab.insert(0, "../Assets/Spaces/");
#endif // _DEBUG

#ifdef _DISTRIBUTE
	prefab.insert(0, "./Assets/Spaces/");
#endif // _DISTRIBUTE


	prefab.append(".json");

	//open the prefab json
	std::string json;
	std::string clearData(json);
	json = MESerializer::OpenFileRead(prefab.c_str());
	const char* bufchar = json.c_str();
	rapidjson::Document d;
	d.Parse(bufchar);

	const rapidjson::Value& head = d["Objects"];
	std::vector<std::string> result(head.Size());
	for (rapidjson::SizeType i = 0; i < head.Size(); ++i)
	{
		result.emplace_back(head[i].GetString());
	}

	return result;
}
