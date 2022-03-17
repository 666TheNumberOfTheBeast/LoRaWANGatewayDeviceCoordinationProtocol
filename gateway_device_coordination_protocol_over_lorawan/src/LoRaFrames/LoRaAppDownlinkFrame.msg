struct LoRaControlDownlink {
    //uint8_t FOptsLen; // Actually it is of 4 bits
    //unsigned int FOptsLen : 4; Does not work in omnet++
    bool FOptsLen[4];
    bool FPending;
    bool ACK;
    bool RFU;
    bool ADR; // Adaptive Data Rate
};

// When the downlink ADR bit is set, it informs the end-device
// that the Network Server is able to send ADR commands.

packet LoRaAppDownlinkFrame {
    uint8_t deviceAddress[4];
    LoRaControlDownlink control;
    uint16_t counter;
    uint8_t port;
    uint8_t payload[11]; // based on data rate and region specs (min 11 bytes)
};