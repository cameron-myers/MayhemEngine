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

	TEST_CLASS(MESpriteSourceManagerTest)
	{

		 TEST_CLASS_INITIALIZE(Init)
		{
			Log::Init();
		}

	private:

		

	public:
		//This gets called before each test is ran
		MESpriteSourceManagerTest()
		{
			
		}
		~MESpriteSourceManagerTest()
		{
			
		}
	};
}
