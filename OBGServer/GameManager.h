#pragma once

class EntityManager;
class PlayerManager;
class ServerInputManager;

class GameManager {
protected:
	EntityManager *entityManager;
	PlayerManager *playerManager;
	ServerInputManager *inputManager;
public:
	GameManager();
	virtual void run();
	virtual ~GameManager();
};
