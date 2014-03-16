#include <assert.h>
#include <iostream>
#include <Interaction.h>
#include <PhysicsUpdate.h>
#include <Socket.h>
#include "ClientConnection.h"

using namespace std;

ClientConnection::ClientConnection(string ip, short port) :
	Connection(new Socket(ip, port))
{
}

void ClientConnection::processData(const SerialData &data) {
	switch(data.type) {
	case TYPE_INTERACTION:
		cout << "Client got an interaction...?" << endl;
		assert(false);
		break;

	case TYPE_PHYSICS_UPDATE:
		assert(data.size == sizeof(PhysicsUpdate));
		firePhysicsUpdate((PhysicsUpdate *) data.data);
		break;

	case TYPE_MESSAGE:
		fireMessage(string((char *) data.data));
		cout << "Got message: " << (char *)data.data << endl;
		break;

	default:
		cout << "Client got an unknown type: " << data.type << endl;
		assert(false);
	}
}

void ClientConnection::handleInteraction(Interaction *action) {
	socket->sendData(TYPE_INTERACTION, action, sizeof(*action));
}

void ClientConnection::handleMessage(const string &message) {
	socket->sendData(TYPE_MESSAGE, message.c_str(), message.length() + 1);
}

ClientConnection::~ClientConnection() {
	
}
