#include <assert.h>
#include <iostream>
#include "Connection.h"
#include "Socket.h"
#include "Thread.h"

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
	while(active) {
		SerialData data = socket->receive();
		if (data.data != NULL) {
			processData(data);
		} else {
			cout << "Fatal error in socket read.  Closing loop." << endl;
			if (active) {
				handleFatalError();
				active = false;
			}
			break;
		}
	}
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
