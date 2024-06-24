#include "MEFunctionalTestExecutor.h"

#include <vector>

#include "Engine.h"
#include "Log.h"
#include "METestSuite.h"
#include "MEWindow.h"


std::stack<MEFunctionalTest*> MEFunctionalTestExecutor::s_TestStack;
//				test name


void MEFunctionalTestExecutor::Init()
{

	tinyxml2::XMLDocument doc;
	doc.LoadFile("../Tests/test_report.xml");
	doc.Clear();
	doc.SaveFile("../Tests/test_report.xml");
	

	//read in Test Suite file from command arg
	const std::string suite = Engine::s_TestingArgs.at("TestingSuite");
	if (suite.empty())
	{
		ME_CORE_ERROR("No testing suite declared!!!")
		glfwSetWindowShouldClose(MEWindow::GetWindow(), true);
	}
		else
	{
		//parse json
		std::string json;
		rapidjson::Document doc;
		std::string clearData(json);
		std::string path = "../Tests/" + suite + ".json";
		json = MESerializer::OpenFileRead(path.c_str());
		doc.Parse(json.c_str());
		//load each name, audioID, etc. into each game object
		const rapidjson::Value& value = doc["TestCases"];

		for (auto iter = value.MemberBegin(); iter != value.MemberEnd(); ++iter)
		{
			//for each case, push test from registry
			if ((*iter).value.IsObject())
			{
				auto object = (*iter).value.GetObj();
				std::string name = (*iter).name.GetString();


				//fill memebers
				MEFunctionalTest* test = s_TestRegistry.at(name);
				test->m_Case = name;
				test->m_Description = object["description"].GetString();
				test->m_IntendedResult = object["intended_result"].GetString();

				if (name.find("Load_") == 0)
				{
					test->m_Class = "Load_Scene";

					dynamic_cast<FTLoadScene*>(test)->m_Scene = object["scene"].GetString();
					dynamic_cast<FTLoadScene*>(test)->m_Object = object["object"].GetString();

				}
				else if (name.find("Interact_") == 0)
				{
					//insert sub_class specific member read here
				}
				//push to stack
				s_TestStack.push(test);

			}

		}
	}
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
