#include "pch.h"

#include <boost/uuid/uuid_io.hpp>

#include "Animation.h"
#include "Sprite.h"
#include "CppUnitTest.h"
#include "Engine.h"
#include "GameObject.h"
#include "Log.h"
#include "MEAssert.h"
#include "SpriteSource.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Misc_Testing
{
	
	TEST_CLASS(MEAssertTest)
	{

		TEST_CLASS_INITIALIZE(Init)
		{
			if (Log::GetCoreLogger() == nullptr)
			{
				Log::Init();
			}
		}
		TEST_CLASS_CLEANUP(Cleanup)
		{
			Log::Free();
		}
	private:
		std::vector<const char*> m_object_list;
		MESpace* m_space = nullptr;
		GameObject m_object;

		void setup_space(bool spaceCondition, std::vector<const char*> objects = std::vector<const char*>())
		{
			m_space = new MESpace("Test");

			if (objects.size() > 0) m_object_list = objects;

			if(spaceCondition == true)
			{
				for (auto obj_name : m_object_list)
				{
					//empty objects with names
					auto obj = new GameObject();
					obj->SetName(obj_name);
					m_space->PutInActive(obj);
				}
			}
			//for incorrect space on purpose;
			else
			{
				for(size_t i = 0; i < m_object_list.size() - 1; ++i)
				{
					//empty objects with names
					auto obj = new GameObject();
					obj->SetName(m_object_list[i]);
					m_space->PutInActive(obj);
				}
			}
			
		}

		void setup_object(bool objectCondition)
		{
			
		}


	public:
		//This gets called before each test is ran
		MEAssertTest():m_object_list({ "Zeppelin1", "StoneTower", "Coin Button" })
		{
			Engine::s_UnitTesting = true;
		}

		~MEAssertTest() override
		{
			if(m_space)
			{
				m_space->Shutdown();
				delete m_space;
			}
			
		}
		


		TEST_METHOD(IsTrueTest)
		{
			bool result = false;
			result = MEAssert::is_true((1 + 1 == 2), "This message failed");

			Assert::IsTrue(result, L"Boolean evaluated incorrectly");
		}

		TEST_METHOD(IsFalseTest)
		{
			bool result = false;
			result = MEAssert::is_true((1 + 1 == 3), "This message failed");

			Assert::IsTrue(result, L"Boolean evaluated incorrectly");
		}

		TEST_METHOD(SpaceAuditUsingListGoodInputTest)
		{
			setup_space(true);

			bool result = MEAssert::space_audit(m_space, m_object_list);

			Assert::IsTrue(result, L"Space Audit failed when it should have passed!");

		}

		TEST_METHOD(SpaceAuditUsingListBadInputTest)
		{

			setup_space(false);

			bool result = MEAssert::space_audit(m_space, m_object_list);

			Assert::IsFalse(result, L"Space Audit passed when it should have failed!");

		}
		TEST_METHOD(SpaceAuditUsingFileGoodSubjectTest)
		{
			setup_space(true, { "MainMenuBackground",
											"GameLogo",
											"StartButton",
											"StartButtonText",
											"QuitButton2",
											"BlackSquare"
			});

			bool result = MEAssert::space_audit(m_space, "TEST_MenuUI");

			Assert::IsTrue(result, L"Space Audit failed when it should have passed!");
		}
		TEST_METHOD(SpaceAuditUsingFileBadSubjectTest)
		{
			setup_space(true, { "MainMenuBackground",
											"GameLogo",
											"StartButtonText",
											"QuitButton2",
			});

			bool result = MEAssert::space_audit(m_space, "TEST_MenuUI");

			Assert::IsFalse(result, L"Space Audit passed when it should have failed!");
		}

		TEST_METHOD(GameObjectAuditUsingListGoodSubjectTest)
		{
			
		}
		TEST_METHOD(GameObjectAuditUsingListBadSubjectTest)
		{

		}

		TEST_METHOD(GameObjectAuditUsingFileGoodSubjectTest)
		{
			
		}

		TEST_METHOD(GameObjectAuditUsingFileBadSubjectTest)
		{

		}


	};
}
