#include <assert.h>
#include <iostream>
#include <Windows.h>
#include "Connection.h"
#include "Socket.h"

using namespace std;

Connection::Connection(Socket *socket) :
	socket(socket), active(false)
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
		processData(data);
	}
}

void Connection::close() {
	this->active = false;
	delete socket;
	socket = NULL;
}

Connection::~Connection() {
	if (socket != NULL)
		close();
}
