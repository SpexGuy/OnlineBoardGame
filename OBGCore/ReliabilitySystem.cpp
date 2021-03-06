#include <assert.h>
#include "ReliabilitySystem.h"

ReliabilitySystem::ReliabilitySystem(unsigned int max_sequence) {
	this->rtt_maximum = rtt_maximum;
	this->max_sequence = max_sequence;
	Reset();
}

void ReliabilitySystem::Reset() {
	FunctionLock lock(thisLock);
	local_sequence = 0;
	remote_sequence = 0;
	sentQueue.clear();
	receivedQueue.clear();
	pendingAckQueue.clear();
	ackedQueue.clear();
	sent_packets = 0;
	recv_packets = 0;
	lost_packets = 0;
	acked_packets = 0;
	sent_bandwidth = 0.0f;
	acked_bandwidth = 0.0f;
	rtt = 0.0f;
	rtt_maximum = 1.0f;
}

void ReliabilitySystem::PacketSent(int size) {
	FunctionLock lock(thisLock);
	if (sentQueue.exists(local_sequence)) {
		printf("local sequence %d exists\n", local_sequence);
		for (PacketQueue::iterator itor = sentQueue.begin(); itor != sentQueue.end(); ++itor)
			printf(" + %d\n", itor->sequence);
	}
	assert(!sentQueue.exists(local_sequence));
	assert(!pendingAckQueue.exists(local_sequence));
	PacketData data;
	data.sequence = local_sequence;
	data.time = 0.0f;
	data.size = size;
	sentQueue.push_back(data);
	pendingAckQueue.push_back(data);
	sent_packets++;
	local_sequence++;
	if (local_sequence > max_sequence)
		local_sequence = 0;
}

void ReliabilitySystem::PacketReceived(unsigned int sequence, int size) {
	FunctionLock lock(thisLock);
	recv_packets++;
	if (receivedQueue.exists(sequence))
		return;
	PacketData data;
	data.sequence = sequence;
	data.time = 0.0f;
	data.size = size;
	receivedQueue.push_back(data);
	if (sequence_more_recent(sequence, remote_sequence, max_sequence))
		remote_sequence = sequence;
}

void ReliabilitySystem::Update(float deltaTime) {
	FunctionLock lock(thisLock);
	acks.clear();
	AdvanceQueueTime(deltaTime);
	UpdateQueues();
	UpdateStats();
#ifdef NET_UNIT_TEST
	Validate();
#endif
}

void ReliabilitySystem::Validate() {
	FunctionLock lock(thisLock);
	sentQueue.verify_sorted(max_sequence);
	receivedQueue.verify_sorted(max_sequence);
	pendingAckQueue.verify_sorted(max_sequence);
	ackedQueue.verify_sorted(max_sequence);
}


// ---------- static functions --------------

int ReliabilitySystem::bit_index_for_sequence(unsigned int sequence, unsigned int ack, unsigned int max_sequence) {
	assert(sequence != ack);
	assert(!sequence_more_recent(sequence, ack, max_sequence));
	if (sequence > ack) {
		//TODO: This fails because too many physics updates per tick
		//if (ack >= 33)
		//	assert(ack < 33);
		if (max_sequence < sequence)
			assert(max_sequence >= sequence);
 		return ack + (max_sequence - sequence);
	} else {
		assert(ack >= 1);
		assert(sequence <= ack - 1);
 		return ack - 1 - sequence;
	}
}

unsigned int ReliabilitySystem::generate_ack_bits(unsigned int ack, const PacketQueue & received_queue, unsigned int max_sequence) {
	unsigned int ack_bits = 0;
	for (PacketQueue::const_iterator itor = received_queue.begin(); itor != received_queue.end(); itor++) {
		if (itor->sequence == ack || sequence_more_recent(itor->sequence, ack, max_sequence))
			break;
		int bit_index = bit_index_for_sequence(itor->sequence, ack, max_sequence);
		if (bit_index <= 31)
			ack_bits |= 1 << bit_index;
	}
	return ack_bits;
}

void ReliabilitySystem::process_ack(unsigned int ack, unsigned int ack_bits,
							PacketQueue & pending_ack_queue, PacketQueue & acked_queue,
							std::vector<unsigned int> & acks, unsigned int & acked_packets,
							float & rtt, unsigned int max_sequence)
{
	if (pending_ack_queue.empty())
		return;

	PacketQueue::iterator itor = pending_ack_queue.begin();
	while (itor != pending_ack_queue.end()) {
		bool acked = false;

		if (itor->sequence == ack) {
			acked = true;
		} else if (!sequence_more_recent(itor->sequence, ack, max_sequence)) {
			int bit_index = bit_index_for_sequence(itor->sequence, ack, max_sequence);
			if (bit_index <= 31)
				acked = (ack_bits >> bit_index) & 1;
		}

		if (acked) {
			rtt += (itor->time - rtt) * 0.1f;

			acked_queue.insert_sorted(*itor, max_sequence);
			acks.push_back(itor->sequence);
			acked_packets++;
			itor = pending_ack_queue.erase(itor);
		} else {
			++itor;
		}
	}
}


//TODO:[MW] WTF Why is this function?
void ReliabilitySystem::AdvanceQueueTime(float deltaTime) {
	FunctionLock lock(thisLock);
	for (PacketQueue::iterator itor = sentQueue.begin(); itor != sentQueue.end(); itor++)
		itor->time += deltaTime;

	for (PacketQueue::iterator itor = receivedQueue.begin(); itor != receivedQueue.end(); itor++)
		itor->time += deltaTime;

	for (PacketQueue::iterator itor = pendingAckQueue.begin(); itor != pendingAckQueue.end(); itor++)
		itor->time += deltaTime;

	for (PacketQueue::iterator itor = ackedQueue.begin(); itor != ackedQueue.end(); itor++)
		itor->time += deltaTime;
}

void ReliabilitySystem::UpdateQueues() {
	FunctionLock lock(thisLock);
	const float epsilon = 0.001f;

	while (sentQueue.size() && sentQueue.front().time > rtt_maximum + epsilon)
		sentQueue.pop_front();

	if (receivedQueue.size()) {
		const unsigned int latest_sequence = receivedQueue.back().sequence;
		const unsigned int minimum_sequence = latest_sequence >= 34 ? (latest_sequence - 34) : max_sequence - (34 - latest_sequence);
		while (receivedQueue.size() && !sequence_more_recent(receivedQueue.front().sequence, minimum_sequence, max_sequence))
			receivedQueue.pop_front();
	}

	while (ackedQueue.size() && ackedQueue.front().time > rtt_maximum * 2 - epsilon)
		ackedQueue.pop_front();

	while (pendingAckQueue.size() && pendingAckQueue.front().time > rtt_maximum + epsilon) {
		pendingAckQueue.pop_front();
		lost_packets++;
	}
}

void ReliabilitySystem::UpdateStats() {
	FunctionLock lock(thisLock);
	int sent_bytes_per_second = 0;
	for (PacketQueue::iterator itor = sentQueue.begin(); itor != sentQueue.end(); ++itor)
		sent_bytes_per_second += itor->size;
	int acked_packets_per_second = 0;
	int acked_bytes_per_second = 0;
	for (PacketQueue::iterator itor = ackedQueue.begin(); itor != ackedQueue.end(); ++itor) {
		if (itor->time >= rtt_maximum) {
			acked_packets_per_second++;
			acked_bytes_per_second += itor->size;
		}
	}
	sent_bytes_per_second /= rtt_maximum;
	acked_bytes_per_second /= rtt_maximum;
	sent_bandwidth = sent_bytes_per_second * (8 / 1000.0f);
	acked_bandwidth = acked_bytes_per_second * (8 / 1000.0f);
}

void ReliabilitySystem::ProcessAck(unsigned int ack, unsigned int ack_bits) {
	FunctionLock lock(thisLock);
	process_ack(ack, ack_bits, pendingAckQueue, ackedQueue, acks, acked_packets, rtt, max_sequence);
}

unsigned int ReliabilitySystem::GenerateAckBits() {
	FunctionLock lock(thisLock);
	return generate_ack_bits(GetRemoteSequence(), receivedQueue, max_sequence);
}

ReliabilitySystem::~ReliabilitySystem() {
	FunctionLock lock(thisLock);
}
