#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>
#include <AssetPack.h>
#include <EntityManager.h>
#include <json.h>
#include <Socket.h>
#include "GameManager.h"
#include "MessageBroadcaster.h"
#include "PlayerListener.h"
#include "PlayerManager.h"
#include "PlayerStatusBroadcaster.h"
#include "ServerConnection.h"
#include "ServerSocket.h"

using namespace std;

GameManager::GameManager() {
	cout << "Init socket" << endl;
	SocketInit();
	cout << "Create Server Socket" << endl;
	playerManager = new PlayerManager();
	entityManager = new EntityManager();

	playerManager->registerPlayerListener(new PlayerStatusBroadcaster());
	playerManager->registerServerMessageListener(new MessageBroadcaster());
	playerManager->registerInteractionListener(entityManager);
	entityManager->registerPhysicsUpdateListener(playerManager);
}

void GameManager::run() {
	running = true;

	ifstream file("assets.json");
	if (!file) {
		cout << "Could not open assets.json" << endl;
		return;
	}
	Json::Value root;
	Json::Reader reader;
	if (!reader.parse(file, root)) {
		cout << "Could not parse JSON file" << endl;
		file.close();
		return;
	}
	file.close();
	AssetPack *pack = new AssetPack();
	if (!pack->initialize(root)) {
		cout << "Invalid assets.json header" << endl;
		return;
	}
	vector<Entity *> entities;
	if (!pack->loadGame(entities)) {
		cout << "Failed to load entities" << endl;
		return;
	}
	for (Entity *entity : entities) {
		entityManager->addEntity(entity);
	}

	int time = clock();
	entityManager->start();
	cout << "Waiting for Connection" << endl;
	playerManager->start(time, 0xABC0);

	while(running) {
		time = clock();
		entityManager->update(time);
		entityManager->createPhysicsUpdates();
		playerManager->update(time);
		sleep(max((1000/60) - int(clock() - time), 0));
	}
	playerManager->close();
	cout << "Server closed" << endl;
}

GameManager::~GameManager() {
	delete playerManager;
	cout << "Server deleted" << endl;
	SocketClose();
	cout << "All closed up" << endl;
}
