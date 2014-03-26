#include "Asset.h"
#include "CollisionShapeInflater.h"
#include "Entity.h"


btRigidBody *Asset::createRigidBody(const btTransform &position) {
	btMotionState *motionState = new btDefaultMotionState(orientation * position);
	btCollisionShape *shape = collider->inflate();
    btVector3 fallInertia(0,0,0);
    shape->calculateLocalInertia(mass,fallInertia);
	btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, shape, fallInertia);
	return new btRigidBody(constructionInfo);
}

Entity *Asset::createEntity(const btTransform &position, int id) {
	return new Entity(this, id, createRigidBody(position));
}

Asset::~Asset() {

}