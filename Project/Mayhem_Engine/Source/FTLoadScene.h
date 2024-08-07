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
#include <variant>

class MEFunctionalTest;

class FTLoadScene : public MEFunctionalTest
{

public:
	static FTLoadScene* CreateTest(const char* _suite)
	{
		return new FTLoadScene(_suite);
	};
	FTLoadScene() = default;
	FTLoadScene(const char* suite):MEFunctionalTest(suite){};
	FTLoadScene(const FTLoadScene& rhs):MEFunctionalTest(rhs)
	{
		m_Scene = rhs.m_Scene;
		m_Object = rhs.m_Object;
	};

	virtual void Init();
	virtual std::string Read();
	virtual void Update(float dt);
	virtual void Shutdown();

	std::string m_Scene;
	std::string m_Object;
};
