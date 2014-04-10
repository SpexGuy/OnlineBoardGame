#include "PhysicsUpdate.h"

PhysicsUpdate *PhysicsUpdate::create(int entityId, const btTransform& transform, const btVector3& linearVel, const btVector3& angularVel) {
	PhysicsUpdate *toReturn = (PhysicsUpdate *) malloc(sizeof(PhysicsUpdate));
	toReturn->entityId = entityId;
	toReturn->transform = transform;
	toReturn->linearVel = linearVel;
	toReturn->angularVel = angularVel;
	return toReturn;
}

void PhysicsUpdate::emancipate() {
	free(this);
}