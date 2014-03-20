#include <assert.h>
#include <iostream>
#include <Windows.h>
#include "Connection.h"
#include "Socket.h"

using namespace std;

Connection::Connection(Socket *socket) :
	socket(socket),
	active(false),
	loopEnded(true)
{
	
}

DWORD WINAPI ConnectionThreadLoop(_In_ LPVOID c) {
	cout << "Socket Thread started apparently" << endl;
	((Connection *)c)->loop();
	cout << "Socket Thread ended" << endl;
	return 0;
}

void Connection::start() {
	active = true;
	loopEnded = false;
	DWORD threadId;
	thread = CreateThread(
			NULL,					// default security attributes
			0,						// use default stack size
			ConnectionThreadLoop,	// thread function
			this,					// argument to thread function
			0,						// use default creation flags
			&threadId);				// returns the thread identifier
	this->threadId = threadId;
	if (thread == NULL) {
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
	loopEnded = true;
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
	while(!loopEnded)
		;
}
