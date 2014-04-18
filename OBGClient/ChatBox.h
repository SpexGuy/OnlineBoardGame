#pragma once
#include <CriticalSection.h>
#include <string>
#include <vector>
#include <Renderable.h>
#include "MessageListener.h"
#include "MessageEventBroadcaster.h"

#define NUM_MESSAGES 10
#define INPUT_BOX_SIZE 30

class ChatBox :
	public MessageEventBroadcaster,
	public MessageListener,
	public Renderable
{
private:
	std::string *messages[NUM_MESSAGES];
	CriticalSection inputLock;
	std::vector<char> input;
	bool editMode;

public:
	ChatBox();
	virtual void handleMessage(const std::string &message);
	virtual void pushCharacter(char c);
	virtual void popCharacter();
	virtual void enterEditMode();
	virtual void submit();
	virtual void cancel();
	virtual void render();
	virtual ~ChatBox();
	inline std::string** getMessages() { return messages; };

	inline bool isEditMode() { return editMode; }
};
