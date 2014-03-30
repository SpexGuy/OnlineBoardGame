#pragma once
#include <vector>
#include "btBulletDynamicsCommon.h"

using std::vector;
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
class Interaction {
private:
	Interaction();
public:
	vector<int> ids;
	btVector3 mousePos;
	Interaction(const btVector3& mousePos, const vector<int> &ids);
	~Interaction();
};

struct SerializedInteraction {
	btVector3 mousePos;
	int numIds;
	int ids[0];
};
