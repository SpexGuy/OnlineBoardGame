#pragma once
#include <vector>

class MessageListener;

class MessageEventBroadcaster {
private:
	std::vector<MessageListener *> listeners;

protected:
	void fireMessage(const std::string &message);

public:
	MessageEventBroadcaster() {};
	virtual void registerMessageListener(MessageListener *listener);
	virtual ~MessageEventBroadcaster() {};
};
