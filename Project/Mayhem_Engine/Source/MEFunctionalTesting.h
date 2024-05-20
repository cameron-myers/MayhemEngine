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


class MEFunctionalTesting
{
	enum TestStatus
	{
		Invalid = -1,
		Passed = 0,
		Failed,
		Running,
		Aborted,
	};

public:
	static void Init();

	static void Update();

	static void Shutdown();
	
	static void PushTest();

	static void PopTest();

private:

	static std::stack<MEFunctionalTest> s_TestStack;
	//				test name
	static std::map<std::string, MEFunctionalTest> s_TestMap;

};
