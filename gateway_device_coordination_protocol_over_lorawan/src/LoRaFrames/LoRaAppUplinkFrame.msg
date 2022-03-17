struct LoRaControlUplink {
    //uint8_t FOptsLen; // Actually it is of 4 bits
    //unsigned int FOptsLen : 4; Does not work in omnet++
    bool FOptsLen[4];
    bool ClassB;
    bool ACK;
    bool ADRACKReq; // ADR acknowledgment request
    bool ADR; 		// Adaptive Data Rate
};

// When the uplink ADR bit is set, end-devices will be optimized to use
// the fastest data rate and minimum TX power possible.

// The end-device SHALL periodically validate whether the Network is still receiving the uplink frames.
// After ADR_ACK_LIMIT uplinks (ADRACKCnt ≥ ADR_ACK_LIMIT) without receiving a Class A downlink response,
// the end-device SHALL set the ADR acknowledgment request bit (ADRACKReq) on uplink
// transmissions. The Network is REQUIRED to respond with a class A downlink frame.

packet LoRaAppUplinkFrame {
    uint8_t deviceAddress[4];
    LoRaControlUplink control;
    uint16_t counter;
    uint8_t port;
    uint8_t payload[11]; // based on data rate and region specs (min 11 bytes)
};