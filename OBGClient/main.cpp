#include <assert.h>
#include <iostream>
#include <sstream>
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
	////input loop
	//string input;
	//while(true) {
	//	getline(cin, input);
	//	if (input.rfind("/", 0) == 0) {
	//		if (input.rfind("/exit", 0) == 0)
	//			break;
	//		if (input.rfind("/user ", 0) == 0) {
	//			string un = input.substr(6, string::npos);
	//			conn->setUsername(un);
	//		}
	//	} else {
	//		conn->handleMessage(input);
	//	}
	//}
	//delete conn;
	//SocketClose();
	//cout << "All Done" << endl;

	GameManager *gm = new GameManager(argc, argv);
	gm->run();
	system("pause");
	delete gm;
	return 0;
}
