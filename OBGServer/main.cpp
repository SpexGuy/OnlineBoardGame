#include <assert.h>
#include <iostream>
#include <string>
#include <Socket.h>
#include "GameManager.h"
#include "ServerConnection.h"
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
	//ServerConnection *conn = new ServerConnection(NULL, server->getNextConnection());
	//cout << "Connection! Starting thread" << endl;
	//conn->start();
	//system("pause");
	//conn->sendMessage("Hello, ppls!");
	//cout << "Sent message" << endl;
	//system("pause");
	//conn->close();
	//delete conn;
	//delete server;
	//SocketClose();
	//cout << "All closed up" << endl;

	return 0;
}
