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
	private:
		btTransform transform;
		BoxInflater *box;;
		Asset *asset;
		Entity *entity;

	public:
		TEST_METHOD_INITIALIZE(setup) {
			transform = btTransform(btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(1.0, 2.0, 3.0));
			box = new BoxInflater(btVector3(0.5, 0.5, 0.5));
			asset = new Asset("Box", "1", 1.0, btVector3(), transform, box);
			entity = new Entity(asset, 5, transform);
		}

		TEST_METHOD(EntityShowAndHideTest)
		{
			Assert::IsFalse(entity->getHidden());
			entity->hide();
			Assert::IsTrue(entity->getHidden());
			entity->show();
			Assert::IsFalse(entity->getHidden());
		}

		TEST_METHOD_CLEANUP(teardown) {
			delete box;
			delete asset;
			delete entity;
		}
	};
}
