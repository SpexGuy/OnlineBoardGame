#pragma once
#include <vector>
#include "btBulletDynamicsCommon.h"
#include <stdint.h>

#define ROT_POS_X 1 << 7
#define ROT_NEG_X 1 << 6
#define ROT_POS_Y 1 << 4
#define ROT_NEG_Y 1 << 3
#define ROT_POS_Z 1 << 1
#define ROT_NEG_Z 1 << 0

using std::vector;
class Interaction {
private:
	Interaction();
public:
	uint8_t rotations;
	vector<int> ids;
	btVector3 mousePos;
	Interaction(const btVector3& mousePos, const vector<int> &ids, uint8_t rotations);
	~Interaction();
};

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
 *	ClientConnection::handleInteraction(..)
 *	and the deserialization code in
 *	ServerConnection::processData(..).
 */
#pragma warning(disable:4200)
struct SerializedInteraction {
	btVector3 mousePos;
	int numIds;
	uint8_t rotations;
	int ids[0];
};
#pragma warning(default:4200)
