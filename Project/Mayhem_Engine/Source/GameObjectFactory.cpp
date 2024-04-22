/*********************************************************************************************************************/
/*!
  \file              GameObjectFactory.cpp
  \author            Nicholas Leben
  \par               Email: nick.leben\@digipen.edu
  \par               Course: GAM200
  \par               Copyright © 2022 DigiPen, All rights reserved.
  \par				 credit to: https://www.youtube.com/watch?v=dZr-53LAlOw (thecherno on youtube)

  \brief
	this implements the spdlog library
 */
 /*********************************************************************************************************************/

/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/
#include "stdafx.h"
#include "GameObjectFactory.h"
#include "GameObject.h"
#include "MESerializer.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <string>
#include "MESpace.h"
#include "MESpaceManager.h"
/*!********************************************************************************************************************
	\brief
		builds a game object
	\param objectName
		const char* to the name of the object (also the fiilename)
	\return
		this function returns a pointer to a clone of the game object
**********************************************************************************************************************/

/*
GameObjectPtr GameObjectFactoryBuild(const char* objectName)
{
	if (objectName)
	{

		MESpace* space = MESpaceManager::GetCurrentSpace();
		GameObjectManager* inactive = space->GetInactive();
		GameObjectManager* active = space->GetActive();
		GameObjectPtr activeObj = active->GetObjectByName(objectName);
		GameObjectPtr inactiveObj = inactive->GetObjectByName(objectName);
		
		if (!inactiveObj && !activeObj)
		{
			char pathName[FILENAME_MAX] = "";
			sprintf_s(pathName, _countof(pathName), "../Assets/%s.json", objectName);

			std::string json;
			std::string clearData(json);
			json = MESerializer::OpenFileRead(pathName);
			const char* bufchar = json.c_str();
			rapidjson::Document d;
			d.Parse(bufchar);

			activeObj = new GameObject;
			activeObj->Read(pathName);
			active->Add(activeObj);
			return activeObj;
		}
		else if (activeObj)
		{
			GameObjectPtr gamClone = activeObj->Clone();

			return gamClone;
		}
		else if (inactiveObj)
		{
			GameObjectPtr gamClone = inactiveObj->Clone();

			return gamClone;
		}
	}
	return NULL;
}
*/
