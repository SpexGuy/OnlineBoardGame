#include <assert.h>
#include <iostream>
#include <Socket.h>
#include <UDPConnection.h>
#include <Thread.h>
#include "ClientConnection.h"
#include "GameManager.h"

using namespace std;

int main(int argc, char *argv[]) {
	GameManager *gm = new GameManager(argc, argv);
	gm->run();
	delete gm;
	system("pause");
	return 0;
}
