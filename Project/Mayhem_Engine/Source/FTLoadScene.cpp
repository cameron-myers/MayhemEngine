#include "MEFunctionalTest.h"
#include "FTLoadScene.h"

#include "MESpace.h"
#include "MESpaceManager.h"


/**
 * \brief 
 */
void FTLoadScene::Init()
{
	Read();
	MESpaceManager::SetSpaceInclusive(m_Scene);
}

void FTLoadScene::Read()
{
	const rapidjson::Value& v = m_json_document["TestCase"];
	m_Scene = v["scene"].GetString();
	m_Object = v["object"].GetString();

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
	}
	else if(*(space->GetName()) != m_Scene)
	{
		m_Output += "Assert Error: Incorrect scene loaded! Loaded scene was: " + *(space->GetName()) + "should have loaded: " + m_Scene;
	}
	//error
	else if(space->GetActive()->GetObjectByName(m_Object) == nullptr)
	{
		m_Output += "Assert Error: Scene may not have loaded correctly could not find object: " + m_Object;
	}
}

/**
 * \brief 
 */
void FTLoadScene::Shutdown()
{
}
