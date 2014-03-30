#include "Interaction.h"

Interaction::Interaction(const btVector3& mousePos, const vector<int> &ids) {
	this->mousePos=mousePos;
	this->ids = ids;
}

Interaction::~Interaction() {
}