#include "stdafx.h"
#include "CppUnitTest.h"
#include "Entity.h"
#include "Asset.h"
#include "CollisionShapeInflater.h"
#include "EntityManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace OBGCoreTests
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			EntityManager *entityManager = new EntityManager();
			SphereInflater *sphere = new SphereInflater(1.0);
			Asset *asset = new Asset("Sphere", "1", 1.0f, btVector3(), btTransform(), sphere); 
			Entity *entity = asset->createEntity(btTransform(), 1);

			entityManager->addEntity(entity);

			std::map<int, Entity *> testEntities = std::map<int, Entity *>();
			testEntities[entity->getId()] = entity;

			for(std::pair<int, Entity *> e : testEntities) {
				Entity *actual = entityManager->getEntities().at(e.second->getId());
				Assert::IsTrue(e.second == actual);
			}
		}

	};
}