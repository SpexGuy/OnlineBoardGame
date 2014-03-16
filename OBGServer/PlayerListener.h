#pragma once

class ServerConnection;

class PlayerListener {
public:
	virtual void playerJoined(ServerConnection *p) = 0;
	virtual void playerLeft(ServerConnection *p) = 0;
	virtual ~PlayerListener() {}
};
