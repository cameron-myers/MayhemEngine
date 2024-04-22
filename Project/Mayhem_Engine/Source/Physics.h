/*****************************************************************//**
 * \file         Physics.h
 * \author       Nick Lebem, ehtna mclaughlin
 * \par          Email: nick.leben\@digipen.edu, ethan.mclaughlin\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#pragma once

/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/
#include "glm/glm.hpp"
#include "Component.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"

class Component;
typedef Component* ComponentPtr;
typedef class Physics* PhysicsPtr;

class Physics :public Component
{
public:
	Physics();
	~Physics();
	Physics(const Physics& rhs);
	virtual PhysicsPtr Clone()const override;
	void Read(const char* path);
	glm::vec3 GetOldTranslation();
	glm::vec3 GetVelocity();
	glm::vec3 GetAcceleration();
	float GetRotationalVelocity();

	void SetAcceleration(const glm::vec3 acceleration);
	void SetVelocity(const glm::vec3 velocity);
	void SetRotationalVelocity(float _rotationalVelocity);

	void Save() const override;

	void Update(float dt);
private:
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 oldTranslation;
	float rotationalVelocity;

};

