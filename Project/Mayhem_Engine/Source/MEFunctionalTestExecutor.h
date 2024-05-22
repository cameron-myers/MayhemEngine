/*********************************************************************
 * \file         MEFunctionalTesting.h
 * \author       Cameron Myers
 * \brief		 
 *
 *********************************************************************/

#pragma once
#include <map>
#include <memory>
#include <stack>
#include <string>

class MEFunctionalTest;


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
	//				test name
	static std::map<std::string, MEFunctionalTest*> s_TestMap;

};
