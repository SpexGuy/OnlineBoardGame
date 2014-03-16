#include <assert.h>
#include <iostream>
#include <Windows.h>
#include "PlayerManager.h"
#include "ServerConnection.h"
#include "ServerSocket.h"

using namespace std;

PlayerManager::PlayerManager(short port) :
	socket(new ServerSocket(port)),
	active(false)
{

}

DWORD WINAPI PlayerManagerThreadLoop(_In_ LPVOID pm) {
	cout << "PM Thread started apparently" << endl;
	((PlayerManager *)pm)->loop();
	cout << "PM Thread ended" << endl;
	return 0;
}

void PlayerManager::start() {
	active = true;
	DWORD threadId;
	thread = CreateThread(
			NULL,						// default security attributes
			0,							// use default stack size
			PlayerManagerThreadLoop,	// thread function
			this,						// argument to thread function
			0,							// use default creation flags
			&threadId);					// returns the thread identifier
	this->threadId = threadId;
	if (thread == NULL) {
		cout << "Could not create client thread" << endl;
		assert(false);
	}
}

void PlayerManager::loop() {
	while(active) {
		ServerConnection *conn = new ServerConnection(this, socket->getNextConnection());
		conn->start();
		addPlayer(conn);
	}
}

void PlayerManager::addPlayer(ServerConnection *player) {
	//TODO: Synchronize players array
	players.push_back(player);
	firePlayerJoined(player);
}

void PlayerManager::handlePhysicsUpdate(PhysicsUpdate *physicsUpdate) {
	//TODO: Synchronize players
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

void PlayerManager::disconnectPlayer(ServerConnection *player) {
	bool found = false;
	//TODO: Synchronize players array
	for (unsigned int c = 0; c < players.size(); c++) {
		if (players[c] == player) {
			found = true;
			players.erase(players.begin()+c);
			break;
		}
	}
	assert(found);
	firePlayerLeft(player);
	delete player;
}

void PlayerManager::broadcast(string message, ServerConnection *exclude) {
	//TODO: Synchronize players array
	for (unsigned int c = 0; c < players.size(); c++) {
		if (players[c] != exclude) {
			players[c]->sendMessage(message);
		}
	}
}

void PlayerManager::close() {
	this->active = false;
	delete socket;
	socket = NULL;
}

PlayerManager::~PlayerManager() {
	if (active) close();
	//TODO: Serialize players
	for (int c = 0; c < players.size(); c++) {
		delete players[c];
	}
	players.clear();
}
