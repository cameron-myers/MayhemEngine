#include "MEFunctionalTestExecutor.h"

#include <vector>

#include "METestSuite.h"
#include "MEWindow.h"


std::stack<MEFunctionalTest*> MEFunctionalTestExecutor::s_TestStack;
//				test name
std::map<std::string, MEFunctionalTest*> MEFunctionalTestExecutor::s_TestMap;


void MEFunctionalTestExecutor::Init()
{
	s_TestMap = TEST_SUITE;
	s_TestStack.push(s_TestMap["Load_Level"]);
	s_TestStack.top()->Init();
}


void MEFunctionalTestExecutor::Update(float dt)
{
	s_TestStack.top()->Update(dt);
	if(s_TestStack.top()->m_Status != Running)
	{
		s_TestStack.top()->Shutdown();
		s_TestStack.pop();
		if(!s_TestStack.empty())
		{
			s_TestStack.top()->Init();
		}
		else
		{
			Shutdown();
		}
	}
}

void MEFunctionalTestExecutor::Shutdown()
{
	glfwSetWindowShouldClose(MEWindow::GetWindow(), GLFW_TRUE);
}

void MEFunctionalTestExecutor::PushTest()
{
}

void MEFunctionalTestExecutor::PopTest()
{
}
