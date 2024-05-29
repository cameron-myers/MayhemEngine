/*********************************************************************
 * \file         MEAssert.h
 * \author       Cameron Myers
 * \brief		 
 *
 *********************************************************************/

#pragma once
#include <list>
#include <Log.h>

class MESpace;
class GameObject;

class MEAssert
{
public:

	static bool is_true(const bool condition, const char* message)
	{
		if(condition == false)
		{
			ME_ERROR("MEAssert: " + std::string(message))
		}
		return true;
	};
	static bool is_false(const bool condition, const char* message)
	{
		if (condition == true)
		{
			ME_ERROR("MEAssert: " + std::string(message))
		}
		return true;
	}

	/**
	 * \brief Audits a Space for its GameObjects using an inputted list of names
	 * \param subject MESpace to audit
	 * \param expectedObjects list of objects to look for
	 * \return
	 */
	static bool space_audit(const MESpace* subject, const std::list<const char*>& expectedObjects)
	{
		_CRT_UNUSED(expectedObjects);
		_CRT_UNUSED(subject);

		return true;
	}

	/**
	 * \brief Audits a Space for its GameObjects using the MESpace JSON
	 * \param subject MESpace to audit
	 * \param spaceName JSON to load for auditing
	 * \return
	 */
	static bool space_audit(const MESpace* subject, const char* spaceName)
	{
		_CRT_UNUSED(spaceName);
		_CRT_UNUSED(subject);
		return true;
	}

	/**
	 * \brief Audits a GameObject for its children using an inputted list of names
	 * \param subject GameObject to audit
	 * \param expectedChildren list of children to look for
	 * \return 
	 */
	static bool object_audit(const GameObject* subject, const std::list<const char*>& expectedChildren)
	{
		_CRT_UNUSED(subject);
		_CRT_UNUSED(expectedChildren);
		return true;
	}

	/**
	 * \brief Audits a GameObject for its children directly from the JSON
	 * \param subject GameObject to audit
	 * \param objectName Name of the object for file read
	 * \return 
	 */
	static bool object_audit(const GameObject* subject, const char* objectName)
	{
		_CRT_UNUSED(subject);
		_CRT_UNUSED(objectName);
		return true;
	}

};
