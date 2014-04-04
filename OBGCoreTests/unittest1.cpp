#include "stdafx.h"
#include "CppUnitTest.h"
#include "Entity.h"
#include "Asset.h"
#include "CollisionShapeInflater.h"
#include "EntityManager.h"
#include <iostream>
#include "Interaction.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace OBGCoreTests
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		EntityManager *entityManager;
		SphereInflater *sphere;
		Asset *asset;
		Entity *entity;

		void setup() {
			entityManager = new EntityManager();
			sphere = new SphereInflater(1.0);
			asset = new Asset("Sphere", "1", 1.0f, btVector3(), btTransform(), sphere); 
			entity = asset->createEntity(btTransform(), 1);
		}

		void teardown() {
			delete entityManager;
			delete sphere;
			delete asset;
			delete entity;
		}
		
		TEST_METHOD(TestMethod1)
		{
			setup();
			entityManager->addEntity(entity);

			std::map<int, Entity *> testEntities = std::map<int, Entity *>();
			testEntities[entity->getId()] = entity;

			std::cout << testEntities[1] << std::endl;
			
			for(std::pair<int, Entity *> e : testEntities) {
				Entity *actual = entityManager->getEntities().at(e.second->getId());
				std::cout << actual << std::endl;
				Assert::IsTrue(e.second == actual);
			}

			teardown();
		}

		TEST_METHOD(TestMethod2)
		{
			setup();
			
			entityManager->addEntity(entity);
			std::vector<int> vec = std::vector<int>();
			vec.push_back(1);
			Interaction *interaction = new Interaction(btVector3(1, 2, 0), vec);
			entityManager->handleInteraction(interaction);
			btVector3 gravity = entityManager->getEntityById(1)->getPhysicsBody()->getGravity();
			Assert::AreEqual(0.0, (double) (gravity.getX() + gravity.getY() + gravity.getZ()));

			vec.pop_back();
			vec.push_back(-1);
			delete interaction;
			interaction = new Interaction(btVector3(1, 2, 0), vec);

			entityManager->handleInteraction(interaction);
			gravity = entityManager->getEntityById(1)->getPhysicsBody()->getGravity();

			Assert::AreEqual(entityManager->getWorld()->getGravity().getX(), gravity.getX());
			Assert::AreEqual(entityManager->getWorld()->getGravity().getY(), gravity.getY());
			Assert::AreEqual(entityManager->getWorld()->getGravity().getZ(), gravity.getZ());

			teardown();
		}
		

	};
}