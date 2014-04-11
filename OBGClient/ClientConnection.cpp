#include <assert.h>
#include <iostream>
#include <Interaction.h>
#include <PhysicsUpdate.h>
#include <Socket.h>
#include "ClientConnection.h"

using namespace std;

ClientConnection::ClientConnection() :
	Connection(new Socket()),
	fileDownloaded(false, false),
	udpClient(0xABCD4321, 500, this)
{}

bool ClientConnection::connect(int time, const Address &server, int localPort) {
	if (!socket->open(server, localPort))
		return false;
	if (!udpClient.start(time, server, localPort+1))
		return false;
	return true;
}

void ClientConnection::update(int time) {
	udpClient.update(time);
}

void ClientConnection::handleMessage(int type, uint8_t *data, int len) {
	switch(type) {
	case TYPE_PHYSICS_UPDATE:
		assert(len == sizeof(PhysicsUpdate));
		firePhysicsUpdate((PhysicsUpdate *) data);
		break;
	default:
		cout << "Unknown UDP type: " << type;
	}
}

void ClientConnection::processData(uint8_t type, const uint8_t *data, uint16_t size) {
	switch(type) {
	case TYPE_FILE:
		cout << "Client got a file: " << (char *)data << endl;
		fileDownloaded.notify();
		break;

	case TYPE_INTERACTION:
		cout << "Client got an interaction...? Through TCP?!" << endl;
		assert(false);
		break;

	case TYPE_PHYSICS_UPDATE:
		cout << "Got physics update through TCP?" << endl;
		assert(size == sizeof(PhysicsUpdate));
		firePhysicsUpdate((PhysicsUpdate *) data);
		break;

	case TYPE_MESSAGE:
		cout << "Got message: " << (char *)data << endl;
		fireMessage(string((char *) data));
		break;

	default:
		cout << "Client got an unknown type: " << type << endl;
	}
}

void ClientConnection::handleFatalError() {
	cout << "Fatal Connection Error.  Attempting to reconnect." << endl;
	cout << "JK, This program's not that fancy. Goodbye." << endl;
}

void ClientConnection::setUsername(const string &un) {
	socket->sendData(TYPE_SET_USERNAME, (const uint8_t *)un.c_str(), un.length()+1);
}

void ClientConnection::handleInteraction(Interaction *action) {
	//allocate SerializedInteraction with enough ints at the end
	int size = sizeof(SerializedInteraction) + action->ids.size()*sizeof(int);
	struct SerializedInteraction *serial = (SerializedInteraction *)alloca(size);
	serial->mousePos = action->mousePos;
	serial->numIds = action->ids.size();
	serial->rotations = action->rotations;
	for (unsigned int c = 0; c < action->ids.size(); c++) {
		serial->ids[c] = action->ids[c];
	}
	udpClient.sendPacket(TYPE_INTERACTION, size, (uint8_t *)serial);
}

void ClientConnection::handleMessage(const string &message) {
	socket->sendData(TYPE_MESSAGE, (const uint8_t *)message.c_str(), message.length() + 1);
}

void ClientConnection::downloadFile(const string &filename) {
	//ensure not set from async load
	fileDownloaded.reset();
	socket->sendData(TYPE_FILE_REQUEST, (const uint8_t *)filename.c_str(), filename.length() + 1);
	//wait for file to download
	cout << "waiting for file to download" << endl;
	fileDownloaded.wait();
	cout << "fileDownloaded triggered" << endl;
}

ClientConnection::~ClientConnection() {
	fileDownloaded.notify();
}
