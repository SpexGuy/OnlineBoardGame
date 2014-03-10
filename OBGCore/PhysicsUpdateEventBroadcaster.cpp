#include "PhysicsUpdateEventBroadcaster.h"
#include "PhysicsUpdateListener.h"

using namespace std;

void PhysicsUpdateEventBroadcaster::
	fire(PhysicsUpdate *u)
{
	for (unsigned int c = 0; c < listeners.size(); c++) {
		listeners[c]->handlePhysicsUpdate(u);
	}
}

void PhysicsUpdateEventBroadcaster::
	registerPhysicsUpdateListener(PhysicsUpdateListener *listener)
{
	listeners.push_back(listener);
}

