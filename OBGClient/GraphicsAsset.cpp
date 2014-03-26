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
	btMotionState *motionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), position));
	btCompoundShape *collisionShape = new btCompoundShape();
	//btConvexTriangleMeshShape *collisionShape = new btConvexTriangleMeshShape(collider);
	btSphereShape *collisionMesh = new btSphereShape(1);
	collisionShape->addChildShape(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)), collisionMesh);
    btVector3 fallInertia(0,0,0);
    collisionShape->calculateLocalInertia(mass,fallInertia);
	btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, collisionShape, fallInertia);
	btRigidBody *physicsBody = new btRigidBody(constructionInfo);
	

	return new GraphicsEntity(this, id, physicsBody);
}
