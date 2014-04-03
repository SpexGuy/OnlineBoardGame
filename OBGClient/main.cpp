#include <assert.h>
#include <iostream>
#include <Socket.h>
#include <UDPConnection.h>
#include <Thread.h>
#include "ClientConnection.h"
#include "GameManager.h"

using namespace std;

UDPClient *client;
UDPServer *server;

int testRoutine0(void *param) {
	bool connection = client->connect(Address(0x7F000001, 0xABC0));
	cout << "Connected: " << connection << endl;
	return connection;
}

int testRoutine1(void *param) {
	int type;
	uint8_t data[100];
	while(client->isRunning()) {
		int len = client->receivePacket(type, 100, data);
		cout << "Client got packet of size " << len << " and type " << type << endl;
	}
	return 0;
}

int testRoutine2(void *param) {
	Address source;
	int type;
	uint8_t data[100];
	while(true) {
		int len = server->receivePacket(source, type, 100, data);
		cout << "Server got packet of size " << len << " and type " << type << endl;
	}
	return 0;
}

int main(int argc, char *argv[]) {
	SocketInit();
	client = new UDPClient(0xABCD4321, 100);
	server = new UDPServer(0xABCD4321, 100);
	server->start(0,0xABC0);
	client->start(500);
	Thread *connect = new Thread(testRoutine0, NULL);
	Thread *clientread = new Thread(testRoutine1, NULL);
	Thread *serverread = new Thread(testRoutine2, NULL);
	connect->start();
	serverread->start();
	clientread->start();
	int time = 0;
	for (int c = 0; c < 10; c++) {
		sleep(5);
		time += 5;
		server->update(time);
		client->update(500 + time);
		server->broadcastPacket(TYPE_CONNECT_DECLINE, 0, NULL);
		client->sendPacket(TYPE_CONNECT_REQUEST, 0, NULL);
	}
	SocketClose();
	cout << "Test Finished" << endl;
	system("pause");

	//GameManager *gm = new GameManager(argc, argv);
	//gm->run();
	//system("pause");
	//delete gm;
	//return 0;
}
