packet LoRaPhysicalFrame {
    uint32_t preamble;     // Actually it is of 8 symbols
    uint16_t syncWord;     // Actually it is of 4.25 symbols
    uint8_t payloadLength; // Actually it is of 8 symbols	|
    uint8_t FECRate;       // Forward Error Correction rate | => Header (8 simbols size)
    bool CRCPresence;	   //							    |
    uint16_t headerCRC;    // Cyclic Redundancy Check
    
    // ======================
    // These parameters are not explicitly present in the physical frame
    uint8_t transmissionPower;
    float channelFrequency;
    uint8_t codingRate;
    uint8_t spreadingFactor;
    float bandwidth;
    bool IQPolarizationInverted;
    // ======================
    
    uint8_t payload[59];   // App payload based on data rate and region specs (min 11 bytes)
    uint16_t payloadCRC;   // Cyclic Redundancy Check
};
