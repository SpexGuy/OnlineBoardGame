#pragma once
#include <Address.h>
#include <Connection.h>
#include <Event.h>
#include <InteractionListener.h>
#include <PhysicsUpdateEventBroadcaster.h>
#include <UDPConnection.h>
#include "MessageEventBroadcaster.h"
#include "MessageListener.h"

class ClientConnection :
	public Connection,
	public PhysicsUpdateEventBroadcaster,
	public MessageEventBroadcaster,
	public InteractionListener,
	public MessageListener,
	public UDPClientListener
{
protected:
	Event fileDownloaded;
	UDPClient udpClient;
	void processData(uint8_t type, const uint8_t *data, uint16_t len);
	virtual void handleFatalError();

public:
	ClientConnection();

	virtual bool connect(int time, const Address &addr, int localPort);
	virtual bool update(int time);
	virtual void setUsername(const std::string &username);
	virtual void downloadFile(const std::string &filename);
	virtual void handleInteraction(Interaction *action);
	virtual void handleMessage(const std::string &message);

	virtual void handleMessage(int type, uint8_t *data, int len);

	virtual ~ClientConnection();
};
