#pragma once
#include <string>

class Connection;

class ServerMessageListener {
public:
	virtual void handleMessage(const std::string &msg, Connection *sender) = 0;
	virtual ~ServerMessageListener() {}
};
