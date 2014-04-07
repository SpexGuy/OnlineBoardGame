#include "stdafx.h"
#include "CppUnitTest.h"
#include "Asset.h"
#include "Entity.h"
#include "CollisionShapeInflater.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace OBGCoreTests
{
	TEST_CLASS(EntityTest)
	{
	public:
		btTransform transform;
		BoxInflater *box;;
		Asset *asset;
		Entity *entity;

		void setup()
		{
			transform = btTransform(btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(1.0, 2.0, 3.0));
			box = new BoxInflater(btVector3(0.5, 0.5, 0.5));
			asset = new Asset("Box", "1", 1.0, btVector3(), transform, box);
			entity = new Entity(asset, 5, transform);
		}

		void teardown()
		{
			delete box;
			delete asset;
			delete entity;
		}

		TEST_METHOD(ShowAndHide)
		{
			setup();
			Assert::IsFalse(entity->getHidden());
			entity->hide();
			Assert::IsTrue(entity->getHidden());
			entity->show();
			Assert::IsFalse(entity->getHidden());
		}

	};
}