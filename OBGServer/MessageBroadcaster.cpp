#include "MessageBroadcaster.h"
#include "PlayerManager.h"
#include "ServerConnection.h"

using namespace std;

void MessageBroadcaster::handleMessage(const string &msg, ServerConnection *conn) {
	conn->getManager()->broadcast(conn->getName().append(": ").append(msg), conn);
}
