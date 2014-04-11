#include "Interaction.h"

Interaction::Interaction(const btVector3& mousePos, const vector<int> &ids, uint8_t rotations) {
	this->mousePos=mousePos;
	this->ids = ids;
	this->rotations = rotations;
}

Interaction::~Interaction() {
}