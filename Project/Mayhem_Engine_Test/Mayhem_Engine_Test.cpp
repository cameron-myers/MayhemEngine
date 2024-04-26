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

	TEST_CLASS(AnimationComponentTest)
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
		AnimationComponentTest()
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
		~AnimationComponentTest()
		{
			delete game_object;
		}

		TEST_METHOD(ReadTest)
		{

		}

		TEST_METHOD(AnimationAdvanceFrameTest)
		{
			SetupGameObject();

			AnimationPtr animation = game_object->Has(Animation);

			int curr_frame = animation->getFrameIndex();
			animation->AnimationAdvanceFrame();
			int post_frame = animation->getFrameIndex();

			std::wstring message = L"Animation Frame did not advance! frame was: " + std::to_wstring(post_frame) + L"should have been: " + std::to_wstring(curr_frame + 1);
			Assert::IsTrue(post_frame == curr_frame + 1, message.c_str());
		}

		TEST_METHOD(AnimationAdvanceFrameTestLoop)
		{
			SetupGameObject();
			//test for frame advance loop
			AnimationPtr animation = game_object->Has(Animation);
			animation->SetisLooping(true);

			//at end
			animation->SetFrameIndex((animation->getFrameCount() * 3) - 1);

			int curr_frame = animation->getFrameIndex();

			animation->AnimationAdvanceFrame();

			int post_frame = animation->getFrameIndex();

			std::wstring message = L"Animation Frame did not loop! frame was: " + std::to_wstring(post_frame) + L"should have been: " + std::to_wstring(curr_frame - animation->getFrameCount() + 1);
			//index is not local to anim so the beggining isnt really 0
			Assert::IsTrue(post_frame == (curr_frame - animation->getFrameCount() + 1), message.c_str());

		}
		TEST_METHOD(AnimationAdvanceFrameTestNoLoop)
		{
			SetupGameObject();

			//test for frame advance loop
			AnimationPtr animation = game_object->Has(Animation);
			animation->SetisLooping(false);

			animation->SetFrameIndex((animation->getFrameCount() * 3) - 1);
			int curr_frame = animation->getFrameIndex();
			animation->AnimationAdvanceFrame();
			int post_frame = animation->getFrameIndex();

			std::wstring message = L"Animation Frame changed when it shouldn't have! frame was: " + std::to_wstring(post_frame) + L"should have been: " + std::to_wstring(curr_frame);
			Assert::IsTrue(post_frame == curr_frame, message.c_str());
		}

		//This function advances frame but keeps in the row
		TEST_METHOD(AnimationAdvanceDifferentFrameTestNextFrame)
		{
			//test for frame advance
			//test for frame advance loop

			SetupGameObject();

			AnimationPtr animation = game_object->Has(Animation);
			animation->SetisDiffAnimations(true);

			animation->SetFrameIndex(animation->getFrameCount());
			int curr_frame = animation->getFrameIndex();
			animation->AnimationAdvanceFrame();
			int post_frame = animation->getFrameIndex();

			std::wstring message = L"Animation Frame did not advance! frame was: " + std::to_wstring(post_frame) + L"should have been: " + std::to_wstring(curr_frame + 1);
			Assert::IsTrue(post_frame == curr_frame + 1, message.c_str());
		}

		TEST_METHOD(AnimationAdvanceDifferentFrameTestLoop)
		{
			//test for frame advance loop

			SetupGameObject();

			AnimationPtr animation = game_object->Has(Animation);
			//frame count is the number of cols in the spritesheet
			animation->SetFrameCount(game_object->Has(Sprite)->GetSpriteSource()->GetNumCols());
			animation->SetisLooping(true);
			animation->SetisDiffAnimations(true);

			//at end
			animation->SetFrameIndex((animation->getFrameCount() * 3)-1);

			int curr_frame = animation->getFrameIndex();

			animation->AnimationAdvanceFrame();

			int post_frame = animation->getFrameIndex();

			std::wstring message = L"Animation Frame did not loop! frame was: " + std::to_wstring(post_frame) + L"should have been: " + std::to_wstring(curr_frame - animation->getFrameCount() + 1);
			//index is not local to anim so the beggining isnt really 0
			Assert::IsTrue(post_frame == (curr_frame - animation->getFrameCount() + 1), message.c_str());
		}

		TEST_METHOD(AnimationAdvanceDifferentFrameTestNoLoop)
		{

			SetupGameObject();

			//test for frame advance no loop
			AnimationPtr animation = game_object->Has(Animation);
			//frame count is the number of cols in the spritesheet
			animation->SetFrameCount(game_object->Has(Sprite)->GetSpriteSource()->GetNumCols());
			animation->SetisLooping(false);
			animation->SetisDiffAnimations(true);

			animation->SetFrameIndex((animation->getFrameCount() * 3) - 1);
			int curr_frame = animation->getFrameIndex();
			animation->AnimationAdvanceFrame();
			int post_frame = animation->getFrameIndex();

			std::wstring message = L"Animation Frame changed when it shouldn't have! frame was: " + std::to_wstring(post_frame) + L"should have been: " + std::to_wstring(curr_frame);
			Assert::IsTrue(post_frame == curr_frame, message.c_str());
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
			//test parameter setting and that frame index is correct on sprite
			
		}
		TEST_METHOD(UpdateTestNextFrame)
		{
			//test that if frame rate is over advance frame
		}
		TEST_METHOD(UpdateTestWait)
		{
			//test that if frame rate is under dont advance
		}

		//gets the row of the sprite sheet that the sprite index is in
		TEST_METHOD(GetRowOfCurrentIndexTest)
		{
			SetupGameObject();
			AnimationPtr animation = game_object->Has(Animation);

			SpritePtr sprite = game_object->Has(Sprite);

			//row should be 3
			animation->SetFrameIndex(16);

			int result = animation->GetRowOfCurrentIndex();

			std::wstring message = L"Resulting row is incorrect, row was: " + std::to_wstring(result) + L" should have been: " + std::to_wstring(3);
			Assert::IsTrue(result == 3, message.c_str());

		}

		
		TEST_METHOD(SaveTest)
		{
			
		}
	};
}
