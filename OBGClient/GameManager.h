#pragma once
#include "Address.h"

class GraphicsManager;
class ClientEntityManager;
class ClientConnection;
class UserInputHandler;

class GameManager {
protected:
	GraphicsManager		*graphicsManager;
	ClientEntityManager	*entityManager;
	ClientConnection	*connection;
	UserInputHandler	*inputHandler;

	Address serverIp;

	bool connected;

public:
	static GameManager *inst();

	GameManager(int argc, char *argv[]);
	virtual void run();
	virtual void update(int time);
	virtual void display();
	virtual void keyPressed(int key, int scancode, int mods);
	virtual void keyReleased(int key, int scancode, int mods);
	virtual void mousePressed(int button, int mods);
	virtual void mouseReleased(int button, int mods);
	virtual void mouseMoved(int x, int y);
	virtual ~GameManager();

	//accessors
	inline GraphicsManager		*getGraphicsManager()	{ return graphicsManager;	}
	inline ClientEntityManager	*getEntityManager()		{ return entityManager;		}
	inline ClientConnection		*getConnection()		{ return connection;		}
	inline UserInputHandler		*getInputHandler()		{ return inputHandler;		}
};
