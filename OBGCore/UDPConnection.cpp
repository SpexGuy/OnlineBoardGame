#include <assert.h>
#include <iostream>
#include "UDPConnection.h"

using namespace std;

#define MAX_PACKET_SIZE 4096

int UDPClientReadThread(void *client) {
	return ((UDPClient *)client)->loop();
}

UDPClient::UDPClient(uint32_t protocolId, int timeout, UDPClientListener *listener) :
	protocolId(protocolId),
	timeout(timeout),
	listener(listener),
	reliabilitySystem(MAX_SEQNO),
	thread(UDPClientReadThread, this)
{}

bool UDPClient::start(int time, const Address &server, uint16_t port) {
	this->server = server;
	this->time = lastMessageTime = time;
	if (!socket.open(port))
		return false;
	thread.start();
	return true;
}

bool UDPClient::sendPacket(int type, int len, const uint8_t *data) {
	assert(isRunning());
	assert(server.GetAddress());
	//make packet
	int size = sizeof(Packet) + len;
	Packet *pack = (Packet *) alloca(size);
	pack->protocol = protocolId;
	pack->seqno = reliabilitySystem.GetLocalSequence();
	pack->ack = reliabilitySystem.GetRemoteSequence();
	pack->ack_bits = reliabilitySystem.GenerateAckBits();
	pack->message.type = type;
	memcpy(pack->message.data, data, len);
	//send packet
	if (socket.send(server, (uint8_t *)pack, size) != size)
		return false;
	reliabilitySystem.PacketSent(len);
	return true;
}

int UDPClient::loop() {
	assert(isRunning());
	Address sender;
	Packet *pack = (Packet *)malloc(MAX_PACKET_SIZE);
	while(isRunning()) {
		int received_bytes = socket.receive(sender, (uint8_t *)pack, MAX_PACKET_SIZE);
		if (received_bytes < 0)
			return received_bytes;
		if (received_bytes < sizeof(Packet))
			continue;
		if (pack->protocol != protocolId)
			continue;
		if (sender != server)
			continue;
		//message is valid
		//update tracking data
		lastMessageTime = time;
		reliabilitySystem.PacketReceived(pack->seqno, received_bytes - sizeof(Packet));
		reliabilitySystem.ProcessAck(pack->ack, pack->ack_bits);
		//let listener handle packet
		listener->handleMessage(pack->message.type, pack->message.data, received_bytes - sizeof(Packet));
	}
	return 0;
}

bool UDPClient::update(int time) {
	assert(isRunning());
	int dt = time - this->time;
	this->time = time;
	reliabilitySystem.Update(dt);
	if (isUnresponsive()) {
		cout << "Server has timed out" << endl;
		return false;
	}
	return true;
}

void UDPClient::stop() {
	reliabilitySystem.Reset();
	socket.close();
}




int UDPServerReadThread(void *server) {
	return ((UDPServer *)server)->loop();
}

UDPServer::UDPServer(uint32_t protocolId, int timeout, UDPServerListener *listener) :
	protocolId(protocolId),
	timeout(timeout),
	listener(listener),
	thread(UDPServerReadThread, this)
{}

bool UDPServer::start(int time, uint16_t port) {
	assert(port != ANY_PORT);
	assert(!isRunning());
	this->time = time;
	if (!socket.open(port))
		return false;
	thread.start();
	return true;
}

bool UDPServer::broadcastPacket(int type, int len, const uint8_t *data, const Address &exclude) {
	bool success = true;
	for (pair<const Address, ClientInfo> &client : clients) {
		if (client.first != exclude)
			success &= sendPacket(client.first, type, len, data);
	}
	return success;
}

bool UDPServer::sendPacket(const Address &destination, int type, int len, const uint8_t *data) {
	assert(isRunning());
	assert(clients.find(destination) != clients.end());
	//make packet
	ClientInfo &client = clients[destination];
	int size = sizeof(Packet) + len;
	Packet *pack = (Packet *) alloca(size);
	pack->protocol = protocolId;
	pack->seqno = client.reliabilitySystem.GetLocalSequence();
	pack->ack = client.reliabilitySystem.GetRemoteSequence();
	pack->ack_bits = client.reliabilitySystem.GenerateAckBits();
	pack->message.type = type;
	memcpy(pack->message.data, data, len);
	//send packet
	if (socket.send(destination, (uint8_t *)pack, size) != size)
		return false;
	client.reliabilitySystem.PacketSent(len);
	return true;
}

bool UDPServer::sendUnindexedPacket(const Address &destination, int type, int len, const uint8_t *data) {
	assert(isRunning());
	assert(clients.find(destination) == clients.end());
	//make packet
	int size = sizeof(Packet) + len;
	Packet *pack = (Packet *) alloca(size);
	pack->protocol = protocolId;
	pack->seqno = 0;
	pack->ack = 0;
	pack->ack_bits = 0;
	pack->message.type = type;
	memcpy(pack->message.data, data, len);
	//send packet
	return socket.send(destination, (uint8_t *)pack, size) == size;
}

int UDPServer::loop() {
	assert(isRunning());
	Address from;
	Packet *pack = (Packet *)malloc(MAX_PACKET_SIZE);
	while(isRunning()) {
		int received_bytes = socket.receive(from, (uint8_t *)pack, MAX_PACKET_SIZE);
		if (received_bytes < 0)
			return received_bytes;
		if (received_bytes < sizeof(Packet))
			continue;
		if (pack->protocol != protocolId)
			continue;
		if (clients.find(from) == clients.end())
			continue;
		//message is valid
		//update tracking info
		clients[from].lastMessageTime = time;
		clients[from].reliabilitySystem.PacketReceived(pack->seqno, received_bytes - sizeof(Packet));
		clients[from].reliabilitySystem.ProcessAck(pack->ack, pack->ack_bits);
		//send packet to listener
		listener->handleMessage(from, pack->message.type, pack->message.data, received_bytes - sizeof(Packet));
	}
	return 0;
}

void UDPServer::addClient(const Address &client) {
	clients[client].reliabilitySystem = ReliabilitySystem(MAX_SEQNO);
	clients[client].lastMessageTime = time;
}

bool UDPServer::update(int time) {
	assert(isRunning());
	int dt = time - this->time;
	this->time = time;
	bool success = true;
	for (auto iter = clients.begin(), end = clients.end(); iter != end; iter++) {
		iter->second.reliabilitySystem.Update(dt);
		if (isUnresponsive(iter->first)) {
			if (listener->handleUnresponsiveClient(iter->first)) {
				printf("Client %d.%d.%d.%d:%d has timed out\n", iter->first.GetA(), iter->first.GetB(), iter->first.GetC(), iter->first.GetD(), iter->first.GetPort());
				iter = clients.erase(iter);
			}
			success = false;
		}
	}
	return success;
}

bool UDPServer::isUnresponsive(const Address &addr) {
	assert(clients.find(addr) != clients.end());
	return time - clients[addr].lastMessageTime > timeout;
}

void UDPServer::stop() {
	socket.close();
	clients.clear();
}
