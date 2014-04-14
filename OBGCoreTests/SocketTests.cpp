#include "stdafx.h"
#include "CppUnitTest.h"

#include <iostream>
#include <string>

#include <Connection.h>
#include <Event.h>
#include <ServerSocket.h>
#include <Socket.h>
#include <Thread.h>
#include <UDPSocket.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

// --------------- Thread Utility Functions ------------------
Socket *sock;
int BasicServerSocket(void *server) {
	sock = ((ServerSocket *)server)->getNextConnection();
	return 0;
}

uint8_t data[4096];
uint8_t type;
uint16_t len;
int BasicStreamSocket(void *vsocket) {
	Socket *socket = (Socket *)vsocket;
	len = socket->receive(type, data, sizeof(data));
	return 0;
}

uint8_t rawData[4096];
uint16_t rawLeft;
uint16_t rawToRead;
int BasicRawStreamSocket(void *vsocket) {
	Socket *socket = (Socket *)vsocket;
	rawLeft = socket->readRawBytes(rawData, rawToRead);
	return 0;
}



// --------------- Tests -------------------------------------
namespace OBGTests {
	TEST_CLASS(SocketTests) {
	public:
		TEST_METHOD_INITIALIZE(start) {
			sock = NULL;
			SocketInit();
			Assert::IsTrue(_CrtCheckMemory() == TRUE);
		}
		TEST_METHOD(TCPConnectTest) {
			bool success = false;
			//setup server
			ServerSocket *serverListener = new ServerSocket();
			success = serverListener->open(0xABC0);
			Assert::IsTrue(success, L"Failed to start server socket on port 0xABC0");
			Thread serverThread(BasicServerSocket, serverListener);
			success = serverThread.start();
			Assert::IsTrue(success, L"Failed to start server socket thread");
			//setup client
			Socket *client = new Socket();
			success = client->open(Address::TCPAddress(0x7F000001, 0xABC0));
			Assert::IsTrue(success, L"Failed to open socket");
			serverThread.waitForTerminate();
			Socket *server = sock;
			Assert::IsNotNull(server, L"ServerSocket did not get connection");
			Assert::AreEqual(server->getPeer().GetAddress(), uint32_t(0x7F000001));
			Assert::IsTrue(_CrtCheckMemory() == TRUE);
		}
		TEST_METHOD(UDPConnectTest) {
			bool success = false;
			UDPSocket *client = new UDPSocket();
			UDPSocket *server = new UDPSocket();
			Address serverAddr = Address::UDPAddress(0x7F000001, 0xABC1);
			Address clientAddr = Address::UDPAddress(0x7F000001, 0xABC9);
			success = client->open(clientAddr.GetUDPPort());
			Assert::IsTrue(success, L"Failed to open client on port 0xABC9");
			success = server->open(serverAddr.GetUDPPort());
			Assert::IsTrue(success, L"Failed to open server on port 0xABC1");
			const uint8_t *data = (const uint8_t *)"Message";
			int bytesSent = client->send(serverAddr, data, 8);
			Assert::AreEqual(8, bytesSent, L"Sent the wrong number of bytes");
			Address from;
			uint8_t *recv = (uint8_t *)alloca(30);
			int bytesRecv = server->receive(from, recv, 30);
			Assert::AreEqual(from, clientAddr, L"Got message from wrong source");
			Assert::AreEqual(8, bytesRecv, L"Received the wrong number of bytes");
			Assert::AreEqual((const char *)data, (const char *)recv, L"Messages did not match");
			client->close();
			server->close();
			Assert::IsTrue(_CrtCheckMemory() == TRUE);
		}
		TEST_METHOD_CLEANUP(stop) {
			SocketClose();
			sock = NULL;
			Assert::IsTrue(_CrtCheckMemory() == TRUE);
		}
	};

	TEST_CLASS(StreamSocketTests) {
	private:
		Socket *client;
		Socket *server;
	public:
		TEST_METHOD_INITIALIZE(start) {
			SocketInit();
			//setup listener
			ServerSocket *serverListener = new ServerSocket();
			serverListener->open(0xABC0);
			Thread serverThread(BasicServerSocket, serverListener);
			serverThread.start();
			//setup client end
			client = new Socket();
			client->open(Address::TCPAddress(0x7F000001, 0xABC0));
			//setup server end
			serverThread.waitForTerminate();
			server = sock;
			//cleanup
			serverListener->close();
			delete serverListener;
			Assert::IsTrue(_CrtCheckMemory() == TRUE);
		}
		TEST_METHOD(TCPRawTest) {
			rawLeft = -2;
			rawToRead = 11;
			memcpy(rawData, "fail", 5);
			Thread sthread(BasicRawStreamSocket, server);
			sthread.start();
			char *msg = "hellololol";
			int left = client->sendRawBytes((const uint8_t *)msg, 11);
			Assert::AreEqual(left, 0, L"Failed to send bytes");
			sthread.waitForTerminate();
			Assert::AreEqual(0, int(rawLeft), L"Did not receive data");
			Assert::AreEqual((const char *)msg, (const char *)rawData, L"Messages did not match");
			Assert::IsTrue(_CrtCheckMemory() == TRUE);
		}
		TEST_METHOD(TCPDataTest) {
			cout << 0 << endl;
			len = -2;
			memcpy(data, "fail", 5);
			Thread sthread(BasicStreamSocket, server);
			cout << 1 << endl;
			sthread.start();
			cout << 2 << endl;
			char *msg = "hellololol";
			cout << 3 << endl;
			int left = client->sendData(TYPE_INTERACTION, (const uint8_t *)msg, 11);
			Assert::AreEqual(left, 0, L"Failed to send bytes");
			cout << 4 << endl;
			sthread.waitForTerminate();
			cout << 6 << endl;
			Assert::AreNotEqual(-2, int(len), L"Did not receive data");
			Assert::AreEqual(11, int(len), L"Received wrong size");
			cout << 7 << endl;
			Assert::AreEqual(TYPE_INTERACTION, int(type), L"Received wrong type");
			cout << 8 << endl;
			Assert::AreEqual((const char *)msg, (const char *)data, L"Messages did not match");
			cout << 9 << endl;
			Assert::IsTrue(_CrtCheckMemory() == TRUE);
			cout << 10 << endl;
		}

		TEST_METHOD_CLEANUP(stop) {
			delete client;
			delete server;
			SocketClose();
			Assert::IsTrue(_CrtCheckMemory() == TRUE);
		}
	};
}
