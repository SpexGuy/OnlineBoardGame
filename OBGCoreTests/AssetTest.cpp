#include "stdafx.h"
#include "CppUnitTest.h"
#include "btBulletDynamicsCommon.h"
#include "Asset.h"
#include "Entity.h"
#include "CollisionShapeInflater.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace OBGCoreTests
{
	TEST_CLASS(AssetTest)
	{
	public:
		btTransform transform;
		BoxInflater *box;;
		Asset *asset;

		void setup()
		{
			transform = btTransform(btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(1.0, 2.0, 3.0));
			box = new BoxInflater(btVector3(0.5, 0.5, 0.5));
			asset = new Asset("Box", "1", 1.0, btVector3(), transform, box);
		}

		void teardown()
		{
			delete box;
			delete asset;
		}

		TEST_METHOD(EntityCreation)
		{
			setup();

			Entity *entity = new Entity(asset, 5, transform);
			Assert::AreEqual(5, entity->getId());
			btVector3 origin = transform.getOrigin();
			btTransform actualTransform;
			entity->getWorldTransform(actualTransform);
			btVector3 actual = actualTransform.getOrigin();

			Assert::AreEqual(origin.getX(), actual.getX());
			Assert::AreEqual(origin.getY(), actual.getY());
			Assert::AreEqual(origin.getZ(), actual.getZ());

			teardown();
		}

	};
}