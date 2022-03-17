packet LoRaJoinRequestFrame {
    uint8_t joinEUI[8];
    uint8_t devEUI[8];
    uint16_t devNonce;
};