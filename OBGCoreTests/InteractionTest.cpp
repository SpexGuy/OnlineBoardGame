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
		
		TEST_METHOD(Constructor)
		{
			vector<int> vec = vector<int>();
			vec.push_back(1);
			Interaction *interaction = new Interaction(btVector3(1.0, 2.0, 3.0), vec);

			Assert::AreEqual(1.0, (double) interaction->mousePos.getX());
			Assert::AreEqual(2.0, (double) interaction->mousePos.getY());
			Assert::AreEqual(3.0, (double) interaction->mousePos.getZ());
			Assert::AreEqual(1, vec.at(0));
		}
	};
}