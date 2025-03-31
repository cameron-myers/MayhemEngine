#pragma once
/*********************************************************************************************************************/
/*!
  \file              MESerializer.cpp
  \author            Cameron Myers, nate.white, ethan mclaughlin
  \par               Email: cameron.myers\@digipen.edu, nate.white\@digipen.edu, ethan.mclaughlin\@digipen.edu
  \par               Course: GAM200
  \par               Copyright � 2022 DigiPen, All rights reserved.

  \brief
		this file has functions that do serialization/de-serialization things

 */
 /*********************************************************************************************************************/


/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/
#include <Log.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include "MESerializer.h"
#include "MEResources.h"
#include "Mesh.h"
#include "SpriteSource.h"
#include "GameObject.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "Transform.h"
#include "Physics.h"
#include "Sprite.h"
#include "Animation.h"
#include "Collider.h"
#include "Stats.h"
#include "Behavior.h"
#include "Audio.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filewritestream.h"
#include <Shlobj_core.h>
#include <filesystem>

#include "Engine.h"
#include "stdio.h"
#include "rapidjson/pointer.h"

using namespace rapidjson;
Timer* MESerializer::SaveCooldown;

/*!********************************************************************************************************************
	\brief
		this function does a thing
	\param x
		this parameter is for a thing
	\return
		this function returns thing
**********************************************************************************************************************/
void MESerializer::Init()
{
	// hello, my name is cole and i desparately crave the warm embrace of God()
}

/*!********************************************************************************************************************
	\brief
		NOT USED RN
	\param x
		this parameter is for a thing
	\return
		this function returns thing
**********************************************************************************************************************/
GameObject MESerializer::LoadGameObject(const char* filepath)
{
	GameObject newGameObject;

	std::string json;
	std::string clearData(json);
	json = MESerializer::OpenFileRead(filepath);
	const char* bufchar = json.c_str();
	Document d;
	d.Parse(bufchar);
	//load each name, audioID, etc. into each game object
	const rapidjson::Value& head = d["GameObject"];
	std::string name = head["name"].GetString();

	newGameObject.SetName(name);

	const Value& tHead = d["Transform"];
	//const Value& translation = translation["name"];
	const Value& xHalfSize = head["xHalfSize"];
	const Value& yHalfSize = head["yHalfSize"];

	//new stuff
	return newGameObject;
}
/*!********************************************************************************************************************
	\brief
		this function does a thing
	\param x
		this parameter is for a thing
	\return
		this function returns thing
**********************************************************************************************************************/
Mesh* MESerializer::LoadMesh(const char* filepath)
{
	std::string json;
	json = OpenFileRead(filepath);
	const char* bufchar = json.c_str();
	Document d;
	d.Parse(bufchar);

	const Value& head = d["Mesh"];
	const Value& name = head["name"];
	const Value& xHalfSize = head["xHalfSize"];
	const Value& yHalfSize = head["yHalfSize"];
	const Value& vert = head["VerteciesCount"];
	
	const Value& verts = head["Vertecies"];
	int vertCount = vert.GetInt();
	Vertex* vertices = new Vertex[vertCount];
	glm::vec4 rgba{0};
	glm::vec2 xy{0};
	glm::vec2 uv{0};
	for (int i = 0; i < vertCount; i++)
	{
	
		const Value& currentVertex = verts.GetArray()[i];
		xy.x = currentVertex["position_x"].GetFloat();
		xy.y = currentVertex["position_y"].GetFloat();
		uv.x = currentVertex["u"].GetFloat();
		uv.y = currentVertex["v"].GetFloat();
		rgba.r = currentVertex["r"].GetFloat();
		rgba.g = currentVertex["g"].GetFloat();
		rgba.b = currentVertex["b"].GetFloat();
		rgba.a = currentVertex["a"].GetFloat();
		vertices[i] = Vertex(xy, uv, rgba);
	}
	
	Mesh* temp = new Mesh();
	temp->SetName(name.GetString());
	temp->SetxHalfSize(xHalfSize.GetFloat());
	temp->SetyHalfSize(yHalfSize.GetFloat());
	temp->SetVertexCount(vert.GetInt());
	temp->SetVertices(vertices);

	return temp;
}

/*!********************************************************************************************************************
	\brief
		Loads data from the sprite source json
	\param filepath
		json file to read data from
	\return
		this function returns thing
**********************************************************************************************************************/
SpriteSource MESerializer::LoadSpriteSource(const char* filepath)
{
	std::string json;
	json = OpenFileRead(filepath);
	const char* bufchar = json.c_str();
	Document d;
	d.Parse(bufchar);

	//Get references to each value given a key
	const Value& head = d["SpriteSource"];
	const Value& name = head["name"];
	const Value& path = head["path"];
	const Value& numCols = head["numCols"];
	const Value& numRows = head["numRows"];

	std::string file = path.GetString();

// insert relative pathing for texture files
#ifdef _DEBUG
	file.insert(0, Engine::MAYHEM_DIR("\\Assets\\Art\\"));
#endif // _DEBUG

#ifdef _DISTRIBUTE
	file.insert(0, "./Assets/Art/");
#endif // _DISTRIBUTE

	
	SpriteSource temp = SpriteSource();
	//parse and store each value into the SpriteSource object
	temp.SetName(name.GetString());
	temp.SetPath(file);
	temp.SetNumRows(numRows.GetInt());
	temp.SetNumCols(numCols.GetInt());

	return temp;
}


void MESerializer::SaveGameObject(GameObject* Obj, int itterater)
{
	if (!SaveCooldown)
	{
		SaveCooldown = METimerManager::MakeTimer(0.2);
	}

	wchar_t filename[248];
	FILE* json = NULL;

	WCHAR path[MAX_PATH];
	HRESULT hr = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path);

	std::filesystem::path filepath = path;
	if (METimerManager::check(SaveCooldown))
	{
		std::filesystem::remove_all(filepath / "DigiPen" / "ZeppelinRush");
		METimerManager::set(0.2, SaveCooldown);
	}
	std::filesystem::create_directories(filepath / "DigiPen" / "ZeppelinRush");

	//wsprintf(filename, TEXT("%s\\%s"), path, TEXT("DigiPen\\ZeppelinRush\\TEMPTEST_REMOVE.json"));
	WCHAR itt[248]; //we shouldent need more then 10 didgets
	swprintf_s(itt, L"DigiPen\\ZeppelinRush\\%S(%d).json",(Obj->GetName()).c_str(), itterater);

	wsprintf(filename, TEXT("%s\\%s"), path, itt);


	_wfopen_s(&json, filename, L"w+");

	if (!json)
	{
		std::cout << "CANT MAKE JSON FILE TO SAVE!!!!!!!!!!" << std::endl;
		return;
	}
	char buf[1]; //number is arbitrary

	FileWriteStream strooom(json, buf, sizeof(buf));

	PrettyWriter<FileWriteStream> writer(strooom);

	writer.SetIndent(' ',0);
	writer.SetFormatOptions(kFormatSingleLineArray);

	writer.StartObject();
	writer.Key("GameObject");
	writer.StartObject();

	writer.Key("Name:");
	writer.String("TEMPNAME MAKE SURE TO FIX");

	if (Obj->Has(Transform))
	{
		writer.Key("Transform");
		writer.StartObject();
		//Obj->GetComponent<Transform>(cTransform)->GetMatrix();
		Obj->Has(Transform)->Save();
		writer.EndObject();
	}

	if (Obj->Has(Sprite))
	{
		writer.Key("Sprite");
		writer.StartObject();
		//Obj->GetComponent<Transform>(cTransform)->GetMatrix();
		Obj->Has(Sprite)->Save();
		writer.EndObject();
	}

	if (Obj->Has(Animation))
	{
		writer.Key("Animation");
		writer.StartObject();
		//Obj->GetComponent<Transform>(cTransform)->GetMatrix();
		Obj->Has(Animation)->Save();
		writer.EndObject();
	}

	if (Obj->Has(Physics))
	{
		writer.Key("Physics");
		writer.StartObject();
		//Obj->GetComponent<Transform>(cTransform)->GetMatrix();
		Obj->Has(Physics)->Save();
		writer.EndObject();
	}

	if (Obj->Has(Collider))
	{
		writer.Key("Collider");
		writer.StartObject();
		//Obj->GetComponent<Transform>(cTransform)->GetMatrix();
		Obj->Has(Collider)->Save();
		writer.EndObject();
	}

	if (Obj->Has(Stats))
	{
		writer.Key("Stats");
		writer.StartArray();
		writer.StartObject();
		//Obj->GetComponent<Transform>(cTransform)->GetMatrix();
		Obj->Has(Stats)->Save();
		writer.EndObject();
		writer.EndArray();
	}

	if (Obj->Has(Behavior))
	{
		writer.Key("Behavior");
		writer.String(Obj->Has(Behavior)->Name.c_str());
	}

	if (Obj->Has(Audio)) //audio is empty as we dont serialize anything in there, however the tag has to exist
	{
		writer.Key("Audio");
		writer.StartObject();
		//Obj->Has(Audio)->Save(&writer); //Nothing in audio
		writer.EndObject();
	}

	writer.EndObject();
	writer.EndObject();

	fclose(json);
}


void MESerializer::SaveGameObjectTransform(GameObject* Obj)
{
	if (Obj == NULL) //null checks to make sure things dont crash
	{
		return;
	}
	if (Obj->Has(Transform) == NULL)
	{
		return;
	}

	char pathName[FILENAME_MAX] = "";

	if(Obj->HasParent())
	{
		#ifdef _DEBUG
				sprintf_s(pathName, _countof(pathName), Engine::MAYHEM_DIR("\\Assets\\GameObjects\\Children\\%s.json").c_str(), Obj->GetName().c_str());
		#endif // _DEBUG

		#ifdef _DISTRIBUTE
				sprintf_s(pathName, _countof(pathName), "./Assets/GameObjects/Children/%s.json", Obj->GetName().c_str());
		#endif // _DISTRIBUTE
	}
	else
	{
		#ifdef _DEBUG
				sprintf_s(pathName, _countof(pathName), Engine::MAYHEM_DIR("\\Assets\\GameObjects\\%s.json").c_str(), Obj->GetName().c_str());
		#endif // _DEBUG

		#ifdef _DISTRIBUTE
				sprintf_s(pathName, _countof(pathName), "./Assets/GameObjects/%s.json", Obj->GetName().c_str());
		#endif // _DISTRIBUTE
	}


	std::string jsonstr;
	std::string clearData(jsonstr);
	jsonstr = MESerializer::OpenFileRead(pathName);
	const char* bufchar = jsonstr.c_str();
	Document d;
	d.Parse(bufchar);
	if (Obj->HasParent())
	{
		//Value& tran = d["translation"];
		Value* tranx = GetValueByPointer(d, "/GameObject/Transform/translation/0");
		tranx->SetFloat(Obj->Has(Transform)->GetRelativeTran().x);
		Value* trany = GetValueByPointer(d, "/GameObject/Transform/translation/1");
		trany->SetFloat(Obj->Has(Transform)->GetRelativeTran().y);
	}
	else
	{
		//Value& tran = d["translation"];
		Value* tranx = GetValueByPointer(d, "/GameObject/Transform/translation/0");
		tranx->SetFloat(Obj->Has(Transform)->GetTranslation().x);
		Value* trany = GetValueByPointer(d, "/GameObject/Transform/translation/1");
		trany->SetFloat(Obj->Has(Transform)->GetTranslation().y);
	}
	

	Value* rot = GetValueByPointer(d, "/GameObject/Transform/rotation");
	rot->SetFloat(Obj->Has(Transform)->GetRotation());

	Value* scalex = GetValueByPointer(d, "/GameObject/Transform/scale/0");
	scalex->SetFloat(Obj->Has(Transform)->GetScale().x);
	Value* scaley = GetValueByPointer(d, "/GameObject/Transform/scale/1");
	scaley->SetFloat(Obj->Has(Transform)->GetScale().y);


	//wchar_t filename[248];
	FILE* json = nullptr;

	//WCHAR path[MAX_PATH];
	//HRESULT hr = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path);

	//std::filesystem::remove_all(filepath / "DigiPen" / "ZeppelinRush");
	//std::filesystem::create_directories(filepath / "DigiPen" / "ZeppelinRush");

	//wsprintf(filename, TEXT("%s\\%s"), path, TEXT("DigiPen\\ZeppelinRush\\TEMPTEST_REMOVE.json"));
	//WCHAR itt[248]; //we shouldent need more then 10 didgets
	char itt[248]; //we shouldent need more then 10 didgets
	if (Obj->HasParent())
	{
		//swprintf_s(itt, L"Assets\\GameObjects\\Children\\%S.json", (Obj->GetName()).c_str());
		sprintf_s(itt, Engine::MAYHEM_DIR("Assets\\GameObjects\\Children\\%s.json").c_str(), (Obj->GetName()).c_str());
	}
	else
	{
		//swprintf_s(itt, L"Assets\\GameObjects\\%S.json", (Obj->GetName()).c_str());
		sprintf_s(itt, Engine::MAYHEM_DIR("\\Assets\\GameObjects\\%s.json").c_str(), (Obj->GetName()).c_str());
	}

	//_wfopen_s(&json, itt, L"w+");
	fopen_s(&json, itt, "w+");

	if (!json)
	{
		std::cout << "CANT MAKE JSON FILE TO SAVE!!!!!!!!!!" << std::endl;
		return;
	}

	char writeBuffer[65500] = "\0";
	FileWriteStream os(json, writeBuffer, sizeof(writeBuffer));
	PrettyWriter<FileWriteStream> writer(os);
	d.Accept(writer);
	fclose(json);
}

/**
 * \brief Save a given objects Sprite componenet
 * \param Obj 
 */
void MESerializer::SaveGameObjectSprite(GameObject* Obj)
{

	if (Obj == NULL) //null checks to make sure things dont crash
	{
		return;
	}
	if (Obj->Has(Transform) == NULL)
	{
		return;
	}

	char pathName[FILENAME_MAX] = "";

	if (Obj->HasParent())
	{
#ifdef _DEBUG
		sprintf_s(pathName, _countof(pathName), Engine::MAYHEM_DIR("\\Assets\\GameObjects\\Children\\%s.json").c_str(), Obj->GetName().c_str());
#endif // _DEBUG

#ifdef _DISTRIBUTE
		sprintf_s(pathName, _countof(pathName), "./Assets/GameObjects/Children/%s.json", Obj->GetName().c_str());
#endif // _DISTRIBUTE
	}
	else
	{
#ifdef _DEBUG
		sprintf_s(pathName, _countof(pathName), Engine::MAYHEM_DIR("\\Assets\\GameObjects\\%s.json").c_str(), Obj->GetName().c_str());
#endif // _DEBUG

#ifdef _DISTRIBUTE
		sprintf_s(pathName, _countof(pathName), "./Assets/GameObjects/%s.json", Obj->GetName().c_str());
#endif // _DISTRIBUTE
	}


	std::string jsonstr;
	std::string clearData(jsonstr);
	jsonstr = MESerializer::OpenFileRead(pathName);
	const char* bufchar = jsonstr.c_str();
	Document d;
	d.Parse(bufchar);

	Sprite* sprite = Obj->Has(Sprite);
	/*Value* frame_index = GetValueByPointer(d, "/GameObject/Sprite/frameIndex");
	frame_index->SetInt(sprite->GetFrame());*/


	Value* alpha = GetValueByPointer(d, "/GameObject/Sprite/alpha");
	alpha->SetFloat(sprite->GetAlpha());

	Value* r = GetValueByPointer(d, "/GameObject/Sprite/RGB/0");
	Value* g = GetValueByPointer(d, "/GameObject/Sprite/RGB/1");
	Value* b = GetValueByPointer(d, "/GameObject/Sprite/RGB/2");
	r->SetFloat(sprite->GetRGB().x);
	g->SetFloat(sprite->GetRGB().y);
	b->SetFloat(sprite->GetRGB().z);


	Value* sprite_source = GetValueByPointer(d, "/GameObject/Sprite/SpriteSource");
	std::string temp = sprite->GetSpriteSource()->GetName().c_str();
	sprite_source->SetString(StringRef(temp.c_str()), temp.length());


	//wchar_t filename[248];
	FILE* json = NULL;

	//WCHAR path[MAX_PATH];
	//HRESULT hr = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path);

	//std::filesystem::remove_all(filepath / "DigiPen" / "ZeppelinRush");
	//std::filesystem::create_directories(filepath / "DigiPen" / "ZeppelinRush");

	//wsprintf(filename, TEXT("%s\\%s"), path, TEXT("DigiPen\\ZeppelinRush\\TEMPTEST_REMOVE.json"));
	//WCHAR itt[248]; //we shouldent need more then 10 didgets
	char itt[248]; //we shouldent need more then 10 didgets
	if (Obj->HasParent())
	{
		//swprintf_s(itt, L"Assets\\GameObjects\\Children\\%S.json", (Obj->GetName()).c_str());
		sprintf_s(itt, Engine::MAYHEM_DIR("\\Assets\\GameObjects\\Children\\%s.json").c_str(), (Obj->GetName()).c_str());
	}
	else
	{
		//swprintf_s(itt, L"Assets\\GameObjects\\%S.json", (Obj->GetName()).c_str());
		sprintf_s(itt, Engine::MAYHEM_DIR("\\Assets\\GameObjects\\%s.json").c_str(), (Obj->GetName()).c_str());
	}

	//_wfopen_s(&json, itt, L"w+");
	fopen_s(&json, itt, "w+");

	if (!json)
	{
		std::cout << "CANT MAKE JSON FILE TO SAVE!!!!!!!!!!" << std::endl;
		return;
	}

	char writeBuffer[65500] = "\0";
	FileWriteStream os(json, writeBuffer, sizeof(writeBuffer));
	PrettyWriter<FileWriteStream> writer(os);
	d.Accept(writer);
	fclose(json);
}

/*!********************************************************************************************************************
	\brief
		this function does a thing
	\param x
		this parameter is for a thing
	\return
		this function returns thing
**********************************************************************************************************************/
void MESerializer::Destroy()
{
}
/// <summary>
/// read a glm vector3
/// </summary>
/// <param name="buffer"></param>
/// <returns>vec3 that has been read</returns>
static glm::vec3 readVec3(const char* buffer)
{
	glm::vec3 result = {0,0,0};
	std::string json;
	json = MESerializer::OpenFileRead(buffer);
	const char* bufchar = json.c_str();
	Document d;
	d.Parse(bufchar);

	const Value& a = d["translation"];

	for (SizeType i = 0; i < a.Size(); i++)
	{
		result[i] = a[i].GetFloat();
	}

	return result;
}

/*!********************************************************************************************************************
	\todo
		add support for printing out the error message
	\brief
		This function opens a file for reading
	\param filename
		name of the file to open
	\return
		returns an opened file pointer
**********************************************************************************************************************/
std::string MESerializer::OpenFileRead(const char* filename)
{

	std::string buffer;
	std::stringstream strStream;
	std::ifstream stream;
	//open the file
	stream.open(filename);
	

	if( stream.is_open() )
	{
		strStream << stream.rdbuf();
		buffer = strStream.str();

		ME_CORE_INFO("File:" + static_cast<std::string>(filename) + " was opened for read" );
		//ME_CORE_TRACE(buffer);
		return buffer;

	}
	else
	{
		ME_CORE_ERROR("File Error: " + stream.exceptions());
		ME_CORE_ERROR("File:" + static_cast<std::string>(filename) + " could not be opened");
		return "\0";
	}
}
