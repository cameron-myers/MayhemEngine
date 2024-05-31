#include "pch.h"

#include <boost/uuid/uuid_io.hpp>

#include "Animation.h"
#include "Sprite.h"
#include "CppUnitTest.h"
#include "GameObject.h"
#include "Log.h"
#include "SpriteSource.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Misc_Testing
{

	TEST_CLASS(MEAssertTest)
	{

		TEST_CLASS_INITIALIZE(Init)
		{
			Log::Init();
		}

	private:


	public:
		//This gets called before each test is ran
		MEAssertTest()
		{

		}
		~MEAssertTest()
		{
		
		}



		
	};
}
