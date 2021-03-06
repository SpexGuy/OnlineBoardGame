#ifdef _WIN32
#include <assert.h>
#include <fstream>
#include <iostream>
#include <WinSock2.h>
#include "Socket.h"

#define FILE_BUFFER_SIZE 1024

using namespace std;

WSAData socketData;

void SocketInit() {
	if (WSAStartup(MAKEWORD(2,0), &socketData) != NO_ERROR) {
		cout << "Failed to start WSA" << endl;
		assert(false);
	}
}

void SocketClose() {
	WSACleanup();
}

Socket::Socket() :
	socketFD(0),
	peer()
{}

bool Socket::open(const Address &peer, uint16_t localPort) {
	assert(!isOpen());
	this->peer = peer;
	socketFD = socket(PF_INET, SOCK_STREAM, 0);
	if (socketFD <= 0) {
		cerr << "Could not create socket" << endl;
		socketFD = 0;
		return false;
	}

	if (localPort != 0) {
		// bind to port
		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons((unsigned short) localPort);
		if (bind(socketFD, (const sockaddr*) &address, sizeof(sockaddr_in)) < 0) {
			printf("failed to bind socket\n");
			close();
			return false;
		}
	}

	struct sockaddr target;
	struct sockaddr_in *target4 = (sockaddr_in *) &target;

	target4->sin_family = AF_INET;
	target4->sin_port = htons(peer.GetTCPPort());
	target4->sin_addr.s_addr = htonl(peer.GetAddress());
	memset(target4->sin_zero, 0, sizeof(target4->sin_zero));

	if (connect(socketFD, &target, sizeof(target)) < 0) {
		cout << "Unable to connect!" << endl;
		closesocket(socketFD);
		socketFD = 0;
		return false;
	}
	return true;
}

int Socket::sendRawBytes(const uint8_t *byteData, uint16_t size) {
	if (!isOpen()) return -1;
	int bytesLeft = size;
	while(bytesLeft > 0) {
		int bytesSent = send(socketFD, (const char *)byteData, bytesLeft, 0);
		if (bytesSent < 0) {
			cout << "Failed to send bytes" << endl;
			return bytesLeft;
		}
		byteData += bytesSent;
		bytesLeft -= bytesSent;
	}
	return 0;
}

int Socket::readRawBytes(uint8_t *data, uint16_t size) {
	int bytesLeft = size;
	while(bytesLeft > 0) {
		int bytesRecv = recv(socketFD, (char *)data, bytesLeft, 0);
		if (bytesRecv < 0) {
			cout << "Could not receive bytes" << endl;
			return bytesLeft;
		}
		if (bytesRecv == 0) {
			cout << "Disconnection!" << endl;
			return bytesLeft;
		}
		data += bytesRecv;
		bytesLeft -= bytesRecv;
	}
	return 0;
}

int Socket::sendData(uint8_t type, const uint8_t *data, uint16_t size) {
	if (!isOpen()) return -1;
	uint16_t nsize = htons(size);

	int bytesLeft = sendRawBytes((uint8_t *) &type, sizeof(type));
	if (bytesLeft != 0) return bytesLeft;

	bytesLeft = sendRawBytes((uint8_t *) &nsize, sizeof(nsize));
	if (bytesLeft != 0) return bytesLeft;

	bytesLeft = sendRawBytes((uint8_t *) data, size);
	return bytesLeft;
}

int Socket::sendFile(const string &filename) {
	if (!isOpen()) return -1;
	cout << "Sending file: " << filename << endl;
	//open file for binary read
	ifstream file(filename.c_str(), ios::in | ios::binary | ios::ate);
	if (!file.is_open()) {
		cout << "Could not open file '" << filename << "'." << endl;
		return -2;
	}
	long size = (long) file.tellg();
	cout << "file length is " << size;
	file.seekg(0, ios::beg);
	//send header data
	int bl = sendData(TYPE_FILE, (const uint8_t *)filename.c_str(), filename.length()+1);
	if (bl != 0) {
		cout << "Could not send file header" << endl;
		file.close();
		return -1;
	}
	cout << "Sent file header" << endl;
	bl = sendRawBytes((uint8_t *) &size, sizeof(size));
	if (bl != 0) {
		cout << "Could not send file header" << endl;
		file.close();
		return -1;
	}
	long bytesLeft = size;
	char *buffer = new char[FILE_BUFFER_SIZE];
	while (bytesLeft > 0) {
		int chunkSize = bytesLeft < FILE_BUFFER_SIZE ? bytesLeft : FILE_BUFFER_SIZE;
		file.read(buffer, chunkSize);
		bl = sendRawBytes((uint8_t *)buffer, chunkSize);
		if (bl != 0) {
			cout << "Could not send bytes" << endl;
			file.close();
			delete[] buffer;
			return -1;
		}
		bytesLeft -= chunkSize;
		cout << "Sent " << (size - bytesLeft) << " of " << size << " bytes." << endl;
	}
	file.close();
	delete[] buffer;
	return 0;
}

int Socket::receive(uint8_t &type, uint8_t *data, uint16_t maxSize) {
	assert(isOpen());
	int bytesLeft = readRawBytes(&type, sizeof(type));
	if (bytesLeft != 0) return -1;

	uint16_t size;
	bytesLeft = readRawBytes((uint8_t *)&size, sizeof(size));
	size = ntohs(size);
	if (bytesLeft != 0) return -1;

	if (maxSize < size) {
		cout << "Read Buffer Too Small!!!!!" << endl;
		assert(false);
		return -1;
	}
	bytesLeft = readRawBytes(data, size);
	if (bytesLeft != 0) return -1;

	if (type == TYPE_FILE) {
		cout << "Receiving a file: " << data << endl;
		long fileLength;
		bytesLeft = readRawBytes((uint8_t *)&fileLength, sizeof(fileLength));
		if (bytesLeft != 0) return -1;

		cout << "Length is " << fileLength << endl;
		ofstream outFile((char *)data);
		char *buffer = new char[FILE_BUFFER_SIZE];
		assert(buffer != NULL);

		long fileBytesLeft = -1;

		while(fileBytesLeft > 0) {
			int chunkSize = fileBytesLeft < FILE_BUFFER_SIZE ? fileBytesLeft : FILE_BUFFER_SIZE;
			bytesLeft = readRawBytes((uint8_t *)buffer, chunkSize);
			if (bytesLeft != 0) {
				delete[] buffer;
				return -1;
			}
			outFile.write(buffer, chunkSize);
			fileBytesLeft -= chunkSize;
			cout << "Received " << (fileLength - fileBytesLeft) << " of " << fileLength << " bytes." << endl;
		}
		delete[] buffer;
	}
	return size;
}

void Socket::close() {
	if (socketFD != 0) {
		closesocket(socketFD);
		socketFD = 0;
	}
}

Socket::~Socket() {
	close();
}

#endif
