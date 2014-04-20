#pragma once
#include <vector>
#include <stdint.h>
#include "btBulletDynamicsCommon.h"

/** This class has no constructor or destructor.
 *	It should be allocated with malloc() and
 *	destroyed with free().
 *	This is an artifact of serialization.
 *	
 *	Since this type will be serialized,
 *	do not give it any virtual functions.
 *	If you must give it a pointer, variable-length
 *	type, or virtual function, you must change
 *	the serialization code in
 *	ServerConnection::sendUpdate(..)
 *	and the deserialization code in
 *	ClientConnection::processData(..).
 */

class EntityUpdate {
private:
	int entityId;
	btTransform transform;
	btVector3 linearVel;
	btVector3 angularVel;
public:
	EntityUpdate(int entityId, const btTransform &transform, const btVector3 &linearVel, const btVector3 &angularVel) :
		entityId(entityId), transform(transform), linearVel(linearVel), angularVel(angularVel)
	{}
	inline int getEntityId() const { return entityId; }
	inline const btTransform &getWorldTransform() const { return transform; }
	inline const btVector3 &getLinearVelocity() const { return linearVel; }
	inline const btVector3 &getAngularVelocity() const { return angularVel; }
};

#pragma warning(disable: 4200)
struct SerialPhysicsUpdate {
	uint16_t size;
	EntityUpdate updates[0];
};
#pragma warning(default: 4200)

class PhysicsUpdate : public std::vector<EntityUpdate> {
private:
	inline int arrayByteSize() { return size() * sizeof(EntityUpdate); }
public:
	PhysicsUpdate() {}
	PhysicsUpdate(SerialPhysicsUpdate *upd, int len);

	inline int serialByteSize() { return arrayByteSize() + sizeof(SerialPhysicsUpdate); }
	bool addEntity(int entityId, const btTransform &transform, const btVector3 &linearVel, const btVector3 &angularVel);
	void fill(SerialPhysicsUpdate *memory);
};
