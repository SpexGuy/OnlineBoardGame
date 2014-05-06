#include <iostream>
#include <assert.h>
#include "UDPConnection.h"
#include "PhysicsUpdate.h"

using namespace std;

PhysicsUpdate::PhysicsUpdate(SerialPhysicsUpdate *upd, int len) {
	assert(len == sizeof(*upd) + upd->size * sizeof(EntityUpdate));
	if (len >= sizeof(*upd)) {
		for (int c = 0; c < upd->size; c++) {
			push_back(upd->updates[c]);
		}
	} else {
		cerr << "Physics Update too small" << endl;
	}
}

bool PhysicsUpdate::addEntity(int entityId, bool active, const btTransform &transform, const btVector3 &linearVel, const btVector3 &angularVel) {
	push_back(EntityUpdate(entityId, active, transform, linearVel, angularVel));
	return serialByteSize() + sizeof(EntityUpdate) <= MAX_PACKET_SIZE;
}


void PhysicsUpdate::fill(SerialPhysicsUpdate *memory) {
	memory->size = size();
	if (size() > 0)
		memcpy(memory->updates, &((*this)[0]), arrayByteSize());
}
