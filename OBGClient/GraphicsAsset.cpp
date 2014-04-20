#include <btBulletDynamicsCommon.h>
#include <GraphicsMesh.h>
#include <Material.h>
#include "GraphicsAsset.h"


using namespace glm;

void GraphicsAsset::draw(const mat4 &world) {
	material->bind();
	mesh->draw(world);
}

GraphicsEntity *GraphicsAsset::createEntity(const btTransform &position, int id) {	
	GraphicsEntity *e = new GraphicsEntity(this, id, position, shakeType);
	inflateRigidBody(e);
	return e;
}
