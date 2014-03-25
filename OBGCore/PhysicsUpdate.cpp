#include "PhysicsUpdate.h"

PhysicsUpdate* PhysicsUpdate::create(int entityId, const btDefaultMotionState& motion, const btVector3& inertia) {
	PhysicsUpdate* toReturn = (PhysicsUpdate *) malloc(sizeof(PhysicsUpdate));
	toReturn->entityId = entityId;
	toReturn->inertia = inertia;
	toReturn->motion = motion;

	return toReturn;
}

void PhysicsUpdate::emancipate() {
	free(this);
}