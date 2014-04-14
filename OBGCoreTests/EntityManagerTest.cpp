#include "stdafx.h"
#include "CppUnitTest.h"
#include "Entity.h"
#include "Asset.h"
#include "CollisionShapeInflater.h"
#include "EntityManager.h"
#include <iostream>
#include "Interaction.h"
#include "PhysicsUpdate.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace OBGCoreTests
{		
	TEST_CLASS(UnitTest1)
	{
	private:
		EntityManager *entityManager;
		BoxInflater *box;
		Asset *asset;
		Entity *entity;
	public:
		TEST_METHOD_INITIALIZE(setup) {
			entityManager = new EntityManager();
			box = new BoxInflater(btVector3(0.25, 0.25, 0.25));
			asset = new Asset("Box", "1", 10.0f, btVector3(0,0,0), btTransform(btQuaternion(0,0,0,1), btVector3(0,0,0)), box); 
			entity = asset->createEntity(btTransform(btQuaternion(0,0,0,1), btVector3(0,1,0)), 1);
		}
		
		TEST_METHOD(AddEntityTest)
		{
			entityManager->addEntity(entity);

			std::map<int, Entity *> testEntities = std::map<int, Entity *>();
			testEntities[entity->getId()] = entity;

			std::cout << testEntities[1] << std::endl;
			
			for(std::pair<int, Entity *> e : testEntities) {
				Entity *actual = entityManager->getEntities().at(e.second->getId());
				std::cout << actual << std::endl;
				Assert::AreEqual(e.second->getId(), actual->getId());
			}
		}

		TEST_METHOD(HandleInteractionPickupAndDropTest)
		{
			entityManager->addEntity(entity);
			std::vector<int> vec = std::vector<int>();
			vec.push_back(1);
			Interaction *interaction = new Interaction(btVector3(1, 2, 0), vec, 0);
			entityManager->handleInteraction(interaction);
			btVector3 gravity = entityManager->getEntityById(1)->getPhysicsBody()->getGravity();
			Assert::AreEqual(0.0, (double) (gravity.getX() + gravity.getY() + gravity.getZ()));

			vec.pop_back();
			vec.push_back(-1);
			delete interaction;
			interaction = new Interaction(btVector3(1, 2, 0), vec, 0);

			entityManager->handleInteraction(interaction);
			gravity = entityManager->getEntityById(1)->getPhysicsBody()->getGravity();

			Assert::AreEqual(entityManager->getWorld()->getGravity(), gravity);
		}

		TEST_METHOD(IntersectingEntityHitTest)
		{
			entityManager->addEntity(entity);

			Entity* actual = entityManager->getIntersectingEntity(btVector3(0.0, 5.0, 0.0), btVector3(0.0, -5, 0.0), std::vector<int>());
			Assert::IsNotNull(actual, L"No intersecting entity");
			Assert::AreEqual(entity->getId(), actual->getId(), L"Identities not equal");
		}

		TEST_METHOD(IntersectingEntityMissTest)
		{
			entityManager->addEntity(entity);

			Entity* actual = entityManager->getIntersectingEntity(btVector3(1.0, 3.0, 0.0), btVector3(1.0, -0.5, 0.0), std::vector<int>());
			Assert::IsNull(actual);
		}

		TEST_METHOD(PhysicsUpdateHandleTest)
		{
			btVector3 position(4,2,3);
			entityManager->addEntity(entity);
			PhysicsUpdate update(entity->getId(), btTransform(btQuaternion(), 
				position), btVector3(0, 0, 0), btVector3(0, 0, 0));

			entityManager->handlePhysicsUpdate(&update);
			btTransform transform = entity->getPhysicsBody()->getWorldTransform();
			Assert::AreEqual(position, transform.getOrigin());
		}

		TEST_METHOD(PhysicsUpdateCreationTest)
		{
			btVector3 expAngular(0,1,0);
			btVector3 expLinear(1,2,3);
			btVector3 expPos(4,5,6);

			entityManager->addEntity(entity);

			EntityManager *testManager = new EntityManager();
			BoxInflater *testBox = new BoxInflater(btVector3(0.25, 0.25, 0.25));
			Asset* testAsset = new Asset("Box", "1", 10.0f, btVector3(0,0,0), btTransform(btQuaternion(0,0,0,1), btVector3(0,2,0)), testBox); 
			Entity *testEntity = testAsset->createEntity(btTransform(btQuaternion(0,0,0,1), btVector3(0,0,0)), 1);
			btTransform transform = btTransform(btQuaternion(0,0,0,1), expPos);
			testEntity->getPhysicsBody()->setAngularVelocity(expAngular);
			testEntity->getPhysicsBody()->setLinearVelocity(expLinear);
			testEntity->getPhysicsBody()->setWorldTransform(transform);
			testManager->addEntity(testEntity);

			testManager->registerPhysicsUpdateListener(entityManager);

			testManager->createPhysicsUpdates();

			btRigidBody *actualBody = entity->getPhysicsBody();
			btTransform actualTransform = actualBody->getWorldTransform();

			Assert::AreEqual(expPos, actualTransform.getOrigin());
			Assert::AreEqual(expLinear, actualBody->getLinearVelocity());
			Assert::AreEqual(expAngular, actualBody->getAngularVelocity());
		}

		TEST_METHOD(EntityClearTest)
		{
			entityManager->addEntity(entity);
			Assert::AreEqual(entity->getId(), entityManager->getEntityById(entity->getId())->getId());
			entityManager->clear();
			Assert::AreEqual(0, (int) entityManager->getEntities().size());
		}

		TEST_METHOD(GetEntityByIdTest)
		{
			entityManager->addEntity(entity);
			Assert::AreEqual(entity->getId(), entityManager->getEntityById(entity->getId())->getId());
		}

		TEST_METHOD(StartEntityManagerTest)
		{
			//not really anything to confirm works here, so just run the method to check for errors
			entityManager->start();
		}

		TEST_METHOD(UpdateEntityManagerTest)
		{
			//Not much for us to verify since bullet is so well tested and documented,
			//so here we just verify there are no errors.
			entityManager->start();
			entityManager->update();
		}

		TEST_METHOD_CLEANUP(teardown) {
			delete entityManager;
			delete box;
			delete asset;
			delete entity;
		}

	};
}