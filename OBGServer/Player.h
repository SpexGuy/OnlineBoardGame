#pragma once
#include <string>

class Connection;

class Player {
protected:
	Connection *conn;
	std::string name;

public:



	virtual ~Player();

	inline Connection *getConnection()	{ return conn; }
	inline std::string getName()		{ return name; }
};
