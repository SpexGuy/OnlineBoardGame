#include "EntityManager.h"
#include "Entity.h"
	
EntityManager::EntityManager() {
	//Set up the physics world
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver;
    world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	world->setGravity(btVector3(0.0f, -1.0f, 0.0f));
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

void EntityManager::update() {
	//Step physics simulation
	//world->stepSimulation();

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

Entity* EntityManager::getEntityByIndex(int i) {
	return entities.at(i);
}

Entity* EntityManager::getIntersectingEntity(const btVector3& from, const btVector3& to, Interaction* interaction) {
	return NULL;
}

void EntityManager::createPhysicsUpdates() {
	
}