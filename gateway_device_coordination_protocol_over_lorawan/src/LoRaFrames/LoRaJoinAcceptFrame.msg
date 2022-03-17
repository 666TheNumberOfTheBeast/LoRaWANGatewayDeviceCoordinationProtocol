packet LoRaJoinAcceptFrame {
    uint32_t joinNonce;   // Actually it is of 3 bytes
    uint8_t netID[3];
    uint8_t devAddr[4];
    uint8_t DLSettings;
    uint8_t RXDelay;
    uint8_t CFList[16];
};