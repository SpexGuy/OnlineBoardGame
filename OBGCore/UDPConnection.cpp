#include <assert.h>
#include <iostream>
#include "UDPConnection.h"

using namespace std;

UDPClient::UDPClient(uint32_t protocolId, int timeout) :
	protocolId(protocolId),
	timeout(timeout),
	state(Disconnected),
	running(false),
	reliabilitySystem(MAX_SEQNO)
{}

bool UDPClient::start(int time, uint16_t port) {
	assert(state == Disconnected);
	this->time = lastMessageTime = time;
	if (!socket.open(port))
		return false;
	running = true;
	return true;
}

bool UDPClient::connect(const Address &server, int attemptMillis, int packetSendRate) {
	assert(socket.isOpen());
	assert(running);
	assert(state == Disconnected);
	assert(server.GetAddress());
	//update state
	state = Connecting;
	this->server = server;
	//send connect message until connected or time expires
	for (int millis = 0; state == Connecting && millis < attemptMillis; millis += packetSendRate) {
		if (!sendPacket(TYPE_CONNECT_REQUEST, 0, NULL)) {
			cout << "Could not transmit connection packet" << endl;
			break;
		}
		sleep(packetSendRate);
	}
	if (state == Connecting)
		state = ConnectFail;
	return state == Connected;
}

bool UDPClient::sendPacket(int type, int len, const uint8_t *data) {
	assert(running);
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

int UDPClient::receivePacket(int &type, int len, uint8_t *data) {
	assert(running);
	Address sender;
	int size = sizeof(Packet) + len;
	Packet *pack = (Packet *)alloca(size);
	while(socket.isOpen()) {
		int received_bytes = socket.receive(sender, (uint8_t *)pack, size);
		if (received_bytes < 0)
			return received_bytes;
		if (received_bytes < sizeof(Packet))
			continue;
		if (pack->protocol != protocolId)
			continue;
		if (sender != server)
			continue;
		//message is valid
		if (len < received_bytes-sizeof(Packet))
			cout << "Not enough memory to hold received packet" << endl;
		lastMessageTime = time;
		reliabilitySystem.PacketReceived(pack->seqno, len);
		reliabilitySystem.ProcessAck(pack->ack, pack->ack_bits);
		//internally handle some packets
		if (preHandlePacket(pack))
			continue;
		//return packet
		type = pack->message.type;
		memcpy(data, pack->message.data, min(len, received_bytes));
		return received_bytes - sizeof(Packet);
	}
	return 0;
}

bool UDPClient::preHandlePacket(Packet *pack) {
	if (pack->message.type == TYPE_CONNECT_ACCEPT) {
		if (state == Connecting) {
			state = Connected;
			cout << "Connected to server" << endl;
		}
		return true;
	}
	if (pack->message.type == TYPE_CONNECT_DECLINE) {
		if (state == Connecting) {
			state = ConnectFail;
			cout << "Server declined connection" << endl;
		}
		return true;
	}
	return false;
}

bool UDPClient::update(int time) {
	assert(running);
	int dt = time - this->time;
	this->time = time;
	reliabilitySystem.Update(dt);
	if (isConnected() && isUnresponsive()) {
		cout << "Server has timed out" << endl;
		state = Disconnected;
		running = false;
		reliabilitySystem.Reset();
		return false;
	}
	return true;
}

void UDPClient::stop() {
	socket.close();
}




UDPServer::UDPServer(uint32_t protocolId, int timeout) :
	protocolId(protocolId),
	timeout(timeout),
	running(false)
{}

bool UDPServer::start(int time, uint16_t port) {
	assert(port != ANY_PORT);
	assert(!running);
	this->time = time;
	if (!socket.open(port))
		return false;
	running = true;
	return true;
}

bool UDPServer::broadcastPacket(int type, int len, const uint8_t *data) {
	bool success = true;
	for (pair<const Address, ClientInfo> &client : clients) {
		success &= sendPacket(client.first, type, len, data);
	}
	return success;
}

bool UDPServer::sendPacket(const Address &destination, int type, int len, const uint8_t *data) {
	assert(running);
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
	assert(running);
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

int UDPServer::receivePacket(Address &from, int &type, int len, uint8_t *data) {
	assert(running);
	int size = sizeof(Packet) + len;
	Packet *pack = (Packet *)alloca(size);
	while(socket.isOpen()) {
		int received_bytes = socket.receive(from, (uint8_t *)pack, size);
		if (received_bytes < 0)
			return received_bytes;
		if (received_bytes < sizeof(Packet))
			continue;
		if (pack->protocol != protocolId)
			continue;
		if (declined.find(from) != declined.end()) {
			sendUnindexedPacket(from, TYPE_CONNECT_DECLINE, 0, NULL);
			continue;
		}
		if (clients.find(from) == clients.end()) {
			handleUnknownClient(from, pack, received_bytes);
			continue;
		}
		//message is valid
		if (len < received_bytes-sizeof(Packet))
			cout << "Not enough memory to hold received packet" << endl;
		clients[from].lastMessageTime = time;
		clients[from].reliabilitySystem.PacketReceived(pack->seqno, len);
		clients[from].reliabilitySystem.ProcessAck(pack->ack, pack->ack_bits);
		//internally handle some packets
		if (preHandlePacket(from, pack, received_bytes))
			continue;
		//return packet
		type = pack->message.type;
		memcpy(data, pack->message.data, min(len, received_bytes));
		return received_bytes - sizeof(Packet);
	}
	return 0;
}

void UDPServer::handleUnknownClient(const Address &from, Packet *pack, int len) {
	if (pack->message.type == TYPE_CONNECT_REQUEST) {
		if (acceptNewPlayer(from, pack, len)) {
			printf("Accepted connection from %d.%d.%d.%d:%d\n", from.GetA(), from.GetB(), from.GetC(), from.GetD(), from.GetPort());
			clients[from].reliabilitySystem = ReliabilitySystem(MAX_SEQNO);
			clients[from].lastMessageTime = time;
			clients[from].reliabilitySystem.PacketReceived(pack->seqno, len);
			clients[from].reliabilitySystem.ProcessAck(pack->ack, pack->ack_bits);
			sendPacket(from, TYPE_CONNECT_ACCEPT, 0, NULL);
			onConnect(from);
		} else {
			printf("Declined connection from %d.%d.%d.%d:%d\n", from.GetA(), from.GetB(), from.GetC(), from.GetD(), from.GetPort());
			declined.insert(from);
			sendUnindexedPacket(from, TYPE_CONNECT_DECLINE, 0, NULL);
		}
	} else {
		cout << "Valid packet from unknown source..." << endl;
	}
}

bool UDPServer::acceptNewPlayer(const Address &addr, Packet *request, int len) {
	return true;
}

bool UDPServer::preHandlePacket(const Address &from, Packet *request, int len) {
	if (request->message.type == TYPE_CONNECT_REQUEST) {
		sendPacket(from, TYPE_CONNECT_ACCEPT, 0, NULL);
		return true;
	}
	return false;
}

bool UDPServer::update(int time) {
	assert(running);
	int dt = time - this->time;
	this->time = time;
	bool success = true;
	for (auto iter = clients.begin(), end = clients.end(); iter != end; iter++) {
		iter->second.reliabilitySystem.Update(dt);
		if (isUnresponsive(iter->first)) {
			printf("Client %d.%d.%d.%d:%d has timed out\n", iter->first.GetA(),
					iter->first.GetB(), iter->first.GetC(), iter->first.GetD(), iter->first.GetPort());
			onDisconnect(iter->first);
			iter = clients.erase(iter);
		}
		success = false;
	}
	return success;
}

bool UDPServer::isUnresponsive(const Address &addr) {
	assert(clients.find(addr) != clients.end());
	return time - clients[addr].lastMessageTime > timeout;
}

void UDPServer::stop() {
	socket.close();
}

