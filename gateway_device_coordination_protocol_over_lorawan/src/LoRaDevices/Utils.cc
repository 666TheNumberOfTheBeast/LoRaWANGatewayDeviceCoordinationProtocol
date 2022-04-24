#include "Utils.h"


// ********************* MESSAGE ARRAY GETTERS & SETTERS *********************
// OMNeT++ getter & setter methods for arrays in messages return the item located at the array index k
// instead of the full-array. So, the functions copy the arrays index by index

// XXX: use a single method for set and another for get instead of n and n

void getArrayInMessageJoinRequest(
        LoRaJoinRequestFrame* instance,
        uint8_t (LoRaJoinRequestFrame::*funcp)(size_t) const,
        uint8_t* dest, uint8_t size) {
    if (!instance || !dest)
            return;

    for (int i=0; i<size; i++)
        dest[i] = (instance->*funcp)(i);
}

void getArrayInMessageJoinAccept(
        LoRaJoinAcceptFrame* instance,
        uint8_t (LoRaJoinAcceptFrame::*funcp)(size_t) const,
        uint8_t* dest, uint8_t size) {
    if (!instance || !dest)
            return;

    for (int i=0; i<size; i++)
        dest[i] = (instance->*funcp)(i);
}

void getArrayInMessageUplink(
        LoRaAppUplinkFrame* instance,
        uint8_t (LoRaAppUplinkFrame::*funcp)(size_t) const,
        uint8_t* dest, uint8_t size) {
    if (!instance || !dest)
            return;

    for (int i=0; i<size; i++)
        dest[i] = (instance->*funcp)(i);
}

void getArrayInMessageDownlink(
        LoRaAppDownlinkFrame* instance,
        uint8_t (LoRaAppDownlinkFrame::*funcp)(size_t) const,
        uint8_t* dest, uint8_t size) {
    if (!instance || !dest)
            return;

    for (int i=0; i<size; i++)
        dest[i] = (instance->*funcp)(i);
}

void getArrayInMessageDatalink(
        LoRaDatalinkFrame* instance,
        uint8_t (LoRaDatalinkFrame::*funcp)(size_t) const,
        uint8_t* dest, uint8_t size) {
    if (!instance || !dest)
            return;

    for (int i=0; i<size; i++)
        dest[i] = (instance->*funcp)(i);
}

void getArrayInMessageIp(
        IPv4Packet* instance,
        uint8_t (IPv4Packet::*funcp)(size_t) const,
        uint8_t* dest, uint8_t size) {
    if (!instance || !dest)
            return;

    for (int i=0; i<size; i++)
        dest[i] = (instance->*funcp)(i);
}


void setArrayInMessageJoinRequest(
        LoRaJoinRequestFrame* instance,
        void (LoRaJoinRequestFrame::*funcp)(size_t, uint8_t),
        uint8_t* src, uint8_t size) {
    if (!instance || !src)
            return;

    for (int i=0; i<size; i++)
        (instance->*funcp)(i, src[i]);
}

void setArrayInMessageJoinAccept(
        LoRaJoinAcceptFrame* instance,
        void (LoRaJoinAcceptFrame::*funcp)(size_t, uint8_t),
        uint8_t* src, uint8_t size) {
    if (!instance || !src)
            return;

    for (int i=0; i<size; i++)
        (instance->*funcp)(i, src[i]);
}

void setArrayInMessageUplink(
        LoRaAppUplinkFrame* instance,
        void (LoRaAppUplinkFrame::*funcp)(size_t, uint8_t),
        uint8_t* src, uint8_t size) {
    if (!instance || !src)
            return;

    for (int i=0; i<size; i++)
        (instance->*funcp)(i, src[i]);
}

void setArrayInMessageDownlink(
        LoRaAppDownlinkFrame* instance,
        void (LoRaAppDownlinkFrame::*funcp)(size_t, uint8_t),
        uint8_t* src, uint8_t size) {
    if (!instance || !src)
            return;

    for (int i=0; i<size; i++)
        (instance->*funcp)(i, src[i]);
}

void setArrayInMessageDatalink(
        LoRaDatalinkFrame* instance,
        void (LoRaDatalinkFrame::*funcp)(size_t, uint8_t),
        uint8_t* src, uint8_t size) {
    if (!instance || !src)
            return;

    for (int i=0; i<size; i++)
        (instance->*funcp)(i, src[i]);
}

void setArrayInMessageIp(
        IPv4Packet* instance,
        void (IPv4Packet::*funcp)(size_t, uint8_t),
        uint8_t* src, uint8_t size) {
    if (!instance || !src)
            return;

    for (int i=0; i<size; i++)
        (instance->*funcp)(i, src[i]);
}

// ********************* MESSAGE ARRAY GETTERS & SETTERS END *********************



// ********************* MESSAGE VALIDATIONS *********************

/*uint8_t isValidMessageIp(
        omnetpp::cMessage* msg, uint8_t* address,
        std::map<std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::tuple<omnetpp::cMessage*, omnetpp::cMessage*, uint32_t, uint32_t>>& mapTcp,
        uint8_t* srcAddress, uint8_t* destAddress, uint32_t sequenceNumber, uint32_t ackNumber, uint8_t* payload) {
    IPv4Packet* ipMsg = dynamic_cast<IPv4Packet*>(msg);
    if (!ipMsg)
        return BAD_FRAME;

    // The frame has been sent over IP.
    getArrayInMessageIp(ipMsg, &IPv4Packet::getDestAddress, destAddress, IPv4_ADDRESS_SIZE);

    // Check if the packet is intended for the current network server IP address
    if (memcmp(address, destAddress, IPv4_ADDRESS_SIZE))
        return BAD_ADDRESS;

    getArrayInMessageIp(ipMsg, &IPv4Packet::getSrcAddress, srcAddress, IPv4_ADDRESS_SIZE);

    // Check if the address is in the map
    std::array<uint8_t, IPv4_ADDRESS_SIZE> srcAddr;
    memcpy(srcAddr.data(), srcAddress, IPv4_ADDRESS_SIZE);

    auto it = mapTcp.find(srcAddr);
    if (it == mapTcp.end())
        // The address is not in the map
        return 0;

    // The address is in the map.
    // Check if the frame is a retransmission
    std::tuple<omnetpp::cMessage*, omnetpp::cMessage*, uint32_t, uint32_t> tuple = it->second;

    EV << "Received sequenceNumber: " << (int) sequenceNumber << "\n";
    EV << "Stored sequence number: " << (int) std::get<2>(tuple) << "\n";
    if (sequenceNumber <= std::get<2>(tuple))
        return BAD_COUNTER;

    // Simplified version of TCP/IP control without packet ordering

    // Get encapsulated packet
    omnetpp::cPacket* encapsulatedPacket = ipMsg->getEncapsulatedPacket();

    // Check if the packet has an encapsulated packet
    if (encapsulatedPacket == nullptr)
        // No, get the IP packet payload
        getArrayInMessageIp(ipMsg, &IPv4Packet::getPayload, payload, LORA_FRAME_SIZE_APP_PAYLOAD);

    return 0;
}

uint8_t isValidMessageTcp(omnetpp::cMessage* msg, unsigned* port, uint32_t* sequenceNumber, uint32_t* ackNumber) {
    TCPSegment* tcpMsg = dynamic_cast<TCPSegment*>(msg);
    if (!tcpMsg)
        return BAD_FRAME;

    // The frame has been sent over TCP.
    // Check if the message port is valid
    *port = tcpMsg->getDestPort();
    if (!isValidPort(*port, false))
        return BAD_PORT;

    // Get sequence and ACK numbers
    *sequenceNumber = tcpMsg->getSequenceNumber();
    *ackNumber      = tcpMsg->getACKnumber();

    return 0;
}

uint8_t isValidMessageUdp(omnetpp::cMessage* msg, unsigned* port) {
    UDPSegment* udpMsg = dynamic_cast<UDPSegment*>(msg);
    if (!udpMsg)
        return BAD_FRAME;

    // The frame has been sent over UDP.
    // Check if the message port is valid
    *port = udpMsg->getDestPort();
    if (!isValidPort(*port, false))
        return BAD_PORT;

    return 0;
}*/

// ********************* MESSAGE VALIDATIONS END *********************



// ********************* MIC *********************

// Compute AES 128 CMAC of the message using the provided key and storing the result in the cmac buffer
//void aes128_cmac(const char* key, uint8_t* message, uint8_t* cmac, std::ostream& stream) {
//void aes128_cmac(const char* key, uint8_t* message, uint8_t* cmac) {
void aes128CMAC(uint8_t* key, uint8_t* message, size_t messageSize, uint8_t* cmac) {
    if (!key || !message || !cmac || messageSize <= 0)
        return;

    //=========================
    // For OpenSSL 1.1.1

    size_t macSize;

    std::cout << "message length = " << messageSize << " bytes (" << messageSize*8 << " bits)\n";

    CMAC_CTX* ctx = CMAC_CTX_new();
    //===================================
    //int res = CMAC_Init(ctx, key, KEY_LORAWAN_SIZE, EVP_aes_128_cbc(), nullptr);
    //std::cout << "CMAC_Init success? " << std::boolalpha << (res ? true : false) << "\n";
    //res = CMAC_Update(ctx, message, messageSize);
    //std::cout << "CMAC_Update success? " << (res ? true : false) << "\n";
    //res = CMAC_Final(ctx, cmac, &macSize);
    //std::cout << "CMAC_Final success? " << (res ? true : false) << std::dec << "\n";
    //===================================

    //===================================
    CMAC_Init(ctx, key, KEY_LORAWAN_SIZE, EVP_aes_128_cbc(), nullptr);
    CMAC_Update(ctx, message, messageSize);
    CMAC_Final(ctx, cmac, &macSize);
    //===================================
    CMAC_CTX_free(ctx);


    /*stream << "CMAC: ";
    for(int i=0; i < macSize; i++)
        stream << std::hex << (unsigned) cmac[i];
    stream << std::dec << "\nCMAC len: " << macSize << "\n";*/
    //=========================


    //=========================
    // EXAMPLE
    // For OpenSSL 1.1.1

    // K, M and T from
    // http://csrc.nist.gov/publications/nistpubs/800-38B/Updated_CMAC_Examples.pdf
    // D.1 AES-128

    // K: 2b7e1516 28aed2a6 abf71588 09cf4f3c
    /*unsigned char keyy[16] = {
        0x2b,0x7e,0x15,0x16,
        0x28,0xae,0xd2,0xa6,
        0xab,0xf7,0x15,0x88,
        0x09,0xcf,0x4f,0x3c };

    // M: 6bc1bee2 2e409f96 e93d7e11 7393172a Mlen: 128
    unsigned char message[16] = {
        0x6b,0xc1,0xbe,0xe2,
        0x2e,0x40,0x9f,0x96,
        0xe9,0x3d,0x7e,0x11,
        0x73,0x93,0x17,0x2a };

    unsigned char mac[16] = {};
    size_t maclen;

    CMAC_CTX* ctx = CMAC_CTX_new();
    CMAC_Init(ctx, keyy, 16, EVP_aes_128_cbc(), NULL);
    stream << "message length = " << sizeof(message) << " bytes (" << sizeof(message)*8 << " bits)\n";

    CMAC_Update(ctx, message, sizeof(message));
    CMAC_Final(ctx, mac, &maclen);

    stream << "CMAC: ";
    for(int i=0; i < maclen; i++)
        stream << std::hex << (unsigned) mac[i];
    stream << std::dec << "\n";

    // expected result T = 070a16b4 6b4d4144 f79bdd9d d04a287c

    CMAC_CTX_free(ctx);
    return 0;*/
    //=========================


    //=========================
    // For OpenSSL 3.0
    // EXAMPLE WITH CMAC
    // https://www.openssl.org/docs/man3.0/man3/EVP_MAC_CTX.html

    /*EVP_MAC *mac = EVP_MAC_fetch(NULL, "CMAC", NULL);
    const char *cipher = "AES-128-CBC";
    const char *digest = "MY_MAC_DIGEST";
    const char *key = "MY_KEY";
    EVP_MAC_CTX *ctx = NULL;

    unsigned char buf[4096];
    size_t read_l;
    size_t final_l;

    size_t i;

    OSSL_PARAM params[3];
    size_t params_n = 0;

    if (cipher != NULL)
        params[params_n++] =
            OSSL_PARAM_construct_utf8_string("cipher", (char*)cipher, 0);
    if (digest != NULL)
        params[params_n++] =
            OSSL_PARAM_construct_utf8_string("digest", (char*)digest, 0);
    params[params_n] = OSSL_PARAM_construct_end();

    if (mac == NULL
        || key == NULL
        || (ctx = EVP_MAC_CTX_new(mac)) == NULL
        || !EVP_MAC_init(ctx, (const unsigned char *)key, strlen(key),
                         params))
        goto err;

    while ( (read_l = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
        if (!EVP_MAC_update(ctx, buf, read_l))
            goto err;
    }

    if (!EVP_MAC_final(ctx, buf, &final_l, sizeof(buf)))
        goto err;

    printf("Result: ");
    for (i = 0; i < final_l; i++)
        printf("%02X", buf[i]);
    printf("\n");

    EVP_MAC_CTX_free(ctx);
    EVP_MAC_free(mac);
    exit(0);

    err:
        EVP_MAC_CTX_free(ctx);
        EVP_MAC_free(mac);
        fprintf(stderr, "Something went wrong\n");
        ERR_print_errors_fp(stderr);
        exit (1);*/
    //==========================
}

void createMessageBlockMIC(
        uint8_t dlHeader,
        uint8_t* deviceAddress, uint8_t control, uint8_t* counter, uint8_t port, uint8_t* payload,
        uint8_t dir, uint8_t len,
        uint8_t* dest) {
    size_t msgSize = 16 + LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;
    uint8_t block[msgSize] = {
        // Block zero
        0x49,
        0x0,
        0x0,
        0x0,
        0x0,
        dir,
        deviceAddress[0],
        deviceAddress[1],
        deviceAddress[2],
        deviceAddress[3],
        counter[0],
        counter[1],
        counter[2],
        counter[3],
        0x0,
        len,
        // Append datalink (MAC) header fields
        dlHeader,
        // Append frame (APP) header fields
        deviceAddress[0],
        deviceAddress[1],
        deviceAddress[2],
        deviceAddress[3],
        control,
        counter[0],
        counter[1],
        port
    };

    // Append the message payload to the block
    memcpy(&block[16+LORA_FRAME_SIZE_DATALINK_HEADER+LORA_FRAME_SIZE_APP_HEADER], payload, LORA_FRAME_SIZE_APP_PAYLOAD);

    // Copy in the destination
    memcpy(dest, block, msgSize);
}

void createMessageBlockMIC(LoRaDatalinkFrame* datalinkMsg, LoRaAppUplinkFrame* appMsg, uint8_t* dest) {
    if (!datalinkMsg || !appMsg || !dest)
        return;

    // Get frame header fields
    uint8_t deviceAddress[IPv4_ADDRESS_SIZE];
    getArrayInMessageUplink(appMsg, &LoRaAppUplinkFrame::getDeviceAddress, deviceAddress, IPv4_ADDRESS_SIZE);
    LoRaControlUplink control = appMsg->getControl();
    uint16_t counter          = appMsg->getCounter();
    uint8_t port              = appMsg->getPort();

    // Get the payload
    uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {};
    getArrayInMessageUplink(appMsg, &LoRaAppUplinkFrame::getPayload, payload, LORA_FRAME_SIZE_APP_PAYLOAD);

    // Convert the control field from 8 single bits to a single 8 bits field
    uint8_t control_ = control.FOptsLen[0]      |
                       control.FOptsLen[1] << 1 |
                       control.FOptsLen[2] << 2 |
                       control.FOptsLen[3] << 3 |
                       control.ClassB      << 4 |
                       control.ACK         << 5 |
                       control.ADRACKReq   << 6 |
                       control.ADR         << 7;

    /*EV << "control: " << std::bitset<8>(control_) << "\n";
    EV << "control.FOptsLen[0]: " << control.FOptsLen[0] << "\n";
    EV << "control.FOptsLen[1]: " << control.FOptsLen[1] << "\n";
    EV << "control.FOptsLen[2]: " << control.FOptsLen[2] << "\n";
    EV << "control.FOptsLen[3]: " << control.FOptsLen[3] << "\n";
    EV << "control.FPending: " << control.FPending << "\n";
    EV << "control.ACK: " << control.ACK << "\n";
    EV << "control.RFU: " << control.RFU << "\n";
    EV << "control.ADR: " << control.ADR << "\n";*/

    // Convert the datalink header fields (3 bytes in the simulation for simplicity)
    // to a single byte as it actually is
    uint8_t dlHeader = (datalinkMsg->getMajor() & 0x3)      | // 2 bits (00000011)
                       (datalinkMsg->getRFU()   & 0x7) << 2 | // 3 bits (00000111)
                       (datalinkMsg->getFType() & 0x7) << 5;  // 3 bits (00000111)

    /*std::cout << "dlHeader: " << std::bitset<8>(dlHeader) << "\n";
    std::cout << "datalinkMsg->getFType(): " << std::bitset<8>(datalinkMsg->getFType()) << "\n";
    std::cout << "datalinkMsg->getRFU(): " << std::bitset<8>(datalinkMsg->getRFU()) << "\n";
    std::cout << "datalinkMsg->getMajor(): " << std::bitset<8>(datalinkMsg->getMajor()) << "\n";*/


    // Convert the counter to array of bytes
    uint8_t counter_[4];
    uintToBytes(counter, counter_);

    uint8_t dir = 0x0;

    // Get the lower 8 bits of the length of the frame because LoRaWAN requires a 8 bits int
    // but in OMNeT++ the function returns a 64 bits int
    uint8_t len = datalinkMsg->getByteLength() & 0xFF;

    createMessageBlockMIC(dlHeader, deviceAddress, control_, counter_, port, payload, dir, len, dest);
}

void createMessageBlockMIC(LoRaDatalinkFrame* datalinkMsg, LoRaAppDownlinkFrame* appMsg, uint8_t* dest) {
    if (!datalinkMsg || !appMsg || !dest)
        return;

    // Get frame header fields
    uint8_t deviceAddress[IPv4_ADDRESS_SIZE];
    getArrayInMessageDownlink(appMsg, &LoRaAppDownlinkFrame::getDeviceAddress, deviceAddress, IPv4_ADDRESS_SIZE);
    LoRaControlDownlink control = appMsg->getControl();
    uint16_t counter            = appMsg->getCounter();
    uint8_t port                = appMsg->getPort();

    // Get the payload
    uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {};
    getArrayInMessageDownlink(appMsg, &LoRaAppDownlinkFrame::getPayload, payload, LORA_FRAME_SIZE_APP_PAYLOAD);

    // Convert the control field from 8 single bits to a single 8 bits field
    uint8_t control_ = control.FOptsLen[0]      |
                       control.FOptsLen[1] << 1 |
                       control.FOptsLen[2] << 2 |
                       control.FOptsLen[3] << 3 |
                       control.FPending    << 4 |
                       control.ACK         << 5 |
                       control.RFU         << 6 |
                       control.ADR         << 7;

    // Convert the datalink header fields (3 bytes in the simulation for simplicity)
    // to a single byte as it actually is
    uint8_t dlHeader = (datalinkMsg->getMajor() & 0x3)      | // 2 bits (00000011)
                       (datalinkMsg->getRFU()   & 0x7) << 2 | // 3 bits (00000111)
                       (datalinkMsg->getFType() & 0x7) << 5;  // 3 bits (00000111)

    // Convert the counter to array of bytes
    uint8_t counter_[4];
    uintToBytes(counter, counter_);

    uint8_t dir = 0x1;

    // Get the lower 8 bits of the length of the frame because LoRaWAN requires a 8 bits int
    // but in OMNeT++ the function returns a 64 bits int
    uint8_t len = datalinkMsg->getByteLength() & 0xFF;

    createMessageBlockMIC(dlHeader, deviceAddress, control_, counter_, port, payload, dir, len, dest);
}

void calculateMICUplink(LoRaDatalinkFrame* datalinkMsg, LoRaAppUplinkFrame* appMsg, uint8_t* keyMIC) {
    if (!datalinkMsg || !appMsg || !keyMIC)
        return;

    std::cout << "======================\n";

    size_t msgSize = 16 + LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;
    uint8_t msg[msgSize] = {};
    createMessageBlockMIC(datalinkMsg, appMsg, msg);

    // Compute the MIC of the message
    uint8_t cmac[16] = {};
    aes128CMAC(keyMIC, msg, msgSize, cmac);

    printKey(cmac, sizeof(cmac), "CMAC", std::cout);

    setArrayInMessageDatalink(datalinkMsg, &LoRaDatalinkFrame::setMIC, cmac, LORA_FRAME_SIZE_DATALINK_MIC);
}

void calculateMICDownlink(LoRaDatalinkFrame* datalinkMsg, LoRaAppDownlinkFrame* appMsg, uint8_t* keyMIC) {
    if (!datalinkMsg || !appMsg || !keyMIC)
        return;

    std::cout << "======================\n";

    size_t msgSize = 16 + LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;
    uint8_t msg[msgSize] = {};
    createMessageBlockMIC(datalinkMsg, appMsg, msg);

    // Compute the MIC of the message
    uint8_t cmac[16] = {};
    aes128CMAC(keyMIC, msg, msgSize, cmac);

    printKey(cmac, sizeof(cmac), "CMAC", std::cout);

    setArrayInMessageDatalink(datalinkMsg, &LoRaDatalinkFrame::setMIC, cmac, LORA_FRAME_SIZE_DATALINK_MIC);
}

//void calculateMIC(LoRaDatalinkFrame* datalinkMsg, omnetpp::cMessage* appMsg, uint8_t* keyMIC) {
void calculateMIC(LoRaDatalinkFrame* datalinkMsg, uint8_t* keyMIC) {
    if (!datalinkMsg || !keyMIC)
        return;

    LoRaAppUplinkFrame* appMsg_ = dynamic_cast<LoRaAppUplinkFrame*>(datalinkMsg->getEncapsulatedPacket());
    if (appMsg_) {
        calculateMICUplink(datalinkMsg, appMsg_, keyMIC);
        return;
    }

    LoRaAppDownlinkFrame* appMsg__ = dynamic_cast<LoRaAppDownlinkFrame*>(datalinkMsg->getEncapsulatedPacket());
    if (appMsg__) {
        calculateMICDownlink(datalinkMsg, appMsg__, keyMIC);
        return;
    }

    /*LoRaJoinRequestFrame* joinRequestMsg = dynamic_cast<LoRaJoinRequestFrame*>(datalinkMsg->getEncapsulatedPacket());
    if (joinRequestMsg)
        return calculateMICJoinRequest(datalinkMsg, joinRequestMsg, keys, keysSize);

    LoRaJoinAcceptFrame* joinAcceptMsg = dynamic_cast<LoRaJoinAcceptFrame*>(datalinkMsg->getEncapsulatedPacket());
    if (joinAcceptMsg)
        return calculateMICJoinAccept(datalinkMsg, joinAcceptMsg, keys, keysSize);*/
}
// ********************* MIC END *********************

// ********************* VALIDATE MIC *********************

bool isValidMICJoinRequest(LoRaDatalinkFrame* datalinkMsg, LoRaJoinRequestFrame* joinMsg, uint8_t* keys[], int keysSize) {
    if (!datalinkMsg || !joinMsg || !keys)
        return false;

    // Get the MIC of the message
    uint8_t msgMIC[4];
    getArrayInMessageDatalink(datalinkMsg, &LoRaDatalinkFrame::getMIC, msgMIC, 4);

    // Get frame fields
    uint8_t joinEUI[EUI_SIZE];
    uint8_t devEUI[EUI_SIZE];
    getArrayInMessageJoinRequest(joinMsg, &LoRaJoinRequestFrame::getJoinEUI, joinEUI, EUI_SIZE);
    getArrayInMessageJoinRequest(joinMsg, &LoRaJoinRequestFrame::getDevEUI, devEUI, EUI_SIZE);
    uint16_t devNonce = joinMsg->getDevNonce();

    uint8_t devNonce_[4];
    uintToBytes(devNonce, devNonce_);

    size_t messageSize = 3 + EUI_SIZE*2 + 2;
    uint8_t block[messageSize] = {
        // Append MAC header fields
        datalinkMsg->getFType(),
        datalinkMsg->getRFU(),
        datalinkMsg->getMajor()
    };

    // Append frame fields
    memcpy(&block[3], joinEUI, EUI_SIZE);
    memcpy(&block[3+EUI_SIZE], devEUI, EUI_SIZE);
    memcpy(&block[3+EUI_SIZE*2], devNonce_, 2);

    // Init CMAC
    uint8_t cmac[16] = {};

    std::cout << "======================\n";
    printKey(msgMIC, sizeof(msgMIC), "msgMIC", std::cout);

    // Iterate the keys
    for (int i=0; i<keysSize; i++) {
        // Check the MIC of the message for data integrity & authentication
        // CMAC = aes128_cmac(AppKey, MHDR | JoinEUI | DevEUI | DevNonce)
        // MIC = CMAC[0..3]

        // Compute the MIC of the message using the selected key
        uint8_t* key = keys[i];
        aes128CMAC(key, block, messageSize, cmac);

        printKey(cmac, sizeof(cmac), "CMAC", std::cout);

        // Compare the MIC of the message with the computed one
        if (!memcmp(msgMIC, cmac, LORA_FRAME_SIZE_DATALINK_MIC))
            return true;
    }

    return false;
}

bool isValidMICJoinAccept(LoRaDatalinkFrame* datalinkMsg, LoRaJoinAcceptFrame* joinMsg, uint8_t* keys[], int keysSize) {
    if (!datalinkMsg || !joinMsg || !keys)
        return false;

    // Get the MIC of the message
    uint8_t msgMIC[4];
    getArrayInMessageDatalink(datalinkMsg, &LoRaDatalinkFrame::getMIC, msgMIC, 4);

    // Get frame fields
    uint8_t networkId[NETWORK_ID_SIZE];
    uint8_t address[IPv4_ADDRESS_SIZE];
    uint8_t cfList[16];

    uint32_t joinNonce = joinMsg->getJoinNonce();
    getArrayInMessageJoinAccept(joinMsg, &LoRaJoinAcceptFrame::getNetID, networkId, NETWORK_ID_SIZE);
    getArrayInMessageJoinAccept(joinMsg, &LoRaJoinAcceptFrame::getDevAddr, address, IPv4_ADDRESS_SIZE);
    uint8_t DLSettings = joinMsg->getDLSettings();
    uint8_t RXDelay    = joinMsg->getRXDelay();
    getArrayInMessageJoinAccept(joinMsg, &LoRaJoinAcceptFrame::getCFList, cfList, 16);

    uint8_t joinNonce_[4];
    uintToBytes(joinNonce, joinNonce_);

    size_t messageSize = 15 + 16;
    uint8_t block[messageSize] = {
        // Append MAC header fields
        datalinkMsg->getFType(),
        datalinkMsg->getRFU(),
        datalinkMsg->getMajor(),
        // Append frame fields
        joinNonce_[0],
        joinNonce_[1],
        joinNonce_[2],
        networkId[0],
        networkId[1],
        networkId[2],
        address[0],
        address[1],
        address[2],
        address[3],
        DLSettings,
        RXDelay
    };

    // NON SO QUANTO SENSO ABBIA, TANTO VALE COPIARLA
    // Append the cfList if it contains some value
    //uint8_t voidBuffer[16] = {};
    //if (memcmp(cfList, voidBuffer, 16))
        memcpy(&block[15], cfList, 16);

    // Init CMAC
    uint8_t cmac[16] = {};

    std::cout << "======================\n";
    printKey(msgMIC, sizeof(msgMIC), "msgMIC", std::cout);

    // Iterate the keys
    for (int i=0; i<keysSize; i++) {
        // Check the MIC of the message for data integrity & authentication
        // CMAC = aes128_cmac(AppKey, MHDR | JoinNonce | NetID | DevAddr | DLSettings | RXDelay | CFList)
        // MIC = CMAC[0..3]

        // Compute the MIC of the message using the selected key
        uint8_t* key = keys[i];
        aes128CMAC(key, block, messageSize, cmac);

        printKey(cmac, sizeof(cmac), "CMAC", std::cout);

        // Compare the MIC of the message with the computed one
        if (!memcmp(msgMIC, cmac, LORA_FRAME_SIZE_DATALINK_MIC))
            return true;
    }

    return false;
}

bool isValidMICUplink(LoRaDatalinkFrame* datalinkMsg, LoRaAppUplinkFrame* appMsg, uint8_t* keys[], int keysSize) {
    if (!datalinkMsg || !appMsg || !keys)
        return false;

    // Get the MIC of the message
    uint8_t msgMIC[4];
    getArrayInMessageDatalink(datalinkMsg, &LoRaDatalinkFrame::getMIC, msgMIC, 4);

    //============
    // Get frame header fields
    /*uint8_t deviceAddress[IPv4_ADDRESS_SIZE];
    getArrayInMessageUplink(appMsg, &LoRaAppUplinkFrame::getDeviceAddress, deviceAddress, IPv4_ADDRESS_SIZE);
    LoRaControlUplink control = appMsg->getControl();
    uint16_t counter = appMsg->getCounter();
    uint8_t port = appMsg->getPort();

    // Get the payload
    uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {};
    getArrayInMessageUplink(appMsg, &LoRaAppUplinkFrame::getPayload, payload, LORA_FRAME_SIZE_APP_PAYLOAD);

    // Convert the control field from 8 single bits to a single 8 bits field
    uint8_t control_ = control.FOptsLen[0]      |
                       control.FOptsLen[1] << 1 |
                       control.FOptsLen[2] << 2 |
                       control.FOptsLen[3] << 3 |
                       control.ClassB      << 4 |
                       control.ACK         << 5 |
                       control.ADRACKReq   << 6 |
                       control.ADR         << 7;

    /*EV << "control: " << std::bitset<8>(control_) << "\n";
    EV << "control.FOptsLen[0]: " << control.FOptsLen[0] << "\n";
    EV << "control.FOptsLen[1]: " << control.FOptsLen[1] << "\n";
    EV << "control.FOptsLen[2]: " << control.FOptsLen[2] << "\n";
    EV << "control.FOptsLen[3]: " << control.FOptsLen[3] << "\n";
    EV << "control.FPending: " << control.FPending << "\n";
    EV << "control.ACK: " << control.ACK << "\n";
    EV << "control.RFU: " << control.RFU << "\n";
    EV << "control.ADR: " << control.ADR << "\n";*/

    // Convert the counter to array of bytes
    /*uint8_t counter_[4];
    uintToBytes(counter, counter_);

    uint8_t blockMsg[10] = {
        // Append MAC header fields
        datalinkMsg->getFType(),
        datalinkMsg->getRFU(),
        datalinkMsg->getMajor(),
        // Append frame header fields
        control_,
        counter_[0],
        counter_[1],
        port,
    };

    uint8_t dir = 0x0; // For uplinks

    // Get the lower 8 bits of the length of the frame because LoRaWAN requires a 8 bits int
    // but in OMNeT++ the function returns a 64 bits int
    uint8_t len = datalinkMsg->getByteLength() & 0xFF;

    size_t messageSize = 16 + sizeof(blockMsg) + LORA_FRAME_SIZE_APP_PAYLOAD;
    uint8_t block[messageSize] = {
        0x49,
        0x0,
        0x0,
        0x0,
        0x0,
        dir,
        deviceAddress[0],
        deviceAddress[1],
        deviceAddress[2],
        deviceAddress[3],
        counter_[0],
        counter_[1],
        counter_[2],
        counter_[3],
        0x0,
        len
    };

    // Append the message to the block
    memcpy(&block[16], blockMsg, sizeof(blockMsg));
    memcpy(&block[16+sizeof(blockMsg)], payload, LORA_FRAME_SIZE_APP_PAYLOAD);*/
    //============

    //============
    size_t msgSize = 16 + LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;
    uint8_t msg[msgSize] = {};
    createMessageBlockMIC(datalinkMsg, appMsg, msg);
    //============

    // Init CMAC
    uint8_t cmac[16] = {};

    std::cout << "======================\n";
    printKey(msgMIC, sizeof(msgMIC), "msgMIC", std::cout);

    // Iterate the keys
    for (int i=0; i<keysSize; i++) {
        // Check the MIC of the message for data integrity & authentication

        // Compute the MIC of the message using the selected key
        uint8_t* key = keys[i];
        //aes128CMAC(key, block, messageSize, cmac);
        aes128CMAC(key, msg, msgSize, cmac);

        printKey(cmac, sizeof(cmac), "CMAC", std::cout);

        // Compare the MIC of the message with the computed one
        if (!memcmp(msgMIC, cmac, LORA_FRAME_SIZE_DATALINK_MIC))
            return true;
    }

    return false;
}

bool isValidMICDownlink(LoRaDatalinkFrame* datalinkMsg, LoRaAppDownlinkFrame* appMsg, uint8_t* keys[], int keysSize) {
    if (!datalinkMsg || !appMsg || !keys)
        return false;

    // Get the MIC of the message
    uint8_t msgMIC[4];
    getArrayInMessageDatalink(datalinkMsg, &LoRaDatalinkFrame::getMIC, msgMIC, 4);

    //============
    // Get frame header fields
    /*uint8_t deviceAddress[IPv4_ADDRESS_SIZE];
    getArrayInMessageDownlink(appMsg, &LoRaAppDownlinkFrame::getDeviceAddress, deviceAddress, IPv4_ADDRESS_SIZE);
    LoRaControlDownlink control = appMsg->getControl();
    uint16_t counter = appMsg->getCounter();
    uint8_t port = appMsg->getPort();

    // Get the payload
    uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {};
    getArrayInMessageDownlink(appMsg, &LoRaAppDownlinkFrame::getPayload, payload, LORA_FRAME_SIZE_APP_PAYLOAD);

    // Convert the control field from 8 single bits to a single 8 bits field
    uint8_t control_ = control.FOptsLen[0]      |
                       control.FOptsLen[1] << 1 |
                       control.FOptsLen[2] << 2 |
                       control.FOptsLen[3] << 3 |
                       control.FPending    << 4 |
                       control.ACK         << 5 |
                       control.RFU         << 6 |
                       control.ADR         << 7;

    // Convert the counter to array of bytes
    uint8_t counter_[4];
    uintToBytes(counter, counter_);

    uint8_t blockMsg[10] = {
        // Append MAC header fields
        datalinkMsg->getFType(),
        datalinkMsg->getRFU(),
        datalinkMsg->getMajor(),
        // Append frame header fields
        control_,
        counter_[0],
        counter_[1],
        port,
    };

    uint8_t dir = 0x1; // For downlinks

    // Get the lower 8 bits of the length of the frame because LoRaWAN requires a 8 bits int
    // but in OMNeT++ the function returns a 64 bits int
    uint8_t len = datalinkMsg->getByteLength() & 0xFF;

    size_t messageSize = 16 + sizeof(blockMsg) + LORA_FRAME_SIZE_APP_PAYLOAD;
    uint8_t block[messageSize] = {
        0x49,
        0x0,
        0x0,
        0x0,
        0x0,
        dir,
        deviceAddress[0],
        deviceAddress[1],
        deviceAddress[2],
        deviceAddress[3],
        counter_[0],
        counter_[1],
        counter_[2],
        counter_[3],
        0x0,
        len
    };

    // Append the message to the block
    memcpy(&block[16], blockMsg, sizeof(blockMsg));
    memcpy(&block[16+sizeof(blockMsg)], payload, LORA_FRAME_SIZE_APP_PAYLOAD);*/
    //============

    //============
    size_t msgSize = 16 + LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;
    uint8_t msg[msgSize] = {};
    createMessageBlockMIC(datalinkMsg, appMsg, msg);
    //============

    // Init CMAC
    uint8_t cmac[16] = {};

    std::cout << "======================\n";
    printKey(msgMIC, sizeof(msgMIC), "msgMIC", std::cout);

    // Iterate the keys
    for (int i=0; i<keysSize; i++) {
        // Check the MIC of the message for data integrity & authentication

        // Compute the MIC of the message using the selected key
        uint8_t* key = keys[i];
        //aes128CMAC(key, block, messageSize, cmac);
        aes128CMAC(key, msg, msgSize, cmac);

        printKey(cmac, sizeof(cmac), "CMAC", std::cout);

        // Compare the MIC of the message with the computed one
        if (!memcmp(msgMIC, cmac, LORA_FRAME_SIZE_DATALINK_MIC))
            return true;
    }

    return false;
}

bool isValidMIC(LoRaDatalinkFrame* datalinkMsg, omnetpp::cMessage* appMsg, uint8_t* keys[], int keysSize) {
    if (!datalinkMsg || !appMsg || !keys)
        return false;

    LoRaAppUplinkFrame* appMsg_ = dynamic_cast<LoRaAppUplinkFrame*>(datalinkMsg->getEncapsulatedPacket());
    if (appMsg_)
        return isValidMICUplink(datalinkMsg, appMsg_, keys, keysSize);

    LoRaAppDownlinkFrame* appMsg__ = dynamic_cast<LoRaAppDownlinkFrame*>(datalinkMsg->getEncapsulatedPacket());
    if (appMsg__)
        return isValidMICDownlink(datalinkMsg, appMsg__, keys, keysSize);

    LoRaJoinRequestFrame* joinRequestMsg = dynamic_cast<LoRaJoinRequestFrame*>(datalinkMsg->getEncapsulatedPacket());
    if (joinRequestMsg)
        return isValidMICJoinRequest(datalinkMsg, joinRequestMsg, keys, keysSize);

    LoRaJoinAcceptFrame* joinAcceptMsg = dynamic_cast<LoRaJoinAcceptFrame*>(datalinkMsg->getEncapsulatedPacket());
    if (joinAcceptMsg)
        return isValidMICJoinAccept(datalinkMsg, joinAcceptMsg, keys, keysSize);

    return false;
}
// ********************* VALIDATE MIC END *********************



// ********************* MESSAGE CREATIONS *********************

omnetpp::cPacket* createMessagePhysical(
        const char* name,
        uint8_t spreadingFactor, uint8_t transmissionPower,
        float bandwidth, float channelFrequency,
        bool polarization,
        LoRaDatalinkFrame* datalinkMsg, uint8_t payloadSize) {
    if (!name || !datalinkMsg)
        return nullptr;

    LoRaPhysicalFrame* physicalMsg = new LoRaPhysicalFrame(name);
    physicalMsg->setSyncWord(SYNC_WORD);
    physicalMsg->setPayloadLength(payloadSize);

    physicalMsg->setSpreadingFactor(spreadingFactor);
    physicalMsg->setTransmissionPower(transmissionPower);
    physicalMsg->setBandwidth(bandwidth);
    physicalMsg->setChannelFrequency(channelFrequency);
    physicalMsg->setCodingRate(CODING_RATE);
    physicalMsg->setIQPolarizationInverted(polarization);

    physicalMsg->encapsulate(datalinkMsg);

    return physicalMsg;
}

omnetpp::cPacket* createMessageJoinRequest(
        const char* name,
        uint8_t* joinEUI, uint8_t* devEUI, uint16_t devNonce,
        uint8_t* keyMIC,
        uint8_t spreadingFactor, uint8_t transmissionPower,
        float bandwidth, float channelFrequency) {
    if (!name || !joinEUI || !devEUI || !keyMIC)
        return nullptr;

    LoRaJoinRequestFrame* joinRequestMsg = new LoRaJoinRequestFrame(name);
    setArrayInMessageJoinRequest(joinRequestMsg, &LoRaJoinRequestFrame::setJoinEUI, joinEUI, EUI_SIZE);
    setArrayInMessageJoinRequest(joinRequestMsg, &LoRaJoinRequestFrame::setDevEUI, devEUI, EUI_SIZE);
    joinRequestMsg->setDevNonce(devNonce);

    LoRaDatalinkFrame* datalinkMsg = new LoRaDatalinkFrame(name);
    datalinkMsg->setFType(JOIN_REQUEST);

    // Calculate MIC.
    // Convert nonce to array of bytes
    uint8_t devNonce_[4];
    uintToBytes(devNonce, devNonce_);

    size_t messageSize = 3 + EUI_SIZE*2 + 2;
    uint8_t block[messageSize] = {
        // Append MAC header fields
        datalinkMsg->getFType(),
        datalinkMsg->getRFU(),
        datalinkMsg->getMajor()
    };

    // Append frame fields
    memcpy(&block[3], joinEUI, EUI_SIZE);
    memcpy(&block[3+EUI_SIZE], devEUI, EUI_SIZE);
    memcpy(&block[3+EUI_SIZE*2], devNonce_, 2);

    std::cout << "======================\n";

    // Compute the MIC of the message
    uint8_t cmac[16] = {};
    aes128CMAC(keyMIC, block, messageSize, cmac);

    printKey(cmac, sizeof(cmac), "CMAC", std::cout);

    setArrayInMessageDatalink(datalinkMsg, &LoRaDatalinkFrame::setMIC, cmac, 4);
    datalinkMsg->encapsulate(joinRequestMsg);

    return createMessagePhysical(
        name,
        spreadingFactor, transmissionPower,
        bandwidth, channelFrequency,
        false, datalinkMsg, LORA_FRAME_SIZE_JOIN_REQUEST);
}

omnetpp::cPacket* createMessageJoinAccept(
        const char* name,
        uint32_t joinNonce, uint8_t* netID, uint8_t* endDeviceAddress,
        uint8_t DLSettings, uint8_t RXDelay, uint8_t* CFList,
        uint8_t* keyMIC,
        uint8_t spreadingFactor, uint8_t transmissionPower,
        float bandwidth, float channelFrequency) {
    if (!name || !netID || !endDeviceAddress || !keyMIC)
        return nullptr;

    LoRaJoinAcceptFrame* joinAcceptMsg = new LoRaJoinAcceptFrame(name);
    joinAcceptMsg->setJoinNonce(joinNonce);
    setArrayInMessageJoinAccept(joinAcceptMsg, &LoRaJoinAcceptFrame::setNetID, netID, NETWORK_ID_SIZE);
    setArrayInMessageJoinAccept(joinAcceptMsg, &LoRaJoinAcceptFrame::setDevAddr, endDeviceAddress, IPv4_ADDRESS_SIZE);
    joinAcceptMsg->setDLSettings(DLSettings);
    joinAcceptMsg->setRXDelay(RXDelay);

    if (CFList)
        setArrayInMessageJoinAccept(joinAcceptMsg, &LoRaJoinAcceptFrame::setCFList, CFList, sizeof(CFList));

    LoRaDatalinkFrame* datalinkMsg = new LoRaDatalinkFrame(name);
    datalinkMsg->setFType(JOIN_ACCEPT);

    // Calculate MIC.
    // Convert nonce to array of bytes
    uint8_t joinNonce_[4];
    uintToBytes(joinNonce, joinNonce_);

    size_t messageSize = 15 + 16;
    uint8_t block[messageSize] = {
        // Append MAC header fields
        datalinkMsg->getFType(),
        datalinkMsg->getRFU(),
        datalinkMsg->getMajor(),
        // Append frame fields
        joinNonce_[0],
        joinNonce_[1],
        joinNonce_[2],
        netID[0],
        netID[1],
        netID[2],
        endDeviceAddress[0],
        endDeviceAddress[1],
        endDeviceAddress[2],
        endDeviceAddress[3],
        DLSettings,
        RXDelay
    };

    if (CFList)
        memcpy(&block[15], CFList, 16);

    std::cout << "======================\n";

    // Compute the MIC of the message
    uint8_t cmac[16] = {};
    aes128CMAC(keyMIC, block, messageSize, cmac);

    printKey(cmac, sizeof(cmac), "CMAC", std::cout);

    setArrayInMessageDatalink(datalinkMsg, &LoRaDatalinkFrame::setMIC, cmac, 4);
    datalinkMsg->encapsulate(joinAcceptMsg);

    return createMessagePhysical(
        name,
        spreadingFactor, transmissionPower,
        bandwidth, channelFrequency,
        true, datalinkMsg, LORA_FRAME_SIZE_JOIN_ACCEPT);
}

omnetpp::cPacket* createMessageUplink(
        const char* name, bool ack,
        uint8_t* endDeviceAddress, uint8_t endDeviceAddressSize,
        uint16_t counter, uint8_t port,
        uint8_t* payload, uint8_t payloadSize,
        uint8_t* keyMIC,
        uint8_t spreadingFactor, uint8_t transmissionPower,
        float bandwidth, float channelFrequency) {
    if (!name || !endDeviceAddress || !payload || !keyMIC)
        return nullptr;

    LoRaControlUplink* control = new LoRaControlUplink();
    control->ACK = ack;

    LoRaAppUplinkFrame* appMsg = new LoRaAppUplinkFrame(name);
    setArrayInMessageUplink(appMsg, &LoRaAppUplinkFrame::setDeviceAddress, endDeviceAddress, endDeviceAddressSize);
    appMsg->setControl(*control);
    appMsg->setCounter(counter);
    appMsg->setPort(port);
    setArrayInMessageUplink(appMsg, &LoRaAppUplinkFrame::setPayload, payload, payloadSize);

    LoRaDatalinkFrame* datalinkMsg = new LoRaDatalinkFrame(name);
    ack ? datalinkMsg->setFType(UPLINK_CONFIRMED) : datalinkMsg->setFType(UPLINK_UNCONFIRMED);

    //===========================
    // Calculate MIC.
    // Convert the control field from 8 single bits to a single 8 bits field
    /*uint8_t control_ = control->FOptsLen[0]      |
                       control->FOptsLen[1] << 1 |
                       control->FOptsLen[2] << 2 |
                       control->FOptsLen[3] << 3 |
                       control->ClassB      << 4 |
                       control->ACK         << 5 |
                       control->ADRACKReq   << 6 |
                       control->ADR         << 7;

    // Convert the counter to array of bytes
    uint8_t counter_[4];
    uintToBytes(counter, counter_);

    uint8_t blockMsg[10] = {
        // Append MAC header fields
        datalinkMsg->getFType(),
        datalinkMsg->getRFU(),
        datalinkMsg->getMajor(),
        // Append frame header fields
        control_,
        counter_[0],
        counter_[1],
        port,
    };

    uint8_t dir = 0x0;

    // Get the lower 8 bits of the length of the frame because LoRaWAN requires a 8 bits int
    // but in OMNeT++ the function returns a 64 bits int
    uint8_t len = datalinkMsg->getByteLength() & 0xFF;

    size_t messageSize = 16 + sizeof(blockMsg) + LORA_FRAME_SIZE_APP_PAYLOAD;
    uint8_t block[messageSize] = {
        0x49,
        0x0,
        0x0,
        0x0,
        0x0,
        dir,
        endDeviceAddress[0],
        endDeviceAddress[1],
        endDeviceAddress[2],
        endDeviceAddress[3],
        counter_[0],
        counter_[1],
        counter_[2],
        counter_[3],
        0x0,
        len
    };

    // Append the message to the block
    memcpy(&block[16], blockMsg, sizeof(blockMsg));
    memcpy(&block[16+sizeof(blockMsg)], payload, LORA_FRAME_SIZE_APP_PAYLOAD);*/
    //========================

    //============
    //========================
    /*size_t msgSize = 16 + LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;
    uint8_t msg[msgSize] = {};
    createMessageBlockMIC(datalinkMsg, appMsg, msg);
    //========================

    std::cout << "======================\n";

    // Compute the MIC of the message
    uint8_t cmac[16] = {};
    //aes128CMAC(keyMIC, block, messageSize, cmac);
    aes128CMAC(keyMIC, msg, msgSize, cmac);

    printKey(cmac, sizeof(cmac), "CMAC", std::cout);

    setArrayInMessageDatalink(datalinkMsg, &LoRaDatalinkFrame::setMIC, cmac, LORA_FRAME_SIZE_DATALINK_MIC);
    datalinkMsg->encapsulate(appMsg);*/
    //============

    //============
    datalinkMsg->encapsulate(appMsg);
    //calculateMIC(datalinkMsg, appMsg, keyMIC);
    calculateMIC(datalinkMsg, keyMIC);
    //============

    return createMessagePhysical(
        name,
        spreadingFactor, transmissionPower,
        bandwidth, channelFrequency,
        false, datalinkMsg, payloadSize);
}

omnetpp::cPacket* createMessageDownlink(
        const char* name, bool ack,
        uint8_t* endDeviceAddress, uint8_t endDeviceAddressSize,
        uint16_t counter, uint8_t port,
        uint8_t* payload, uint8_t payloadSize,
        uint8_t* keyMIC,
        uint8_t spreadingFactor, uint8_t transmissionPower,
        float bandwidth, float channelFrequency) {
    if (!name || !endDeviceAddress || !payload || !keyMIC)
        return nullptr;

    LoRaControlDownlink* control = new LoRaControlDownlink();
    control->ACK = ack;

    LoRaAppDownlinkFrame* appMsg = new LoRaAppDownlinkFrame(name);
    setArrayInMessageDownlink(appMsg, &LoRaAppDownlinkFrame::setDeviceAddress, endDeviceAddress, endDeviceAddressSize);
    appMsg->setControl(*control);
    appMsg->setCounter(counter);
    appMsg->setPort(port);
    setArrayInMessageDownlink(appMsg, &LoRaAppDownlinkFrame::setPayload, payload, payloadSize);

    LoRaDatalinkFrame* datalinkMsg = new LoRaDatalinkFrame(name);
    ack ? datalinkMsg->setFType(DOWNLINK_CONFIRMED) : datalinkMsg->setFType(DOWNLINK_UNCONFIRMED);

    //========================
    // Calculate MIC.
    // Convert the control field from 8 single bits to a single 8 bits field
    /*uint8_t control_ = control->FOptsLen[0]      |
                       control->FOptsLen[1] << 1 |
                       control->FOptsLen[2] << 2 |
                       control->FOptsLen[3] << 3 |
                       control->FPending    << 4 |
                       control->ACK         << 5 |
                       control->RFU         << 6 |
                       control->ADR         << 7;

    // Convert the counter to array of bytes
    uint8_t counter_[4];
    uintToBytes(counter, counter_);

    uint8_t blockMsg[10] = {
        // Append MAC header fields
        datalinkMsg->getFType(),
        datalinkMsg->getRFU(),
        datalinkMsg->getMajor(),
        // Append frame header fields
        control_,
        counter_[0],
        counter_[1],
        port,
    };

    uint8_t dir = 0x1;

    // Get the lower 8 bits of the length of the frame because LoRaWAN requires a 8 bits int
    // but in OMNeT++ the function returns a 64 bits int
    uint8_t len = datalinkMsg->getByteLength() & 0xFF;

    size_t messageSize = 16 + sizeof(blockMsg) + LORA_FRAME_SIZE_APP_PAYLOAD;
    uint8_t block[messageSize] = {
        0x49,
        0x0,
        0x0,
        0x0,
        0x0,
        dir,
        endDeviceAddress[0],
        endDeviceAddress[1],
        endDeviceAddress[2],
        endDeviceAddress[3],
        counter_[0],
        counter_[1],
        counter_[2],
        counter_[3],
        0x0,
        len
    };

    // Append the message to the block
    memcpy(&block[16], blockMsg, sizeof(blockMsg));
    memcpy(&block[16+sizeof(blockMsg)], payload, LORA_FRAME_SIZE_APP_PAYLOAD);*/
    //========================

    //============
    //========================
    /*size_t msgSize = 16 + LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;
    uint8_t msg[msgSize] = {};
    createMessageBlockMIC(datalinkMsg, appMsg, msg);
    //========================

    std::cout << "======================\n";

    // Compute the MIC of the message
    uint8_t cmac[16] = {};
    //aes128CMAC(keyMIC, block, messageSize, cmac);
    aes128CMAC(keyMIC, msg, msgSize, cmac);

    printKey(cmac, sizeof(cmac), "CMAC", std::cout);

    setArrayInMessageDatalink(datalinkMsg, &LoRaDatalinkFrame::setMIC, cmac, LORA_FRAME_SIZE_DATALINK_MIC);
    datalinkMsg->encapsulate(appMsg);*/
    //============

    //============
    datalinkMsg->encapsulate(appMsg);
    //calculateMIC(datalinkMsg, appMsg, keyMIC);
    calculateMIC(datalinkMsg, keyMIC);
    //============

    return createMessagePhysical(
        name,
        spreadingFactor, transmissionPower,
        bandwidth, channelFrequency,
        true, datalinkMsg, payloadSize);
}

omnetpp::cPacket* createMessageIp(
        const char* name,
        uint8_t* srcAddress, uint8_t* destAddress, uint8_t addressSize,
        uint8_t* payload, uint8_t payloadSize,
        omnetpp::cPacket* encapsulatedPacket) {
    if (!name || !srcAddress || !destAddress)
        return nullptr;

    IPv4Packet* ipMsg = new IPv4Packet(name);
    setArrayInMessageIp(ipMsg, &IPv4Packet::setSrcAddress, srcAddress, addressSize);
    setArrayInMessageIp(ipMsg, &IPv4Packet::setDestAddress, destAddress, addressSize);

    if (encapsulatedPacket != nullptr)
        ipMsg->encapsulate(encapsulatedPacket);
    else
        setArrayInMessageIp(ipMsg, &IPv4Packet::setPayload, payload, payloadSize);

    return ipMsg;
}

omnetpp::cPacket* createMessageTcp(
        const char* name,
        uint16_t srcPort, uint16_t destPort,
        uint32_t seqNumber, uint32_t ackNumber, bool ack,
        uint8_t* payload, uint8_t payloadSize,
        omnetpp::cPacket* encapsulatedPacket) {
    if (!name)
        return nullptr;

    TCPSegment* msg = new TCPSegment(name);
    msg->setSrcPort(srcPort);
    msg->setDestPort(destPort);
    msg->setSequenceNumber(seqNumber);
    msg->setACKnumber(ackNumber);
    msg->setACK(ack);

    if (encapsulatedPacket != nullptr)
        msg->encapsulate(encapsulatedPacket);
    //else
    //    setArrayInMessageTcp(msg, &LoRaAppDownlinkFrame::setPayload, payload, payloadSize);

    return msg;
}

omnetpp::cPacket* createMessageUdp(
        const char* name,
        uint16_t srcPort, uint16_t destPort,
        uint8_t* payload, uint8_t payloadSize,
        omnetpp::cPacket* encapsulatedPacket) {
    if (!name)
        return nullptr;

    UDPSegment* msg = new UDPSegment(name);
    msg->setSrcPort(srcPort);
    msg->setDestPort(destPort);

    if (encapsulatedPacket != nullptr)
        msg->encapsulate(encapsulatedPacket);
    //else
    //    setArrayInMessageUdp(msg, &LoRaAppDownlinkFrame::setPayload, payload, payloadSize);

    return msg;
}


// ********************* MESSAGE CREATIONS END *********************



// ********************* MESSAGE BROADCASTING *********************

void sendBroadcast(
        omnetpp::cSimpleModule* instance, omnetpp::cMessage* msg, omnetpp::simtime_t delay, const char* gatename) {
    if (!instance || !msg || !gatename)
        return;

    // $o and $i suffix is used to identify the input/output part of a two way gate
    /*char gate_[50];
    strncpy(gate_, gateName, 50);
    strncat(gate_, "$o", 2);*/

    // In OMNeT++, broadcast must be implemented through a loop that creates copies (duplicates) of the message object.
    int outGateBaseId = instance->gateBaseId(gatename);
    //std::cout << "gate size: " << instance->gateSize(gateName) << "\n";

    //==============
    /*for (int i=0; i < INT_MAX; i++) {
        // The end device doesn't know how many gateways it is connected to,
        // catch the out of bound exception to exit from the loop
        try {
            instance->send(i==0 ? msg : msg->dup(), outGateBaseId+i);
        } catch (const std::exception& e) {
            break;
        }
    }*/
    //==============

    //==============
    int gateSize = instance->gateSize(gatename);

    for (int i=0; i < gateSize; i++) {
        // Actually the end device doesn't know how many gateways it is connected to and vice versa,
        // however in the simulation it is connected to nearby gateways through a gate
        delay > 0 ? instance->sendDelayed(i==0 ? msg : msg->dup(), delay, outGateBaseId+i) :
                    instance->send(i==0 ? msg : msg->dup(), outGateBaseId+i);
    }
    //==============
}

// Send LoRa broadcast message
void sendBroadcast(omnetpp::cSimpleModule* instance, omnetpp::cMessage* msg, const char* gatename) {
    sendBroadcast(instance, msg, 0, gatename);
}

// Send delayed LoRa broadcast message
void sendDelayedBroadcast(
        omnetpp::cSimpleModule* instance, omnetpp::cMessage* msg, omnetpp::simtime_t delay, const char* gatename) {
    sendBroadcast(instance, msg, delay, gatename);
}

// ********************* MESSAGE BROADCASTING END *********************


void setChannelLoRaDatarate(omnetpp::cSimpleModule* instance, omnetpp::cPacket* msg, const char* gatename) {
    // Calculate preamble and frame airtime
    auto tuple = calculateTimeOnAir(msg);
    double airtimeFrame = std::get<1>(tuple);

    // Check if frame airtime is valid
    if (airtimeFrame < 0)
        return;

    // Convert frame time on air from ms to s
    airtimeFrame /= 1000;

    // Calculate transmission data rate in bit/s
    double datarate = calculateDatarate(msg);

    // Get gate size
    int gateSize = instance->gateSize(gatename);

    // Update channels data rate and delay
    for (int i=0; i < gateSize; i++) {
        // OUT->IN connection
        omnetpp::cGate* outGate = instance->gate(gatename, i);
        omnetpp::cDatarateChannel* channel = dynamic_cast<omnetpp::cDatarateChannel*>(outGate->getChannel());
        channel->setDatarate(datarate);
        channel->setDelay(airtimeFrame);
        channel->callInitialize();

        //EV << "deviceGate OUT:" << deviceGate << "\n";

        // IN<-OUT connection
        omnetpp::cGate* inGate = outGate->getNextGate();
        inGate = inGate->getOtherHalf();
        channel = dynamic_cast<omnetpp::cDatarateChannel*>(inGate->getChannel());
        channel->setDatarate(datarate);
        channel->setDelay(airtimeFrame);
        channel->callInitialize();

        //EV << "gatewayGate OUT:" << gatewayGate << "\n";
    }
}


// ********************* MESSAGE SENDING *********************

void sendSecurely(
        omnetpp::cSimpleModule* instance,
        std::map<omnetpp::cMessage*, std::tuple<omnetpp::cPacket*, std::string, int>>& eventTimeoutChannelTransmissions,
        omnetpp::cPacket* pkt, omnetpp::simtime_t delay, const char* gatename, int index) {
    if (!instance || !pkt || !gatename)
        return;

    //EV << "------ SEND SECURELY\n";
    //EV << "gatename: " << gatename << "\n";
    //EV << "index: " << index << "\n";


    omnetpp::simtime_t txFinishTime = 0;

    // Check if the message is sent on a single channel
    if (index >= 0) {
        // Get the finish time of channel
        omnetpp::cChannel* txChannel = instance->gate(gatename, index)->getTransmissionChannel();
        if (!txChannel)
            return;

        txFinishTime = txChannel->getTransmissionFinishTime();
    }
    else {
        // The message is sent on multiple channels (broadcast)
        int gateSize = instance->gateSize(gatename);

        // Get the max finish time among channels
        for (int i=0; i < gateSize; i++) {
            omnetpp::cChannel* txChannel = instance->gate(gatename, i)->getTransmissionChannel();
            if (!txChannel)
                return;

            omnetpp::simtime_t txFinishTime_ = txChannel->getTransmissionFinishTime();
            if (txFinishTime_ > txFinishTime)
                txFinishTime = txFinishTime_;
        }
    }

    std::cout << "txFinishTime: " << txFinishTime << "\n";


    //============================
    // Do not use sendDelayed and sendDelayedBroadcast because block the entire channel

    // Check if the channel is free after delay
    /*if (txFinishTime <= omnetpp::simTime() + delay) {
        // Check if the message is a LoRa frame to set the data rate
        LoRaPhysicalFrame* phyMsg = dynamic_cast<LoRaPhysicalFrame*>(pkt);
        if (phyMsg)
            setChannelLoRaDatarate(instance, pkt, gatename);

        if (delay > 0)
            // Channel will be free after delay => send out delayed packet
            index >= 0 ? instance->sendDelayed(pkt, delay, gatename, index) :
                         sendDelayedBroadcast(instance, pkt, delay, gatename);
        else
            // Channel is free => send out packet immediately
            index >= 0 ? instance->send(pkt, gatename, index) : sendBroadcast(instance, pkt, gatename);
    }
    else {
        // Store packet in the map and schedule timer
        //EV << "Schedule timeout\n";
        omnetpp::cMessage* eventTimeout = new omnetpp::cMessage("timeoutChannelTX");
        eventTimeoutChannelTransmissions[eventTimeout] =
                std::tuple<omnetpp::cPacket*, std::string, int> {pkt, gatename, index};

        instance->scheduleAt(txFinishTime, eventTimeout);
    }*/
    //============================


    //============================
    // Do not use sendDelayed and sendDelayedBroadcast because block the entire channel, use timers.

    // Check if the channel is free and no delayed message has to be sent
    if (delay == 0 && txFinishTime <= omnetpp::simTime()) {
        // Check if the message is a LoRa frame to set the data rate
        LoRaPhysicalFrame* phyMsg = dynamic_cast<LoRaPhysicalFrame*>(pkt);
        if (phyMsg)
            setChannelLoRaDatarate(instance, pkt, gatename);

        // Channel is free => send out the packet immediately
        index >= 0 ? instance->send(pkt, gatename, index) : sendBroadcast(instance, pkt, gatename);
    }
    else {
        // Store packet in the map and schedule timer
        //EV << "Schedule timeout\n";
        omnetpp::cMessage* eventTimeout = new omnetpp::cMessage("timeoutChannelTX");
        eventTimeoutChannelTransmissions[eventTimeout] =
                std::tuple<omnetpp::cPacket*, std::string, int> {pkt, gatename, index};

        omnetpp::simtime_t scheduledTime = omnetpp::simTime() + delay;

        // Check if the channel is free after delay
        if (txFinishTime <= scheduledTime)
            // Channel will be free after delay => schedule timer to send out the packet
            instance->scheduleAt(scheduledTime, eventTimeout);
        else
            instance->scheduleAt(txFinishTime, eventTimeout);
    }
    //============================
}

void sendSecurely(
        omnetpp::cSimpleModule* instance,
        std::map<omnetpp::cMessage*, std::tuple<omnetpp::cPacket*, std::string, int>>& eventTimeoutChannelTransmissions,
        omnetpp::cPacket* pkt, const char* gatename, int index)  {
    sendSecurely(instance, eventTimeoutChannelTransmissions, pkt, 0, gatename, index);
}

void sendDelayedSecurely(
        omnetpp::cSimpleModule* instance,
        std::map<omnetpp::cMessage*, std::tuple<omnetpp::cPacket*, std::string, int>>& eventTimeoutChannelTransmissions,
        omnetpp::cPacket* pkt, omnetpp::simtime_t delay, const char* gatename, int index) {
    sendSecurely(instance, eventTimeoutChannelTransmissions, pkt, delay, gatename, index);
}

void sendBroadcastSecurely(
        omnetpp::cSimpleModule* instance,
        std::map<omnetpp::cMessage*, std::tuple<omnetpp::cPacket*, std::string, int>>& eventTimeoutChannelTransmissions,
        omnetpp::cPacket* pkt, const char* gatename) {
    sendSecurely(instance, eventTimeoutChannelTransmissions, pkt, 0, gatename, -1);
}

void sendDelayedBroadcastSecurely(
        omnetpp::cSimpleModule* instance,
        std::map<omnetpp::cMessage*, std::tuple<omnetpp::cPacket*, std::string, int>>& eventTimeoutChannelTransmissions,
        omnetpp::cPacket* pkt, omnetpp::simtime_t delay, const char* gatename) {
    sendSecurely(instance, eventTimeoutChannelTransmissions, pkt, delay, gatename, -1);
}

// ********************* MESSAGE SENDING END *********************



// ********************* MESSAGE PHYSICAL CALCULATIONS *********************

// Calculate datarate of the frame in bits/s
double calculateDatarate(uint8_t msgSF, float msgBW, uint8_t msgCR) {
    // Calculate data rate (multiply by 1000 to convert from bits/ms to bits/s)
    return msgSF * (4.0 / (4 + msgCR)) / (pow(2, msgSF) / msgBW) * 1000;
}

// Calculate datarate of the frame in bits/s
double calculateDatarate(omnetpp::cPacket* msg) {
    if (!msg)
        return -1;

    LoRaPhysicalFrame* phyMsg = dynamic_cast<LoRaPhysicalFrame*>(msg);
    if (!phyMsg)
        return -1;

    uint8_t msgSF = phyMsg->getSpreadingFactor();
    float msgBW   = phyMsg->getBandwidth();
    uint8_t msgCR = phyMsg->getCodingRate();

    return calculateDatarate(msgSF, msgBW, msgCR);
}

// Calculate expected time on air of preamble and entire frame in ms
//std::tuple<double, double> calculateTimeOnAir(omnetpp::cPacket* msg)
std::tuple<double, double> calculateTimeOnAir(omnetpp::cMessage* msg) {
    if (!msg)
        return std::tuple<double, double> {-1, -1};

    LoRaPhysicalFrame* phyMsg = dynamic_cast<LoRaPhysicalFrame*>(msg);
    if (!phyMsg)
        return std::tuple<double, double> {-1, -1};

    LoRaDatalinkFrame* dlMsg = dynamic_cast<LoRaDatalinkFrame*>(phyMsg->getEncapsulatedPacket());
    if (!dlMsg)
        return std::tuple<double, double> {-1, -1};

    uint8_t frameType = dlMsg->getFType();

    uint8_t msgSF          = phyMsg->getSpreadingFactor();
    float msgBW            = phyMsg->getBandwidth();
    uint8_t msgPayloadSize = phyMsg->getPayloadLength();
    uint8_t msgCR          = phyMsg->getCodingRate();

    //EV << "SF: " << (int) msgSF << "\n";
    //EV << "BW: " << msgBW << " KHz\n";
    //EV << "PS: " << (int) msgPayloadSize << " Bytes\n";
    //EV << "CR: " << (int) msgCR << "\n";

    double symbolDuration = pow(2, msgSF) / msgBW;
    //EV << "Symbol duration: " << symbolDuration << " ms\n";

    // LoRa preamble is constituted of 8 + 4.25 = 12.25 symbols
    double preambleDuration = 12.25 * symbolDuration;
    //EV << "Expected preamble time-on-air: " << preambleDuration << " ms\n";

    // LoRa physical header is constituted of 8 symbols
    // while physical payload symbols have to be calculated
    int size = msgPayloadSize + LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC;

    // Add the frame app layer header size if the message is not a join request or accept
    if (frameType == UPLINK_UNCONFIRMED ||
        frameType == UPLINK_CONFIRMED ||
        frameType == DOWNLINK_UNCONFIRMED ||
        frameType == DOWNLINK_CONFIRMED)
        size += LORA_FRAME_SIZE_APP_HEADER;

    // By default the frame header is explicit
    //bool explicitHeader = true;
    //int h = explicitHeader ? 0 : 1;
    int h = 0;

    // By default low data rate optimization is enabled only for SF11 and SF12 on 125 kHz bandwidth
    int de = msgBW == 125 && msgSF >= 11 ? 1 : 0;

    double payloadSymbols = 8 + std::max(
            //ceil((8 * msgPayloadSize - 4 * msgSF + 28 + 16 - 20 * h) / (4.0 * (msgSF - 2 * de))) * (4 + msgCR),
            ceil((8 * size - 4 * msgSF + 28 + 16 - 20 * h) / (4.0 * (msgSF - 2 * de))) * (4 + msgCR),
            0.0);

    double payloadDuration = payloadSymbols * symbolDuration;
    //EV << "Physical payload symbols: " << payloadSymbols << "\n";
    //EV << "Expected physical payload time-on-air: " << payloadDuration << " ms\n";

    double timeOnAir = preambleDuration + payloadDuration;
    //EV << "Expected frame time-on-air: " << timeOnAir << " ms\n";

    return {preambleDuration, timeOnAir};
}

// ********************* MESSAGE PHYSICAL CALCULATIONS END *********************



// ********************* MESSAGE SUBMITTINGS *********************

/*void sendMessageTcp(
        omnetpp::cSimpleModule* instance,
        uint8_t* address,
        std::map<std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::tuple<omnetpp::cMessage*, omnetpp::cMessage*, uint32_t, uint32_t>>& mapTcp,
        std::map<std::array<uint8_t, IPv4_ADDRESS_SIZE>, int>& routingTable,
        std::map<omnetpp::cMessage*, std::array<uint8_t, IPv4_ADDRESS_SIZE>>& eventTimeouts,

        const char* messageName, const char* timeoutName,
        std::array<uint8_t, IPv4_ADDRESS_SIZE>& destAddress, unsigned port,
        uint8_t* payload, unsigned payloadSize,
        omnetpp::cPacket* encapsulatedPacket,
        char* gateName, float timeout) {
    // Check if the dest IP address is already in the map for TCP/IP connections
    std::tuple<omnetpp::cMessage*, omnetpp::cMessage*, uint32_t, uint32_t> tuple;
    auto it = mapTcp.find(destAddress);
    if (it != mapTcp.end()) {
        // The network server is in the map, get the entry
        tuple = it->second;

        // Cancel old timeout
        instance->cancelEvent(std::get<1>(tuple));

        // Update TCP/IP sequence number
        std::get<2>(tuple) += 1;
    }
    else {
        // Create timeout
        omnetpp::cMessage* eventTimeout = new omnetpp::cMessage(timeoutName);
        std::get<1>(tuple) = eventTimeout;

        // Add timeout in the inverted index
        eventTimeouts[eventTimeout] = destAddress;

        // Init counters
        std::get<2>(tuple) = 0;
        std::get<3>(tuple) = 0;
    }

    // Create TCP/IP packet
    omnetpp::cPacket* msgOut = createMessageIp(
            messageName,
            address, destAddress.data(), IPv4_ADDRESS_SIZE,
            payload, payloadSize,
            encapsulatedPacket);
    msgOut = createMessageTcp(
            messageName,
            port, port,
            std::get<2>(tuple), std::get<3>(tuple), true,
            nullptr, 0,
            msgOut);

    // Store the entry in the table
    std::get<0>(tuple) = msgOut;
    mapTcp[destAddress] = tuple;

    // In OMNeT++ the gateway does not know which is the gate index associated to the IP address.
    // Check if the address is in the routing table to retrieve the gate index
    auto itRT = routingTable.find(destAddress);
    if (itRT != routingTable.end())
        instance->send(msgOut->dup(), gateName, itRT->second);
    else
        // The address is not in the routing table, send in broadcast
        sendBroadcast(instance, msgOut->dup(), gateName);

    instance->scheduleAt(omnetpp::simTime()+timeout, std::get<1>(tuple));
}*/

// ********************* MESSAGE SUBMITTINGS END *********************



// ********************* CONVERSIONS *********************

// Convert unsigned to array of bytes
void uintToBytes(unsigned n, uint8_t* dest) {
    if (!dest)
        return;

    // 11111111 00000000 = 0xff00 &
    // 00000000 11111111 = 0x00ff =
    // --------------------------
    // 00000000 00000000 = 0x0000

    // 00000000 11111111 = 0xff00 >> 8 &
    // 00000000 11111111 = 0x00ff      =
    // -------------------------------
    // 00000000 11111111 = 0x00ff

    //===================
    // Big-endianness
    /*dest[0] = (n >> 24) & 0xFF; // mask the [32,24] bits => upper 8 bits
    dest[1] = (n >> 16) & 0xFF; // mask the [24,16] bits
    dest[2] = (n >> 8) & 0xFF;  // mask the [16,8] bits
    dest[3] = n & 0xFF;*/         // mask the [8,0] bits   => lower 8 bits

    // Little-endianness
    /*dest[0] = n & 0xFF;         // mask the [8,0] bits   => lower 8 bits
    dest[1] = (n >> 8) & 0xFF;  // mask the [16,8] bits
    dest[2] = (n >> 16) & 0xFF; // mask the [24,16] bits
    dest[3] = (n >> 24) & 0xFF;*/ // mask the [32,24] bits => upper 8 bits
    //===================

    //===================
    int size = sizeof(unsigned);
    int sizeBits = 8 * size;

    // Big-endianness
    for (int i=0; i < size; i++)
        dest[i] = (n >> (sizeBits - 8 * (i+1))) & 0xFF;
    //===================
}

// Convert array of bytes to unsigned
unsigned bytesToUint(uint8_t* bytes) {
    if (!bytes)
        return 0;

    //===================
    // Big-endianness
    //return bytes[0] << 24 | bytes[1] << 16 | bytes[2] << 8 | bytes[3];

    // Little-endianness
    //return bytes[0] | bytes[1] << 8 | bytes[2] << 16 | bytes[3] << 24;
    //===================

    //===================
    unsigned res = 0;
    int size = sizeof(unsigned);
    int sizeBits = 8 * size;

    // Big-endianness
    for (int i=0; i < size; i++)
        res |= bytes[i] << (sizeBits - 8 * (i+1));

    return res;
    //===================
}

// Convert long unsigned to array of bytes
void luintToBytes(long unsigned n, uint8_t* dest) {
    if (!dest)
        return;

    int size = sizeof(long unsigned);
    int sizeBits = 8 * size;

    // Big-endianness
    for (int i=0; i < size; i++)
        dest[i] = (n >> (sizeBits - 8 * (i+1))) & 0xFF;
}

// Convert array of bytes to long unsigned
long unsigned bytesToLuint(uint8_t* bytes) {
    if (!bytes)
        return 0;

    long unsigned res = 0;
    int size = sizeof(long unsigned);
    int sizeBits = 8 * size;

    // Big-endianness
    for (int i=0; i < size; i++)
        res |= bytes[i] << (sizeBits - 8 * (i+1));

    return res;
}

// ********************* CONVERSIONS END *********************



// ********************* SECURITY *********************

// Generate a nonce in [0, max[
unsigned generateNonce(unsigned max) {
    return rand() % max;
}

// Generate key from root key and nonce
//const char* generateKey(char rootKey[], unsigned nonce1, unsigned nonce2, uint8_t networkId[]) {
const char* generateKey(uint8_t rootKey[], unsigned nonce1, unsigned nonce2, uint8_t networkId[]) {
    if (!rootKey || !networkId)
        return "";

    // In OTAA
    // NwkSKey = aes128_encrypt(AppKey, 0x01 | JoinNonce | NetID | DevNonce | pad16)
    // AppSKey = aes128_encrypt(AppKey, 0x02 | JoinNonce | NetID | DevNonce | pad16)

    // TODO
    //return aes128_encrypt(rootKey, 0x01 | nonce1 | networkId | nonce2 | pad16)
    return "";
}

void printKey(uint8_t* src, size_t size, const char* name, std::ostream& stream) {
    if (!src || !name || size <= 0)
        return;

    stream << name << ": 0x";
    for (size_t i=0; i<size; i++)
        //stream << std::hex << (unsigned) src[i];
        stream << " " << std::setfill('0') << std::setw(2) << std::hex << (unsigned) src[i];
    stream << std::dec << "\n";
}

// ********************* SECURITY END *********************



// ********************* LoRa *********************

bool isInLoRaRange(unsigned ax, unsigned ay, unsigned bx, unsigned by, double* distance, float range) {
    // Calculate euclidean distance
    unsigned dx = bx - ax;
    unsigned dy = by - ay;
    *distance  = sqrt(dx*dx + dy*dy);

    // Check if a is in b radio range
    return *distance < range ? true : false;
}

// Simulate RSSI associated with an input message
int calculateRSSI(omnetpp::cSimpleModule* instance, unsigned ax, unsigned ay, unsigned bx, unsigned by, std::ostream& stream) {
    // RSSI (min -120 dBm, max -30 dBm) the closer to 0 the better the signal is.
    // Represented with positive values and associated with an input signal
    //return rand() % 91 + 30;

    // Keep constant for evaluating the load distribution of the algorithm
    //return 40;

    if (!instance)
        return -1;

    //stream << "Sender pos X: " << ax << "\n";
    //stream << "Sender pos Y: " << ay << "\n";
    //stream << "Receiver pos X: " << bx << "\n";
    //stream << "Receiver pos Y: " << by << "\n";

    // Calculate euclidean distance
    unsigned dx = ax - bx;
    unsigned dy = ay - by;
    double d    = sqrt(dx*dx + dy*dy);

    stream << "Euclidean distance: " << d << "\n";

    //int rssi = -10 * PATH_LOSS_EXP * log10(d) + MEASURED_POWER;

    // Calculate estimation of RSSI from path loss model
    return instance->normal(-10 * PATH_LOSS_EXP * log10(d) + MEASURED_POWER, 2);
}

void getPhysicalParameters(
        int region, double* dutyCycle,
        std::map<float, std::vector<std::tuple<float, uint8_t>>>& bandwidths,
        std::map<float, std::vector<float>>& channelFrequencies,
        bool getBandwidths/*=true*/) {
    std::vector<float> bandwidths_;
    std::vector<std::tuple<uint8_t, float, float, uint8_t, uint8_t>> frequencies;
    uint8_t transmissionPowerMax = 0;
    std::map<uint8_t, std::vector<uint8_t>> transmissionPowers;

    switch(region) {
        case REGION_EU868:
            // Append the bandwidths
            bandwidths_.push_back(BANDWIDTH_125);
            bandwidths_.push_back(BANDWIDTH_250);

            // Append the channel frequencies, starting frequency, step frequency,
            // max spreading factor and number of spreading factors for each bandwidth
            frequencies.push_back(
                    std::tuple<uint8_t, float, float, uint8_t, uint8_t>(
                            CHANNEL_FREQUENCIES_BW_125_EU_868,
                            CHANNEL_FREQUENCY_START_BW_125_EU_868,
                            CHANNEL_FREQUENCY_STEP_BW_125_EU_868,
                            SPREADING_FACTOR_MAX_BW_125_EU_868,
                            SPREADING_FACTOR_NUM_BW_125_EU_868));
            frequencies.push_back(
                    std::tuple<uint8_t, float, float, uint8_t, uint8_t>(
                            CHANNEL_FREQUENCIES_BW_250_EU_868,
                            CHANNEL_FREQUENCY_START_BW_250_EU_868,
                            CHANNEL_FREQUENCY_STEP_BW_250_EU_868,
                            SPREADING_FACTOR_MAX_BW_250_EU_868,
                            SPREADING_FACTOR_NUM_BW_250_EU_868));

            transmissionPowerMax = MAX_TX_POWER_EU_868;
            if (dutyCycle)
                *dutyCycle = DUTY_CYCLE_EU_868;
            break;
        case REGION_EU433:
            // Append the bandwidths
            bandwidths_.push_back(BANDWIDTH_125);
            bandwidths_.push_back(BANDWIDTH_250);

            // Append the channel frequencies, starting frequency, step frequency,
            // max spreading factor and number of spreading factors for each bandwidth
            frequencies.push_back(
                    std::tuple<uint8_t, float, float, uint8_t, uint8_t>(
                            CHANNEL_FREQUENCIES_BW_125_EU_433,
                            CHANNEL_FREQUENCY_START_BW_125_EU_433,
                            CHANNEL_FREQUENCY_STEP_BW_125_EU_433,
                            SPREADING_FACTOR_MAX_BW_125_EU_433,
                            SPREADING_FACTOR_NUM_BW_125_EU_433));
            frequencies.push_back(
                    std::tuple<uint8_t, float, float, uint8_t, uint8_t>(
                            CHANNEL_FREQUENCIES_BW_250_EU_433,
                            CHANNEL_FREQUENCY_START_BW_250_EU_433,
                            CHANNEL_FREQUENCY_STEP_BW_250_EU_433,
                            SPREADING_FACTOR_MAX_BW_250_EU_433,
                            SPREADING_FACTOR_NUM_BW_250_EU_433));

            transmissionPowerMax = MAX_TX_POWER_EU_433;
            if (dutyCycle)
                *dutyCycle = DUTY_CYCLE_EU_433;
            break;
        case REGION_US915:
            // Append the bandwidths
            bandwidths_.push_back(BANDWIDTH_125);
            bandwidths_.push_back(BANDWIDTH_500);

            // Append the channel frequencies, starting frequency, step frequency,
            // max spreading factor and number of spreading factors for each bandwidth
            frequencies.push_back(
                    std::tuple<uint8_t, float, float, uint8_t, uint8_t> {
                            CHANNEL_FREQUENCIES_BW_125_US_915,
                            CHANNEL_FREQUENCY_START_BW_125_US_915,
                            CHANNEL_FREQUENCY_STEP_BW_125_US_915,
                            SPREADING_FACTOR_MAX_BW_125_US_915,
                            SPREADING_FACTOR_NUM_BW_125_US_915});
            frequencies.push_back(
                    std::tuple<uint8_t, float, float, uint8_t, uint8_t> {
                            CHANNEL_FREQUENCIES_BW_500_UP_US_915,
                            CHANNEL_FREQUENCY_START_BW_500_UP_US_915,
                            CHANNEL_FREQUENCY_STEP_BW_500_UP_US_915,
                            SPREADING_FACTOR_MAX_BW_500_UP_US_915,
                            SPREADING_FACTOR_NUM_BW_500_UP_US_915});

            transmissionPowerMax = MAX_TX_POWER_US_915;
            if (dutyCycle)
                *dutyCycle = DUTY_CYCLE_US_915;
            break;
        default:
            throw std::invalid_argument("Invalid region parameter");
    }

    // Calculate the channel frequencies and transmission powers associated to spreading factors
    uint8_t tp = 0;
    for (unsigned i=0; i<bandwidths_.size(); i++) {
        float bw   = bandwidths_[i];
        auto tuple = frequencies[i];
        int frequencyChannels      = std::get<0>(tuple);
        float frequencyStart       = std::get<1>(tuple);
        float frequencyStep        = std::get<2>(tuple);
        uint8_t spreadingFactorMax = std::get<3>(tuple);
        uint8_t spreadingFactorNum = std::get<4>(tuple);

        for (int j=0; j<frequencyChannels; j++) {
            if (j == 0)
                channelFrequencies[bw] = std::vector<float>{frequencyStart + j*frequencyStep};
            else
                channelFrequencies[bw].push_back(frequencyStart + j*frequencyStep);

            if (getBandwidths && spreadingFactorNum > 0) {
                int sf = spreadingFactorMax - j;
                auto tuple = std::tuple<float, uint8_t> {bw, transmissionPowerMax - 2*tp};

                if (bandwidths.find(sf) == bandwidths.end())
                    bandwidths[sf] = std::vector<std::tuple<float, uint8_t>> {tuple};
                else
                    bandwidths[sf].push_back(tuple);

                spreadingFactorNum--;
                tp++;
            }
        }
    }
}

void getPhysicalParameters(int region, std::map<float, std::vector<float>>& channelFrequencies) {
    std::map<float, std::vector<std::tuple<float, uint8_t>>> bandwidths;
    getPhysicalParameters(region, nullptr, bandwidths, channelFrequencies, false);
}

// ********************* LoRa END *********************



// ********************* BACKGROUND NOISE *********************

// Calculate BER probability according to the experimental BER curves
float calculateProbabilityBER(int maxBer, int snrThreshold, int snr) {
    //EV << "maxBer: " << maxBer << "\n";
    //EV << "snrThreshold: " << snrThreshold << "\n";
    //EV << "snr: " << snr << "\n";

    // Check if the max BER is valid
    if (maxBer < 0 || maxBer > 100)
        return -1;

    float probDrop = 0;

    // Check if the SNR is greater than the max value
    if(snr <= -55)
        probDrop = maxBer;
    // Else check if the SNR is below the threshold
    else if (snr < snrThreshold) {
        // Equation approximating the BER curve: y = c - e^(s x) for x <= 55 - abs(snrThreshold) <=>
        // -y+c = e^(s x) <=>
        // ln(-y+c) = s x <=>
        // ln(-y+c)/x = s <=>
        // ln(c)/x = s

        // Derive the slope of the curve constrained in x <= 55 - abs(snrThreshold)
        float slope = log(maxBer + 1) / (55 - abs(snrThreshold));

        // Calculate drop probability
        probDrop = maxBer + 1 - exp(slope * (55 - abs(snr)));
    }

    // Divide the probability by 100 to bound it in [0,1]
    return probDrop / 100;
}

// Return Signal to Interference plus Noise Ratio and message drop probability
std::tuple<double, float> applyExternalNoise(
        omnetpp::cMessage* msg, int rssi, uint8_t sf, float bw,
        std::map<long, std::tuple<double, float>>& interferences, std::ostream& stream) {
    if (!msg)
        return std::tuple<int, float> {1, -1};

    stream << "Applying external noise...\n";

    // Thermal noise
    double thermalNoisePower = THERMAL_NOISE_POWER_SPECTRAL_DENSITY + NOISE_FIGURE_RX + 10 * log10(bw*1000);
    stream << "Thermal noise: " << thermalNoisePower << "\n";

    // Additive White Gaussian Noise (AWGN)
    //thermalNoisePower = normal(0, thermalNoisePower);
    //EV << "Thermal noise (AWGN): " << thermalNoisePower << "\n";

    // AWGN disabled because has zero mean and is not suitable to compare with RSSI that is negative (too influence)


    // External interference due to other technologies operating on the same unlicensed frequencies
    // (e.g. Wi-Fi, Bluetooth)
    float externalInterferenceProbability = 1 - pow(
            (1 - PROBABILITY_OCCURRENCE_EXTERNAL_INTERFERENCE),
            ceil(bw/EXTERNAL_INTERFERENCE_BANDWIDTH));

    stream << "externalInterferenceProbability: " << externalInterferenceProbability << "\n";

    double externalInterferencePower = 0;

    // Check if the external probability occurs
    //if (((rand() + (int) uniform(0,100)) % 101) / 100.0 < externalInterferenceProbability) {
    if ((rand() % 101) / 100.0 < externalInterferenceProbability) {
        // Interval of interference power in dBm considered for external interference is [-105, -65] almost [-105, -80]
        std::random_device rd {};
        std::mt19937 generator { rd() };

        // With the following parameters the curve is not exactly equal to the experimental one because
        // the interval is [-108, -98].
        // It requires GEV type II instead of GEV type I but it is not defined in C++, so, use this approximation
        float location = -105;
        float scale    = 1.65;
        std::extreme_value_distribution<float> distribution(location, scale);

        //for (int i=0; i<20; i++)
        //    stream << "distribution(generator): " << distribution(generator) << "\n";

        externalInterferencePower = distribution(generator) + 10 * log10(PROBABILITY_OCCURRENCE_EXTERNAL_INTERFERENCE);
        stream << "External Interference Power: " << externalInterferencePower << "\n";

        //stream << "10 * log10(PROBABILITY_OCCURRENCE_EXTERNAL_INTERFERENCE): " << 10 * log10(PROBABILITY_OCCURRENCE_EXTERNAL_INTERFERENCE) << "\n";
    }

    // Calculate Signal to Interference plus Noise Ratio for the signal.
    // To sum Noise and Interference, convert them from dBm to mW
    double noiseAndInterferencePower = (pow(10, thermalNoisePower/10) + pow(10, externalInterferencePower/10));
    stream << "NoiseAndInterference power: " << noiseAndInterferencePower << " mW\n";
    // Then convert the sum from mW to dBm
    //int sinr = rssi - 10 * log10(pow(10, thermalNoisePower/10) + pow(10, externalInterferencePower/10));
    int sinr = rssi - 10 * log10(noiseAndInterferencePower);
    int sinrThreshold = 0;
    int maxBer        = 0;
    stream << "SINR: " << sinr << "\n";

    //stream << "thermalNoisePower (mW): " << pow(10, thermalNoisePower/10) << "\n";
    //stream << "externalInterferencePower (mW): " << pow(10, externalInterferencePower/10) << "\n";

    // Get the SINR threshold
    if (bw == BANDWIDTH_125)
        switch (sf) {
            case 7:
                sinrThreshold = -35; // (-30-23-35-33-44-43)/6
                maxBer = 47; // (50+50+36+50)/4
                break;
            case 8:
                sinrThreshold = -35;
                maxBer = 48;
                break;
            case 9:
                sinrThreshold = -35; // (-40-32-42-43-54)/6
                maxBer = 49; // (50+58+37+49)/4
                break;
            case 10:
                sinrThreshold = -40;
                maxBer = 29;
                break;
            case 11:
                sinrThreshold = -45;
                maxBer = 25;
                break;
            case 12:
                sinrThreshold = -48; // (-48-40-50-52)/4
                maxBer = 22; // (30+49+2+5)/4
                break;
            default:
                throw std::invalid_argument("Invalid spreading factor");
        }
    else if (bw == BANDWIDTH_250)
        switch (sf) {
            case 7:
                sinrThreshold = -28; // (-24-20-26-23-39-36)/6
                maxBer = 49; // (49+49+50+48)/4
                break;
            case 8:
                sinrThreshold = -28;
                maxBer = 50;
                break;
            case 9:
                sinrThreshold = -28; // (-34-29-35--37-52-53)/6
                maxBer = 51; // (49+50+52+53)/4
                break;
            case 10:
                sinrThreshold = -33;
                maxBer = 47;
                break;
            case 11:
                sinrThreshold = -36;
                maxBer = 44;
                break;
            case 12:
                sinrThreshold = -43; // (-40-38-46-47)/4
                maxBer = 40; // (48+50+30+30)/4
                break;
            default:
                throw std::invalid_argument("Invalid spreading factor");
        }
    else
        switch (sf) {
            case 7:
                sinrThreshold = -26; // (-20-17-22-22-34-38)/6
                maxBer = 50; // (49+50+50+50)/4
                break;
            case 8:
                sinrThreshold = -29;
                maxBer = 49;
                break;
            case 9:
                sinrThreshold = -35; // (-30-26-31-30-48-44)/6
                maxBer = 47; // (49+50+40+50)/4
                break;
            case 10:
                sinrThreshold = -36;
                maxBer = 49;
                break;
            case 11:
                sinrThreshold = -37;
                maxBer = 51;
                break;
            case 12:
                sinrThreshold = -38; // (-37-36-40-40)/4
                maxBer = 53; // (49+55+48+58)/4
                break;
            default:
                throw std::invalid_argument("Invalid spreading factor");
        }

    float probDrop = 0;

    // Check if the snr is below the threshold
    if (sinr < sinrThreshold) {
        stream << "The message is affected by external interference!\n";

        // Calculate the probability the message is dropped
        probDrop = calculateProbabilityBER(maxBer, sinrThreshold, sinr);
        stream << "The message is dropped with a probability of " << probDrop <<"\n";
    }

    //auto tuple = std::tuple<int, float> {sinr, probDrop};
    auto tuple = std::tuple<double, float> {noiseAndInterferencePower, probDrop};

    // Initialize a corresponding entry in the interferences map
    //interferences[msg] = tuple;
    long msgId = msg->getTreeId();
    interferences[msgId] = tuple;

    return tuple;
}

// Return Signal to Interference plus Noise Ratio and message drop probability
std::tuple<double, float> applyExternalNoise(
        omnetpp::cMessage* msg, int rssi,
        std::map<long, std::tuple<double, float>>& interferences, std::ostream& stream) {
    if (!msg)
        return std::tuple<int, float> {-1, -1};

    // Get message parameters (sent by source module)
    LoRaPhysicalFrame* msg_ = dynamic_cast<LoRaPhysicalFrame*>(msg);
    if (!msg_)
        return std::tuple<int, float> {-1, -1};

    return applyExternalNoise(msg, rssi, msg_->getSpreadingFactor(), msg_->getBandwidth(), interferences, stream);
}

// ********************* BACKGROUND NOISE END *********************



/*long factorial(int n) {
    if (n <= 1)
        return 1;

    return n * factorial(n-1);
}*/

/*int binomialCoefficient(int n, int k) {
    if (n < 0 || k < 0 || k > n)
        return 0;

    if (k == 0 || k == n)
        return 1;

    //return factorial(n) / (factorial(k) * factorial(n-k));

    // n! / (k! (n-k)!) = (n * (n-1) * ... * (n-k+1)) / k!

    int res = 1;

    for (int i=1; i<k; i++) {
        res *= (n - i + 1);
        res /= i;
    }

    return res;
}*/
