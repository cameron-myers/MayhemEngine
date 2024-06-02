/*********************************************************************
 * \file         MEAssert.h
 * \author       Cameron Myers
 * \brief		 
 *
 *********************************************************************/

#pragma once
#include <list>
#include <Log.h>

#include "MESpace.h"

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
	 * \return true if all is good
	 */
	static bool space_audit(MESpace* subject, const std::vector<const char*>& expectedObjects)
	{
		auto subject_list =subject->GetActive()->GetObjectNames();
		bool flag = false;
		//insta fail for size difference

		if (subject_list.size() != expectedObjects.size())
		{
			ME_WARN("Space audit( " + *(subject->GetName()) + ")subject and expected object list sizes were not the same.")
			return false;
		}
		//objects in the subject list are usually in the same order so best case is o(n)
		//iterate through the list
		for(int i = 0; i < expectedObjects.size(); ++i)
		{
			flag = false;
			//check for same index
			if(subject_list[i] == expectedObjects[i])
			{
				continue;
			}
			//iterative search(THIS IS STUPID)
			else
			{
				for(int j = 0; j < expectedObjects.size(); ++j)
				{
					if(subject_list[j] == expectedObjects[i])
					{
						flag = true;
						break;
					}
				}
				if (flag == false)
				{
					return false;
				}
			}
		}


		return true;
	}

	/**
	 * \brief Audits a Space for its GameObjects using the MESpace JSON
	 * \param subject MESpace to audit
	 * \param spaceName JSON to load for auditing
	 * \return true if all is good
	 */
	static bool space_audit(MESpace* subject, const char* spaceName)
	{
		auto subject_list = subject->GetActive()->GetObjectNames();
		auto expectedObjects = MESpace::GetActiveListFromFile(spaceName);

		bool flag = false;
		//insta fail for size difference

		if (subject_list.size() < expectedObjects.size())
		{
			ME_WARN("Space audit( " + *(subject->GetName()) + ")subject and expected object list sizes were not the same.")
				return false;
		}
		//objects in the subject list are usually in the same order so best case is o(n)
		//iterate through the list
		for (int i = 0; i < expectedObjects.size(); ++i)
		{
			flag = false;
			//check for same index
			if (subject_list[i] == expectedObjects[i])
			{
				continue;
			}
			//iterative search(THIS IS STUPID)
			else
			{
				for (int j = 0; j < expectedObjects.size(); ++j)
				{
					if (subject_list[j] == expectedObjects[i])
					{
						flag = true;
						break;
					}
				}
				if (flag == false)
				{
					return false;
				}
			}
		}


		return true;
	}

	/**
	 * \brief Audits a GameObject for its children using an inputted list of names
	 * \param subject GameObject to audit
	 * \param expectedChildren list of children to look for
	 * \return true if all is good
	 */
	static bool object_audit(GameObject* subject, const std::vector<const char*>& expectedChildren)
	{
		auto children = subject->GetChildNames();
		bool flag = true;

		if (children.size() != expectedChildren.size())
		{
			ME_WARN("GameObject audit( " + subject->GetName() + ")subject and expected child list sizes were not the same.")
			return false;
		}

		for(int i = 0; i <= children.size(); ++i)
		{
			for(auto child : children)
			{
				if(child == expectedChildren[i])
				{
					flag = true;
					break;
				}
				else
				{
					flag = false;
				}
			}
			if (flag == false) return false;
		}

		return true;
	}

	/**
	 * \brief Audits a GameObject for its children directly from the JSON
	 * \param subject GameObject to audit
	 * \param objectName Name of the object for file read
	 * \return true if all is good
	 */
	static bool object_audit(const GameObject* subject, const char* objectName)
	{

		auto children = const_cast<GameObject*>(subject)->GetChildNames();

		auto expected_children = GameObject::GetChildrenFromFile(objectName);


		bool flag = true;

		if (children.size() != expected_children.size())
		{
			ME_WARN("GameObject audit( ", const_cast<GameObject*>(subject)->GetName()," subject and expected child list sizes were not the same.")
				return false;
		}

		for (int i = 0; i <= children.size(); ++i)
		{
			for (auto child : children)
			{
				if (child == expected_children[i])
				{
					flag = true;
					break;
				}
				else
				{
					flag = false;
				}
			}
			if (flag == false) return false;
		}

		return true;

	}

};
