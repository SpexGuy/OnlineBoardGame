#pragma once
#include <string>

class MessageListener {
public:
	virtual void handleMessage(const std::string &msg) = 0;
	virtual ~MessageListener() {}
};
