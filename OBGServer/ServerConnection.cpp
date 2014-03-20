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
	string message;
	switch(data.type) {

	case TYPE_FILE:
		cout << "Server received a file?  " << (char *)data.data << endl;
		free(data.data);
		break;

	case TYPE_FILE_REQUEST:
		cout << "Received a file request for: " << (char *)data.data << endl;
		socket->sendFile(string((char *)data.data));
		free(data.data);
		break;

	case TYPE_PHYSICS_UPDATE:
		cout << "...The server just got a physics update.  WTF?!" << endl;
		free(data.data);
		assert(false);
		break;

	case TYPE_INTERACTION:
		assert(data.size == sizeof(Interaction));
		owner->handleInteraction((Interaction *) data.data);
		break;

	case TYPE_MESSAGE:
		owner->handleMessage(string((char *)data.data), this);
		free(data.data);
		break;

	case TYPE_SET_USERNAME:
		message = name;
		name = string((char *)data.data);
		free(data.data);
		message = message.append(" changed his username to ").append(name);
		owner->broadcast(message, NULL);
		break;

	default:
		cout << "The server just got an unknown message type: " << data.type << endl;
		free(data.data);
		assert(false);
	}
}

void ServerConnection::handleFatalError() {
	owner->disconnectPlayer(this);
}

bool ServerConnection::isUnresponsive() {
	//TODO:[MW] connection is unresponsive
	return false;
}

void ServerConnection::sendUpdate(PhysicsUpdate *update) {
	//With TCP, socket disconnect is detected by
	//reads. When we switch, this will need to detect.
	socket->sendData(TYPE_PHYSICS_UPDATE, update, sizeof(PhysicsUpdate));
}

void ServerConnection::sendMessage(const string &msg) {
	socket->sendData(TYPE_MESSAGE, msg.c_str(), msg.length() + 1);
}

ServerConnection::~ServerConnection() {

}
