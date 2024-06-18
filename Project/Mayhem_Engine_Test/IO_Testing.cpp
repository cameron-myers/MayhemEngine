#include "pch.h"

#include <boost/uuid/uuid_io.hpp>

#include "Animation.h"
#include "Sprite.h"
#include "CppUnitTest.h"
#include "GameObject.h"
#include "Log.h"
#include "SpriteSource.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Mayhem_Engine_Unit_Testing
{

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
