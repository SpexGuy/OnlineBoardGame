#include <GraphicsContext.h>
#include "ChatBox.h"

#define FONT_SIZE 10
#define LINE_SIZE 22
#define X_OFFSET 5
#define Y_OFFSET 8

using namespace glm;
using namespace std;

ChatBox::ChatBox() {
	//fill messages with NULL
	memset(messages, 0, sizeof(messages));
	editMode = false;
}

void ChatBox::handleMessage(const string &msg) {
	//delete dropped message
	if (messages[NUM_MESSAGES-1] != NULL)
		delete messages[NUM_MESSAGES-1];
	//advance all messages
	for (int c = NUM_MESSAGES-1; c > 0; c--) {
		messages[c] = messages[c-1];
	}
	//add new message
	messages[0] = new std::string(msg);
}

void ChatBox::pushCharacter(char c) {
	FunctionLock lock(inputLock);
	if (input.size() < INPUT_BOX_SIZE)
		input.push_back(c);
}

void ChatBox::popCharacter() {
	FunctionLock lock(inputLock);
	if (input.size() > 0)
		input.pop_back();
}

void ChatBox::enterEditMode() {
	editMode = true;
}

void ChatBox::submit() {
	assert(editMode);
	editMode = false;
	FunctionLock lock(inputLock);
		if (input.size() == 0) return;
		input.push_back('\0');
		string str(&input[0]);
		input.clear();
	lock.unlock();
	fireMessage(str);
	handleMessage(string("me: ").append(str));
}

void ChatBox::cancel() {
	editMode = false;
	FunctionLock lock(inputLock);
	input.clear();
}

void ChatBox::render() {
	float x = X_OFFSET;
	float y = Y_OFFSET;
	if (editMode) {
		FunctionLock lock(inputLock);
			bool hasNext = input.size() < INPUT_BOX_SIZE;
			if (hasNext) input.push_back('_');
			input.push_back('\0');
			GraphicsContext::inst()->drawText2D(x, y, &input[0], FONT_SIZE, vec3(1.0f, 1.0f, 1.0f));
			input.pop_back();
			if (hasNext) input.pop_back();
		lock.unlock();
		y += LINE_SIZE;
	}
	for (int c = 0; c < NUM_MESSAGES && messages[c] != NULL; c++) {
		GraphicsContext::inst()->drawText2D(x, y, messages[c]->c_str(), FONT_SIZE, vec3(1.0f, 1.0f, 1.0f));
		y += LINE_SIZE;
	}
}

ChatBox::~ChatBox() {
	for (int c = 0; c < NUM_MESSAGES && messages[c] != NULL; c++) {
		delete messages[c];
	}
}
