#pragma once

class Asset;

class Entity {
protected:
	Asset *type;
	int id;
	bool hidden;

public:
	virtual void shake();
	virtual void hide();
	virtual void show();
};
