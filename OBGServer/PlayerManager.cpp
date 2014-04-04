#include <assert.h>
#include <iostream>
#include <Interaction.h>
#include "PlayerManager.h"
#include "ServerConnection.h"
#include "ServerSocket.h"
#include "Socket.h"

using namespace std;

int PlayerManagerThreadLoop(void *pm) {
	cout << "PM Thread started apparently" << endl;
	((PlayerManager *)pm)->loop();
	cout << "PM Thread ended" << endl;
	return 0;
}

PlayerManager::PlayerManager() :
	socket(),
	active(false),
	thread(PlayerManagerThreadLoop, this),
	udpServer(0xABCD4321, 500, this),
	playersLock()
{

}

bool PlayerManager::start(int time, uint16_t port) {
	active = true;
	if (!socket.open(port)) {
		cout << "Could not open ServerSocket" << endl;
		return false;
	}
	if (!thread.start()) {
		cout << "Could not create client thread" << endl;
		return false;
	}
	if (!udpServer.start(time, port+1)) {
		cout << "Could not start udpServer" << endl;
		return false;
	}
	return true;
}

void PlayerManager::loop() {
	while(active) {
		Socket *sock = socket.getNextConnection();
		if (sock == NULL) {
			active = false;
			break;
		} else if (active) {
			ServerConnection *conn = new ServerConnection(this, sock);
			conn->start();
			addPlayer(conn);
		} else {
			delete sock;
		}
	}
}

void PlayerManager::addPlayer(ServerConnection *player) {
	FunctionLock lock(playersLock);
	players.push_back(player);
	lock.unlock();
	firePlayerJoined(player);
}

void PlayerManager::handlePhysicsUpdate(PhysicsUpdate *physicsUpdate) {
	FunctionLock lock(playersLock);
	for (unsigned int c = 0; c < players.size(); c++) {
		players[c]->sendUpdate(physicsUpdate);
	}
}

void PlayerManager::handleMessage(const string &msg, ServerConnection *sender) {
	fireMessage(msg, sender);
}

void PlayerManager::handleInteraction(Interaction *action) {
	fireInteraction(action);
}

void PlayerManager::handleMessage(const Address &from, int type, uint8_t *data, int len) {
	switch(type) {
	case TYPE_INTERACTION: {
		SerializedInteraction *si = (SerializedInteraction *)data;
		assert(len >=  si->numIds * sizeof(si->ids[0]) + sizeof(si));
		vector<int> ids;
		for (int c = 0; c < si->numIds; c++)
			ids.push_back(si->ids[c]);
		Interaction action(si->mousePos, ids);
		handleInteraction(&action);
		break;
	}
	default:
		cout << "Unknown UDP message type: " << type << endl;
	}
}

bool PlayerManager::handleUnresponsiveClient(const Address &client) {
	//TODO:[MW] identify and remove player
	return false;
}

void PlayerManager::disconnectPlayer(ServerConnection *player) {
	bool found = false;
	FunctionLock lock(playersLock);
	for (auto iter = players.begin(), end = players.end();
			iter != end; ++iter)
	{
		if (*iter == player) {
			found = true;
			players.erase(iter);
			break;
		}
	}
	lock.unlock();
	assert(found);
	firePlayerLeft(player);
	delete player;
}

void PlayerManager::broadcast(string message, ServerConnection *exclude) {
	FunctionLock lock(playersLock);
	for (unsigned int c = 0; c < players.size(); c++) {
		if (players[c] != exclude) {
			players[c]->sendMessage(message);
		}
	}
}

void PlayerManager::close() {
	this->active = false;
	socket.close();
}

PlayerManager::~PlayerManager() {
	if (active) close();

	FunctionLock lock(playersLock);
	for (unsigned int c = 0; c < players.size(); c++) {
		delete players[c];
	}
	players.clear();
}
