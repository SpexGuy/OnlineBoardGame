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

	bool running;
	bool visible;
	bool connected;

public:
	static GameManager *inst();

	GameManager(int argc, char *argv[]);
	virtual void run();
	virtual void update();
	virtual void display();
	virtual void keyPressed(unsigned char c, int x, int y);
	virtual void keyReleased(unsigned char c, int x, int y);
	virtual void specialKeyPressed(int k, int x, int y);
	virtual void mousePressed(int button, int state, int x, int y);
	virtual void mouseDragged(int x, int y);
	virtual void mouseMoved(int x, int y);
	virtual void visibilityChanged(int state);
	virtual void reshape(int x, int y);
	virtual void close();
	virtual ~GameManager();

	//accessors
	inline GraphicsManager		*getGraphicsManager()	{ return graphicsManager;	}
	inline ClientEntityManager	*getEntityManager()		{ return entityManager;		}
	inline ClientConnection		*getConnection()		{ return connection;		}
	inline UserInputHandler		*getInputHandler()		{ return inputHandler;		}
};
