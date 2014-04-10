/*
	Simple Network Library from "Networking for Game Programmers"
	http://www.gaffer.org/networking-for-game-programmers
	Author: Glenn Fiedler <gaffer@gaffer.org>
	Adapted by Martin Wickham <martin@wikiwickham.com>
*/
#pragma once
#include <vector>
#include "CriticalSection.h"
#include "PacketQueue.h"

class ReliabilitySystem {
private:
	ReliabilitySystem &operator=(const ReliabilitySystem &other);

	unsigned int max_sequence;
	unsigned int local_sequence;		// local sequence number for most recently sent packet
	unsigned int remote_sequence;		// remote sequence number for most recently received packet
		
	unsigned int sent_packets;			// total number of packets sent
	unsigned int recv_packets;			// total number of packets received
	unsigned int lost_packets;			// total number of packets lost
	unsigned int acked_packets;			// total number of packets acked

	float sent_bandwidth;				// approximate sent bandwidth over the last second
	float acked_bandwidth;				// approximate acked bandwidth over the last second
	float rtt;							// estimated round trip time
	float rtt_maximum;					// maximum expected round trip time (hard coded to one second for the moment)

	std::vector<unsigned int> acks;		// acked packets from last set of packet receives. cleared each update!

	PacketQueue sentQueue;				// sent packets used to calculate sent bandwidth (kept until rtt_maximum)
	PacketQueue pendingAckQueue;		// sent packets which have not been acked yet (kept until rtt_maximum * 2)
	PacketQueue receivedQueue;			// received packets for determining acks to send (kept up to most recent recv sequence - 32)
	PacketQueue ackedQueue;				// acked packets (kept until rtt_maximum * 2)

protected:
	/* Always take these locks in top-down order */
	CriticalSection thisLock;
	void AdvanceQueueTime(float deltaTime);
	void UpdateQueues();
	void UpdateStats();

	// utility functions
	static inline bool sequence_more_recent(unsigned int s1, unsigned int s2, unsigned int max_sequence) { return (s1 > s2) && (s1 - s2 <= max_sequence/2) || (s2 > s1) && (s2 - s1 > max_sequence/2); }
	static int bit_index_for_sequence(unsigned int sequence, unsigned int ack, unsigned int max_sequence);
	static unsigned int generate_ack_bits(unsigned int ack, const PacketQueue & received_queue, unsigned int max_sequence);
	static void process_ack(unsigned int ack, unsigned int ack_bits,
							PacketQueue & pending_ack_queue, PacketQueue & acked_queue,
							std::vector<unsigned int> & acks, unsigned int & acked_packets,
							float & rtt, unsigned int max_sequence);

public:
	ReliabilitySystem(unsigned int max_sequence = 0x0000FFFF);
	void Reset();
	void PacketSent(int size);
	void PacketReceived(unsigned int sequence, int size);
	unsigned int GenerateAckBits();
	void ProcessAck(unsigned int ack, unsigned int ack_bits);
	void Update(float deltaTime);
	void Validate();
	virtual ~ReliabilitySystem();

	// data accessors
	inline unsigned int GetLocalSequence() const { return local_sequence; }
	inline unsigned int GetRemoteSequence() const { return remote_sequence; }
	inline unsigned int GetMaxSequence() const { return max_sequence; }
	inline unsigned int GetSentPackets() const { return sent_packets; }
	inline unsigned int GetReceivedPackets() const { return recv_packets; }
	inline unsigned int GetLostPackets() const { return lost_packets; }
	inline unsigned int GetAckedPackets() const { return acked_packets; }
	inline float GetSentBandwidth() const { return sent_bandwidth; }
	inline float GetAckedBandwidth() const { return acked_bandwidth; }
	inline float GetRoundTripTime() const { return rtt; }
	inline int GetHeaderSize() const { return 12; }

};
