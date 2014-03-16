#include <assert.h>
#include <iostream>
#include <string>
#include <Socket.h>
#include "GameManager.h"

using namespace std;

int main(int argc, char *argv[]) {

	// socket test
	//cout << "Init socket" << endl;
	//SocketInit();
	//cout << "Connecting to Server" << endl;
	//Socket *conn = new Socket("127.0.0.1", 0xABC0);
	//cout << "Connected! Sending data" << endl;
	//conn->sendData(5, "Hello, Server!", 15);
	//cout << "Data Sent. Waiting for response." << endl;
	//SerialData data = conn->receive();
	//cout << "Response received! Validating..." << endl;
	//assert(data.type == 4);
	//assert(data.size == 15);
	//assert(string((char *)data.data) == string("Hello, Client!"));
	//cout << "Data valid! Cleaning up." << endl;
	//delete conn;
	//SocketClose();
	//cout << "Cleanup complete" << endl;


	GameManager *gm = new GameManager(argc, argv);
	gm->run();
	cout << "Enter any button to close" << endl;
	char i;
	cin >> i;
	delete gm;
	return 0;
}
