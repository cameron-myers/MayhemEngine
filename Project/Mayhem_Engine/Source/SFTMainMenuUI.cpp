#include "MEFunctionalTest.h"
#include "SFTMainMenuUI.h"
#include "MEAssert.h"
#include "MESpace.h"
#include "MEFunctionalTest.h"
#include "MESpaceManager.h"
#include "tinyxml2.h"
#include "Log.h"
#include "SFT_Base.h"

void SFTMainMenuUI::Init()
{
	MEFunctionalTest::Init();

	MESpaceManager::SetSpaceInclusive(m_Scene);
}

std::string SFTMainMenuUI::Read()
{
	return "bleh";
}

//write ui testing script here
void SFTMainMenuUI::Update(float dt)
{
	FT_WAIT_FOR(MESpaceManager::GetSpace(m_Scene) == NULL)
	ME_CORE_ERROR("THIS IS A TEST")
	
}

void SFTMainMenuUI::Shutdown()
{
	MESpaceManager::Remove(m_Scene);
	MEFunctionalTest::Shutdown();

}
