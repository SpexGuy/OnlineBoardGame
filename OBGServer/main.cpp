#include <assert.h>
#include <iostream>
#include <string>
#include <Socket.h>
#include "GameManager.h"
#include "MessageBroadcaster.h"
#include "PlayerListener.h"
#include "PlayerManager.h"
#include "PlayerStatusBroadcaster.h"
#include "ServerConnection.h"
#include "ServerSocket.h"

using namespace std;

int main(int argc, char *argv[]) {
	// socket test
	PlayerStatusBroadcaster *psb = new PlayerStatusBroadcaster();
	MessageBroadcaster *mb = new MessageBroadcaster();
	cout << "Init socket" << endl;
	SocketInit();
	cout << "Create Server Socket" << endl;
	PlayerManager *server = new PlayerManager(0xABC0);
	server->registerPlayerListener(psb);
	server->registerServerMessageListener(mb);
	cout << "Waiting for Connection" << endl;
	server->start();
	system("pause");
	server->close();
	cout << "Server closed" << endl;
	delete server;
	cout << "Server deleted" << endl;
	SocketClose();
	cout << "All closed up" << endl;
	delete psb;
	delete mb;

	GameManager *gm = new GameManager();
	gm->run();
	delete gm;
	system("pause");

	return 0;
}
