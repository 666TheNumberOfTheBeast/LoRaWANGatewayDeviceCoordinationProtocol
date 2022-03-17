packet LoRaDatalinkFrame {
    uint8_t FType;       // Actually it is of 3 bits
    uint8_t RFU;         // Actually it is of 3 bits
    uint8_t major;       // Actually it is of 2 bits
    uint8_t payload[19]; // App payload based on data rate and region specs (min 11 bytes) + 8 bytes header
    uint8_t MIC[4];      // Message Integrity Code
};

// FType for six different MAC frame types: 
// - 000 Join-Request,
// - 001 Join-Accept,
// - 010 unconfirmed data uplink,
// - 011 unconfirmed data downlink,
// - 100 confirmed data uplink,
// - 101 confirmed data downlink,
// - 110 RFU,
// - 111 Proprietary