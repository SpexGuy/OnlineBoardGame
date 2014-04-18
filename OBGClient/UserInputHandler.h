#pragma once
#include <InteractionEventBroadcaster.h>
#include "MessageEventBroadcaster.h"
#include <stdint.h>

class ChatBox;
class MousePointer;

class UserInputHandler :
	public MessageEventBroadcaster,
	public InteractionEventBroadcaster
{
private:
	ChatBox *chat;
	std::vector<int> heldList;
	std::vector<int> lastHeldList;
	uint8_t rotations;
	MousePointer *pointer;
	bool wireframe;
public:
	UserInputHandler();
	virtual void start();
	virtual void update(int time);
	virtual void keyPressed(int key, int scancode, int mods);
	virtual void keyReleased(int key, int scancode, int mods);
	virtual void mousePressed(int button, int mods);
	virtual void mouseReleased(int button, int mods);
	virtual void mouseMoved(int x, int y);
	virtual ~UserInputHandler();

	inline ChatBox *getChatBox() { return chat; }
	inline MousePointer *getMousePointer() { return pointer; }
};
