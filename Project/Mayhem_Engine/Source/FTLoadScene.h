/*********************************************************************
 * \file         MEFunctionalTesting.h
 * \author       Cameron Myers
 * \brief		 
 *
 *********************************************************************/

#pragma once
#include <map>
#include <stack>
#include <string>

class MEFunctionalTest;

class FTLoadScene : public MEFunctionalTest
{

public:

	FTLoadScene(const char* name):MEFunctionalTest(name){};
	virtual void Init();
	virtual void Read();
	virtual void Update(float dt);
	virtual void Shutdown();

	std::string m_Scene;
	std::string m_Object;
};
