packet TCPSegment {
    uint16_t srcPort;
    uint16_t destPort;
    uint32_t sequenceNumber;
    uint32_t ACKnumber;
	uint8_t dataOffset; // Actually it is of 4 bits
	uint8_t reserved; 	// Actually it is of 3 bits
	bool NS;  // ECN-nonce
	bool CWR; // Congestion window reduced
	bool ECE; // ECN-Echo
	bool URG; // Indicates that the Urgent pointer field is significant
	bool ACK; // Indicates that the Acknowledgment field is significant
	bool PSH; // Push function
	bool RST; // Reset the connection
	bool SYN; // Synchronize sequence numbers
	bool FIN; // Last packet from sender
	uint16_t windowSize;
    uint16_t checksum;
    uint16_t urgentPointer;
    uint8_t options[40];
    //uint8_t payload[65536];
};