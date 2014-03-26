#include "Interaction.h"

Interaction::Interaction(const btVector3& mousePos, vector<int> ids) {
	this->mousePos=mousePos;
	this->ids = ids;
}

Interaction::~Interaction() {
}