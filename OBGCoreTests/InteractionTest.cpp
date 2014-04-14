#include "stdafx.h"
#include "CppUnitTest.h"
#include "Interaction.h"
#include <vector>

using std::vector;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace OBGCoreTests
{
	TEST_CLASS(InteractionTest)
	{
	public:
		
		TEST_METHOD(InteractionConstructorTest)
		{
			btVector3 expPos(1,2,3);
			vector<int> vec = vector<int>();
			vec.push_back(1);
			Interaction *interaction = new Interaction(expPos, vec, 0);

			Assert::AreEqual(expPos, interaction->mousePos);
			Assert::AreEqual(1, vec.at(0));
		}
	};
}