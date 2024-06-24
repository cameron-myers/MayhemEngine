/*********************************************************************
 * \file         MEFunctionalTesting.h
 * \author       Cameron Myers
 * \brief		 
 *
 *********************************************************************/

#pragma once
#include <unordered_map>
#include <memory>
#include <stack>
#include <string>
#include "MEFunctionalTest.h"
#include "FTLoadScene.h"

class MEFunctionalTest;
class FTLoadScene;

class MEFunctionalTestExecutor
{
	

public:
	static void Init();

	static void Update(float dt);

	static void Shutdown();
	
	static void PushTest();

	static void PopTest();

private:

	static std::stack<MEFunctionalTest*> s_TestStack;
	

	inline const static std::unordered_map<std::string, MEFunctionalTest*> s_TestRegistry
	{std::make_pair("Load_Sandbox", new FTLoadScene("Load_Sandbox")),std::make_pair("Load_MainMenu", new FTLoadScene("Load_MainMenu"))

	};


};
