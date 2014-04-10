#include <assert.h>
#include <iostream>
#include "Connection.h"
#include "Socket.h"
#include "Thread.h"

#define MAX_PACKET_SIZE 4096

using namespace std;

int ConnectionThreadLoop(void *c) {
	cout << "Socket Thread started apparently" << endl;
	((Connection *)c)->loop();
	cout << "Socket Thread ended" << endl;
	return 0;
}

Connection::Connection(Socket *socket) :
	socket(socket),
	active(false),
	runThread(ConnectionThreadLoop, this)
{
	assert(socket);
}

void Connection::start() {
	active = true;
	if (!runThread.start()) {
		cout << "Could not create client thread" << endl;
		assert(false);
	}
}

void Connection::loop() {
	uint8_t *data = new uint8_t[MAX_PACKET_SIZE];
	uint8_t type;
	while(active) {
		int len = socket->receive(type, data, MAX_PACKET_SIZE);
		if (!active) break;
		if (len >= 0) {
			processData(type, data, len);
		} else {
			cout << "Fatal error in socket read.  Closing loop." << endl;
			handleFatalError();
			active = false;
			break;
		}
	}
	delete[] data;
}

int Connection::sendFile(const std::string &file) {
	return socket->sendFile(file);
}

void Connection::close() {
	this->active = false;
	if (socket != NULL) {
		delete socket;
		socket = NULL;
	}
}

Connection::~Connection() {
	close();
	runThread.waitForTerminate();
}
