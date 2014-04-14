#include "stdafx.h"
#include "CppUnitTest.h"
#include "CollisionMeshLoader.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace OBGCoreTests
{
	TEST_CLASS(CollisionMeshLoaderTest)
	{
	public:

		
		TEST_METHOD(LoadCollisionMeshTest)
		{
			std::istringstream stream = std::istringstream(
				"v 0.0 0.0 1.0 \n"
				"v 0.0 0.0 0.0 \n"
				"v 1.0 0.0 1.0 \n"
				"v 1.0 0.0 0.0 \n"
				);

			std::vector<btVector3> *verts = loadCollisionPoints(&stream);

			Assert::AreEqual(btVector3(0.0, 0.0, 1.0), verts->at(0));
			Assert::AreEqual(btVector3(0.0, 0.0, 0.0), verts->at(1));
			Assert::AreEqual(btVector3(1.0, 0.0, 1.0), verts->at(2));
			Assert::AreEqual(btVector3(1.0, 0.0, 0.0), verts->at(3));
			 
			delete verts;
		}

	};
}