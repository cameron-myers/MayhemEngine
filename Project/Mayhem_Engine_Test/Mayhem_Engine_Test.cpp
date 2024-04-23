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
	public:
		
		TEST_METHOD(AnimationAdvanceFrameTest)
		{
			GameObjectPtr obj = new GameObject();

			AnimationPtr animation = new Animation();
			SpritePtr sprite = new Sprite();

			obj->Add(animation);
			obj->Add(sprite);

			animation->SetFrameCount(3);
			int curr_frame = animation->getFrameIndex();
			animation->AnimationAdvanceFrame();
			int post_frame = animation->getFrameIndex();

			Assert::IsTrue(post_frame == curr_frame + 1, L"Test");
			delete obj;
		}
	};
}
