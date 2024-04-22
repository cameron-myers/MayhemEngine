/*********************************************************************************************************************/
/*!
  \file              GameObject.cpp
  \author            Nicholas Leben
  \par               Email: nick.leben\@digipen.edu
  \par               Course: GAM200
  \par               Copyright © 2022 DigiPen, All rights reserved.
  \par				 credit to: https://www.youtube.com/watch?v=dZr-53LAlOw (thecherno on youtube)

  \brief
	this implements the game object class
 */
 /*********************************************************************************************************************/


/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/

#include "ChildObject.h"
#include "GameObject.h"
#include "Behavior.h"

/**
 * Default constructor for Child Objects.
 * 
 */
ChildObject::ChildObject() :name("\0")
{
	active = true;
	translationOffset = { 0,0,0 };
	Parent = nullptr;
	for (unsigned i = 0; i < cCount; ++i)
	{
		components[i] = nullptr;
	}
}
/**
 * Default destructor for Child Objects.
 * 
 */
ChildObject::~ChildObject()
{
	for (Component* cmp : components)
	{
		if (cmp)
		{
			delete cmp;
			cmp = NULL;
		}
	}
	name.clear();
}
/**
 * Copy constructor for child objects.
 * 
 * \param cpy
 *		the Child Object to be copied
 */
ChildObject::ChildObject(const ChildObject* cpy)
{
	translationOffset = cpy->translationOffset;
	active = cpy->active;
	Parent = cpy->Parent;

	for (unsigned i = 0; i < cCount; ++i)
	{
		components[i] = nullptr;
	}

	for (int i = 0; i < cCount; ++i)
	{
		if (cpy->components[i])
		{
			components[i] = cpy->components[i]->Clone();
			components[i]->Parent(this->Parent);
		}
	}
	setName(cpy->name);
}
/**
 * Clone function for Child Object.
 * 
 * \return ChildObject*
 *		the cloned game object
 */
ChildObject* ChildObject::Clone()
{
	ChildObject* clone = new ChildObject;

	clone->active = this->active;
	clone->setName(this->name);
	clone->translationOffset = this->translationOffset;
	clone->Parent = this->Parent;
	for (ComponentPtr component : components)
	{
		if (component)
		{
			clone->Add(component->Clone());
		}
	}
	return clone;
}
/**
 * Add a component to a Child Object.
 * 
 * \param component
 *		the component to be added to the object
 */
void ChildObject::Add(ComponentPtr component)
{
	if (component)
	{
		components[component->Type()] = component;
	}
}
/**
 * Update function for Child Objects.
 * 
 * \param dt
 *		if you don't know what dt is you should probably find a job at target
 */
void ChildObject::Update(float dt)
{
	for (ComponentPtr comp : components)
	{
		comp->Update(dt);
	}

	Transform* transform = this->Has(Transform);
	glm::vec3 newTranslation;
	Transform* other = Parent->Has(Transform);
	glm::vec3 oldTranslatrion = other->GetTranslation();
	newTranslation = { oldTranslatrion.x + translationOffset.x, oldTranslatrion.y + translationOffset.y, 0 };
	transform->SetTranslation(newTranslation);
}
/**
 * Draw the components of the Child Object.
 * 
 */
void ChildObject::Draw()
{
	for (ComponentPtr comp : components)
	{
		if (GetActive())
		{
			comp->Draw();
		}
	}
}
/**
 * Get a component from the Child Object.
 * 
 * \param type
 *		the type of component to get from the Child
 * 
 * \return
 *		the Component that has been gotten
 *  
 */
ComponentPtr ChildObject::Get(TypeEnum type)
{
	if (components[type])
	{
		return components[type];
	}
	return nullptr;
}
/**
 * Free all of the components.
 * 
 */
void ChildObject::Free()
{
	for (ComponentPtr comp : components)
	{
		comp->Free();
	}
}
/**
 * Gets the status of the activity for the Child Object.
 * 
 * \return 
 *		if the Child Object is active (T/F) which determines if it will be drawn
 */
bool ChildObject::GetActive()
{
	return active;
}
/**
 * Sets the status of activity for the Child Object.
 * 
 * \param status
 *		a boolean to set the status of the Object to
 */
void ChildObject::SetActive(bool status)
{
	active = status;
}

void ChildObject::Read(const char* path)
{

}

void ChildObject::Write()
{

}
/**
 * Gets the name of the Child Object.
 * 
 * \return 
 *		returns an std::string of the name of the Child Object
 */
std::string ChildObject::GetName()
{
	return name;
}
/**
 * Check if the Child Object has a specified name.
 * 
 * \param input
 *		the name to check for
 * 
 * \return
 *		boolean for if the Child Object is named the Input
 */
bool ChildObject::IsNamed(std::string input)
{
	if (name == input)
	{
		return true;
	}
	else
	{
		return false;
	}
}
/**
 * Sets the name of the Child Object.
 * 
 * \param 
 *		std::string to set the Child Name to
 */
void ChildObject::setName(std::string input)
{
	name = input;
}
/**
 * Gets the parent Game Object of the Child.
 * 
 * \return 
 *		The parenting GameObject*
 */
GameObject* ChildObject::GetParent()
{
	return Parent;
}
/**
 * Set the Parent GameObject*.
 * 
 * \param parent
 *		GameObject* to the parent object
 */
void ChildObject::SetParent(GameObject* parent)
{
	Parent = parent;
}
