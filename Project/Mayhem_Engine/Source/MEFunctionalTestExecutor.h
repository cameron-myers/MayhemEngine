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
#include "SFTMainMenuUI.h"


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
	

	inline const static std::unordered_map<std::string, MEFunctionalTest* > s_TestRegistry
	{std::make_pair("Load_Sandbox", FTLoadScene::CreateTest("Load_Sandbox")),std::make_pair("Load_MainMenu", FTLoadScene::CreateTest("Load_MainMenu")),
		std::make_pair("Interact_MainMenu", SFTMainMenuUI::CreateTest("Interact_MainMenu"))

	};


};
