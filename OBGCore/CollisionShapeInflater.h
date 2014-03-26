#include <vector>
#include <btBulletDynamicsCommon.h>

class CollisionShapeInflater {
public:
	virtual btCollisionShape *inflate() = 0;
	virtual ~CollisionShapeInflater() {}
};

class SphereInflater :
	public CollisionShapeInflater {
protected:
	btScalar radius;
public:
	SphereInflater(btScalar radius) :
		radius(radius) {}
	virtual btSphereShape *inflate();
	virtual ~SphereInflater() {}
};

class BoxInflater :
	public CollisionShapeInflater {
private:
	btVector3 halfExtents;
public:
	BoxInflater(const btVector3 &halfExtents) :
		halfExtents(halfExtents) {}
	virtual btBoxShape *inflate();
	virtual ~BoxInflater() {}
};

class CylinderInflater :
	public CollisionShapeInflater {
private:
	btVector3 halfExtents;
public:
	CylinderInflater(const btVector3 &halfExtents) :
		halfExtents(halfExtents) {}
	virtual btCylinderShape *inflate();
	virtual ~CylinderInflater() {}
};

class CapsuleInflater :
	public CollisionShapeInflater {
private:
	btScalar radius;
	btScalar distance;
public:
	CapsuleInflater(btScalar radius, btScalar distance) :
		radius(radius), distance(distance) {}
	virtual btCapsuleShape *inflate();
	virtual ~CapsuleInflater() {}
};

class ConeInflater :
	public CollisionShapeInflater {
private:
	btScalar radius;
	btScalar height;
public:
	ConeInflater(btScalar radius, btScalar height) :
		radius(radius), height(height) {}
	virtual btConeShape *inflate();
	virtual ~ConeInflater() {}
};

class MultiSphereInflater :
	public CollisionShapeInflater {
private:
	btVector3 *positions;
	btScalar *radii;
	int num;
public:
	MultiSphereInflater(btVector3 *positions, btScalar *radii, int num) :
		positions(positions), radii(radii), num(num) {}
	virtual btMultiSphereShape *inflate();
	virtual ~MultiSphereInflater() {
		delete[] positions;
		delete[] radii;
	}
};

class ConvexHullInflater :
	public CollisionShapeInflater {
private:
	std::vector<btVector3> points;
public:
	ConvexHullInflater(const std::vector<btVector3> &points) :
		points(points) {}
	virtual btConvexHullShape *inflate();
	virtual ~ConvexHullInflater() {}
};

class CompoundInflater :
	public CollisionShapeInflater {
private:
	std::vector<std::pair<btTransform, CollisionShapeInflater *>> inflaters;
public:
	CompoundInflater() {}
	void addInflater(const btTransform &transform, CollisionShapeInflater *inflater);
	virtual btCompoundShape *inflate();
	virtual ~CompoundInflater();
};
