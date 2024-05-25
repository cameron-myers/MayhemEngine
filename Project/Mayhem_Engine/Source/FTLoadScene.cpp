#include "MEFunctionalTest.h"
#include "FTLoadScene.h"

#include "MESpace.h"
#include "MESpaceManager.h"
#include "tinyxml2.h"


/**
 * \brief 
 */
void FTLoadScene::Init()
{
	MEFunctionalTest::Init();
	m_json_buffer = MEFunctionalTest::Read().c_str();
	Read();
	MESpaceManager::SetSpaceInclusive(m_Scene);
}

std::string FTLoadScene::Read()
{

	std::string json;
	rapidjson::Document doc;
	std::string clearData(json);
	std::string path = "../Tests/" + m_Name + +".json";
	json = MESerializer::OpenFileRead(path.c_str());
	m_json_buffer = json.c_str();
	doc.Parse(m_json_buffer);
	//load each name, audioID, etc. into each game object
	const rapidjson::Value& value = doc["TestCase"];

	m_Scene = value["scene"].GetString();
	m_Object = value["object"].GetString();
	return m_json_buffer;
}

/**
 * \brief 
 * \param dt 
 */
void FTLoadScene::Update(float dt)
{
	MESpace* space = MESpaceManager::GetSpace(m_Scene);
	//error
	if(space == nullptr)
	{
		m_Output += "Assert Error: No scene was loaded!";
		m_Status = Failed;

	}
	else if(*(space->GetName()) != m_Scene)
	{
		m_Output += "Assert Error: Incorrect scene loaded! Loaded scene was: " + *(space->GetName()) + "should have loaded: " + m_Scene;
		m_Status = Failed;
	}
	//error
	else if(space->GetActive()->GetObjectByName(m_Object) == nullptr)
	{
		m_Output += "Assert Error: Scene may not have loaded correctly could not find object: " + m_Object;
		m_Status = Failed;

	}
	else
	{
		m_Status = Passed;
	}
}

/**
 * \brief 
 */
void FTLoadScene::Shutdown()
{
	MEFunctionalTest::Shutdown();
	tinyxml2::XMLDocument doc;
	doc.NewDeclaration(NULL);

	auto root = doc.NewElement("testsuites");
	doc.InsertFirstChild(root);

	auto suite = root->InsertNewChildElement("testsuite");
	suite->SetAttribute("name", "Scene Loading");
	root->InsertEndChild(suite);

	auto _case = suite->InsertNewChildElement("testcase");
	_case->SetAttribute("name", m_Name.c_str());
	_case->SetAttribute("classname", "METestSuite.LoadScene");
	suite->InsertEndChild(_case);
	if(m_Status == Failed)
	{
		auto fail = _case->InsertNewChildElement("failure");
		fail->SetAttribute("message", m_Output.c_str());
		fail->SetAttribute("type", "AssertionError");
		_case->InsertEndChild(fail);

	}

	doc.SaveFile("../Tests/test_report.xml");

	/*FILE* file = nullptr;
	fopen_s(&file, "../Tests/test_report.xml", "w+");
	tinyxml2::XMLPrinter printer(file);
	doc.Print(&printer);
	fclose(file);*/
}
