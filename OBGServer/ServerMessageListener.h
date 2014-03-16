#pragma once
#include <string>

class ServerConnection;

class ServerMessageListener {
public:
	virtual void handleMessage(const std::string &msg, ServerConnection *sender) = 0;
	virtual ~ServerMessageListener() {}
};
