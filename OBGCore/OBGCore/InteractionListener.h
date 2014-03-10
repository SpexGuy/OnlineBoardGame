#pragma once

class Interaction;

class InteractionListener {
public:
	virtual void handleInteraction(Interaction *action) = 0;

	virtual ~InteractionListener() {}
};
