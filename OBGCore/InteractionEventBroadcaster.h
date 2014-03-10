#pragma once
#include <vector>

class InteractionListener;
class Interaction;

class InteractionEventBroadcaster {
private:
	std::vector<InteractionListener *> listeners;

protected:
	void fireInteraction(Interaction *update);

public:
	InteractionEventBroadcaster() {};
	virtual void registerInteractionListener(InteractionListener *listener);
	virtual ~InteractionEventBroadcaster() {};
};
