/*****************************************************************//**
 * \file         Sprite.cpp
 * \author       Cameron Myers, Nate White
 * \par          Email: cameron.myers\@digipen.edu, nate.white\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/

#include "Sprite.h"

#include <iostream>

#include "MERendering.h"
#include "Component.h"
#include "MeshManager.h"
#include "SpriteSourceManager.h"
#include "SpriteSource.h"
#include "Transform.h"
#include "Mesh.h"
#include "GameObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/pointer.h"
#include "MESerializer.h"


/**
 * \brief Sprite constructor
 */
Sprite::Sprite() :Component(cSprite), frameIndex(0), alpha(1.f), mesh(0), spriteSource(0), rgb(1.0f), text(""), textMode((char*)"None")
{
}
/// <summary>
/// copy constructor
/// </summary>
/// <param name="rhs"></param>
Sprite::Sprite(const Sprite& rhs):Component(cSprite)
{
	frameIndex = rhs.frameIndex;
	alpha = rhs.alpha;
	mesh = rhs.mesh;
	spriteSource = rhs.spriteSource;
	rgb = rhs.rgb;
	text = rhs.text;
	textMode = rhs.textMode;
}

/**
 * \brief sprite deconstructor
 */
Sprite::~Sprite()
{

	frameIndex = NULL;
	alpha = NULL;
	mesh = nullptr;
	spriteSource = nullptr;
	text.clear();
	textMode = nullptr;

}

/**
 * \brief clones the sprite
 * \return pointer to the new sprite
 */
Component* Sprite::Clone() const
{
	Sprite* sprite = new Sprite;
	if (sprite)
	{
		*sprite = *this;

		if (this->mesh->IsNamed("Bar_Mesh"))
		{
			sprite->mesh = new Mesh("Bar_Mesh");
		}
	}
	return sprite;
}
/**
 * \brief handles the reading of the sprite
 * \param bufchar the file buffer containing the json file
 */
void Sprite::Read(const char* bufchar)
{
	using namespace rapidjson;
	glm::vec3 result = { 0,0,0 };

	Document d;
	d.Parse(bufchar);

	const rapidjson::Value& ghead = d["GameObject"];

	const Value& head = ghead["Sprite"];
	const Value& FrameIndex = head["frameIndex"];
	const Value& Alpha = head["alpha"];
	if (head.HasMember("RGB"))
	{
		const Value& RGB = head["RGB"];
		glm::vec3 _rgb = { 0,0,0 };

		for (rapidjson::SizeType i = 0; i < RGB.Size(); i++)
		{
			_rgb[i] = RGB[i].GetFloat();
		}
		rgb = _rgb;
	}
	const Value& Mesh = head["Mesh"];
	const Value& Source = head["SpriteSource"];


	if (head.HasMember("Text"))
	{
		const Value& Text = head["Text"];
		
		text = Text.GetString();
	}
	if (head.HasMember("TextMode"))
	{
		const Value& Mode = head["TextMode"];
		if (textMode == "None")
		{
			auto length = Mode.GetStringLength() + 1;
			textMode = new char[Mode.GetStringLength() + 1];
			strcpy_s(textMode, length, Mode.GetString());
		}
		else
		{
			delete textMode;
			auto length = Mode.GetStringLength() + 1;
			textMode = new char[Mode.GetStringLength() + 1];
			strcpy_s(textMode, length, Mode.GetString());
		}

	}
	frameIndex = FrameIndex.GetInt();
	alpha = Alpha.GetFloat();
	std::string mName = Mesh.GetString();
	mesh = MeshManager::Build(mName);
	std::string ssName = Source.GetString();
	spriteSource = SpriteSourceManager::Build(ssName);


}
/// <summary>
/// write sprite data to file
/// </summary>
/// <param name="writer"></param>
void Sprite::Save()
{
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
	Document d;
	d.Parse(bufchar);

	/*Value* frame_index = GetValueByPointer(d, "/GameObject/Sprite/frameIndex");
	frame_index->SetInt(sprite->GetFrame());*/

	SetValueByPointer(d, "/GameObject/Sprite/frameIndex", 0);


	SetValueByPointer(d, "/GameObject/Sprite/alpha", GetAlpha());

	SetValueByPointer(d, "/GameObject/Sprite/RGB/0", GetRGB().x);
	SetValueByPointer(d, "/GameObject/Sprite/RGB/1", GetRGB().y);
	SetValueByPointer(d, "/GameObject/Sprite/RGB/2", GetRGB().z);

	std::string temp = GetMesh()->GetName().c_str();

	SetValueByPointer(d, "/GameObject/Sprite/Mesh", temp.c_str());

	temp = GetSpriteSource()->GetName().c_str();

	SetValueByPointer(d, "/GameObject/Sprite/SpriteSource", temp.c_str());

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
		
		(itt, "../Assets/GameObjects/Children/%s.json", (Parent()->GetName()).c_str());
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
	/*writer->Key("frameIndex");
	writer->Uint(frameIndex);
	writer->Key("alpha");
	writer->Double(alpha);
	writer->Key("Scale");
	writer->StartArray();
	writer->EndArray();
	writer->Key("Mesh");
	writer->String(mesh->GetName().c_str());
	writer->Key("SpriteSource");
	writer->String(spriteSource->GetName().c_str());*/
}

/**
 * \brief Draws the sprite
 */
void Sprite::Draw() const
{
	if (spriteSource == NULL) return;

	Transform* transform = Parent()->Has(Transform);
	glm::vec3 _scale = transform->GetScale();
	glm::vec3 OgTranslation;
	if (Parent()->HasParent())
	{
		OgTranslation = transform->GetRelativeTran();
	}
	else
	{
		OgTranslation = transform->GetTranslation();
	}

	if (textMode != "None")
	{
		const char* Text = text.c_str();
		int numNewLines = 0;
		
		if (!strcmp(textMode, "Center"))
		{//not implemented
			while (*Text != 0)
			{
				if (*Text == '\n')
				{
					numNewLines++;
					transform->SetTranslation(OgTranslation);
					transform->SetTranslation(transform->GetRelativeTran() + glm::vec3(0, -numNewLines * transform->GetScale().y *.75, 0));
				}
				else if (Parent()->HasParent())
				{
					transform->SetTranslation(transform->GetRelativeTran() + glm::vec3(transform->GetScale().x / 2, 0, 0));
				}
				else
				{
					transform->SetTranslation(transform->GetTranslation() + glm::vec3(transform->GetScale().x / 2, 0, 0));
				}
				MEDraw(*spriteSource, *mesh, transform, *Text - 32, glm::vec4(rgb.r, rgb.g, rgb.b, alpha));
				++Text;

			}
			transform->SetTranslation(OgTranslation);
		}
		else if (!strcmp(textMode, "Right"))
		{//not implemented
			
			while (*Text != 0)
			{

				if (*Text == '\n')
				{
					numNewLines++;
					transform->SetTranslation(OgTranslation);
					transform->SetTranslation(transform->GetRelativeTran() + glm::vec3(0, -numNewLines * transform->GetScale().y * .75, 0));
				}
				else if (Parent()->HasParent())
				{
					transform->SetTranslation(transform->GetRelativeTran() + glm::vec3(transform->GetScale().x / 2, 0, 0));
				}
				else
				{
					transform->SetTranslation(transform->GetTranslation() + glm::vec3(transform->GetScale().x / 2, 0, 0));
				}
				MEDraw(*spriteSource, *mesh, transform, *Text - 32, glm::vec4(rgb.r, rgb.g, rgb.b, alpha));
				++Text;
				
			}
			transform->SetTranslation(OgTranslation);
		}
		else
		{
			while (*Text != 0)
			{
				if (*Text == '\n')
				{
					numNewLines++;
					transform->SetTranslation(OgTranslation);
					transform->SetTranslation(transform->GetRelativeTran() + glm::vec3(0, -numNewLines * transform->GetScale().y * .75, 0));
				}
				else if (Parent()->HasParent())
				{
					transform->SetTranslation(transform->GetRelativeTran() + glm::vec3(transform->GetScale().x / 2, 0, 0));
				}
				else
				{
					transform->SetTranslation(transform->GetTranslation() + glm::vec3(transform->GetScale().x / 2, 0, 0));
				}
				MEDraw(*spriteSource, *mesh, transform, *Text - 32, glm::vec4(rgb.r, rgb.g, rgb.b, alpha));
				++Text;

			}
			transform->SetTranslation(OgTranslation);
		}

	}
	else
	{
		MEDraw(*spriteSource, *mesh, transform, frameIndex, glm::vec4(rgb.r, rgb.g, rgb.b, alpha));
	}

}

/**
 * \brief Sets the sprites alpha value
 * \param _alpha 
 */
void Sprite::SetAlpha(float _alpha)
{
	alpha = _alpha;
}

/**
 * \brief Sets the frame of the sprite sheet to render
 * \param _frameIndex 
 */
void Sprite::SetFrame(unsigned _frameIndex)
{
	frameIndex = _frameIndex;
}

/**
 * \brief Sets the mesh for the sprite to use
 * \param meshID 
 */
void Sprite::SetMesh(Mesh* meshID)
{
	mesh =  meshID;
}

/**
 * \brief Sets the sprite source for the sprite
 * \param _spriteSource 
 */
void Sprite::SetSpriteSource(SpriteSource* _spriteSource)
{
	spriteSource = _spriteSource;
}

void Sprite::SetRGB(glm::vec3 _rgb)
{
	rgb = _rgb;
}
/**
 * \brief Gets the alpha of the sprite
 * \return 
 */
float Sprite::GetAlpha()
{
	return alpha;
}

/**
 * \brief Gets the frame the sprite is currently on
 * \return 
 */
unsigned Sprite::GetFrame()
{
	return frameIndex;
}

/**
 * \brief Gets the Mesh that the sprite uses
 * \return 
 */
Mesh* Sprite::GetMesh()
{
	return mesh;
}
void Sprite::SetText(const std::string NewText)
{

		text = NewText;
		//StringLength = static_cast<int>(strnlen_s(NewText,9999)); //big normber
		//text = new char[StringLength];
		//strcpy_s(text, StringLength, NewText);

}

/**
 * \brief Sets the text mode
 * \param _mode 
 */
void Sprite::SetTextMode(const char* _mode)
{
	textMode = (char*)_mode;
}

/**
 * \brief Gets the sprite source that the sprite uses
 * \return 
 */
SpriteSource* Sprite::GetSpriteSource()
{
	return spriteSource;
}
/// <summary>
/// 
/// </summary>
/// <returns>vec3 of RGB values</returns>
glm::vec3 Sprite::GetRGB()
{
	return rgb;
}

/**
 * \brief gets the text
 * \return 
 */
std::string Sprite::GetText()
{
	return text;
}

/**
 * \brief get the text mode
 * \return 
 */
char* Sprite::GetTextMode()
{
	return textMode;
}
