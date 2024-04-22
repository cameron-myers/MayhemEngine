/*****************************************************************//**
 * \file         Collider.h
 * \authors       Nate White,Cameron Myers, Ethan Mclaughlin
 * \par          Email: Nate.white\@digipen.edu,cameron.myers\@digipen.edu, EthanMclaughlin\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "glm/glm.hpp"
#include "glm/gtx/matrix_transform_2d.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "GameObject.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Collider;
typedef Collider * ColliderPtr;
class GameObject;
typedef GameObject * GameObjectPtr;
class Component;
typedef Component* ComponentPtr;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

typedef enum ColliderType
{
	TypeNone,
	TypeArea,
	TypeBox

} ColliderType;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

typedef void(*CollisionEventHandler)(GameObjectPtr gameObject1, GameObjectPtr gameObject2);
/*
typedef struct Collider
{
	// Pointer to the collider's parent game object.
	GameObjectPtr parent;

	// The type of collider used by this component.
	// (Currently, Circle or Line).
	ColliderType type;

	// Pointer to a function that handles collisions between two objects.
	CollisionEventHandler	handler;

	unsigned int	memorySize;

} Collider;
*/
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

class Collider :public Component
{
public:
	Collider();
	Collider(const Collider& rhs);
	~Collider();
	virtual ComponentPtr Clone()const override;
	void Update(float dt) override;
	void Read(const char* bufchar);
	void SetTopLeft(glm::vec3 TopLeft);
	void SetBottomRight(glm::vec3 BotRight);
	void SetBoxSize(glm::vec2 box);
	void Check(ColliderPtr other);
	void SetHandler(CollisionEventHandler eventHandle);
	void IsArea();
	void IsBox();
	glm::vec3 getTopLeft();
	glm::vec3 getBotRight();
	float GetRadius();
	float GetRadiusMod();
	void SetRadius(float _radius);
	void SetRadiusMod(float _radiusMod);
	bool AreaCheck(GameObjectPtr other);
	bool BoxCheck(glm::vec3 other);

	float GetBoxWidth();
	float GetBoxHeight();
	float GetWidthMod();
	float GetHeightMod();
	void Save();

	ColliderType GetType();
	void SetType(ColliderType _type);
private:

	// The type of collider used by this component.
	// (Currently, Circle or Line).
	ColliderType type;

	// Pointer to a function that handles collisions between two objects.
	CollisionEventHandler	handler;

	float radius;

	glm::vec3 topLeft;
	glm::vec3 botRight;

	float widthMod;
	float heightMod;
	float radiusMod;
};


//------------------------------------------------------------------------------
