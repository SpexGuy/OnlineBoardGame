#include <assert.h>
#include <iostream>
#include <Socket.h>
#include <Windows.h>
#include <Interaction.h>
#include <PhysicsUpdate.h>
#include "ServerConnection.h"
#include "PlayerManager.h"

using namespace std;

ServerConnection::ServerConnection(PlayerManager *owner, Socket *sock) :
	Connection(sock), owner(owner)
{

}

void ServerConnection::processData(const SerialData &data) {
	switch(data.type) {
	case TYPE_PHYSICS_UPDATE:
		cout << "...The server just got a physics update.  WTF?!" << endl;
		assert(false);
		break;
	case TYPE_INTERACTION:
		assert(data.size == sizeof(Interaction));
		owner->handleInteraction((Interaction *) data.data);
		break;
	case TYPE_MESSAGE:
		owner->handleMessage(string((char *)data.data), this);
		break;
	default:
		cout << "The server just got an unknown message type: " << data.type << endl;
		assert(false);
	}
}

bool ServerConnection::isUnresponsive() {
	//TODO:[MW] connection is unresponsive
	return false;
}

void ServerConnection::sendUpdate(PhysicsUpdate *update) {
	//TODO: serialize if necessary
	socket->sendData(TYPE_PHYSICS_UPDATE, update, sizeof(PhysicsUpdate));
}

void ServerConnection::sendMessage(const string &msg) {
	socket->sendData(TYPE_MESSAGE, msg.c_str(), msg.length() + 1);
}

ServerConnection::~ServerConnection() {

}
