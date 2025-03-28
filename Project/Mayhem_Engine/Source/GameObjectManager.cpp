/*********************************************************************************************************************/
/*!
  \file              GameObjectManager.cpp
  \author            Nicholas Leben
  \par               Email: nick.leben\@digipen.edu
  \par               Course: GAM200
  \par               Copyright © 2022 DigiPen, All rights reserved.

  \brief
	this implements the manager for game objects
 */
 /*********************************************************************************************************************/

/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/
#include "Log.h"
#include "GameObjectManager.h"

#include <filesystem>

#include "MESerializer.h"
#include "Sprite.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "Component.h"
#include "BackgroundBehavior.h"
#include "Behavior.h"
#include "BehaviorButton.h"
#include "BehaviorSpawnButton.h"
#include "Collider.h"
#include "ZeppelinBehavior.h"
#include "Transform.h"
#include "TowerBehavior.h"
#include "ChildObject.h"
#include "LaneManager.h"
#include "rapidjson/filewritestream.h"
#include "MEInput.h"
#include "MESpace.h"
#include "MESpaceManager.h"
#include "MEEditor.h"
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>

#include "BehaviorEmitterTest.h"
#include "Engine.h"
#include "boost/lexical_cast.hpp"
#pragma warning(disable:4996)

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
unsigned int GameObjectManager::objectCount;
unsigned int GameObjectManager::objectMax;
using namespace rapidjson;
/// <summary>
/// constructor
/// </summary>
GameObjectManager::GameObjectManager()
{

}
/// <summary>
/// param constructor
/// </summary>
/// <param name="_objects"></param>
GameObjectManager::GameObjectManager(std::deque<GameObject*> _objects)
	:objectList(_objects)
{
	objectCount = (int)objectList.size();
	objectMax = 100;
}
/// <summary>
/// destructor
/// </summary>
GameObjectManager::~GameObjectManager()
{
	objectList.clear();
	objectCount = 0;
	objectMax = 100;
}
/*!********************************************************************************************************************
	\brief
		initializes the game object manager
**********************************************************************************************************************/
void GameObjectManager::Init()
{
	for (GameObjectPtr gameObj : objectList)
	{
		if (gameObj)
		{
			BehaviorPtr behavior = gameObj->Has(Behavior);
			if(behavior)
			{
				behavior->Init();
			}
			EmitterBehaviorPtr behavior2 = gameObj->Has(EmitterBehavior);
			if (behavior2)
			{
				behavior2->Init();
			}
			if(gameObj->IsParent())
			{
				for(GameObjectPtr child : gameObj->GetChildren())
				{
					BehaviorPtr behavior = child->Has(Behavior);
					if (behavior)
					{
						behavior->Init();
					}
					EmitterBehaviorPtr behavior2 = child->Has(EmitterBehavior);
					if (behavior2)
					{
						behavior2->Init();
					}
				}
			}
		}
	}
}
/*!********************************************************************************************************************
	\brief
		updates the game object manager and every object in the list
	\param dt
		delta time
**********************************************************************************************************************/
void GameObjectManager::Update(float dt)
{

	//saves all the gameobjects in the manager
	/*
	if (Input::IsKeyReleased(GLFW_KEY_U))
	{
		if (MEEditor::GetSelectedObject())
		{
			MESerializer::SaveGameObjectTransform(MEEditor::GetSelectedObject());
		}
	}
	*/
 	for (GameObjectPtr gameObj : objectList)
	{
		if (gameObj)
		{
			gameObj->Update(dt);

			if (gameObj->Destroyed())
			{
				this->Remove(gameObj);
				objectCount--;
			}
		}
	}
}
/// <summary>
/// check if any objects are colliding
/// </summary>
void GameObjectManager::CheckCollisions()
{
	for (GameObjectPtr gamObj : objectList)
	{
		if (gamObj)
		{
			for (GameObjectPtr gam2 : objectList)
			{
				if (gam2)
				{
					if (gamObj != gam2)
					{
						if (gamObj->HasComponent(cCollider) && gam2->HasComponent(cCollider))
						{
							ColliderPtr col = dynamic_cast<ColliderPtr>(gamObj->Get(cCollider));
							col->Check(dynamic_cast<ColliderPtr>(gam2->Get(cCollider)));
						}
					}
				}
			}
		}
	}
}

/*!********************************************************************************************************************
	\brief
		draws the objects in the list
**********************************************************************************************************************/
void GameObjectManager::Draw()
{
	for (GameObjectPtr gameObj : objectList)
	{
		if (gameObj)
		{
			gameObj->Draw();
		}
	}
}
/*!********************************************************************************************************************
	\brief
		shutsdown the gamn object manager
**********************************************************************************************************************/
void GameObjectManager::Shutdown()
{
	//for (GameObjectPtr gameObj : objectList)
	//{
	//	if (gameObj)
	//	{ /*
	//		FILE* fp = fopen("../Assets/dataDump.json", "wb");
	//		StringBuffer w;
	//		Writer<StringBuffer>writer(w);
	//		writer.StartObject();
	//		writer.SetMaxDecimalPlaces(2);
	//		writer.Key("GameObject");
	//		writer.Key("name");
	//		std::string name = (gameObj->GetName());
	//		writer.String(name.c_str());
	//		if (gameObj->HasComponent(cTransform))
	//		{
	//			Transform* trans = gameObj->GetComponent<Transform>(cTransform);
	//			glm::vec3 translation = trans->GetTranslation();
	//			writer.Key("\Transform");
	//			writer.Key("translation");
	//			writer.StartArray();
	//			writer.Double(translation.x);
	//			writer.Double(translation.y);
	//			writer.Double(translation.z);
	//			writer.EndArray();
	//			writer.IsComplete();
	//			writer.Key("rotation");
	//			double rotation = trans->GetRotation();
	//			writer.Double(rotation);
	//			writer.Key("scale");
	//			writer.Double((trans->GetScale()).x);
	//		}
	//		if (gameObj->HasComponent(cSprite))
	//		{
	//			Sprite* spriter = gameObj->GetComponent<Sprite>(cSprite);
	//			writer.Key("\Sprite");
	//			writer.String("This is the sprite");
	//			writer.Key("frameIndex");
	//			writer.Int(spriter->GetFrame());
	//			writer.Key("alpha");
	//			writer.Double(spriter->GetAlpha());
	//			writer.Key("scale");
	//			writer.StartArray();
	//			writer.Int(spriter->GetScale().x);
	//			writer.Int(spriter->GetScale().y);
	//			writer.EndArray();
	//			writer.IsComplete();
	//			writer.Key("Mesh");
	//			//Mesh* mesh = spriter->GetMesh();
	//			//writer.String(mesh->getName().c_str());
	//			//writer.Key("SpriteSource");
	//			//SpriteSource* spriteSource = spriter->GetSpriteSource();
	//			//writer.String(spriteSource->getName().c_str());
	//		}
	//		if (gameObj->HasComponent(cAnimation))
	//		{
	//			Animation* ani = gameObj->GetComponent<Animation>(cAnimation);
	//			writer.Key("Animation");
	//			writer.Key("FrameCount");
	//			writer.Int(ani->getFrameCount());
	//			writer.Key("FrameDuration");
	//			writer.Double(ani->getFrameDuration());
	//			writer.Key("IsLooping");
	//			writer.Bool(ani->isLooping());
	//		}
	//		if (gameObj->HasComponent(cPhysics))
	//		{
	//			writer.Key("Physics");
	//		}
	//		if (gameObj->HasComponent(cCollider))
	//		{
	//			Collider* col = gameObj->GetComponent<Collider>(cCollider);
	//			writer.Key("Collider");
	//			writer.Key("Type");
	//			if (col->GetType() == TypeArea)
	//			{
	//				writer.String("circle");
	//				writer.Key("\Radius");
	//				//writer.Double(col->GetRadius());
	//			}
	//			if (col->GetType() == TypeBox)
	//			{
	//				writer.String("box");
	//				writer.Key("TopLeft");
	//				writer.StartArray();
	//				writer.Int(col->getTopLeft().x);
	//				writer.Int(col->getTopLeft().y);
	//				writer.Int(col->getTopLeft().z);
	//				writer.EndArray();
	//				writer.Key("BottomRight");
	//				writer.StartArray();
	//				writer.Int(col->getBotRight().x);
	//				writer.Int(col->getBotRight().y);
	//				writer.Int(col->getBotRight().z);
	//				writer.EndArray();
	//			}
	//		}
	//		writer.EndObject();
	//		writer.IsComplete();
	//		fprintf(fp, w.GetString());
	//		fclose(fp);
	//		writer.Reset(w);
	//		*/
	//		//gameObj->Write();
	//		//delete gameObj;
	//		//gameObj = NULL;
	//	}
	//}

	for (GameObjectPtr gameObj : objectList)
	{
		if (gameObj)
		{
			delete gameObj;
		}
	}
	objectList.clear();
	objectCount = 0;
}
/*!********************************************************************************************************************
	\brief
		add a game object to the list
	\param gameObject
		pointer to the game object to add to the list
**********************************************************************************************************************/
void GameObjectManager::Add(GameObjectPtr gameObject)
{

	objectList.push_back(gameObject);
	objectCount += 1;
	gameObject->SetID((int)objectList.size());

	if (objectCount > objectMax)
	{
		objectMax = objectCount;
	}
	return;
}
/*!********************************************************************************************************************
	\brief
		ge
		ts an object from the active list by name
	\param name
		 the string of the name to search for
	\return
		a pointer to the game object with the matching name
**********************************************************************************************************************/
GameObjectPtr GameObjectManager::GetObjectByName(std::string name)
{
	for (GameObjectPtr gamObj : objectList)
	{
		if (gamObj->IsNamed(name))
		{
			return gamObj;
		}
	}
	return nullptr;
}

/**
 * \brief 
 * \param _id 
 * \return 
 */
GameObjectPtr GameObjectManager::GetObjectByID(int _id)
{
	for(auto obj : objectList)
	{
		if(obj->GetID() == _id)
		{
			return obj;
		}
	}
	return nullptr;
}

/**
 * \brief 
 * \return reference to a vector of the objects in the manager
 */
std::vector<GameObject*> GameObjectManager::GetObjects()
{
	std::vector<GameObject*> objs;
	for(auto obj : objectList)
	{
		objs.push_back(obj);
	}
	return objs;
}
/// <summary>
/// get the amount of objects in the manager
/// </summary>
/// <returns>int of object amount</returns>
int GameObjectManager::GetObjectCount()
{
	return (int)objectList.size();
}
/// <summary>
/// get IDs of all objects in the list
/// </summary>
/// <returns>deque of int IDs</returns>
std::deque<int> GameObjectManager::GetObjectIDs()
{
	std::deque<int> temp;
	for (auto obj : objectList)
	{
		temp.push_back(obj->GetID());
	}
	return temp;
}
/// <summary>
/// get all object names
/// </summary>
/// <returns>vector of strings of object names</returns>
std::vector<std::string> GameObjectManager::GetObjectNames()
{
	std::vector<std::string> temp;
	for (auto obj : objectList)
	{
		temp.push_back(obj->GetName());
	}
	return temp;
}
/// <summary>
/// builds the object and adds it to the object manager
/// </summary>
/// <param name="objectName"></param>
/// <returns>the new object</returns>
GameObjectPtr GameObjectManager::FactoryBuild(const char* objectName)
{
	if (objectName)
	{
		GameObjectPtr gamObj = this->GetObjectByName(objectName);

		if (!gamObj)
		{
			char pathName[FILENAME_MAX] = "";

#ifdef _DEBUG
			sprintf_s(pathName, _countof(pathName), MAYHEM_DIR("\\Assets\\GameObjects\\%s.json"), objectName);
#endif // _DEBUG


#ifdef _DISTRIBUTE
			sprintf_s(pathName, _countof(pathName), "./Assets/GameObjects/%s.json", objectName);
#endif // _DISTRIBUTE

			/*
			std::string json;
			std::string clearData(json);
			json = MESerializer::OpenFileRead(pathName);
			const char* bufchar = json.c_str();
			rapidjson::Document d;
			d.Parse(bufchar);
			*/

			gamObj = new GameObject;
			gamObj->Read(pathName);
			this->Add(gamObj);
			
			
			return gamObj;
		}
		else if (gamObj)
		{
			GameObjectPtr gamClone = new GameObject(gamObj);
			this->Add(gamClone);

			return gamClone;
		}
	}
	return NULL;
}
/// <summary>
/// remove the object from the list
/// </summary>
/// <param name="gamObj"></param>
/// <returns></returns>
bool GameObjectManager::Remove(GameObjectPtr gamObj)
{
	for (GameObjectPtr obj2 : objectList)
	{
		if (obj2 == gamObj)
		{
			delete gamObj;
			return 1;
		}
	}
	return 0;
}
/// <summary>
/// build a zeppelin
/// </summary>
/// <param name="startPos"></param>
/// <param name="type"></param>
/// <returns>pointer to the new zeppelin</returns>
GameObjectPtr GameObjectManager::ZeppelinBuild(glm::vec3 startPos, cZeppelinType type)
{
	std::string name = "Zeppelin";
	name += std::to_string(type);

	GameObjectPtr gamObj = GetObjectByName(name);
	if (gamObj)
	{
		//set the name to be unique
		const boost::uuids::uuid uuid = boost::uuids::random_generator()();
		gamObj->SetName(gamObj->GetName() + to_string(uuid));
		//copy
		GameObjectPtr gam = new GameObject(gamObj);
		//revert base zeppelin name
		gamObj->SetName(name);


		if (gam->HasComponent(cTransform))
		{
			TransformPtr tran = gam->Has(Transform);
			tran->SetTranslation(startPos);
		}
		BehaviorPtr test = new Zeppelin(type);
		gam->Add(test);
		test->Init();
		test->SetStateNext(cZeppelinMove);
		this->Add(gam);
		return gam;
	}
	else
	{
		gamObj = new GameObject;
		std::string pathName;
#ifdef _DEBUG
		pathName = MAYHEM_DIR("\\Assets\\GameObjects\\Zeppelin");
#endif // _DEBUG


#ifdef _DISTRIBUTE
		pathName = "./Assets/GameObjects/Zeppelin";
#endif // _DISTRIBUTE

		pathName += std::to_string(type);
		pathName += ".json";

		/*
		std::string json;
		std::string clearData(json);
		json = MESerializer::OpenFileRead(pathName.c_str());
		const char* bufchar = json.c_str();
		rapidjson::Document d;
		d.Parse(bufchar);
		*/

		gamObj->Read(pathName.c_str());
		this->Add(gamObj);

		BehaviorPtr test = new Zeppelin(type);
		gamObj->Add(test);
		test->Init();

		if (gamObj->HasComponent(cTransform))
		{
			TransformPtr tran = gamObj->GetComponent<Transform>(cTransform);
			tran->SetTranslation(startPos);
		}
		return gamObj;
	}
}
/// <summary>
/// get the list of objects
/// </summary>
/// <returns>deque of objects</returns>
std::deque<GameObjectPtr>* GameObjectManager::GetList()
{
	return &objectList;
}
/// <summary>
/// get child of a game object in the manager with a given name
/// </summary>
/// <param name="name"></param>
/// <returns>pointer to child</returns>
GameObject* GameObjectManager::GetChildByName(std::string name)
{
	for (GameObject* gamObj : objectList)
	{
		if (gamObj->GetChildByName(name))
		{
			return gamObj->GetChildByName(name);
		}
	}
	return NULL;
}
/// <summary>
/// remove object from the object manager
/// </summary>
/// <param name="gamObj"></param>
void GameObjectManager::pop(GameObject* gamObj)
{
	for (int i = 0; i != objectList.size(); ++i)
	{
		if (objectList.at(i) == gamObj)
		{
			objectList.erase(objectList.begin() + i);
			--objectCount;
			return;
		}
	}
}
/// <summary>
/// add object to the object manager
/// </summary>
/// <param name="gamObj"></param>
void GameObjectManager::push(GameObject* gamObj)
{
	objectList.push_back(gamObj);
	++objectCount;
}
