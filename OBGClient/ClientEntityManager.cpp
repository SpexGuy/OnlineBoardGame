#include "ClientEntityManager.h"
#include "GLDebugDrawer.h"

void ClientEntityManager::start() {
	EntityManager::start();
	world->setDebugDrawer(new GLDebugDrawer());
	world->getDebugDrawer()->setDebugMode(
		btIDebugDraw::DBG_DrawWireframe |
		btIDebugDraw::DBG_DrawAabb);
}

void ClientEntityManager::update(int time) {
	EntityManager::update(time);
	//world->debugDrawWorld();
}

