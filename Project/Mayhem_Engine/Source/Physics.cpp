/*****************************************************************//**
 * \file         Physics.cpp
 * \author       Nick Lebem, ehtna mclaughlin
 * \par          Email: nick.leben\@digipen.edu, ethan.mclaughlin\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/
#include "Log.h"
#include "Physics.h"
#include "Transform.h"
#include "rapidjson/document.h"
#include "Component.h"
#include "GameObject.h"
/*!********************************************************************************************************************
	\brief
		constructor for Physics
**********************************************************************************************************************/
Physics::Physics()
	:Component(cPhysics),
		oldTranslation{ 0, 0, 0 },
		acceleration{ 0, 0, 0 },
		velocity{ 0, 0 , 0 },
		rotationalVelocity(0.0f)
{
}
/// <summary>
/// copy constructor
/// </summary>
/// <param name="rhs"></param>
Physics::Physics(const Physics& rhs) :Component(cPhysics)
{
	velocity = rhs.velocity;
	acceleration = rhs.acceleration;
	oldTranslation =  rhs.oldTranslation;
	rotationalVelocity = rhs.rotationalVelocity;
}
/*!********************************************************************************************************************
	\brief
		Destructor for Physics
**********************************************************************************************************************/
Physics::~Physics()
{
}
/// <summary>
/// 
/// </summary>
/// <returns>new physics that is clone</returns>
PhysicsPtr Physics::Clone()const
{
	PhysicsPtr trans = new Physics;
	if (trans)
	{
		*trans = *this;
	}
	return trans;
}
/// <summary>
/// read a physics component
/// </summary>
/// <param name="bufchar"></param>
void Physics::Read(const char* bufchar)
{
	//vector to load with results
	glm::vec3 result = { 0,0,0 };
	//json document
	rapidjson::Document d;
	//parse the json
	d.Parse(bufchar);
	const rapidjson::Value& e = d["GameObject"];
	//go into the Physics section
	const rapidjson::Value& value = e["Physics"];
	//set a to thee Physics
	
	if (value.HasMember("velocity"))
	{
		const rapidjson::Value& velo = value["velocity"];

		for (rapidjson::SizeType i = 0; i < velo.Size(); i++)
		{
			result[i] = velo[i].GetFloat();
		}
		velocity = result;
	}

	/*
	const rapidjson::Value& a = value["acceleration"];
	//loop through and load values into the result vector
	for (rapidjson::SizeType i = 0; i < a.Size(); i++)
	{
		result[i] = a[i].GetFloat();
	}
	acceleration = result;
	*/

}

/*!********************************************************************************************************************
	\brief
		getter for oldtranslation
	\return
		the oldtranslation 
**********************************************************************************************************************/
glm::vec3 Physics::GetOldTranslation()
{
	return this->oldTranslation;
}
/*!********************************************************************************************************************
	\brief
		getter for velocity
	\return
		the veloccity
**********************************************************************************************************************/
glm::vec3 Physics::GetVelocity()
{
	return this->velocity;
}
/*!********************************************************************************************************************
	\brief
		getter for acceleration
	\return
		the accelleration 
**********************************************************************************************************************/
glm::vec3 Physics::GetAcceleration()
{
	return this->acceleration;
}

/**
 * \brief gets the rotational velocity
 * \return vec3 rotational velocity
 */
float Physics::GetRotationalVelocity()
{
	return rotationalVelocity;
}
/*!********************************************************************************************************************
	\brief
		setter for acceleration vector
	\param acceleration
		acceleration of the object
**********************************************************************************************************************/
void Physics::SetAcceleration(const glm::vec3 acceleration)
{
	this->acceleration = acceleration;
}
/*!********************************************************************************************************************
	\brief
		setter for velocity rotation
	\param velocity
		velocity of the object
**********************************************************************************************************************/
void Physics::SetVelocity(const glm::vec3 velocity)
{
	this->velocity = velocity;
}
/// <summary>
/// set rotational velocity
/// </summary>
/// <param name="_rotationalVelocity"></param>
void Physics::SetRotationalVelocity(float _rotationalVelocity)
{
	rotationalVelocity = _rotationalVelocity;
}
/// <summary>
/// save a physics component
/// </summary>
/// <param name="writer"></param>
void Physics::Save() const
{

}

/*!********************************************************************************************************************
	\brief
		Update function for physics
	\param Transform
	the transform class
**********************************************************************************************************************/
void Physics::Update(float dt)
{
	auto transform = Parent()->Has(Transform);

	if (Parent()->HasParent())
	{
		//glm::vec3 translation = transform->GetTranslation() + this->Parent()->GetParent()->Has(Transform)->GetTranslation();
		//transform->SetTranslation(translation);

		glm::vec3 translation = transform->GetRelativeTran();

		auto ParentPhys = Parent()->GetParent()->Has(Physics);

		if (ParentPhys)
		{
			//velocity = ParentPhys->velocity;
			translation += ParentPhys->velocity * dt;
			transform->SetRotation(transform->GetRotation() + (ParentPhys->GetRotationalVelocity() * dt));
		}

		oldTranslation = translation;
		//transform->SetTranslation(Parent()->GetParent()->Has(Transform)->GetTranslation() + transform->GetRelativeTran());
		//transform->SetTranslation(translation);
	}
	else
	{
		glm::vec3 translation = transform->GetTranslation();
		velocity += acceleration * dt;
		translation += velocity * dt;

		transform->SetRotation(transform->GetRotation() + (rotationalVelocity * dt));
		oldTranslation = translation;
		transform->SetTranslation(translation);
	}
}

