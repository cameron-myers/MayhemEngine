/*****************************************************************//**
 * \file         Collider.cpp
 * \authors       Nate White,Cameron Myers, Ethan Mclaughlin,Nick Leben
 * \par          Email: Nate.white\@digipen.edu, cameron.myers\@digipen.edu, ethan.mclaughlin\@digipen.edu, nick.leben\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#include "stdafx.h"
#include "Component.h"
#include "Collider.h"
#include "Transform.h"
#include "GameObject.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/pointer.h"
#include "Log.h"
#include "Behavior.h"
#include "Engine.h"
#include "MESerializer.h"
#include "Sprite.h"
#include "MEWindow.h"

bool ColliderIsColliding(ColliderPtr collider1, ColliderPtr collider2);
/**
 * \brief default constructor for collider.
 * 
 */
Collider::Collider()
	:Component(cCollider),
	type(TypeNone),
	handler(nullptr),
	radius(0),
	topLeft{ 0,0,0 },
	botRight{ 0,0,0 }
{

}
/**
 * copy constructor for collider.
 * 
 * \param rhs
 *		collider to copy from
 */
Collider::Collider(const Collider& rhs):Component(cCollider)
{
	type = rhs.type;
	handler = rhs.handler;
	radius = rhs.radius;
	topLeft = rhs.topLeft;
	botRight = rhs.botRight;
}
/**
 * default destructor.
 * 
 */
Collider::~Collider()
{

}
/**
 * clone function for collider.
 * 
 * \return 
 *		pointer to the collider clone
 */
ComponentPtr Collider::Clone()const
{
	ColliderPtr col = new Collider;
	if (col)
	{
		*col = *this;
	}
	return col;
}
/**
 * read function for collider.
 * 
 * \param bufchar
 *		text to read from
 */
void Collider::Read(const char* bufchar)
{
	//vector to load with results
	glm::vec3 result = { 0,0,0 };

	rapidjson::Document d;
	//parse the json
	d.Parse(bufchar);
	const rapidjson::Value& e = d["GameObject"];
	//go into the transform section
	const rapidjson::Value& value = e["Collider"];
	//set a to the type
	const rapidjson::Value& a = value["Type"];
	//string of the type of collider
	std::string Astring = a.GetString();

	
	if (Parent()->Has(Transform))
	{
		Transform* trans = Parent()->Has(Transform);

		if (Astring == "Circle")
		{
			type = TypeArea;

			const rapidjson::Value& b = value["RadiusMod"];

			radiusMod = b.GetFloat();
			//	Get the Radius of current transform and then scale that with a modifier

			radius = trans->GetScale().x*  radiusMod;
		}
		else if (Astring == "Box")
		{
			//read vec3 for top left 
			const rapidjson::Value& b = value["WidthMod"];
			widthMod = b.GetFloat();
			const rapidjson::Value& h = value["HeightMod"];
			heightMod = h.GetFloat();
			type = TypeBox;
			//calculate top left and bottom right
			topLeft = glm::vec3(trans->GetTranslation().x - (trans->GetScale().x / 2.f), trans->GetTranslation().y + (trans->GetScale().y / 2.f), 1.f);
			botRight = glm::vec3(trans->GetTranslation().x + (trans->GetScale().x / 2.f), trans->GetTranslation().y - (trans->GetScale().y / 2.f), 1.f);

			//calculate width and height
			float width = topLeft.x - botRight.x;
			float hight = topLeft.y - botRight.y;

			//find the middle
			float wmid = (topLeft.x + botRight.x) / 2;
			float hmid = (topLeft.y + botRight.y) / 2;

			//modify the width and height
			width *= widthMod;
			hight *= heightMod;

			//modify the positions
			botRight.x = wmid - (width/2);
			botRight.y = hmid - (hight/2);

			topLeft.x = wmid + (width / 2);
			topLeft.y = hmid + (hight / 2);
		}
	}

	
}

void Collider::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	TransformPtr trans = Parent()->Has(Transform);
	//topLeft = glm::vec3(trans->GetTranslation().x - (trans->GetScale().x / 2.f), trans->GetTranslation().y + (trans->GetScale().y / 2.f), 1.f);
	//botRight = glm::vec3(trans->GetTranslation().x + (trans->GetScale().x / 2.f), trans->GetTranslation().y - (trans->GetScale().y / 2.f), 1.f);

	if(type == TypeBox )
	{
		/*//calculate width and height
		float width = trans->GetScale().x * (MEWindow::GetWidth() / 1920.f);
		float height = trans->GetScale().y * (MEWindow::GetHeight() / 1080.f);

		//find the middle
		float wmid = trans->GetTranslation().x;
		float hmid = trans->GetTranslation().y;

		//modify the width and height
		/*width *= widthMod * (MEWindow::GetWidth()/16.f);
		height *= heightMod * (MEWindow::GetHeight() / 9.f);#1#
		width *= widthMod * (MEWindow::GetWidth() / 1920.f);
		height *= heightMod * (MEWindow::GetHeight() / 1080.f);

		//modify the positions
		botRight.x = wmid + (width / 2);
		botRight.y = hmid - (height / 2);

		topLeft.x = wmid - (width / 2);
		topLeft.y = hmid + (height / 2);*/

		float width = trans->GetScale().x * widthMod * (MEWindow::GetWidth() / 1920.f);
		float height = trans->GetScale().y * heightMod * (MEWindow::GetHeight() / 1080.f);

		glm::vec4 box_vec(trans->GetTranslation().x * (MEWindow::GetWidth() / 1920.f), ((trans->GetTranslation().y) * (MEWindow::GetHeight() / 1080.f)), 0, 1);


		topLeft.x = (box_vec.x - width / 2);
		topLeft.y = (box_vec.y + height / 2);
		botRight.x = (box_vec.x + width / 2);
		botRight.y = (box_vec.y - height / 2);

	}
	else if (type == TypeArea)
	{
		radius = (trans->GetScale().x / 2.0f) * radiusMod * (MEWindow::GetWidth() / 1920.f);

	}
	
}
/**
 * check for collision.
 * 
 * \param other
 *		a collider to check
 */
void Collider::Check(ColliderPtr other)
{
	if (this && other)
	{
		if (ColliderIsColliding(this, other))
		{
			if (Parent()->Has(Behavior))
			{
				Parent()->Has(Behavior)->CollisionHandler(other->Parent());
			}
			if (other->Parent()->Has(Behavior))
			{
				other->Parent()->Has(Behavior)->CollisionHandler(Parent());
			}
		}
	}
}
/**
 * set the handler of the collider.
 * 
 * \param eventHandle
 *		the handle to set to
 */
void Collider::SetHandler(CollisionEventHandler eventHandle)
{
	handler = eventHandle;
}
/**
 * set the top left of a box collider.
 * 
 * \param TopLeft
 *		the top left point
 */
void Collider::SetTopLeft(glm::vec3 TopLeft)
{
	topLeft = TopLeft;
}
/**
 * set the bottom right of the box collider.
 * 
 * \param BotRight
 *		the bottom right point
 */
void Collider::SetBottomRight(glm::vec3 BotRight)
{
	botRight = BotRight;
}

void Collider::SetBoxSize(glm::vec2 box)
{
	widthMod = box.x;
	heightMod = box.y;
}

/**
 * check if the collider type is area.
 * 
 */
void Collider::IsArea()
{
	type = TypeArea;
}
/**
 * check if the collider type is box.
 * 
 */
void Collider::IsBox()
{
	type = TypeBox;
}
/**
 * get the radius of an area check.
 * 
 * \return 
 *		the radius of the area check
 */
float Collider::GetRadius()
{
	if (type == TypeArea)
	{
		return radius;
	}
	//IF YOU GOT HERE YOU DID SOMETHING WRONG
	return NULL;
}
float Collider::GetRadiusMod()
{
	if (type == TypeArea)
	{
		return radiusMod;
	}
	//IF YOU GOT HERE YOU DID SOMETHING WRONG
	return NULL;
}
/**
 * set the collider type to area check.
 * 
 * \param radius
 *		the radius of the area check
 */
void Collider::SetRadius(float radius_)
{
	if (type == TypeArea)
	{
		radius = radius_;
	}
}
void Collider::SetRadiusMod(float _radiusMod)
{
	if (type == TypeArea)
	{
		radiusMod = _radiusMod;
	}
}
/**
 * get the top left value of a box collider.
 * 
 * \return 
 *		the top left of the box collider
 */
glm::vec3 Collider::getTopLeft()
{
	return topLeft;
}
/**
 * get the bottom right of a box collider.
 * 
 * \return 
 *		the bottom right of the box collider
 */
glm::vec3 Collider::getBotRight()
{
	return botRight;
}
/**
 * get the type of collision.
 * 
 * \return 
 *		the enum for collision type
 */
ColliderType Collider::GetType()
{
	if (type)
	{
		return type;
	}
	return TypeNone;
}

void Collider::SetType(ColliderType _type)
{
	type = _type;
}

/**
 * check if there is an area collision.
 * 
 * \param other
 *		the game object to check against
 * \return 
 *		whether or not there was collision
 */
bool Collider::AreaCheck(GameObjectPtr other)
{
	TransformPtr pTran = this->Parent()->Has(Transform);
	TransformPtr otherTran = other->Has(Transform);
	glm::vec3 pVec = pTran->GetTranslation();
	glm::vec3 otherVec = otherTran->GetTranslation();

	float dist = glm::distance(pVec, otherVec);

	if (dist <= radius)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/**
 * check if there is a box collsion.
 * 
 * \param other
 *		the vec3 to check against
 * \return 
 *		if the vec3 is within the bounds of the box
 */
bool Collider::BoxCheck(glm::vec3 other)
{
	//ME_CORE_INFO(other.x);
	//ME_CORE_INFO(other.y);

	if (other.x >= topLeft.x && other.x <= botRight.x)
	{
		if (other.y <= topLeft.y && other.y >= botRight.y)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}
/**
 * check if colliders are colliding.
 * 
 * \param collider1
 *		collider to check against
 * \param collider2
 *		collider to check against
 * \return 
 *		if colliders are colliding
 */
bool ColliderIsColliding(ColliderPtr collider1, ColliderPtr collider2)
{
	if (collider1->GetType() == TypeArea)
	{
		GameObjectPtr obj = collider2->Parent();
		if (obj)
		{
			return (collider1->AreaCheck(obj));
		}
	}
	else if (collider1->GetType() == TypeBox)
	{
		GameObjectPtr obj = collider2->Parent();
		TransformPtr tran = dynamic_cast<TransformPtr>(obj->Get(cTransform));

		return (collider1->BoxCheck(tran->GetTranslation()));
	}
	
}
/**
 * the width of the box collider.
 * 
 * \return 
 *		float of the width of the box collider
 */
float Collider::GetBoxWidth()
{
	if(this->type == TypeBox)
	{
		return botRight.x - topLeft.x;
	}
	return 0;
}
/**
 * the hieght of the box collider.
 *
 * \return
 *		float of the height of the box collider
 */
float Collider::GetBoxHeight()
{
	if (this->type == TypeBox)
	{
		return topLeft.y - botRight.y;

	}
	return 0;
}

float Collider::GetWidthMod()
{
	return widthMod;
}

float Collider::GetHeightMod()
{
	return heightMod;
}

/// <summary>
/// save collider component data
/// </summary>
/// <param name="writer"></param>
void Collider::Save()
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

	


	if (type == TypeBox)
	{
		//SetValueByPointer(d, "/GameObject/Collider/WidthMod", "Box");

		EraseValueByPointer(d, "/GameObject/Collider/RadiusMod");

		SetValueByPointer(d, "/GameObject/Collider/WidthMod", widthMod);

		SetValueByPointer(d, "/GameObject/Collider/HeightMod", heightMod);

		SetValueByPointer(d, "/GameObject/Collider/Type", "Box");
	}
	else if (type == TypeArea)
	{
		//SetValueByPointer(d, "/GameObject/Collider/WidthMod", "Circle");

		EraseValueByPointer(d, "/GameObject/Collider/WidthMod");

		EraseValueByPointer(d, "/GameObject/Collider/HeightMod");

		SetValueByPointer(d, "/GameObject/Collider/RadiusMod", radiusMod);

		SetValueByPointer(d, "/GameObject/Collider/Type", "Circle");

	}
		

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







	/*if (type == 1)
	{
		writer->Key("Type");
		writer->String("Circle");
		writer->Key("RadiusMod");
		writer->Double(radius);
	}
	else if (type == 2)
	{
		writer->Key("Type");
		writer->String("Box");

		writer->Key("topLeft");
		writer->StartArray();
		writer->Uint(topLeft.x);
		writer->Uint(topLeft.y);
		writer->Uint(topLeft.z);
		writer->EndArray();

		writer->Key("botRight");
		writer->StartArray();
		writer->Uint(botRight.x);
		writer->Uint(botRight.y);
		writer->Uint(botRight.z);
		writer->EndArray();
		
	}*/
}