#include "Asset.h"
#include "Entity.h"



Entity *Asset::createEntity(const btVector3 &position, int id) {
	btMotionState *motionState = new btDefaultMotionState();
	btCompoundShape *collisionShape = new btCompoundShape();
	btConvexTriangleMeshShape *collisionMesh = new btConvexTriangleMeshShape(collider);
	collisionShape->addChildShape(btTransform(), collisionMesh);
	btRigidBody::btRigidBodyConstructionInfo constructionInfo = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, collisionShape);
	btRigidBody *physicsBody = new btRigidBody(constructionInfo);

	return new Entity(this, id, physicsBody);
}

Asset::~Asset() {

}