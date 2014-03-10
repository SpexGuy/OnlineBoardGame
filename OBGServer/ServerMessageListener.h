#pragma once
#include <string>

class Player;

class ServerMessageListener {
public:
	virtual void handleMessage(const std::string &msg, Player *sender) = 0;
	virtual ~ServerMessageListener() {}
};
