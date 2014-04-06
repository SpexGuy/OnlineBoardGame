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
	assert(file);
	Json::Value root;
	Json::Reader reader;
	if (!reader.parse(file, root)) {
		cout << "Could not parse JSON file" << endl;
		file.close();
		assert(false);
	}
	file.close();
	AssetPack *pack = new AssetPack(root);
	vector<Entity *> entities = pack->loadGame();
	assert(entities.size() > 0);
	for (Entity *entity : entities) {
		entityManager->addEntity(entity);
	}

	int time = clock();
	entityManager->start();
	cout << "Waiting for Connection" << endl;
	playerManager->start(time, 0xABC0);

	while(running) {
		time = clock();
		entityManager->update();
		entityManager->createPhysicsUpdates();
		playerManager->update(time);
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
