#include "InteractionEventBroadcaster.h"
#include "InteractionListener.h"

using namespace std;

void InteractionEventBroadcaster::
	fireInteraction(Interaction *u)
{
	for (unsigned int c = 0; c < listeners.size(); c++) {
		listeners[c]->handleInteraction(u);
	}
}

void InteractionEventBroadcaster::
	registerInteractionListener(InteractionListener *listener)
{
	listeners.push_back(listener);
}

