#include <iostream>
#include "ServerConnection.h"
#include "PlayerManager.h"
#include "PlayerStatusBroadcaster.h"

using namespace std;

void PlayerStatusBroadcaster::playerJoined(ServerConnection *player) {
	cout << "Player joined" << endl;
	player->getManager()->broadcast("A player joined the game!", NULL);
}

void PlayerStatusBroadcaster::playerLeft(ServerConnection *player) {
	cout << "Player left" << endl;
	player->getManager()->broadcast("A player left the game", player);
}
