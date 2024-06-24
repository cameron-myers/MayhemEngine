#include "MEFunctionalTest.h"
#include "FTLoadScene.h"

#include "MEAssert.h"
#include "MESpace.h"
#include "MESpaceManager.h"
#include "tinyxml2.h"


/**
 * \brief 
 */
void FTLoadScene::Init()
{
	MEFunctionalTest::Init();
	//m_json_buffer = MEFunctionalTest::Read().c_str();
	//();
	MESpaceManager::SetSpaceInclusive(m_Scene);
}

std::string FTLoadScene::Read()
{
	return "bleh";
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
	else if (MEAssert::space_audit(space,m_Scene.c_str()) == false)
	{
		m_Output += "Assert Error: AUDIT FAILED! Scene may not have loaded all objects correctly";
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
	MESpaceManager::Remove(m_Scene);
	MEFunctionalTest::Shutdown();

	WriteResults();
	

	/*FILE* file = nullptr;
	fopen_s(&file, "../Tests/test_report.xml", "w+");
	tinyxml2::XMLPrinter printer(file);
	doc.Print(&printer);
	fclose(file);*/
}
