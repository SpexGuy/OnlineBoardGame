#include "ServerMessageEventBroadcaster.h"
#include "ServerMessageListener.h"

using namespace std;

void ServerMessageEventBroadcaster::
	fireMessage(const string &msg, Connection *player)
{
	for (unsigned int c = 0; c < listeners.size(); c++) {
		listeners[c]->handleMessage(msg, player);
	}
}

void ServerMessageEventBroadcaster::
	registerServerMessageListener(ServerMessageListener *listener)
{
	listeners.push_back(listener);
}

