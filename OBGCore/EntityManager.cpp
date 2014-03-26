#include <iostream>
#include "EntityManager.h"
#include "Entity.h"
#include "Asset.h"
#include "Interaction.h"
#include "PhysicsUpdate.h"
	
EntityManager::EntityManager() {
	//Set up the physics world
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration;
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver;
    world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	world->setGravity(btVector3(0.0f, -10.0f, 0.0f));



 //   btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);

 //   btCollisionShape* fallShape = new btSphereShape(1);

	//btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
 //   btRigidBody::btRigidBodyConstructionInfo
 //           groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
 //   btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
 //   world->addRigidBody(groundRigidBody);


 //   btDefaultMotionState* fallMotionState =
 //           new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,50,0)));
 //   btScalar mass = 1;
 //   btVector3 fallInertia(0,0,0);
 //   fallShape->calculateLocalInertia(mass,fallInertia);
 //   btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
 //   btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
 //   world->addRigidBody(fallRigidBody);


 //   for (int i=0 ; i<300 ; i++) {
 //           world->stepSimulation(1/60.f,10);

 //           btTransform trans;
 //           fallRigidBody->getMotionState()->getWorldTransform(trans);

 //           std::cout << "sphere height: " << trans.getOrigin().getY() << std::endl;
 //   }

 //   world->removeRigidBody(fallRigidBody);
 //   delete fallRigidBody->getMotionState();
 //   delete fallRigidBody;

 //   world->removeRigidBody(groundRigidBody);
 //   delete groundRigidBody->getMotionState();
 //   delete groundRigidBody;


 //   delete fallShape;

 //   delete groundShape;
}

EntityManager::~EntityManager() {

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
	entities[e->getId()] = e;
	world->addRigidBody(e->getPhysicsBody());
}

void EntityManager::handleInteraction(Interaction *action) {
	
}

void EntityManager::handlePhysicsUpdate(PhysicsUpdate *update) {
	Entity* ent = entities[update->entityId];

	//Pull it out
	world->removeRigidBody(ent->getPhysicsBody());

	//Lube it up
	delete ent->getPhysicsBody()->getMotionState();
	ent->getPhysicsBody()->setMotionState(new btDefaultMotionState(update->motion));
	ent->getPhysicsBody()->setMassProps(ent->getType()->getMass(), update->inertia);

	//Put it back in
	world->addRigidBody(ent->getPhysicsBody());
}

void EntityManager::start() {
	lastTime = clock();
}

void EntityManager::update() {
	//Step physics simulation

	clock_t currTime = clock();
	world->stepSimulation(float(currTime - lastTime)/float(CLOCKS_PER_SEC), 10);
	lastTime = currTime;

	btTransform transform;
	entities.at(0)->getPhysicsBody()->getMotionState()->getWorldTransform(transform);
	std::cout << "Entity 0: " << transform.getOrigin().getY() << std::endl;
	
	//Combine stackable entities
	for(unsigned int i = 0; i < entities.size(); i ++) {
		for(unsigned int j = i + 1; j < entities.size(); j ++) {
			Entity *ent1 = entities.at(i);
			Entity *ent2 = entities.at(j);

			if(ent1->getType()->getGroup() == ent2->getType()->getGroup()) { //&& similar positions && stackable) {
				//stack them
			}
		}
	}
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

Entity* EntityManager::getIntersectingEntity(const btVector3& from, const btVector3& to, Interaction* interaction) {
	btCollisionWorld::ClosestRayResultCallback callback(from, to);
	world->rayTest(from, to, callback);

	if(callback.hasHit()) {
		const btCollisionObject *collided = callback.m_collisionObject;
		for(pair<int, Entity *> e : entities) {
			if(e.second->getPhysicsBody() == collided) {
				return e.second;
			}
		}
	}
	return NULL;
}

void EntityManager::createPhysicsUpdates() {
	
}