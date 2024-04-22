/*********************************************************************************************************************/
/*!
  \file              Transform.cpp
  \author            Nicholas Leben
  \par               Email: nick.leben\@digipen.edu
  \par               Course: GAM200
  \par               Copyright © 2022 DigiPen, All rights reserved.

  \brief
	the file contains a transform class
 */
 /*********************************************************************************************************************/


/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/
#include "Log.h"
#include "Transform.h"

#include <iostream>
#include <glm/gtx/transform.hpp>
#include "glm/glm.hpp"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/pointer.h"
#include "GameObject.h"
#include "MESerializer.h"

/*!********************************************************************************************************************
	\brief
		constructor for Transform
**********************************************************************************************************************/
Transform::Transform() 
	:Component(cTransform),
	translation { 0, 0, 0 }, 
	rotation  (0.0f),
	scale  { 1, 1 , 1 },
	isDirty  (1),
	matrix (glm::mat4(0.0f))
{
}
/*!********************************************************************************************************************
	\brief
		Destructor for Transform
**********************************************************************************************************************/
Transform::~Transform()
{
	translation = { 0, 0, 0 };
	rotation = NULL;
	scale = { 0, 0 , 0 };
	isDirty = NULL;
	matrix = {0 ,0 ,0 ,0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0 };
}
/*!********************************************************************************************************************
	\brief
		clones the transform
	\return
		a pointer to the cloned transform
**********************************************************************************************************************/
ComponentPtr Transform::Clone()const
{
	TransformPtr tran = new Transform;
	if (tran)
	{
		*tran = *this;
	}
	return tran;
}
/// <summary>
/// copy constructor
/// </summary>
/// <param name="rhs"></param>
Transform::Transform(const TransformPtr& rhs)
{
	translation = rhs->GetRelativeTran();
	rotation = rhs->rotation;
	scale = rhs->scale;
	matrix = rhs->matrix;
	isDirty = rhs->isDirty;
}
/*!********************************************************************************************************************
	\brief
		this function does a thing
	\param x
		this parameter is for a thing
	\return
		this function returns thing
**********************************************************************************************************************/
void Transform::Read(const char* bufchar)
{
	//vector to load with results
	glm::vec3 result = { 0,0,1 };
	//json document
	rapidjson::Document d;
	//parse the json
	d.Parse(bufchar);
	const rapidjson::Value& e = d["GameObject"];
	//go into the transform section
	const rapidjson::Value& value = e["Transform"];
	//set a to thee translation
	if (value.HasMember("translation"))
	{
		const rapidjson::Value& a = value["translation"];
		//loop through and load values into the result vector
		for (rapidjson::SizeType i = 0; i < a.Size(); i++)
		{
			result[i] = a[i].GetFloat();
		}
		translation = result;
	}
	if (value.HasMember("rotation"))
	{
		//set b to rotation
		const rapidjson::Value& b = value["rotation"];
		//set rotation to value found in json
		rotation = b.GetFloat();
	}
	if (value.HasMember("scale"))
	{
		//set c to scale
		const rapidjson::Value& c = value["scale"];
		//loop through and load values into the result vector
		for (rapidjson::SizeType i = 0; i < c.Size(); i++)
		{
			result[i] = c[i].GetFloat();
		}
		scale = result;
	}

}
/// <summary>
/// write transform data to json
/// </summary>
/// <param name="writer"></param>
void Transform::Save()
{

	using rapidjson::Writer;
	using namespace rapidjson;

	char pathName[FILENAME_MAX] = "";

	if (Parent()->HasParent())
	{
#ifdef _DEBUG
		sprintf_s(pathName, _countof(pathName), "../Assets/GameObjects/Children/%s.json", Parent()->GetName().c_str());
#endif // _DEBUG

#ifdef _DISTRIBUTE
		sprintf_s(pathName, _countof(pathName), "./Assets/GameObjects/Children/%s.json", Parent()->GetName().c_str());
#endif // _DISTRIBUTE
	}
	else
	{
#ifdef _DEBUG
		sprintf_s(pathName, _countof(pathName), "../Assets/GameObjects/%s.json", Parent()->GetName().c_str());
#endif // _DEBUG

#ifdef _DISTRIBUTE
		sprintf_s(pathName, _countof(pathName), "./Assets/GameObjects/%s.json", Parent()->GetName().c_str());
#endif // _DISTRIBUTE
	}


	std::string jsonstr;
	std::string clearData(jsonstr);
	jsonstr = MESerializer::OpenFileRead(pathName);
	const char* bufchar = jsonstr.c_str();
	rapidjson::Document d;
	d.Parse(bufchar);
	if (Parent()->HasParent())
	{
		//Value& tran = d["translation"];
		Value* tranx = GetValueByPointer(d, "/GameObject/Transform/translation/0");
		tranx->SetFloat(GetRelativeTran().x);
		Value* trany = GetValueByPointer(d, "/GameObject/Transform/translation/1");
		trany->SetFloat(GetRelativeTran().y);
	}
	else
	{
		//Value& tran = d["translation"];
		Value* tranx = GetValueByPointer(d, "/GameObject/Transform/translation/0");
		tranx->SetFloat(GetTranslation().x);
		Value* trany = GetValueByPointer(d, "/GameObject/Transform/translation/1");
		trany->SetFloat(GetTranslation().y);
	}


	Value* rot = GetValueByPointer(d, "/GameObject/Transform/rotation");
	rot->SetFloat(GetRotation());

	Value* scalex = GetValueByPointer(d, "/GameObject/Transform/scale/0");
	scalex->SetFloat(GetScale().x);
	Value* scaley = GetValueByPointer(d, "/GameObject/Transform/scale/1");
	scaley->SetFloat(GetScale().y);


	//wchar_t filename[248];
	FILE* json = NULL;

	//WCHAR path[MAX_PATH];
	//HRESULT hr = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path);

	//std::filesystem::remove_all(filepath / "DigiPen" / "ZeppelinRush");
	//std::filesystem::create_directories(filepath / "DigiPen" / "ZeppelinRush");

	//wsprintf(filename, TEXT("%s\\%s"), path, TEXT("DigiPen\\ZeppelinRush\\TEMPTEST_REMOVE.json"));
	//WCHAR itt[248]; //we shouldent need more then 10 didgets
	char itt[248]; //we shouldent need more then 10 didgets
	if (Parent()->HasParent())
	{
		//swprintf_s(itt, L"Assets\\GameObjects\\Children\\%S.json", (Obj->GetName()).c_str());
		sprintf_s(itt, "../Assets/GameObjects/Children/%s.json", (Parent()->GetName()).c_str());
	}
	else
	{
		//swprintf_s(itt, L"Assets\\GameObjects\\%S.json", (Obj->GetName()).c_str());
		sprintf_s(itt, "../Assets/GameObjects/%s.json", (Parent()->GetName()).c_str());
	}

	//_wfopen_s(&json, itt, L"w+");
	fopen_s(&json, itt, "w+");

	if (!json)
	{
		std::cout << "CANT MAKE JSON FILE TO SAVE!!!!!!!!!!" << std::endl;
		return;
	}

	char writeBuffer[65500];
	FileWriteStream os(json, writeBuffer, sizeof(writeBuffer));
	PrettyWriter<FileWriteStream> writer(os);
	d.Accept(writer);
	fclose(json);
	/*writer->Key("translation");
	writer->StartArray();
	writer->Uint(translation.x);
	writer->Uint(translation.y);
	writer->Uint(translation.z);
	writer->EndArray();
	writer->Key("rotation");
	writer->Double(rotation);
	writer->Key("scale");
	writer->Double(scale.x); //Scale data is a vec3, however data in the json is a float.*/
}

/*!********************************************************************************************************************
	\brief
		Get the matrix tied to the position values of the transform
	\return
		the matrix tied to the transfrom position/rotation
**********************************************************************************************************************/
glm::mat4 Transform::GetMatrix()
{
	if (this->isDirty)
	{
		glm::mat2 scaleMatrix = { {scale.x, 0.0f}, {0.0f, scale.y} };
		float new_rot = glm::radians(rotation);
		float cos_rot = cos(new_rot);
		float sin_rot = sin(new_rot);
		glm::mat2 rotationMatrix = { {cos_rot, sin_rot}, {-sin_rot, cos_rot} };
		glm::mat2 scal_rot = rotationMatrix * scaleMatrix;
		matrix = scal_rot;
		if(Parent()->HasParent())
		{
			matrix[3] = glm::vec4(GetTranslation(), 1.0f);// { translation.x, translation.y, 0.0f, 1.0f };
		}
		else
		{
			matrix[3] = { translation.x, translation.y, 0.0f, 1.0f };

		}

		isDirty = false;
	}
	return this->matrix;
}
/*!********************************************************************************************************************
	\brief
		getter for translation
	\return
		the translation of the transform
**********************************************************************************************************************/
glm::vec3 Transform::GetTranslation()
{
	if (this->Parent())
	{
		if (this->Parent()->HasParent())
		{
			glm::vec3 _translation = this->Parent()->GetParent()->Has(Transform)->GetTranslation() + this->translation;
			return _translation;
		}
	}
	return this->translation;
}

glm::vec3& Transform::GetTranslationRef()
{
	if (this->Parent())
	{
		if (this->Parent()->HasParent())
		{
			glm::vec3 _translation = this->Parent()->GetParent()->Has(Transform)->GetTranslation() + this->translation;
			return _translation;
		}
	}
	return (this->translation);
}
/*!********************************************************************************************************************
	\brief
		getter for rotation
	\return
		the rotation value of the transform
**********************************************************************************************************************/
float Transform::GetRotation()
{
	return this->rotation;
}
/*!********************************************************************************************************************
	\brief
		getter for scale
	\return
		the scale vector of the transform
**********************************************************************************************************************/
glm::vec3 Transform::GetScale()
{
	return this->scale;
}
/*!********************************************************************************************************************
	\brief
		setter for translation vector
	\param translation
		translation to set the transforms translation to
**********************************************************************************************************************/
void Transform::SetTranslation(const glm::vec3 translation)
{
	this->translation = translation;
	this->isDirty = 1;
}
/*!********************************************************************************************************************
	\brief
		setter for transform rotation
	\param rotation
		the rotation value for the transform
**********************************************************************************************************************/
void Transform::SetRotation(float rotation)
{
	this->rotation = rotation;
	this->isDirty = 1;
}
/*!********************************************************************************************************************
	\brief
		setter for the scale of the transform
	\param scale
		the scale to set the transforms scale to
**********************************************************************************************************************/
void Transform::SetScale(const glm::vec3 scale)
{
	this->scale = scale;
	this->isDirty = 1;
}
/*!********************************************************************************************************************
	\brief
		scale the x and y of the scale vector
	\param _scale
		a float to scale the x and y by
**********************************************************************************************************************/
void Transform::SetScale(float _scale)
{
	scale.x = _scale;
	scale.y = _scale;
}
/// <summary>
/// get relative translation of child
/// </summary>
/// <returns></returns>
glm::vec3 Transform::GetRelativeTran()
{
	return translation;
	//if (this->Parent())
	//{
	//	if (this->Parent()->HasParent())
	//	{
	//		return translation - this->Parent()->GetParent()->Has(Transform)->GetTranslation();
	//	}
	//}
	//return glm::vec3(0);
}

void Transform::SetParticleMatrix(glm::mat2 _scalerot, glm::vec2 _pos)
{
	//set matrix to scalerot and pos



}

void Transform::setDirty(bool bo)
{
	isDirty = bo;
}
