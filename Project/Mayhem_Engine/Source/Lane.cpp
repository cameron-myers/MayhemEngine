/*********************************************************************************************************************/
/*!
 Code Uncounted 

  \brief
	this implements the Lane class
 */
 /*********************************************************************************************************************/


/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/
#include "Log.h"
#include "Lane.h"
#include "GameObject.h"
#include "MESerializer.h"
#include "Sprite.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
std::string Lane::name;
using namespace rapidjson;
/*!********************************************************************************************************************
	\brief
		default constructor 
**********************************************************************************************************************/
Lane::Lane()
{
	this->numTowers = 0;
	this->numZeppelens = 0;
	for (unsigned i = 0; i < cCount; ++i)
	{
		Objects[i] = nullptr;

	}
}

/**
 * \brief  deconstructor
 */
Lane::~Lane()
{

}
/*!********************************************************************************************************************
	\brief
		clone a lane
	\return
		Lane* to the clone
**********************************************************************************************************************/
Lane* Lane::Clone()
{
	Lane* clone = new Lane;
	clone->isDestroyed = this->isDestroyed;
	clone->setName(this->name);
	for (GameObjectPtr object : Objects)
	{
		if (object)
		{
			clone->Add(object->Clone());
		}
	}
	return clone;
}
/*!********************************************************************************************************************
	\brief
		adds a gameobject to the lane 
	\param component
		pointer to the gameobject added to the lane 
**********************************************************************************************************************/
void Lane::Add(GameObjectPtr object)
{
	for (unsigned i = 0; i < cCount; ++i)
	{
		if (Objects[i] == nullptr)
		{
			Objects[i] = object;
		}

	}
}/*!********************************************************************************************************************
	\brief
		finds a object in the lane 
	\param component
		pointer to the object in the lane 
	\return
		Pointer to the object in the lane ( or NULL)
**********************************************************************************************************************/
GameObjectPtr Lane::Find(GameObjectPtr object)
{
	for (unsigned i = 0; i < cCount; ++i)
	{
		if (Objects[i] == object)
		{
			return Objects[i];
		}

	}
	return NULL;
}
/*!********************************************************************************************************************
	\brief
		updates the Objects in the lane 
	\param dt
		delta time
**********************************************************************************************************************/
void Lane::Update(float dt)
{
	for (unsigned i = 0; i < cCount; ++i)
	{
		if (Objects[i] != nullptr)
		{
			Objects[i]->Update(dt);
		}

	}
}
/*!********************************************************************************************************************
	\brief
		draws the objects
**********************************************************************************************************************/
void Lane::Draw()
{
	for (unsigned i = 0; i < cCount; ++i)
	{
		if (Objects[i] != nullptr)
		{
			Objects[i]->Draw();
		}

	}
}
/*!********************************************************************************************************************
	\brief
		frees all Objects
**********************************************************************************************************************/
void Lane::Free()
{
	for (unsigned i = 0; i < cCount; ++i)
	{
		if (Objects[i] != nullptr)
		{
			Objects[i]->Free();
		}

	}
}
/*!********************************************************************************************************************
	\brief
		checks if the lane is destroyed
	\return
		boolean corresponding to the status of destruction
**********************************************************************************************************************/
bool Lane::Destroyed()
{
	return isDestroyed;
}
/*!********************************************************************************************************************
	\brief
		sets lane destroyed to true
**********************************************************************************************************************/
void Lane::Destroy()
{
	isDestroyed = true;
}
/*!********************************************************************************************************************
	\brief
		reads a lane from a json
	\param path
		the path for the json to be read from
**********************************************************************************************************************/
void Lane::Read(const char* path)
{
	std::string json;
	std::string clearData(json);
	json = MESerializer::OpenFileRead(path);
	const char* bufchar = json.c_str();
	Document d;
	d.Parse(bufchar);
	const rapidjson::Value& value = d["Lane"];
	const char* name = value["name"].GetString();
	std::string s(name);
	this->setName(name);

	GameObject* gameobject = new GameObject;
	gameobject->Read(bufchar);
	for (unsigned i = 0; i < cCount; ++i)
	{
		if (Objects[i] == nullptr)
		{
			Objects[i] = gameobject;
		}

	}
}
/*!********************************************************************************************************************
	\brief
		gets the name of the lane
	\return
		string of the lane's name
**********************************************************************************************************************/
std::string Lane::GetName()
{
	return name;
}
/*!********************************************************************************************************************
	\brief
		compares the lane name to the input
	\param input
		string of the name to be compared to
	\return
		boolean corresponding to the status of the comparison
**********************************************************************************************************************/
bool Lane::IsNamed(std::string input)
{
	if (input == name)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/*!********************************************************************************************************************
	\brief
		sets the name of the lane
	\param input
		the string to set the name of the lane to
**********************************************************************************************************************/
void Lane::setName(std::string input)
{
	name = input;
	ME_CORE_INFO(name);
}