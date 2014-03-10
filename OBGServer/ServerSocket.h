#pragma once

class Socket;

class ServerSocket {
public:
	Socket *accept();
};
