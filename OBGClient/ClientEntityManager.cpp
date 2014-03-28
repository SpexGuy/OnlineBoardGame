#include "ClientEntityManager.h"
#include "CollisionMeshRenderer.h"

void ClientEntityManager::start() {
	EntityManager::start();
	//world->setDebugDrawer(new CollisionMeshRenderer());
	//world->getDebugDrawer()->setDebugMode(
	//	btIDebugDraw::DBG_DrawWireframe |
	//	btIDebugDraw::DBG_DrawAabb);
}

void ClientEntityManager::update() {
	EntityManager::update();
	//world->debugDrawWorld();
}

