#include <assert.h>
#include <iostream>
#include <string>
#include <Socket.h>
#include "ClientConnection.h"
#include "GameManager.h"

using namespace std;

int main(int argc, char *argv[]) {

	// socket test
	//cout << "Init socket" << endl;
	//SocketInit();
	//cout << "Connecting to Server" << endl;
	//ClientConnection *conn = new ClientConnection("127.0.0.1", 0xABC0);
	//conn->start();
	//system("pause");
	//conn->handleMessage("Hello, Server!");
	//system("pause");
	//delete conn;
	//SocketClose();
	//cout << "All Done" << endl;

	GameManager *gm = new GameManager(argc, argv);
	gm->run();
	system("pause");
	delete gm;
	return 0;
}
