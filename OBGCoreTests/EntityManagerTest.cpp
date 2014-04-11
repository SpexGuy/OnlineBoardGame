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
	public:
		EntityManager *entityManager;
		BoxInflater *box;
		Asset *asset;
		Entity *entity;

		void setup() {
			entityManager = new EntityManager();
			box = new BoxInflater(btVector3(0.25, 0.25, 0.25));
			asset = new Asset("Box", "1", 10.0f, btVector3(0.0, 0.0, 0.0), btTransform(btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(0.0, 2.0, 0.0)), box); 
			entity = asset->createEntity(btTransform(btQuaternion(), btVector3(0.0 ,1.0, 0.0)), 1);
		}

		void teardown() {
			delete entityManager;
			delete box;
			delete asset;
			delete entity;
		}
		
		TEST_METHOD(AddEntity)
		{
			setup();
			entityManager->addEntity(entity);

			std::map<int, Entity *> testEntities = std::map<int, Entity *>();
			testEntities[entity->getId()] = entity;

			std::cout << testEntities[1] << std::endl;
			
			for(std::pair<int, Entity *> e : testEntities) {
				Entity *actual = entityManager->getEntities().at(e.second->getId());
				std::cout << actual << std::endl;
				Assert::AreEqual(e.second->getId(), actual->getId());
			}

			teardown();
		}

		TEST_METHOD(HandleInteractionPickupAndDrop)
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

		TEST_METHOD(IntersectingEntityHit)
		{
			setup();

			entityManager->addEntity(entity);

			Entity* actual = entityManager->getIntersectingEntity(btVector3(0.0, 3.0, 0.0), btVector3(0.0, -0.5, 0.0));
			Assert::AreEqual(entity->getId(), actual->getId());

			teardown();
		}

		TEST_METHOD(IntersectingEntityMiss)
		{
			setup();
			
			entityManager->addEntity(entity);

			Entity* actual = entityManager->getIntersectingEntity(btVector3(1.0, 3.0, 0.0), btVector3(1.0, -0.5, 0.0));
			Assert::IsNull(actual);


			teardown();
		}

		TEST_METHOD(PhysicsUpdateHandle)
		{
			setup();

			entityManager->addEntity(entity);
			PhysicsUpdate *update = PhysicsUpdate::create(entity->getId(), btTransform(btQuaternion(), 
				btVector3(4, 2, 3)), btVector3(0, 0, 0), btVector3(0, 0, 0));

			entityManager->handlePhysicsUpdate(update);
			update->emancipate();
			btTransform transform = entity->getPhysicsBody()->getWorldTransform();
			btVector3 pos = transform.getOrigin();
			Assert::AreEqual(4.0, (double) pos.getX());
			Assert::AreEqual(2.0, (double) pos.getY());
			Assert::AreEqual(3.0, (double) pos.getZ());

			teardown();
		}

		TEST_METHOD(PhysicsUpdateCreation)
		{
			setup();
			entityManager->addEntity(entity);

			EntityManager *testManager = new EntityManager();
			BoxInflater *testBox = new BoxInflater(btVector3(0.25, 0.25, 0.25));
			Asset* testAsset = new Asset("Box", "1", 10.0f, btVector3(0.0, 0.0, 0.0), btTransform(btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(0.0, 2.0, 0.0)), testBox); 
			Entity *testEntity = testAsset->createEntity(btTransform(btQuaternion(0,0,0,1), btVector3(0.0 ,0.0, 0.0)), 1);
			btTransform transform = btTransform(btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(4.0, 5.0, 6.0));
			testEntity->getPhysicsBody()->setAngularVelocity(btVector3(0.0, 1.0, 0.0));
			testEntity->getPhysicsBody()->setLinearVelocity(btVector3(1.0, 2.0, 3.0));
			testEntity->getPhysicsBody()->setWorldTransform(transform);
			testManager->addEntity(testEntity);

			testManager->registerPhysicsUpdateListener(entityManager);

			testManager->createPhysicsUpdates();

			btRigidBody *actualBody = entity->getPhysicsBody();
			btTransform actualTransform = actualBody->getWorldTransform();
			btVector3 vec = actualTransform.getOrigin();


			Assert::AreEqual(4.0, (double) vec.getX());
			Assert::AreEqual(5.0, (double) vec.getY());
			Assert::AreEqual(6.0, (double) vec.getZ());
			
			vec = actualBody->getLinearVelocity();
			Assert::AreEqual(1.0, (double) vec.getX());
			Assert::AreEqual(2.0, (double) vec.getY());
			Assert::AreEqual(3.0, (double) vec.getZ());
			
			vec = actualBody->getAngularVelocity();
			Assert::AreEqual(0.0, (double) vec.getX());
			Assert::AreEqual(1.0, (double) vec.getY());
			Assert::AreEqual(0.0, (double) vec.getZ());

			teardown();
		}

		TEST_METHOD(clear)
		{
			setup();
			entityManager->addEntity(entity);
			Assert::AreEqual(entity->getId(), entityManager->getEntityById(entity->getId())->getId());
			entityManager->clear();
			Assert::AreEqual(0, (int) entityManager->getEntities().size());
			teardown();
		}

		TEST_METHOD(EntityById)
		{
			setup();
			entityManager->addEntity(entity);
			Assert::AreEqual(entity->getId(), entityManager->getEntityById(entity->getId())->getId());
			teardown();
		}

		TEST_METHOD(Start)
		{
			setup();
			//not really anything to confirm works here, so just run the method to check for errors
			entityManager->start();
			teardown();
		}

		TEST_METHOD(Update)
		{
			//Not much for us to verify since bullet is so well tested and documented,
			//so here we just verify there are no errors.
			setup();
			entityManager->start();
			entityManager->update();
			teardown();
		}
	};
}