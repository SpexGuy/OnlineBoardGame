#pragma once
#include <string>
#include <btBulletDynamicsCommon.h>

class Entity;

class Asset {
private:
	Asset();
protected:
	std::string name;
	std::string group;
	float mass;
	btVector3 centerMass;
	btTriangleMesh *collider;

public:
	inline float getMass() { return mass; }
	inline std::string &getGroup() { return group; }
	Asset(const std::string &name, const std::string &group, float mass,
		  const btVector3 &centerMass, btTriangleMesh *collider)
	 :	name(name),
		group(group),
		mass(mass),
		centerMass(centerMass),
		collider(collider)
	{}

	virtual Entity *createEntity(const btVector3 &position, int id);
	
	virtual ~Asset();
};
