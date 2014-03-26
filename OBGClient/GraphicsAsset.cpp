#include <btBulletDynamicsCommon.h>
#include <GraphicsMesh.h>
#include <Material.h>
#include "GraphicsAsset.h"


using namespace glm;

void GraphicsAsset::draw(const mat4 &world) {
	material->bind();
	mesh->draw(world);
}

GraphicsEntity *GraphicsAsset::createEntity(const btVector3 &position, int id) {
	btMotionState *motionState = new btDefaultMotionState();
	btCompoundShape *collisionShape = new btCompoundShape();
	btConvexTriangleMeshShape *collisionMesh = new btConvexTriangleMeshShape(collider);
	collisionShape->addChildShape(btTransform(), collisionMesh);
	btRigidBody::btRigidBodyConstructionInfo constructionInfo = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, collisionShape);
	btRigidBody *physicsBody = new btRigidBody(constructionInfo);

	return new GraphicsEntity(this, id, physicsBody);
}
