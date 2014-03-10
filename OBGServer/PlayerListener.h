#pragma once

class Player;

class PlayerListener {
public:
	virtual void playerJoined(Player *p) = 0;
	virtual void playerLeft(Player *p) = 0;
	virtual ~PlayerListener() {}
};
