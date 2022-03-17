packet UDPSegment {
    uint16_t srcPort;
    uint16_t destPort;
	uint16_t length;
    uint16_t checksum;
    //uint8_t payload[65536];
};