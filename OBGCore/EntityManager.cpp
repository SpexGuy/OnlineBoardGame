#include <assert.h>
#include <iostream>
#include "EntityManager.h"
#include "Entity.h"
#include "Asset.h"
#include "Interaction.h"
#include "PhysicsUpdate.h"
	
#define MAX_PICKUP_VELOCITY 10
#define PICKUP_POWER 10

using namespace std;

EntityManager::EntityManager() {
	FunctionLock lock(worldLock);
	//Set up the physics world
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration;
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver;
    world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	world->setGravity(btVector3(0.0f, -10.0f, 0.0f));

	btCollisionShape *groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
    btRigidBody::btRigidBodyConstructionInfo
            groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    world->addRigidBody(groundRigidBody);

	btCollisionShape *groundShape2 = new btStaticPlaneShape(btVector3(-1, 1, 0), -2);
	btDefaultMotionState* groundMotionState2 = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
    btRigidBody::btRigidBodyConstructionInfo
            groundRigidBodyCI2(0,groundMotionState2,groundShape2,btVector3(0,0,0));
    btRigidBody* groundRigidBody2 = new btRigidBody(groundRigidBodyCI2);
    world->addRigidBody(groundRigidBody2);

	btCollisionShape *groundShape3 = new btStaticPlaneShape(btVector3(1, 1, 0), -2);
	btDefaultMotionState* groundMotionState3 = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
    btRigidBody::btRigidBodyConstructionInfo
            groundRigidBodyCI3(0,groundMotionState3,groundShape3,btVector3(0,0,0));
    btRigidBody* groundRigidBody3 = new btRigidBody(groundRigidBodyCI3);
    world->addRigidBody(groundRigidBody3);

	btCollisionShape *groundShape4 = new btStaticPlaneShape(btVector3(0, 1, 1), -2);
	btDefaultMotionState* groundMotionState4 = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
    btRigidBody::btRigidBodyConstructionInfo
            groundRigidBodyCI4(0,groundMotionState4,groundShape4,btVector3(0,0,0));
    btRigidBody* groundRigidBody4 = new btRigidBody(groundRigidBodyCI4);
    world->addRigidBody(groundRigidBody4);

	btCollisionShape *groundShape5 = new btStaticPlaneShape(btVector3(0, 1, -1), -2);
	btDefaultMotionState* groundMotionState5 = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
    btRigidBody::btRigidBodyConstructionInfo
            groundRigidBodyCI5(0,groundMotionState5,groundShape5,btVector3(0,0,0));
    btRigidBody* groundRigidBody5 = new btRigidBody(groundRigidBodyCI5);
    world->addRigidBody(groundRigidBody5);
}

EntityManager::~EntityManager() {
	FunctionLock lock(worldLock);
	//Remove the objects from the physics world
	for(pair<int, Entity*> e : entities) {
		world->removeRigidBody(e.second->getPhysicsBody());
	}

	//Delete the physics world and all associated things in it.
	delete world;
    delete solver;
    delete collisionConfiguration;
    delete dispatcher;
    delete broadphase;
}

void EntityManager::addEntity(Entity *e) {
	FunctionLock lock(worldLock);
	entities[e->getId()] = e;
	world->addRigidBody(e->getPhysicsBody());
}

void EntityManager::handleInteraction(Interaction *action) {
	FunctionLock lock(worldLock);
	for(int &i : action->ids) {
		assert(i != 0);
		if(i < 0) {
			btRigidBody& physBody = *entities[-i]->getPhysicsBody();
			physBody.setGravity(world->getGravity());
			physBody.setAngularFactor(btVector3(1, 1, 1));
			physBody.activate(true);
		} else {
			btRigidBody& physBody = *entities[i]->getPhysicsBody();
			btVector3 v = action->mousePos - physBody.getWorldTransform().getOrigin();
			v *= PICKUP_POWER;
			if (v.length() > MAX_PICKUP_VELOCITY) {
				v = MAX_PICKUP_VELOCITY * v.normalized();
			}
			physBody.setLinearVelocity(v);
			physBody.setGravity(btVector3(0,0,0));
			physBody.setAngularFactor(btVector3(0.1, 0.1, 0.1));
			physBody.setAngularVelocity(btVector3(0, 0, 0));
			physBody.activate(true);
		}
	}
}

void EntityManager::createPhysicsUpdates() {
	FunctionLock lock(worldLock);
	for(pair<int, Entity*> p : entities) {
		Entity * e = p.second;
		btRigidBody& physBody = * e->getPhysicsBody();
		btTransform transform = physBody.getWorldTransform();
		btVector3 linear = physBody.getLinearVelocity();
		btVector3 angular = physBody.getAngularVelocity();
		PhysicsUpdate *update = PhysicsUpdate::create(e->getId(), transform, linear, angular);

		firePhysicsUpdate(update);

		update->emancipate();
	}
}

void EntityManager::handlePhysicsUpdate(PhysicsUpdate *update) {
	FunctionLock lock(worldLock);
	Entity* ent = entities[update->entityId];
	
	btRigidBody& physBody = *ent->getPhysicsBody();
	
	physBody.setWorldTransform(update->transform);
	physBody.setLinearVelocity(update->linearVel);
	physBody.setAngularVelocity(update->angularVel);
}

void EntityManager::start() {
	lastTime = clock();
}

void EntityManager::update() {
	//Step physics simulation

	clock_t currTime = clock();
	FunctionLock lock(worldLock);
	world->stepSimulation(float(currTime - lastTime)/float(CLOCKS_PER_SEC), 10);
	lock.unlock();
	lastTime = currTime;
	
	//Combine stackable entities
	//This is the WRONG WAY to iterate physics objects.
	//Use AABBs and the collision broadphase instead.
	//for(auto iter1 = entities.begin(), end = entities.end(); iter1 != end;) {
	//	Entity *ent1 = iter1->second;
	//	for(auto iter2 = ++iter1; iter2 != end; ++iter2) {
	//		Entity *ent2 = iter2->second;

	//		if(ent1->getType()->getGroup() == ent2->getType()->getGroup()) { //&& similar positions && stackable) {
	//			//stack them
	//		}
	//	}
	//}
}

void EntityManager::clear() {
	for(pair<int, Entity*> e : entities) {
		world->removeRigidBody(e.second->getPhysicsBody());	
	}
	entities.clear();
}

Entity* EntityManager::getEntityById(int id) {
	return entities[id];
}

Entity* EntityManager::getIntersectingEntity(const btVector3& from, const btVector3& to) {
	btCollisionWorld::ClosestRayResultCallback callback(from, to);
	FunctionLock lock(worldLock);
		world->rayTest(from, to, callback);
	lock.unlock();

	if(callback.hasHit()) {
		const btCollisionObject *collided = callback.m_collisionObject;
		return (Entity *) ((btRigidBody *)collided)->getMotionState();
	}
	return NULL;
}
