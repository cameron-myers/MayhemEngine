/*****************************************************************//**
 * \file         GameObject.h
 * \author       Nick Leben
 * \par          Email: \@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/

 /*********************************************************************************************************************/
 /* Includes */
 /*********************************************************************************************************************/
#pragma once

#include <cstdio>
#include "Component.h"
#include "Transform.h"
#include "Sprite.h"
#include "Animation.h"
#include <string>

#define Has(type) GetComponent<type>(c##type)

class ChildObject
{
public:
	ChildObject();
	~ChildObject();
	ChildObject(const ChildObject* cpy);
	ChildObject* Clone();
	void Add(ComponentPtr component);
	void Update(float dt);
	void Draw();
	ComponentPtr Get(TypeEnum type);

	template<typename type>
	inline type* GetComponent(TypeEnum typeId)
	{

		return static_cast<type*>(Get(typeId));
	};

	void Free();
	bool GetActive();
	void SetActive(bool status);
	void Read(const char* path);
	void Write();
	std::string GetName();
	bool IsNamed(std::string input);
	void setName(std::string input);
	GameObject* GetParent();
	void SetParent(GameObject* parent);

private:
	GameObject* Parent;
	std::string name;
	bool active;
	glm::vec3 translationOffset;
	ComponentPtr components[cCount];
};

