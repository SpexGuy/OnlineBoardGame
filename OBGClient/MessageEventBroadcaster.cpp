#include "MessageEventBroadcaster.h"
#include "MessageListener.h"

using namespace std;

void MessageEventBroadcaster::
	fireMessage(const string &msg)
{
	for (unsigned int c = 0; c < listeners.size(); c++) {
		listeners[c]->handleMessage(msg);
	}
}

void MessageEventBroadcaster::
	registerMessageListener(MessageListener *listener)
{
	listeners.push_back(listener);
}

