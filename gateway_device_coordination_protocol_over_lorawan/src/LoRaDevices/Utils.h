#pragma once

#include <omnetpp.h>
#include <bitset>

#include <iomanip>

#include <openssl/evp.h>
#include <openssl/cmac.h>

#include <random>

//#include "inet/physicallayer/wireless/common/base/packetlevel/ScalarAnalogModelBase.h"

#include "../IPPackets/IPv4Packet_m.h"
#include "../LoRaFrames/LoRaJoinRequestFrame_m.h"
#include "../LoRaFrames/LoRaJoinAcceptFrame_m.h"
#include "../LoRaFrames/LoRaAppDownlinkFrame_m.h"
#include "../LoRaFrames/LoRaAppUplinkFrame_m.h"
#include "../LoRaFrames/LoRaDatalinkFrame_m.h"
#include "../LoRaFrames/LoRaPhysicalFrame_m.h"
#include "../TransportSegments/TCPSegment_m.h"
#include "../TransportSegments/UDPSegment_m.h"

// LoRa
const float SYNC_WORD          = 0x34;
const uint8_t CODING_RATE      = 1;            // 1 -> 4/5, 2 -> 4/6, 3 -> 4/7, 4 -> 4/8
const float LORA_RANGE         = 5000;         // m
const float INTERFERENCE_RANGE = 2*LORA_RANGE; // m
const int INTERFERENCE_SIR_THRESHOLD                     = -6;   // dB
const float PROBABILITY_OCCURRENCE_EXTERNAL_INTERFERENCE = 0.22; // 22%
const float EXTERNAL_INTERFERENCE_BANDWIDTH              = 7;    // KHz
const int THERMAL_NOISE_POWER_SPECTRAL_DENSITY           = -174; // dBm/Hz
const int NOISE_FIGURE_RX                                = 3;    // dB

// LoRa regional parameters
const float BANDWIDTH_125 = 125; // KHz
const float BANDWIDTH_250 = 250; // KHz
const float BANDWIDTH_500 = 500; // KHz
const int REGION_EU868 = 1;
const int REGION_EU433 = 4;
const int REGION_US915 = 2;
const unsigned DUTY_CYCLE_INTERVAL = 3600; // s

// EU868
const uint8_t CHANNEL_FREQUENCIES_BW_125_EU_868   = 16;
const float CHANNEL_FREQUENCY_START_BW_125_EU_868 = 868.1; // MHz
const float CHANNEL_FREQUENCY_STEP_BW_125_EU_868  = 0.2;   // MHz
const uint8_t SPREADING_FACTOR_MAX_BW_125_EU_868  = 12;
const uint8_t SPREADING_FACTOR_NUM_BW_125_EU_868  = 6;

const uint8_t CHANNEL_FREQUENCIES_BW_250_EU_868   = 1;
const float CHANNEL_FREQUENCY_START_BW_250_EU_868 = 868.1; // MHz
const float CHANNEL_FREQUENCY_STEP_BW_250_EU_868  = 0.2;   // MHz
const uint8_t SPREADING_FACTOR_MAX_BW_250_EU_868  = 7;
const uint8_t SPREADING_FACTOR_NUM_BW_250_EU_868  = 1;

const uint8_t MAX_TX_POWER_EU_868                 = 16;    // dBm
const double DUTY_CYCLE_EU_868                    = 0.01;  // 1%

// EU433
const uint8_t CHANNEL_FREQUENCIES_BW_125_EU_433   = 16;
const float CHANNEL_FREQUENCY_START_BW_125_EU_433 = 433.175; // MHz
const float CHANNEL_FREQUENCY_STEP_BW_125_EU_433  = 0.2;     // MHz
const uint8_t SPREADING_FACTOR_MAX_BW_125_EU_433  = 12;
const uint8_t SPREADING_FACTOR_NUM_BW_125_EU_433  = 6;

const uint8_t CHANNEL_FREQUENCIES_BW_250_EU_433   = 1;
const float CHANNEL_FREQUENCY_START_BW_250_EU_433 = 433.175; // MHz
const float CHANNEL_FREQUENCY_STEP_BW_250_EU_433  = 0.2;   // MHz
const uint8_t SPREADING_FACTOR_MAX_BW_250_EU_433  = 7;
const uint8_t SPREADING_FACTOR_NUM_BW_250_EU_433  = 1;

const uint8_t MAX_TX_POWER_EU_433                 = 12;      // dBm
const double DUTY_CYCLE_EU_433                    = 0.1;     // 10%

// US915
const uint8_t CHANNEL_FREQUENCIES_BW_125_US_915        = 64;
const float CHANNEL_FREQUENCY_START_BW_125_US_915      = 902.3; // MHz
const float CHANNEL_FREQUENCY_STEP_BW_125_US_915       = 0.2;   // MHz
const uint8_t SPREADING_FACTOR_MAX_BW_125_US_915       = 10;
const uint8_t SPREADING_FACTOR_NUM_BW_125_US_915       = 4;

const uint8_t CHANNEL_FREQUENCIES_BW_500_UP_US_915     = 8;
const float CHANNEL_FREQUENCY_START_BW_500_UP_US_915   = 903.0; // MHz
const float CHANNEL_FREQUENCY_STEP_BW_500_UP_US_915    = 1.6;   // MHz
const uint8_t SPREADING_FACTOR_MAX_BW_500_UP_US_915    = 8;
const uint8_t SPREADING_FACTOR_NUM_BW_500_UP_US_915    = 1;

const uint8_t CHANNEL_FREQUENCIES_BW_500_DOWN_US_915   = 8;
const float CHANNEL_FREQUENCY_START_BW_500_DOWN_US_915 = 923.3; // MHz
const float CHANNEL_FREQUENCY_STEP_BW_500_DOWN_US_915  = 0.6;   // MHz
const uint8_t SPREADING_FACTOR_MAX_BW_500_DOWN_US_915  = 12;
const uint8_t SPREADING_FACTOR_NUM_BW_500_DOWN_US_915  = 6;

const uint8_t MAX_TX_POWER_US_915                      = 30;    // dBm
const double DUTY_CYCLE_US_915                         = -1;    // No duty cycle


// Message sizes
const uint8_t LORA_FRAME_SIZE_JOIN_REQUEST    = 18; // Bytes
const uint8_t LORA_FRAME_SIZE_JOIN_ACCEPT     = 28; // Bytes
const uint8_t LORA_FRAME_SIZE_DATALINK_HEADER = 1;  // Bytes
const uint8_t LORA_FRAME_SIZE_DATALINK_MIC    = 4;  // Bytes
const uint8_t LORA_FRAME_SIZE_APP_HEADER      = 8;  // Bytes
const uint8_t LORA_FRAME_SIZE_APP_PAYLOAD     = 11; // Bytes
const uint8_t MESSAGE_SIZE_IPV4_HEADER  = 60; // Bytes
const uint8_t MESSAGE_SIZE_IPV4_PAYLOAD = 70; // Bytes (it is not the actual size but it is largely sufficient for the protocol)
const uint8_t MESSAGE_SIZE_TCP_HEADER  = 60; // Bytes
const uint8_t MESSAGE_SIZE_UDP_HEADER  = 8;  // Bytes

// IP address sizes
const uint8_t IPv4_ADDRESS_SIZE = 4;  // Bytes
const uint8_t IPv6_ADDRESS_SIZE = 16; // Bytes

// Variable sizes
const uint8_t EUI_SIZE         = 8;   // Bytes
const uint8_t NETWORK_ID_SIZE  = 3;   // Bytes
const uint8_t KEY_LORAWAN_SIZE = 16;  // Bytes (128/8 bits = 16 bytes)

// LoRaWAN FTypes of data link frame
const uint8_t JOIN_REQUEST         = 0;
const uint8_t JOIN_ACCEPT          = 1;
const uint8_t UPLINK_UNCONFIRMED   = 2;
const uint8_t DOWNLINK_UNCONFIRMED = 3;
const uint8_t UPLINK_CONFIRMED     = 4;
const uint8_t DOWNLINK_CONFIRMED   = 5;

// Message loss probability
//const float MSG_LOSS_PROBABILITY = 0.1; // 10%
//const float MSG_LOSS_PROBABILITY_IP = 0.01; // 1%
const uint8_t MSG_LOSS_PROBABILITY    = 10; // 10%
const uint8_t MSG_LOSS_PROBABILITY_IP = 1; // 1%

// For calculating RSSI
const float PATH_LOSS_EXP  = 2.4; // FOR RANDOM SCENARIO
//const int MEASURED_POWER   = -30;
//const float PATH_LOSS_EXP  = 4;
//const int MEASURED_POWER   = -60;
//const float PATH_LOSS_EXP  = 2.85; // FOR REAL SCENARIO
//const float PATH_LOSS_EXP  = 2.87; // FOR REAL SCENARIO 20190301
//const float PATH_LOSS_EXP  = 5; // FOR REAL SCENARIO 20200501
//const float PATH_LOSS_EXP  = 4.7; // FOR REAL SCENARIO 20200801
//const float PATH_LOSS_EXP  = 4.9; // FOR REAL SCENARIO 20200804
//const float PATH_LOSS_EXP  = 4.5; // FOR REAL SCENARIO 20200506
const int MEASURED_POWER   = -30;

const double SPEED_LIGHT_ON_AIR = 2.99e8; // m/s

// Retransmissions of an end device GENERATE_COMMON_KEY, HELLO and FORWARD message to minimize loss probability
//const uint8_t RETRANSMISSIONS = 1;
const uint8_t RETRANSMISSIONS = 3;

// Transmission delays of gateways for HELLO_GATEWAY
const uint8_t HELLO_GATEWAY_MAX_DELAY = 30; // Seconds

// Transmission and receving windows of end devices
const uint8_t WINDOW_TX   = 0;
const uint8_t WINDOW_RX_1 = 1;
const uint8_t WINDOW_RX_2 = 2;
const uint8_t WINDOW_STATES_NUMBER = 3;
const uint8_t RX_DELAY_1 = 1; // Seconds
const uint8_t RX_DELAY_2 = 2; // Seconds
const uint8_t TX_DELAY   = 3; // Seconds

// Stages of end devices (first two also for gateways)
const uint8_t STAGE_ACTIVATION               = 0;
const uint8_t STAGE_ACTIVATED                = 1;
const uint8_t STAGE_GENERATE_COMMON_KEY      = 2;
const uint8_t STAGE_HELLO                    = 3;
const uint8_t STAGE_FORWARD                  = 4;
const uint8_t STAGE_PAIRING                  = 5;
const uint8_t STAGE_CONNECTION               = 6;
const uint8_t STAGE_GENERATE_ASSOCIATION_KEY = 7;
const uint8_t STAGE_DATA_PROFILE             = 8;
const uint8_t STAGE_DATA                     = 9;

// LoRaWAN ports of application frame
const uint8_t MSG_PORT_MAC_CMD                  = 0;
const uint8_t MSG_PORT_HELLO_GATEWAY            = 1;
const uint8_t MSG_PORT_GENERATE_COMMON_KEY      = 2;
const uint8_t MSG_PORT_HELLO                    = 3;
const uint8_t MSG_PORT_FORWARD                  = 4;
const uint8_t MSG_PORT_STATS                    = 5;
const uint8_t MSG_PORT_PAIRING_REQUEST          = 6;
const uint8_t MSG_PORT_PAIRING_ACCEPT           = 7;
//const uint8_t MSG_PORT_PAIRING_VERIFY           = 7;
//const uint8_t MSG_PORT_PAIRING_VERIFIED         = 8;
const uint8_t MSG_PORT_CONNECTION               = 8;
const uint8_t MSG_PORT_GENERATE_ASSOCIATION_KEY = 9;
const uint8_t MSG_PORT_DATA_PROFILE             = 10;
const uint8_t MSG_PORT_DATA                     = 11;

// TCP/UDP ports of IP packets
//const unsigned MSG_PORT_FORWARD_JOIN            = 6100;
//const unsigned MSG_PORT_FORWARD_MAC_CMD         = 6101;
const unsigned MSG_PORT_NEARBY_GATEWAYS         = 5000;
const unsigned MSG_PORT_SYNC_COUNTER            = 5001;
//const unsigned MSG_PORT_FORWARD_PAIRING_REQUEST = 5002;
const unsigned MSG_PORT_FORWARD_OVER_IP         = 5002;
//const unsigned MSG_PORT_CONNECTION              = 5005;
const unsigned MSG_PORT_CONNECTION_GATEWAY      = 5005;
const unsigned MSG_PORT_SHARE_DATA_PROFILE      = 5009;
const unsigned MSG_PORT_PROCESSED_DATA          = 5010;

// Message types
const uint8_t MSG_LORA_UPLINK   = 0;
const uint8_t MSG_LORA_DOWNLINK = 1;
const uint8_t MSG_TCP           = 2;
const uint8_t MSG_UDP           = 3;

// LoRaWAN messages
const uint8_t MSG_NULL           = 0;
const uint8_t MSG_JOIN_ACCEPT    = 1;
const uint8_t MSG_MAC_CMD        = 2;
const uint8_t MSG_STATS          = 4;
const uint8_t MSG_PAIRING_ACCEPT = 6;
//const uint8_t MSG_PAIRING_VERIFY = 7;
const uint8_t MSG_ACK            = 12;
const uint8_t MSG_NACK           = 13;

// LoRaWAN messages errors
const uint8_t FRAME_OK       = 0;
const uint8_t BAD_FRAME      = 1;
const uint8_t BAD_PARAMS     = 2;
const uint8_t BAD_ADDRESS    = 3;
const uint8_t BAD_MIC        = 4;
const uint8_t BAD_COUNTER    = 5;
const uint8_t BAD_PORT       = 6;
const uint8_t BAD_REQUEST_ID = 7;

// Key references
/*const uint8_t ROOT_KEY_END_DEVICE           = 0;
const uint8_t ROOT_KEY_GATEWAY              = 1;
const uint8_t COMMON_SESSION_KEY_END_DEVICE = 2;
const uint8_t COMMON_SESSION_KEY_GATEWAY    = 3;*/

// Custom ACK in payload
const uint8_t NACK         = 0;
const uint8_t ACK          = 1;
const uint8_t STATS_UPDATE = 2; // For pairing request based on an old gateway state

// Metrics to select the best gateway
// (Disabled to test a large-scale network)
/*const uint8_t CPU_LOAD_THRESHOLD     = 85; //70; // %
const uint8_t GPU_LOAD_THRESHOLD     = 90; //80; // %
const uint8_t RAM_LOAD_THRESHOLD     = 85; //70; // %
const uint8_t STORAGE_LOAD_THRESHOLD = 90; // %
const uint8_t NETWORK_IN_THRESHOLD   = 5; // kbyte/s
const uint8_t NETWORK_OUT_THRESHOLD  = 5; // kbyte/s*/
const uint8_t CPU_LOAD_THRESHOLD     = 255; // %
const uint8_t GPU_LOAD_THRESHOLD     = 255; // %
const uint8_t RAM_LOAD_THRESHOLD     = 255; // %
const uint8_t STORAGE_LOAD_THRESHOLD = 90; // %
const uint8_t NETWORK_IN_THRESHOLD   = 5; // kbyte/s
const uint8_t NETWORK_OUT_THRESHOLD  = 5; // kbyte/s

// Data profile
const uint8_t LOCATION_STRING_LENGTH = 50;

// Storage size of gateways
//const long STORAGE_SIZE = 8 * pow(10, 9); // Bytes (8 GB)
//const long STORAGE_SIZE = 4 * pow(10, 9); // Bytes (4 GB)
const long STORAGE_SIZE = 8e9; // Bytes (8 GB)
//const long STORAGE_SIZE = 4e9; // Bytes (4 GB)

// Network server IP address
const uint8_t IPv4_ADDRESS_NETWORK_SERVER_0 = 192;
const uint8_t IPv4_ADDRESS_NETWORK_SERVER_1 = 168;
const uint8_t IPv4_ADDRESS_NETWORK_SERVER_2 = 1;
const uint8_t IPv4_ADDRESS_NETWORK_SERVER_3 = 1;


/*typedef struct {
    unsigned value : 24;
} uint24_t;*/




// ********************* MESSAGE ARRAY GETTERS & SETTERS *********************

// OMNeT++ getter & setter methods for arrays in messages return the item located at the array index k
// instead of the full-array. So, the function copies the array index by index

void getArrayInMessageJoinRequest(
        LoRaJoinRequestFrame* instance,
        uint8_t (LoRaJoinRequestFrame::*funcp)(size_t) const,
        uint8_t* dest, uint8_t size);

void getArrayInMessageJoinAccept(
        LoRaJoinAcceptFrame* instance,
        uint8_t (LoRaJoinAcceptFrame::*funcp)(size_t) const,
        uint8_t* dest, uint8_t size);

void getArrayInMessageUplink(
        LoRaAppUplinkFrame* instance,
        uint8_t (LoRaAppUplinkFrame::*funcp)(size_t) const,
        uint8_t* dest, uint8_t size);

void getArrayInMessageDownlink(
        LoRaAppDownlinkFrame* instance,
        uint8_t (LoRaAppDownlinkFrame::*funcp)(size_t) const,
        uint8_t* dest, uint8_t size);

void getArrayInMessageIp(
        IPv4Packet* instance,
        uint8_t (IPv4Packet::*funcp)(size_t) const,
        uint8_t* dest, uint8_t size);


void setArrayInMessageJoinRequest(
        LoRaJoinRequestFrame* instance,
        void (LoRaJoinRequestFrame::*funcp)(size_t, uint8_t),
        uint8_t* src, uint8_t size);

void setArrayInMessageJoinAccept(
        LoRaJoinAcceptFrame* instance,
        void (LoRaJoinAcceptFrame::*funcp)(size_t, uint8_t),
        uint8_t* src, uint8_t size);

void setArrayInMessageUplink(
        LoRaAppUplinkFrame* instance,
        void (LoRaAppUplinkFrame::*funcp)(size_t, uint8_t),
        uint8_t* src, uint8_t size);

void setArrayInMessageDownlink(
        LoRaAppDownlinkFrame* instance,
        void (LoRaAppDownlinkFrame::*funcp)(size_t, uint8_t),
        uint8_t* src, uint8_t size);

void setArrayInMessageIp(
        IPv4Packet* instance,
        void (IPv4Packet::*funcp)(size_t, uint8_t),
        uint8_t* src, uint8_t size);

// ********************* MESSAGE ARRAY GETTERS & SETTERS END *********************



// ********************* MESSAGE VALIDATIONS *********************

/*uint8_t isValidMessageIp(omnetpp::cMessage* msg,
        uint8_t* srcAddress, uint8_t* destAddress, uint32_t sequenceNumber, uint32_t ackNumber, uint8_t* payload);

uint8_t isValidMessageTcp(omnetpp::cMessage* msg, unsigned* port, uint32_t* sequenceNumber, uint32_t* ackNumber);

uint8_t isValidMessageUdp(omnetpp::cMessage* msg, unsigned* port);*/

// ********************* MESSAGE VALIDATIONS END *********************



// ********************* MIC *********************

// Calculate MIC of LoRa frame
//void calculateMIC(LoRaDatalinkFrame* datalinkMsg, omnetpp::cMessage* appMsg, uint8_t* keyMIC);
void calculateMIC(LoRaDatalinkFrame* datalinkMsg, uint8_t* keyMIC);

// Check MIC of LoRa frame
bool isValidMIC(LoRaDatalinkFrame* datalinkMsg, omnetpp::cMessage* appMsg, uint8_t* keys[], int keysSize);

// ********************* MIC END *********************



// ********************* MESSAGE CREATIONS *********************

omnetpp::cPacket* createMessageJoinRequest(
        const char* name,
        uint8_t* joinEUI, uint8_t* devEUI, uint16_t devNonce,
        uint8_t* keyMIC,
        uint8_t spreadingFactor, uint8_t transmissionPower,
        float bandwidth, float channelFrequency);

omnetpp::cPacket* createMessageJoinAccept(
        const char* name,
        uint32_t joinNonce, uint8_t* netID, uint8_t* endDeviceAddress,
        uint8_t DLSettings, uint8_t RXDelay, uint8_t* CFList,
        uint8_t* keyMIC,
        uint8_t spreadingFactor, uint8_t transmissionPower,
        float bandwidth, float channelFrequency);

omnetpp::cPacket* createMessageUplink(
        const char* name, bool ack,
        uint8_t* endDeviceAddress, uint8_t addressSize,
        uint16_t counter, uint8_t port,
        uint8_t* payload, uint8_t payloadSize,
        uint8_t* keyMIC,
        uint8_t spreadingFactor, uint8_t transmissionPower,
        float bandwidth, float channelFrequency);

omnetpp::cPacket* createMessageDownlink(
        const char* name, bool ack,
        uint8_t* endDeviceAddress, uint8_t addressSize,
        uint16_t counter, uint8_t port,
        uint8_t* payload, uint8_t payloadSize,
        uint8_t* keyMIC,
        uint8_t spreadingFactor, uint8_t transmissionPower,
        float bandwidth, float channelFrequency);

omnetpp::cPacket* createMessageIp(
        const char* name,
        uint8_t* srcAddress, uint8_t* destAddress, uint8_t addressSize,
        uint8_t* payload, uint8_t payloadSize,
        omnetpp::cPacket* encapsulatedPacket);

omnetpp::cPacket* createMessageTcp(
        const char* name,
        uint16_t srcPort, uint16_t destPort,
        uint32_t seqNumber, uint32_t ackNumber, bool ack,
        uint8_t* payload, uint8_t payloadSize,
        omnetpp::cPacket* encapsulatedPacket);

omnetpp::cPacket* createMessageUdp(
        const char* name,
        uint16_t srcPort, uint16_t destPort,
        uint8_t* payload, uint8_t payloadSize,
        omnetpp::cPacket* encapsulatedPacket);

// ********************* MESSAGE CREATIONS END *********************



// ********************* MESSAGE BROADCASTING *********************

// Send LoRa broadcast message
void sendBroadcast(omnetpp::cSimpleModule* instance, omnetpp::cMessage* msg, const char* gatename);

// Send delayed LoRa broadcast message
void sendDelayedBroadcast(omnetpp::cSimpleModule* instance, omnetpp::cMessage* msg, omnetpp::simtime_t delay, const char* gatename);

// ********************* MESSAGE BROADCASTING END *********************



// ********************* MESSAGE SENDING *********************

void sendSecurely(
        omnetpp::cSimpleModule* instance,
        std::map<omnetpp::cMessage*, std::tuple<omnetpp::cPacket*, std::string, int>>& eventTimeoutChannelTransmissions,
        omnetpp::cPacket* pkt, const char* gatename, int index);

void sendDelayedSecurely(
        omnetpp::cSimpleModule* instance,
        std::map<omnetpp::cMessage*, std::tuple<omnetpp::cPacket*, std::string, int>>& eventTimeoutChannelTransmissions,
        omnetpp::cPacket* pkt, omnetpp::simtime_t delay, const char* gatename, int index);

void sendBroadcastSecurely(
        omnetpp::cSimpleModule* instance,
        std::map<omnetpp::cMessage*, std::tuple<omnetpp::cPacket*, std::string, int>>& eventTimeoutChannelTransmissions,
        omnetpp::cPacket* pkt, const char* gatename);

void sendDelayedBroadcastSecurely(
        omnetpp::cSimpleModule* instance,
        std::map<omnetpp::cMessage*, std::tuple<omnetpp::cPacket*, std::string, int>>& eventTimeoutChannelTransmissions,
        omnetpp::cPacket* pkt, omnetpp::simtime_t delay, const char* gatename);

// ********************* MESSAGE SENDING END *********************



// ********************* MESSAGE PHYSICAL CALCULATIONS *********************

// Calculate datarate of the frame in bits/s
double calculateDatarate(uint8_t msgSF, float msgBW, uint8_t msgCR);

// Calculate datarate of the frame in bits/s
double calculateDatarate(omnetpp::cPacket* msg);

// Calculate expected time on air of preamble and entire frame in ms
//std::tuple<double, double> calculateTimeOnAir(omnetpp::cPacket* msg);
std::tuple<double, double> calculateTimeOnAir(omnetpp::cMessage* msg);

// ********************* MESSAGE PHYSICAL CALCULATIONS END *********************



// ********************* MESSAGE SUBMITTINGS *********************

/*void sendMessageTcp(
        const char* messageName, const char* timeoutName,
        std::array<uint8_t, IPv4_ADDRESS_SIZE>& destAddress, unsigned port,
        uint8_t* payload, unsigned payloadSize,
        omnetpp::cPacket* encapsulatedPacket);*/

// ********************* MESSAGE SUBMITTINGS END *********************



// ********************* CONVERSIONS *********************

// Convert unsigned to array of bytes
void uintToBytes(unsigned n, uint8_t* dest);

// Convert array of bytes to unsigned
unsigned bytesToUint(uint8_t* bytes);

// Convert long unsigned to array of bytes
void luintToBytes(long unsigned n, uint8_t* dest);

// Convert array of bytes to long unsigned
long unsigned bytesToLuint(uint8_t* bytes);

// ********************* CONVERSIONS END *********************



// ********************* SECURITY *********************

// Generate a nonce in [0, max[
unsigned generateNonce(unsigned max);

// Generate key from root key and nonce
//const char* generateKey(char rootKey[], unsigned nonce1, unsigned nonce2, uint8_t networkId[]);
const char* generateKey(uint8_t rootKey[], unsigned nonce1, unsigned nonce2, uint8_t networkId[]);

void printKey(uint8_t* src, size_t size, const char* name, std::ostream& stream);

// ********************* SECURITY END *********************



// ********************* LoRa *********************

// Return true if A is in LoRa range of B
bool isInLoRaRange(unsigned ax, unsigned ay, unsigned bx, unsigned by, double* distance, float range);

// Simulate RSSI associated with an input message
int calculateRSSI(
        omnetpp::cSimpleModule* instance, unsigned ax, unsigned ay, unsigned bx, unsigned by, std::ostream& stream);

void getPhysicalParameters(
        int region, double* dutyCycle,
        std::map<float, std::vector<std::tuple<float, uint8_t>>>& bandwidths,
        std::map<float, std::vector<float>>& channelFrequencies,
        bool getBandwidths=true);

void getPhysicalParameters(int region, std::map<float, std::vector<float>>& channelFrequencies);
// ********************* LoRa END *********************



// ********************* BACKGROUND NOISE *********************

// Calculate BER probability according to the experimental BER curves
float calculateProbabilityBER(int maxBer, int snrThreshold, int snr);

// Return Signal to Interference plus Noise Ratio and message drop probability
std::tuple<double, float> applyExternalNoise(
        omnetpp::cMessage* msg, int rssi, uint8_t sf, float bw,
        std::map<long, std::tuple<double, float>>& interferences, std::ostream& stream);

// Return Signal to Interference plus Noise Ratio and message drop probability
std::tuple<double, float> applyExternalNoise(
        omnetpp::cMessage* msg, int rssi,
        std::map<long, std::tuple<double, float>>& interferences, std::ostream& stream);
// ********************* BACKGROUND NOISE END *********************


//int binomialCoefficient(int n, int k);
