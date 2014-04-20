#include <assert.h>
#include <iostream>
#include <Interaction.h>
#include <PhysicsUpdate.h>
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
	players[player->getPeer()] = player;
	udpServer.addClient(player->getPeer());
	lock.unlock();
	firePlayerJoined(player);
}

void PlayerManager::update(int time) {
	udpServer.update(time);
}

void PlayerManager::handlePhysicsUpdate(PhysicsUpdate *physicsUpdate) {
	FunctionLock lock(playersLock);
	int len = physicsUpdate->serialByteSize();
	SerialPhysicsUpdate *spu = (SerialPhysicsUpdate *)alloca(len);
	physicsUpdate->fill(spu);
	for (auto &player : players) {
		udpServer.sendPacket(player.first, TYPE_PHYSICS_UPDATE, len, (uint8_t *)spu);
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
		Interaction action(si->mousePos, ids, si->flags);
		handleInteraction(&action);
		break;
	}
	default:
		cout << "Unknown UDP message type: " << type << endl;
	}
}

bool PlayerManager::handleUnresponsiveClient(const Address &client) {
	auto iter = players.find(client);
	if (iter != players.end()) {
		disconnectPlayer(iter->second);
	}
	return true;
}

void PlayerManager::disconnectPlayer(ServerConnection *player) {
	bool found = false;
	FunctionLock lock(playersLock);
	for (auto iter = players.begin(), end = players.end();
			iter != end; ++iter)
	{
		if (iter->second == player) {
			found = true;
			iter = players.erase(iter);
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
	for (auto &player : players) {
		if (player.second != exclude) {
			player.second->sendMessage(message);
		}
	}
}

void PlayerManager::close() {
	this->active = false;
	socket.close();
	udpServer.stop();
}

PlayerManager::~PlayerManager() {
	if (active) close();

	FunctionLock lock(playersLock);
	for (auto &player : players) {
		delete player.second;
	}
	players.clear();
}
