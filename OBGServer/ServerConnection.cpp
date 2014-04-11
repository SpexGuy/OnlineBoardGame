#include <assert.h>
#include <iostream>
#include <Socket.h>
#include <Interaction.h>
#include <PhysicsUpdate.h>
#include "ServerConnection.h"
#include "PlayerManager.h"

using namespace std;

ServerConnection::ServerConnection(PlayerManager *owner, Socket *sock) :
	Connection(sock), owner(owner)
{

}

void ServerConnection::processData(uint8_t type, const uint8_t *data, uint16_t len) {
	string message;
	switch(type) {
	case TYPE_FILE:
		cout << "Server received a file?  " << (char *)data << endl;
		break;

	case TYPE_FILE_REQUEST:
		cout << "Received a file request for: " << (char *)data << endl;
		socket->sendFile(string((char *)data));
		break;

	case TYPE_PHYSICS_UPDATE:
		cout << "...The server just got a physics update.  WTF?!" << endl;
		assert(false);
		break;

	case TYPE_INTERACTION: {
		SerializedInteraction *serial = (SerializedInteraction *)data;
		assert(len == sizeof(SerializedInteraction) + serial->numIds*sizeof(int));
		vector<int> ids(serial->numIds);
		for (int c = 0; c < serial->numIds; c++) {
			ids[c] = serial->ids[c];
		}
		Interaction action(serial->mousePos, ids, serial->rotations);
		owner->handleInteraction(&action);
		break;
	}
	case TYPE_MESSAGE:
		owner->handleMessage(string((char *)data), this);
		break;

	case TYPE_SET_USERNAME:
		message = name;
		name = string((char *)data);
		message = message.append(" changed his username to ").append(name);
		owner->broadcast(message, NULL);
		break;

	default:
		cout << "The server just got an unknown message type: " << type << endl;
		assert(false);
	}
}

void ServerConnection::handleFatalError() {
	owner->disconnectPlayer(this);
}

void ServerConnection::sendUpdate(PhysicsUpdate *update) {
	//With TCP, socket disconnect is detected by
	//reads. When we switch, this will need to detect.
	int sent = socket->sendData(TYPE_PHYSICS_UPDATE, (const uint8_t *)update, sizeof(PhysicsUpdate));
	if (sent != 0) {
		handleFatalError();
	}
}

void ServerConnection::sendMessage(const string &msg) {
	int sent = socket->sendData(TYPE_MESSAGE, (const uint8_t *)msg.c_str(), msg.length() + 1);
	if (sent != 0)
		handleFatalError();
}

ServerConnection::~ServerConnection() {

}
