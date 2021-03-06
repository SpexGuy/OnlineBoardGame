#pragma once
#include <string>
#include <btBulletDynamicsCommon.h>

class Entity;
class CollisionShapeInflater;
class ShakeStrategy;

class Asset {
private:
	Asset();
protected:
	std::string name;
	std::string group;
	float mass;
	btVector3 centerMass;
	btTransform orientation;
	CollisionShapeInflater *collider;
	ShakeStrategy *shakeType;

	void inflateRigidBody(Entity *entity);

public:
	inline float getMass() { return mass; }
	inline std::string &getGroup() { return group; }
	Asset(const std::string &name, const std::string &group, float mass,
		const btVector3 &centerMass, const btTransform &orientation, CollisionShapeInflater *collider, ShakeStrategy *shakeType)
	 :	name(name),
		group(group),
		mass(mass),
		centerMass(centerMass),
		orientation(orientation),
		collider(collider),
		shakeType(shakeType)
	{}

	virtual Entity *createEntity(const btTransform &position, int id);
	
	virtual ~Asset();
};
