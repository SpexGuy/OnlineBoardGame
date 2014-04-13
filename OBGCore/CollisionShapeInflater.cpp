#include "CollisionShapeInflater.h"

using namespace std;

btEmptyShape *EmptyInflater::inflate() {
	return new btEmptyShape();
}

btSphereShape *SphereInflater::inflate() {
	return new btSphereShape(radius);
}

btBoxShape *BoxInflater::inflate() {
	return new btBoxShape(halfExtents);
}

btCylinderShape *CylinderInflater::inflate() {
	return new btCylinderShape(halfExtents);
}

btCapsuleShape *CapsuleInflater::inflate() {
	return new btCapsuleShape(radius, distance);
}

btConeShape *ConeInflater::inflate() {
	return new btConeShape(radius, height);
}

btMultiSphereShape *MultiSphereInflater::inflate() {
	return new btMultiSphereShape(positions, radii, num);
}

btConvexHullShape *ConvexHullInflater::inflate() {
	btConvexHullShape *hull = new btConvexHullShape();
	for (btVector3 &point : points) {
		hull->addPoint(point, false);
	}
	hull->recalcLocalAabb();
	return hull;
}

void CompoundInflater::addInflater(const btTransform &transform, CollisionShapeInflater *inflater) {
	inflaters.push_back(pair<btTransform, CollisionShapeInflater *>(transform, inflater));
}
btCompoundShape *CompoundInflater::inflate() {
	btCompoundShape *ret = new btCompoundShape();
	for (pair<btTransform, CollisionShapeInflater *> &inf : inflaters) {
		ret->addChildShape(inf.first, inf.second->inflate());
	}
	return ret;
}
CompoundInflater::~CompoundInflater() {
	for (pair<btTransform, CollisionShapeInflater *> &inf : inflaters) {
		delete inf.second;
	}
}
