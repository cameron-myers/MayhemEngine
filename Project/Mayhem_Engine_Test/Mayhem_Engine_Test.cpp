#include "pch.h"

#include <boost/uuid/uuid_io.hpp>

#include "Animation.h"
#include "Sprite.h"
#include "CppUnitTest.h"
#include "GameObject.h"
#include "Log.h"
#include "SpriteSource.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MayhemEngineTest
{

	TEST_CLASS(AnimationComponent)
	{
		TEST_CLASS_INITIALIZE(Init)
		{
			Log::Init();
		}
	private:

		void SetupGameObject()
		{
			//Setup the sprite for the test
			SpritePtr sprite = game_object->Has(Sprite);
			SpriteSource* tex = new SpriteSource();
			tex->SetNumRows(5);
			tex->SetNumCols(5);
			sprite->SetSpriteSource(tex);

			//give it a random frame to start at
			sprite->SetFrame(3);
			game_object->Has(Animation)->SetFrameIndex(3);
		}

		GameObjectPtr game_object = nullptr;

	public:
		//This gets called before each test is ran
		AnimationComponent()
		{
			//create some blank components
			SpritePtr sprite = new Sprite();
			AnimationPtr animation = new Animation();

			//init a blank game object
			game_object = new GameObject();

			//add the components to the game object
			//NOTE!!!!
			//This relies on a function to have the proper implementation;
			//TODO Make this non-reliant
			game_object->Add(sprite);
			game_object->Add(animation);
		}
		~AnimationComponent()
		{
			delete game_object;
		}

		TEST_METHOD(ReadTest)
		{

		}

		TEST_METHOD(AnimationAdvanceFrameTest)
		{

			AnimationPtr animation = game_object->Has(Animation);

			animation->SetFrameCount(3);
			int curr_frame = animation->getFrameIndex();
			animation->AnimationAdvanceFrame();
			int post_frame = animation->getFrameIndex();

			std::wstring message = L"Animation Frame did not advance! frame was: " + std::to_wstring(post_frame) + L"should have been: " + std::to_wstring(curr_frame + 1);
			Assert::IsTrue(post_frame == curr_frame + 1, message.c_str());
		}

		//Test for normal use
		TEST_METHOD(SetRowForMultiAnimationTestNormal)
		{
			SetupGameObject();
			AnimationPtr animation = game_object->Has(Animation);
			SpriteSource* tex = game_object->Has(Sprite)->GetSpriteSource();

			int curr_row = 0;
			int new_row = 2;
			//run tested function
			animation->SetRowForMultiAnimation(new_row);

			int post_frame = animation->getFrameIndex();
			//(row*col) - (row - newrow) * col = correct frame
			int correct_frame = tex->GetFrameCount() - (tex->GetNumRows() - new_row) * tex->GetNumCols();
			std::wstring message = L"Frame Index is not correct for intended row, frame was: " + std::to_wstring(post_frame) + L" should have been: " + std::to_wstring(correct_frame);
			Assert::IsTrue(correct_frame == post_frame, message.c_str());

		}

		//test for invalid row handling 
		//intended output is to have nothing change
		TEST_METHOD(SetRowForMultiAnimationTestOutOfBoundsPos)
		{
			SetupGameObject();
			AnimationPtr animation = game_object->Has(Animation);
			SpriteSource* tex = game_object->Has(Sprite)->GetSpriteSource();

			int correct_frame = animation->getFrameIndex();


			int new_row = tex->GetNumRows() + 1;
			animation->SetRowForMultiAnimation(new_row);

			int post_frame = animation->getFrameIndex();
			//(row*col) - (row - newrow) * col = correct frame
			std::wstring message = L"Frame Index is not correct for intended row, frame was: " + std::to_wstring(post_frame) + L" should have been: " + std::to_wstring(correct_frame);
			Assert::IsTrue(correct_frame == post_frame, message.c_str());

		}

		//test for invalid row handling
		//intended output is to have nothing change
		TEST_METHOD(SetRowForMultiAnimationTestOutOfBoundsNeg)
		{
			SetupGameObject();
			AnimationPtr animation = game_object->Has(Animation);
			SpriteSource* tex = game_object->Has(Sprite)->GetSpriteSource();

			int correct_frame = animation->getFrameIndex();
			int curr_row = 0;
			int new_row = -1;
			animation->SetRowForMultiAnimation(new_row);

			int post_frame = animation->getFrameIndex();
			//(row*col) - (row - newrow) * col = correct frame
			std::wstring message = L"Frame Index is not correct for intended row, frame was: " + std::to_wstring(post_frame) + L" should have been: " + std::to_wstring(correct_frame);
			Assert::IsTrue(correct_frame == post_frame, message.c_str());

		}
		TEST_METHOD(AnimationPlayTest)
		{
			
		}
		TEST_METHOD(UpdateTest)
		{
			
		}
		TEST_METHOD(GetRowOfCurrentIndexTest)
		{
			
		}

		TEST_METHOD(SaveTest)
		{
			
		}
	};
}
