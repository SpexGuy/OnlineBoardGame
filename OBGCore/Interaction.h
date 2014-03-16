#pragma once

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
	~Interaction();
public:
	//TODO:[MW]
};
