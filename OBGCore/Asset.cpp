#include "Asset.h"
#include "CollisionShapeInflater.h"
#include "Entity.h"


void Asset::inflateRigidBody(Entity *entity) {
	btCollisionShape *shape = collider->inflate();
    btVector3 fallInertia(0,0,0);
	if (mass != 0)
		shape->calculateLocalInertia(mass,fallInertia);
	btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, entity, shape, fallInertia);
	entity->setRigidBody(new btRigidBody(constructionInfo));
}

Entity *Asset::createEntity(const btTransform &position, int id) {
	Entity *entity = new Entity(this, id, position);
	inflateRigidBody(entity);
	return entity;
}

Asset::~Asset() {

}