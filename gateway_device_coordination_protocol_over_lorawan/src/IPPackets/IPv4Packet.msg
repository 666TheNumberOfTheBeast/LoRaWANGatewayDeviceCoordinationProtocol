packet IPv4Packet {
    uint8_t version; // Actually it is of 4 bits
    uint8_t IHL; 	 // Internet Header Length, actually it is of 4 bits
    uint8_t TOS;
    uint16_t totalLength;
    uint16_t identification;
    uint16_t fragmentOffset;
    uint8_t TTL;
    uint8_t protocol;
    uint16_t headerChecksum;
    uint8_t srcAddress[4];
    uint8_t destAddress[4];
    uint8_t options[40];
    uint8_t payload[65536];
};