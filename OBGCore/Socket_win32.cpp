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

Socket::Socket(const string &ip, short port) {
	socketFD = socket(PF_INET, SOCK_STREAM, 0);
	if (socketFD <= 0) {
		cerr << "Could not create socket" << endl;
		socketFD = 0;
		return;
	}

	struct sockaddr target;
	struct sockaddr_in *target4 = (sockaddr_in *) &target;

	target4->sin_family = AF_INET;
	target4->sin_port = htons(port);
	target4->sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	memset(target4->sin_zero, 0, sizeof(target4->sin_zero));

	if (connect(socketFD, &target, sizeof(target)) < 0) {
		cout << "Unable to connect!" << endl;
		closesocket(socketFD);
		socketFD = 0;
	}
}

int Socket::sendRawBytes(char *byteData, int size) {
	if (!connected) return -1;
	int bytesLeft = size;
	while(bytesLeft > 0) {
		int bytesSent = send(socketFD, byteData, bytesLeft, 0);
		if (bytesSent < 0) {
			cout << "Failed to send bytes" << endl;
			return bytesLeft;
		}
		byteData += bytesSent;
		bytesLeft -= bytesSent;
	}
	return 0;
}

int Socket::readRawBytes(const void *data, int size) {
	char *byteData = (char *) data;
	int bytesLeft = size;
	while(bytesLeft > 0) {
		int bytesRecv = recv(socketFD, byteData, bytesLeft, 0);
		if (bytesRecv < 0) {
			cout << "Could not receive bytes" << endl;
			return bytesLeft;
		}
		if (bytesRecv == 0) {
			cout << "Disconnection!" << endl;
			return bytesLeft;
		}
		byteData += bytesRecv;
		bytesLeft -= bytesRecv;
	}
	return 0;
}

int Socket::sendData(int type, const void *data, int size) {
	if (!connected) return -1;
	int ntype = htonl(type);
	int nsize = htonl(size);

	int bytesLeft = sendRawBytes((char *) &ntype, sizeof(ntype));
	if (bytesLeft != 0) return bytesLeft;

	bytesLeft = sendRawBytes((char *) &nsize, sizeof(nsize));
	if (bytesLeft != 0) return bytesLeft;

	bytesLeft = sendRawBytes((char *) data, size);
	return bytesLeft;
}

int Socket::sendFile(const string &filename) {
	if (!connected) return -1;
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
	int bl = sendData(TYPE_FILE, filename.c_str(), filename.length()+1);
	if (bl != 0) {
		cout << "Could not send file header" << endl;
		file.close();
		return -1;
	}
	cout << "Sent file header" << endl;
	bl = sendRawBytes((char *) &size, sizeof(size));
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
		bl = sendRawBytes(buffer, chunkSize);
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

SerialData Socket::receive() {
	int type;
	int bytesLeft = readRawBytes(&type, sizeof(type));
	type = ntohl(type);
	
	SerialData ret;
	if (bytesLeft != 0) {
		ret.data = NULL;
		return ret;
	}

	int size;
	bytesLeft = readRawBytes(&size, sizeof(size));
	size = ntohl(size);

	if (bytesLeft != 0) {
		ret.data = NULL;
		return ret;
	}


	char *data = (char *) malloc(size);
	if (data == NULL) {
		cout << "Could not malloc " << size << " bytes!" << endl;
		assert(false);
	}
	bytesLeft = readRawBytes(data, size);
	if (bytesLeft != 0) {
		free(data);
		ret.data = NULL;
		return ret;
	}

	if (type == TYPE_FILE) {
		cout << "Receiving a file: " << data << endl;
		long fileLength;
		bytesLeft = readRawBytes(&fileLength, sizeof(fileLength));
		if (bytesLeft != 0) {
			free(data);
			ret.data = NULL;
			return ret;
		}
		cout << "Length is " << fileLength << endl;

		ofstream outFile(data);
		char *buffer = new char[FILE_BUFFER_SIZE];

		long fileBytesLeft = fileLength;

		while(fileBytesLeft > 0) {
			int chunkSize = fileBytesLeft < FILE_BUFFER_SIZE ? fileBytesLeft : FILE_BUFFER_SIZE;
			bytesLeft = readRawBytes(buffer, chunkSize);
			if (bytesLeft != 0) {
				free(data);
				ret.data = NULL;
				delete[] buffer;
				return ret;
			}
			outFile.write(buffer, chunkSize);
			fileBytesLeft -= chunkSize;
			cout << "Received " << (fileLength - fileBytesLeft) << " of " << fileLength << " bytes." << endl;
		}

		delete[] buffer;

	}

	ret.data = data;
	ret.size = size;
	ret.type = type;
	return ret;
}




Socket::~Socket() {
	if (socketFD != 0)
		closesocket(socketFD);
}





#endif
