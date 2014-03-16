#include <assert.h>
#include <iostream>
#include <string>
#include <Socket.h>
#include "GameManager.h"
#include "ServerSocket.h"

using namespace std;

int main(int argc, char *argv[]) {
	GameManager *gm = new GameManager();
	gm->run();
	delete gm;

	// socket test
	//cout << "Init socket" << endl;
	//SocketInit();
	//cout << "Create Server Socket" << endl;
	//ServerSocket *server = new ServerSocket(0xABC0);
	//cout << "Waiting for Connection" << endl;
	//Socket *connection = server->getNextConnection();
	//cout << "Connection! Waiting for data" << endl;
	//SerialData data = connection->receive();
	//cout << "Got Data! Validating..." << endl;
	//assert(data.type == 5);
	//assert(data.size == 15);
	//assert(string((char *)data.data) == string("Hello, Server!"));
	//cout << "Data Valid! Sending response..." << endl;
	//connection->sendData(4, "Hello, Client!", 15);
	//cout << "Data sent. Cleaning up." << endl;
	//delete connection;
	//delete server;
	//SocketClose();
	//cout << "Cleanup done" << endl;

	return 0;
}
