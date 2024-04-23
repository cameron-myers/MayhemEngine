#include "pch.h"

#include "Animation.h"
#include "Sprite.h"
#include "CppUnitTest.h"
#include "GameObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MayhemEngineTest
{
	TEST_CLASS(AnimationComponent)
	{

	private:
		GameObjectPtr game_object = new GameObject();
		const wchar_t* to_wchar(const std::wstring in)
		{
			return in.c_str();
		}
	public:

		TEST_METHOD(ReadTest)
		{

		}

		TEST_METHOD(AnimationAdvanceFrameTest)
		{

			AnimationPtr animation = new Animation();
			SpritePtr sprite = new Sprite();

			game_object->Add(animation);
			game_object->Add(sprite);

			animation->SetFrameCount(3);
			int curr_frame = animation->getFrameIndex();
			animation->AnimationAdvanceFrame();
			int post_frame = animation->getFrameIndex();

			Assert::IsTrue(post_frame == curr_frame + 1, to_wchar(L"Animation Frame did not advance! frame was: "
																			+ std::to_wstring(post_frame) + L"should have been: " + std::to_wstring(curr_frame + 1)));
		}



		TEST_METHOD(SetRowForMultiAnimationTest)
		{
			
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
