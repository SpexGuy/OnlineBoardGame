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

class FileDownloadTest : public PlayerListener {
public:
	virtual void playerJoined(ServerConnection *conn);
	virtual void playerLeft(ServerConnection *conn);
};

void FileDownloadTest::playerJoined(ServerConnection *conn) {
	conn->sendFile("badEarth.obj");
}

void FileDownloadTest::playerLeft(ServerConnection *conn) {
}

int main(int argc, char *argv[]) {
	// socket test
	//PlayerStatusBroadcaster *psb = new PlayerStatusBroadcaster();
	//MessageBroadcaster *mb = new MessageBroadcaster();
	//FileDownloadTest *fdt = new FileDownloadTest();
	//cout << "Init socket" << endl;
	//SocketInit();
	//cout << "Create Server Socket" << endl;
	//PlayerManager *server = new PlayerManager(0xABC0);
	//server->registerPlayerListener(psb);
	//server->registerPlayerListener(fdt);
	//server->registerServerMessageListener(mb);
	//cout << "Waiting for Connection" << endl;
	//server->start();
	//system("pause");
	//server->close();
	//cout << "Server closed" << endl;
	//delete server;
	//cout << "Server deleted" << endl;
	//SocketClose();
	//cout << "All closed up" << endl;
	//delete psb;
	//delete mb;
	//delete fdt;

	GameManager *gm = new GameManager();
	gm->run();
	delete gm;
	system("pause");

	return 0;
}
