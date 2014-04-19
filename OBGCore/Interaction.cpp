#include "Interaction.h"

Interaction::Interaction(const btVector3& mousePos, const vector<int> &ids, uint8_t flags) {
	this->mousePos=mousePos;
	this->ids = ids;
	this->flags = flags;
}

Interaction::~Interaction() {
}