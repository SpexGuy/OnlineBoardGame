#include <assert.h>
#include <iostream>
#include <Interaction.h>
#include <PhysicsUpdate.h>
#include <Socket.h>
#include "ClientConnection.h"

using namespace std;

ClientConnection::ClientConnection(string ip, short port) :
	Connection(new Socket(ip, port)),
	fileDownloaded(false, false)
{
}

void ClientConnection::processData(const SerialData &data) {
	switch(data.type) {
	case TYPE_FILE:
		cout << "Client got a file: " << data.data << endl;
		fileDownloaded.notify();
		free(data.data);
		break;

	case TYPE_INTERACTION:
		cout << "Client got an interaction...?" << endl;
		free(data.data);
		assert(false);
		break;

	case TYPE_PHYSICS_UPDATE:
		assert(data.size == sizeof(PhysicsUpdate));
		firePhysicsUpdate((PhysicsUpdate *) data.data);
		break;

	case TYPE_MESSAGE:
		cout << "Got message: " << (char *)data.data << endl;
		fireMessage(string((char *) data.data));
		free(data.data);
		break;

	default:
		cout << "Client got an unknown type: " << data.type << endl;
		free(data.data);
		assert(false);
	}
}

void ClientConnection::handleFatalError() {
	cout << "Fatal Connection Error.  Attempting to reconnect." << endl;
	cout << "JK, This program's not that fancy. Goodbye." << endl;
}

void ClientConnection::setUsername(const string &un) {
	//With TCP, socket disconnect is detected by
	//reads. When we switch, this will need to detect.
	socket->sendData(TYPE_SET_USERNAME, un.c_str(), un.length()+1);
}

void ClientConnection::handleInteraction(Interaction *action) {
	//allocate SerializedInteraction with enough ints at the end
	int size = sizeof(SerializedInteraction) + action->ids.size()*sizeof(int);
	struct SerializedInteraction *serial = (SerializedInteraction *)malloc(size);
	serial->mousePos = action->mousePos;
	serial->numIds = action->ids.size();
	for (unsigned int c = 0; c < action->ids.size(); c++) {
		serial->ids[c] = action->ids[c];
	}
	socket->sendData(TYPE_INTERACTION, serial, size);
	free(serial);
}

void ClientConnection::handleMessage(const string &message) {
	socket->sendData(TYPE_MESSAGE, message.c_str(), message.length() + 1);
}

void ClientConnection::downloadFile(const string &filename) {
	//ensure not set from async load
	fileDownloaded.reset();
	socket->sendData(TYPE_FILE_REQUEST, filename.c_str(), filename.length() + 1);
	//wait for file to download
	cout << "waiting for file to download" << endl;
	fileDownloaded.wait();
	cout << "fileDownloaded triggered" << endl;
}

ClientConnection::~ClientConnection() {
	fileDownloaded.notify();
}
