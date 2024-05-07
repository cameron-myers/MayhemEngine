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

		void setup_game_object()
		{
			//Setup the m_sprite for the test
			m_sprite = m_game_object->Has(Sprite);
			m_texture = new SpriteSource();
			m_texture->SetNumRows(5);
			m_texture->SetNumCols(5);
			m_sprite->SetSpriteSource(m_texture);
			m_animation->SetFrameDuration(0.033f);
			//give it a random frame to start at
			m_sprite->SetFrame(3);
			m_game_object->Has(Animation)->SetFrameIndex(3);
		}

		GameObjectPtr m_game_object = nullptr;
		AnimationPtr m_animation = nullptr;
		SpritePtr m_sprite = nullptr;
		SpriteSource* m_texture = nullptr;

	public:
		//This gets called before each test is ran
		AnimationComponentTest()
		{
			//create some blank components
			m_sprite = new Sprite();
			m_animation = new Animation();

			//init a blank game object
			m_game_object = new GameObject();

			//add the components to the game object
			//NOTE!!!!
			//This relies on a function to have the proper implementation;
			//TODO Make this non-reliant
			m_game_object->Add(m_sprite);
			m_game_object->Add(m_animation);
		}
		~AnimationComponentTest()
		{
			delete m_game_object;
		}



		TEST_METHOD(AnimationAdvanceFrameTest)
		{
			setup_game_object();

			int curr_frame = m_animation->getFrameIndex();
			m_animation->AnimationAdvanceFrame();
			int post_frame = m_animation->getFrameIndex();

			std::wstring message = L"Animation Frame did not advance! frame was: " + std::to_wstring(post_frame) + L"should have been: " + std::to_wstring(curr_frame + 1);
			Assert::IsTrue(post_frame == curr_frame + 1, message.c_str());
		}

		TEST_METHOD(AnimationAdvanceFrameTestLoop)
		{
			setup_game_object();
			//test for frame advance loop
			m_animation->SetisLooping(true);

			//at end
			m_animation->SetFrameIndex((m_animation->getFrameCount() * 3) - 1);

			int curr_frame = m_animation->getFrameIndex();

			m_animation->AnimationAdvanceFrame();

			int post_frame = m_animation->getFrameIndex();

			std::wstring message = L"Animation Frame did not loop! frame was: " + std::to_wstring(post_frame) + L"should have been: " + std::to_wstring(curr_frame - m_animation->getFrameCount() + 1);
			//index is not local to anim so the beggining isnt really 0
			Assert::IsTrue(post_frame == (curr_frame - m_animation->getFrameCount() + 1), message.c_str());

		}
		TEST_METHOD(AnimationAdvanceFrameTestNoLoop)
		{
			setup_game_object();

			//test for frame advance loop
			m_animation->SetisLooping(false);

			m_animation->SetFrameIndex((m_animation->getFrameCount() * 3) - 1);
			int curr_frame = m_animation->getFrameIndex();
			m_animation->AnimationAdvanceFrame();
			int post_frame = m_animation->getFrameIndex();

			std::wstring message = L"Animation Frame changed when it shouldn't have! frame was: " + std::to_wstring(post_frame) + L"should have been: " + std::to_wstring(curr_frame);
			Assert::IsTrue(post_frame == curr_frame, message.c_str());
		}

		//This function advances frame but keeps in the row
		TEST_METHOD(AnimationAdvanceDifferentFrameTestNextFrame)
		{
			//test for frame advance
			//test for frame advance loop

			setup_game_object();

			m_animation->SetisDiffAnimations(true);

			m_animation->SetFrameIndex(m_animation->getFrameCount());
			int curr_frame = m_animation->getFrameIndex();
			m_animation->AnimationAdvanceFrame();
			int post_frame = m_animation->getFrameIndex();

			std::wstring message = L"Animation Frame did not advance! frame was: " + std::to_wstring(post_frame) + L"should have been: " + std::to_wstring(curr_frame + 1);
			Assert::IsTrue(post_frame == curr_frame + 1, message.c_str());
		}

		TEST_METHOD(AnimationAdvanceDifferentFrameTestLoop)
		{
			//test for frame advance loop

			setup_game_object();

			//frame count is the number of cols in the spritesheet
			m_animation->SetFrameCount(m_texture->GetNumCols());
			m_animation->SetisLooping(true);
			m_animation->SetisDiffAnimations(true);

			//at end
			m_animation->SetFrameIndex((m_animation->getFrameCount() * 3)-1);

			int curr_frame = m_animation->getFrameIndex();

			m_animation->AnimationAdvanceFrame();

			int post_frame = m_animation->getFrameIndex();

			std::wstring message = L"Animation Frame did not loop! frame was: " + std::to_wstring(post_frame) + L"should have been: " + std::to_wstring(curr_frame - m_animation->getFrameCount() + 1);
			//index is not local to anim so the begining isnt really 0
			Assert::IsTrue(post_frame == (curr_frame - m_animation->getFrameCount() + 1), message.c_str());
		}

		TEST_METHOD(AnimationAdvanceDifferentFrameTestNoLoop)
		{

			setup_game_object();

			//test for frame advance no loop
			//frame count is the number of cols in the spritesheet
			m_animation->SetFrameCount(m_texture->GetNumCols());
			m_animation->SetisLooping(false);
			m_animation->SetisDiffAnimations(true);

			m_animation->SetFrameIndex((m_animation->getFrameCount() * 3) - 1);
			int curr_frame = m_animation->getFrameIndex();
			m_animation->AnimationAdvanceFrame();
			int post_frame = m_animation->getFrameIndex();

			std::wstring message = L"Animation Frame changed when it shouldn't have! frame was: " + std::to_wstring(post_frame) + L"should have been: " + std::to_wstring(curr_frame);
			Assert::IsTrue(post_frame == curr_frame, message.c_str());
		}

		//Test for normal use
		TEST_METHOD(SetRowForMultiAnimationTestNormal)
		{
			setup_game_object();

			int curr_row = 0;
			int new_row = 2;
			//run tested function
			m_animation->SetRowForMultiAnimation(new_row);

			int post_frame = m_animation->getFrameIndex();
			//(row*col) - (row - newrow) * col = correct frame
			int correct_frame = m_texture->GetFrameCount() - (m_texture->GetNumRows() - new_row) * m_texture->GetNumCols();

			std::wstring message = L"Frame Index is not correct for intended row, frame was: " + std::to_wstring(post_frame) + L" should have been: " + std::to_wstring(correct_frame);
			Assert::IsTrue(correct_frame == post_frame, message.c_str());

		}

		//test for invalid row handling 
		//intended output is to have nothing change
		TEST_METHOD(SetRowForMultiAnimationTestOutOfBoundsPos)
		{
			setup_game_object();

			int correct_frame = m_animation->getFrameIndex();


			int new_row = m_texture->GetNumRows() + 1;
			m_animation->SetRowForMultiAnimation(new_row);

			int post_frame = m_animation->getFrameIndex();

			//(row*col) - (row - newrow) * col = correct frame
			std::wstring message = L"Frame Index is not correct for intended row, frame was: " + std::to_wstring(post_frame) + L" should have been: " + std::to_wstring(correct_frame);
			Assert::IsTrue(correct_frame == post_frame, message.c_str());

		}

		//test for invalid row handling
		//intended output is to have nothing change
		TEST_METHOD(SetRowForMultiAnimationTestOutOfBoundsNeg)
		{
			setup_game_object();


			int correct_frame = m_animation->getFrameIndex();
			int curr_row = 0;
			int new_row = -1;
			m_animation->SetRowForMultiAnimation(new_row);

			int post_frame = m_animation->getFrameIndex();
			//(row*col) - (row - newrow) * col = correct frame
			std::wstring message = L"Frame Index is not correct for intended row, frame was: " + std::to_wstring(post_frame) + L" should have been: " + std::to_wstring(correct_frame);
			Assert::IsTrue(correct_frame == post_frame, message.c_str());

		}

		TEST_METHOD(AnimationPlayTest)
		{
			setup_game_object();


			//test parameter setting and that frame index is correct on m_sprite
			m_animation->AnimationPlay(5, 0.033f, true, 3);

			std::wstring message = L"Frame Index is not correct, frame was: " + std::to_wstring(m_animation->getFrameIndex()) + L" should have been: " + std::to_wstring(3);
			Assert::IsTrue(3 == m_animation->getFrameIndex(), message.c_str());

		}
		TEST_METHOD(UpdateTestNextFrame)
		{
			//test that if frame rate is over advance frame
			setup_game_object();
			m_animation->AnimationPlay(5, 0.033f, true, 0);
			int pre_frame = m_animation->getFrameIndex();
			m_animation->Update(0.05f);
			int post_frame = m_animation->getFrameIndex();

			std::wstring message = L"Frame Index is not correct, frame was: " + std::to_wstring(post_frame) + L" should have been: " + std::to_wstring(pre_frame + 1);
			Assert::IsTrue(post_frame == pre_frame + 1, message.c_str());
			

		}
		TEST_METHOD(UpdateTestWait)
		{
			//test that if frame rate is under dont advance
			//test that if frame rate is over advance frame
			setup_game_object();
			m_animation->AnimationPlay(5, 0.033f, true, 0);
			int pre_frame = m_animation->getFrameIndex();
			m_animation->Update(0.01f);
			int post_frame = m_animation->getFrameIndex();

			std::wstring message = L"Frame Index is not correct, frame was: " + std::to_wstring(post_frame) + L" should have been: " + std::to_wstring(pre_frame);
			Assert::IsTrue(post_frame == pre_frame, message.c_str());


		}

		//gets the row of the m_sprite sheet that the m_sprite index is in
		TEST_METHOD(GetRowOfCurrentIndexTest)
		{
			setup_game_object();
			AnimationPtr animation = m_game_object->Has(Animation);

			SpritePtr sprite = m_game_object->Has(Sprite);

			//row should be 3
			animation->SetFrameIndex(16);

			int result = animation->GetRowOfCurrentIndex();

			std::wstring message = L"Resulting row is incorrect, row was: " + std::to_wstring(result) + L" should have been: " + std::to_wstring(3);
			Assert::IsTrue(result == 3, message.c_str());

		}

		

	};

	TEST_CLASS(SerializationTest)
	{

		GameObjectPtr game_object = nullptr;

	public:

		TEST_METHOD(AnimationSaveTest)
		{
		}
	};

	TEST_CLASS(DeserializationTest)
	{

	public:
		TEST_METHOD(AnimationReadTest)
		{

		}

	};
}
