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

namespace Mayhem_Engine_Unit_Testing
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
		GameObject* m_object;

		void setup_single_object(bool objectCondition, std::string objName, std::vector<const char*> children = std::vector<const char*>())
		{
			m_space = new MESpace(static_cast<const char*>("Test"));
			m_object = new GameObject();
			m_object->SetName(objName);
			m_space->GetActive()->Add(m_object);

			for (auto child : children)
			{
				auto chi = new GameObject();
				chi->SetName(child);
				m_object->AddChild(chi);
			}
		}

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
			setup_single_object(true, "TEST_Object", { "GoldText", "CoinButtonIcon" });


			bool result = MEAssert::object_audit(m_object, { "GoldText", "CoinButtonIcon" });

			Assert::IsTrue(result, L"Game Object Audit failed when it should have passed!");
		}
		TEST_METHOD(GameObjectAuditUsingListBadSubjectTest)
		{
			setup_single_object(true, "TEST_Object", { "GoldText" });


			bool result = MEAssert::object_audit(m_object, {"GoldText", "CoinButtonIcon"});

			Assert::IsFalse(result, L"GameObject Audit passed when it should have failed!");
		}

		TEST_METHOD(GameObjectAuditUsingFileGoodSubjectTest)
		{
			setup_single_object(true, "TEST_Object", { "GoldText", "CoinButtonIcon" });


			bool result = MEAssert::object_audit(m_object, "TEST_Object");

			Assert::IsTrue(result, L"Game Object Audit failed when it should have passed!");
		}

		TEST_METHOD(GameObjectAuditUsingFileBadSubjectTest)
		{
			setup_single_object(true, "TEST_Object", {"GoldText"});


			bool result = MEAssert::object_audit(m_object, "TEST_Object");

			Assert::IsFalse(result, L"GameObject Audit passed when it should have failed!");
		}


	};
}
