#include <string.h>
#include <omnetpp.h>

#include <map>
#include <list>
//#include <utility> // For std::pair
#include <tuple>
#include <random>

#include "Utils.h"

using namespace omnetpp;
using namespace std;


class LoRaGateway : public cSimpleModule/*, public cListener*/ {
  private:
    // ============ CLASS CONSTANTS ==============
    // $o and $i suffix is used to identify the input/output part of a two way gate
    const char* LORA_GATE_BASENAME = "lorag";
    const char* LORA_GATE_IN       = "lorag$i";
    const char* LORA_GATE_OUT      = "lorag$o";
    const char* IP_GATE_IN    = "ethg$i";
    const char* IP_GATE_OUT   = "ethg$o";
    // ============ CLASS CONSTANTS ==============

    // ============ CLASS VARIABLES ==============
    // Identifiers
    uint8_t devEUI[EUI_SIZE];
    uint8_t networkId[NETWORK_ID_SIZE];

    // Root key for session key with the associated end device
    uint8_t associationKey[KEY_LORAWAN_SIZE];

    // Root key for common session key with end device and gateways in end device radio range
    uint8_t commonEndDeviceKey[KEY_LORAWAN_SIZE];

    // Map indexed by end device addresses in the radio range
    // and value the common session key shared with end device and gateways in end device radio range
    std::map<std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::array<uint8_t, KEY_LORAWAN_SIZE>> commonEndDeviceSKeys;

    // Root key for common session key with gateways in the radio range
    uint8_t commonGatewaysKey[KEY_LORAWAN_SIZE];

    // Session key exclusively shared with network server for IP packets
    uint8_t nwkSKey[KEY_LORAWAN_SIZE];


    // Gateway IP address assigned by the network server
    uint8_t address[IPv4_ADDRESS_SIZE];

    // Network server IP address
    std::array<uint8_t, IPv4_ADDRESS_SIZE> networkServerAddress;

    // Resources
    double cpuLoad;            // %
    double gpuLoad;            // %
    double ramLoad;            // %
    long storageOccupied;      // bytes
    unsigned long networkIn;   // byte/s
    unsigned long networkOut;  // byte/s

    // Stage of the gateway (activated or not)
    uint8_t stage;

    // Set of IP addresses of neighbor gateways
    std::set<std::array<uint8_t, IPv4_ADDRESS_SIZE>> neighborGatewayAddresses;

    // Map indexed by cluster session key
    // and value the corresponding set of IP addresses included in the cluster.
    // The union of all subsets in this map is equal to the set neighborGatewayAddresses
    std::map<std::array<uint8_t, KEY_LORAWAN_SIZE>, std::set<std::array<uint8_t, IPv4_ADDRESS_SIZE>>> clusters;

    // Map indexed by end device, gateway, network server addresses (SOLO END DEVICE?)
    // and value a tuple composed of:
    // - last message sent to the address,
    // - corresponding timeout,
    // - request ID,
    // - level number,
    // - fCntUp,
    // - fCntDown,
    // - IP address of the associated gateway,
    // - is data profile spread to network server?,
    // - IP address of the peer who forwarded the end device message for retrieving the cluster session key
    //std::map<std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::tuple<cPacket*, cMessage*, uint8_t, uint8_t, uint16_t, uint16_t, std::array<uint8_t, IPv4_ADDRESS_SIZE>>> map;
    //std::map<std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::tuple<cPacket*, cMessage*, uint8_t, uint8_t, uint16_t, uint16_t, std::array<uint8_t, IPv4_ADDRESS_SIZE>, bool>> map;
    std::map<std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::tuple<cPacket*, cMessage*, uint8_t, uint8_t, uint16_t, uint16_t, std::array<uint8_t, IPv4_ADDRESS_SIZE>, bool, std::array<uint8_t, IPv4_ADDRESS_SIZE>>> map;

    // Map indexed by end device addresses associated with the gateway
    // and value a tuple composed of:
    // - symmetric session key,
    // - data profile (in our deployment, the data profile is represented only by the location string)
    // - time at which the association is performed
    //std::map<std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::tuple<std::array<uint8_t, KEY_LORAWAN_SIZE>, std::string, std::time_t>> associations;
    std::map<std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::tuple<std::array<uint8_t, KEY_LORAWAN_SIZE>, std::string, simtime_t>> associations;

    // Map indexed by gateway and network server addresses
    // and value a tuple composed of:
    // - last message sent to the address,
    // - corresponding timeout,
    // - sequence number IN,
    // - sequence number OUT,
    // - ACK sequence number
    //std::map<std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::tuple<cPacket*, cMessage*, uint32_t, uint32_t, uint32_t>> mapTcp;
    std::map<std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::tuple<std::list<cPacket*>, cMessage*, uint32_t, uint32_t, uint32_t>> mapTcp;

    // Routing table for associating an IP address (gateway or network server) with a gate index
    std::map<std::array<uint8_t, IPv4_ADDRESS_SIZE>, int> routingTable;


    // Timers duration
    float timeoutResources; // s
    float timeoutLoRa;      // s
    float timeoutTcp;       // s

    // Timeout to update resources
    cMessage* eventTimeoutResources;

    // Timeout to send NEARBY_GATEWAYS message
    cMessage* eventTimeoutNearbyGateways;

    // Inverted index with timeouts for retransmissions and corresponding address
    std::map<cMessage*, std::array<uint8_t, IPv4_ADDRESS_SIZE>> eventTimeoutRetransmissions;

    // Timeout to handle the transmissions on the channels to avoid busy channel error
    std::map<cMessage*, std::tuple<cPacket*, std::string, int>> eventTimeoutChannelTransmissions;


    // For statistics
    // Protocol
    unsigned long messagesSentCount;
    unsigned long messagesReceivedCount;
    unsigned long messagesLostCount;
    unsigned long messagesInvalidCount;

    unsigned long messagesSentLoRaCount;
    unsigned long messagesReceivedLoRaCount;
    unsigned long messagesLostLoRaCount;
    unsigned long messagesInvalidLoRaCount;
    unsigned long messagesSentLoRaGatewaysCount;
    unsigned long messagesReceivedLoRaGatewaysCount;
    unsigned long messagesLostLoRaGatewaysCount;
    //unsigned long messagesInvalidLoRaGatewaysCount;
    unsigned long messagesSentLoRaDevicesCount;
    unsigned long messagesReceivedLoRaDevicesCount;
    unsigned long messagesLostLoRaDevicesCount;
    //unsigned long messagesInvalidLoRaDevicesCount;

    unsigned long messagesSentIpCount;
    unsigned long messagesReceivedIpCount;
    unsigned long messagesLostIpCount;
    unsigned long messagesInvalidIpCount;
    unsigned long messagesSentIpGatewaysCount;
    unsigned long messagesReceivedIpGatewaysCount;
    unsigned long messagesLostIpGatewaysCount;
    //unsigned long messagesInvalidIpGatewaysCount;
    unsigned long messagesSentIpServerCount;
    unsigned long messagesReceivedIpServerCount;
    unsigned long messagesLostIpServerCount;
    //unsigned long messagesInvalidIpServerCount;

    unsigned long interferencesCount;
    unsigned long interferencesPossibleCount;

    // Data
    unsigned long messagesSentDataCount;
    unsigned long messagesReceivedDataCount;
    unsigned long messagesLostDataCount;
    unsigned long messagesInvalidDataCount;

    unsigned long messagesSentGatewaysDataCount;
    unsigned long messagesReceivedGatewaysDataCount;

    unsigned long messagesSentServerDataCount;
    unsigned long messagesReceivedServerDataCount;

    unsigned long messagesReceivedDevicesDataCount;

    unsigned long interferencesDataCount;
    unsigned long interferencesPossibleDataCount;


    // Position of the gateway for creating communication channels only with gateways and end devices in the radio range
    unsigned posX;
    unsigned posY;


    // Variables for physical layer
    // Map indexed by the bandwidth and values the channel frequencies
    std::map<float, std::vector<float>> channelFrequencies;


    // Variables for handling interferences
    //std::set<int> neighborDevicesInterferences;
    std::set<cModule*> neighborDevicesInterferences;
    std::set<cModule*> neighborGatewaysInterferences;

    // List of tuples composed of:
    // - messages sent by neighbor gateways
    // - sender module
    // - corresponding sending and expiration times (preamble and time on air)
    std::list<std::tuple<cPacket*, cModule*, simtime_t, simtime_t, simtime_t>> neighborMessages;

    // Map for handling interferences indexed by the message and value a tuple composed of:
    // - the SINR resulting after applying external noise
    // - the probability that the message is dropped
    //std::map<cMessage*, std::tuple<int, float>> interferences;
    // Use the shared ID among a message and its copies instead of the pointer of the message
    // because the sensor sends in broadcast multiple duplicates
    //std::map<long, std::tuple<int, float>> interferences;
    // Replace SINR with sum of interferences and noise in mW
    std::map<long, std::tuple<double, float>> interferences;

    // Variable to automatically stop the simulation when all end device have finished the protocol run
    int endDeviceTerminations;
    // ============ CLASS VARIABLES ==============

    // ============ CLASS FUNCTIONS ==============
    bool isValidPort(unsigned port, uint8_t msgType);
    uint8_t isValidLoRaFrame(
            //cMessage* msg, bool* isUplinkFrame,
            cMessage* msg, uint8_t* frameType,
            uint8_t* deviceAddress, uint16_t* counter, unsigned* port, uint8_t* payload,
            uint8_t* spreadingFactor, uint8_t* transmissionPower,
            float* bandwidth, float* channelFrequency);
    uint8_t isValidMessageIp(
            cMessage* msg,
            uint8_t* srcAddress, uint8_t* destAddress,
            bool isTcp, uint32_t sequenceNumber, uint32_t ackNumber, uint8_t* payload);
    uint8_t isValidMessageTcp(cMessage* msg, unsigned* port, uint32_t* sequenceNumber, uint32_t* ackNumber);
    uint8_t isValidMessageUdp(cMessage* msg, unsigned* port);
    void printError(uint8_t error);
    void processMessageFromTransportLayer(cMessage* msg);
    void processMessageFromLoRaLayer(cMessage* msg, /*const*/ std::array<uint8_t, IPv4_ADDRESS_SIZE> srcAddress={0,0,0,0});
    void decryptUplink(
            uint8_t* src, uint8_t* dest, uint8_t* decryptKey,
            std::array<uint8_t, IPv4_ADDRESS_SIZE>& endDeviceAddress,
            std::array<uint8_t, IPv4_ADDRESS_SIZE>& srcAddress );
    void decryptDownlink(
        uint8_t* src, uint8_t* dest, uint8_t* decryptKey, std::array<uint8_t, IPv4_ADDRESS_SIZE>& srcAddress);
    uint8_t processData(uint8_t data);
    bool isStateChanged(unsigned timestamp);

    void sendMessageHelloGateway(
            //uint8_t* endDeviceAddress, uint16_t counter,
            std::array<uint8_t, IPv4_ADDRESS_SIZE>& endDeviceAddress, uint16_t counter,
            uint8_t spreadingFactor, uint8_t transmissionPower, float bandwidth, float channelFrequency);
    void sendMessageNearbyGateway();
    void sendMessageCommonKeyAck(
            uint8_t* endDeviceAddress, uint16_t counter, bool ack,
            uint8_t spreadingFactor, uint8_t transmissionPower, float bandwidth, float channelFrequency);
    void sendMessageShareCounter(uint8_t* endDeviceAddress, uint16_t counter);
    void sendMessageConnection(uint8_t* endDeviceAddress, uint8_t requestId, uint16_t counter, uint8_t res);
    //void sendMessageDataProfile(uint8_t* endDeviceAddress, const char* dataProfile, int dataProfileSize);
    void sendMessageProcessedData(
            uint8_t* endDeviceAddress, uint8_t processedData, uint8_t* timestamp,
            const char* dataProfile, int dataProfileSize);
    void sendMessageTcp(
            const char* messageName, const char* timeoutName,
            std::array<uint8_t, IPv4_ADDRESS_SIZE>& destAddress, unsigned port,
            uint8_t* payload, unsigned payloadSize, cPacket* encapsulatedPacket);
    void sendMessageIp(
            cPacket* msg, std::array<uint8_t, IPv4_ADDRESS_SIZE>& destAddress, bool sendDuplicate, simtime_t delay=0);
    void forwardMessageIp(
            cMessage* msg, const char* name,
            std::array<uint8_t, IPv4_ADDRESS_SIZE>& destAddress, unsigned port, bool reencrypt, simtime_t delay=0);
    void forwardMessageToNeighbors(cMessage* msg, uint8_t* decryptKey);
    void sendMessageLoRa(
            cPacket* msg, bool isTowardsDevices, bool delayTransmission, double delay=0,
            bool retransmit=false, cMessage* eventTimeout=nullptr);
    void notifyNeighborGateways(
            cPacket* msg, cModule* sender, simtime_t sendingTime, simtime_t arrivalPreamble, simtime_t arrivalFrame);
    //void notifyNeighborDevices(cPacket* msgInterference, std::list<std::tuple<cPacket*, int, bool>>& interferedMessages);
    void notifyNeighborDevices(cPacket* msgInterference, std::list<std::tuple<cPacket*, cModule*, bool>>& interferedMessages);
    //void notifyNeighborGateways(cPacket* msgInterference, std::list<std::tuple<cPacket*, int, bool>>& interferedMessages);
    void notifyNeighborGateways(cPacket* msgInterference, std::list<std::tuple<cPacket*, cModule*, bool>>& interferedMessages);
    void verifyTransmissionInterference(cPacket* msg, simtime_t sendingTime, bool isTowardsDevices);
    bool surviveMessageToLoRaInterference(cMessage* msg);
    //float calculateProbabilityBER(int maxBer, int snrThreshold, int snr);
    //std::tuple<int, float> applyExternalNoise(cMessage* msg, int rssi, uint8_t sf, float bw);
    //std::tuple<double, float> applyExternalNoise(cMessage* msg, int rssi, uint8_t sf, float bw);
    //std::tuple<int, float> applyExternalNoise(cMessage* msg, int rssi);
    //std::tuple<double, float> applyExternalNoise(cMessage* msg, int rssi);
    void sendSignalInterference(bool isDataInterferer, bool isDataInterfered);
    void sendSignalSentIp(bool isDataMessage, std::array<uint8_t, IPv4_ADDRESS_SIZE>& destAddress);

    uint8_t getCpuLoad();
    uint8_t getGpuLoad();
    uint8_t getRamLoad();
    uint8_t getStorageLoad();
    uint8_t getNetworkIn();
    uint8_t getNetworkOut();
    bool checkResources();
    void updateResources();
    // ============ CLASS FUNCTIONS ==============

    // ============ CLASS SIGNALS ==============
    // Class signals for statistic collection
    // Protocol
    simsignal_t signalSent;
    simsignal_t signalReceived;
    simsignal_t signalLost;
    simsignal_t signalInvalid;

    simsignal_t signalSentLoRa;
    simsignal_t signalReceivedLoRa;
    simsignal_t signalLostLoRa;
    simsignal_t signalInvalidLoRa;

    simsignal_t signalSentLoRaGateways;
    simsignal_t signalReceivedLoRaGateways;
    simsignal_t signalLostLoRaGateways;
    //simsignal_t signalInvalidLoRaGateways;

    simsignal_t signalSentLoRaDevices;
    simsignal_t signalReceivedLoRaDevices;
    simsignal_t signalLostLoRaDevices;
    //simsignal_t signalInvalidLoRaDevices;

    simsignal_t signalSentIp;
    simsignal_t signalReceivedIp;
    simsignal_t signalLostIp;
    simsignal_t signalInvalidIp;

    simsignal_t signalSentIpGateways;
    simsignal_t signalReceivedIpGateways;
    simsignal_t signalLostIpGateways;
    //simsignal_t signalInvalidIpGateways;

    simsignal_t signalSentIpServer;
    simsignal_t signalReceivedIpServer;
    simsignal_t signalLostIpServer;
    //simsignal_t signalInvalidIpServer;


    simsignal_t signalSentCount;
    simsignal_t signalReceivedCount;
    simsignal_t signalLostCount;
    simsignal_t signalInvalidCount;

    simsignal_t signalSentLoRaCount;
    simsignal_t signalReceivedLoRaCount;
    simsignal_t signalLostLoRaCount;
    simsignal_t signalInvalidLoRaCount;

    simsignal_t signalSentLoRaGatewaysCount;
    simsignal_t signalReceivedLoRaGatewaysCount;
    simsignal_t signalLostLoRaGatewaysCount;
    //simsignal_t signalInvalidLoRaGatewaysCount;

    simsignal_t signalSentLoRaDevicesCount;
    simsignal_t signalReceivedLoRaDevicesCount;
    simsignal_t signalLostLoRaDevicesCount;
    //simsignal_t signalInvalidLoRaDevicesCount;

    simsignal_t signalSentIpCount;
    simsignal_t signalReceivedIpCount;
    simsignal_t signalLostIpCount;
    simsignal_t signalInvalidIpCount;

    simsignal_t signalSentIpGatewaysCount;
    simsignal_t signalReceivedIpGatewaysCount;
    simsignal_t signalLostIpGatewaysCount;
    //simsignal_t signalInvalidIpGatewaysCount;

    simsignal_t signalSentIpServerCount;
    simsignal_t signalReceivedIpServerCount;
    simsignal_t signalLostIpServerCount;
    //simsignal_t signalInvalidIpServerCount;

    simsignal_t signalInterference;
    simsignal_t signalInterferencePossible;
    simsignal_t signalInterferenceCount;
    simsignal_t signalInterferencePossibleCount;


    // Data
    simsignal_t signalSentData;
    simsignal_t signalReceivedData;
    simsignal_t signalLostData;
    simsignal_t signalInvalidData;

    simsignal_t signalSentGatewaysData;
    simsignal_t signalReceivedGatewaysData;

    simsignal_t signalSentServerData;
    simsignal_t signalReceivedServerData;

    simsignal_t signalReceivedDevicesData;

    simsignal_t signalSentDataCount;
    simsignal_t signalReceivedDataCount;
    simsignal_t signalLostDataCount;
    simsignal_t signalInvalidDataCount;

    simsignal_t signalSentGatewaysDataCount;
    simsignal_t signalReceivedGatewaysDataCount;

    simsignal_t signalSentServerDataCount;
    simsignal_t signalReceivedServerDataCount;

    simsignal_t signalReceivedDevicesDataCount;

    simsignal_t signalInterferenceData;
    simsignal_t signalInterferencePossibleData;
    simsignal_t signalInterferenceDataCount;
    simsignal_t signalInterferencePossibleDataCount;


    simsignal_t signalRSSI;

    simsignal_t signalConnected;
    simsignal_t signalCpu;
    simsignal_t signalGpu;
    simsignal_t signalRam;
    simsignal_t signalStorage;
    simsignal_t signalNetworkIn;
    simsignal_t signalNetworkOut;
    // ============ CLASS SIGNALS ==============

  protected:
    // The following redefined virtual function holds the algorithm
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void refreshDisplay() const override;

  public:
    LoRaGateway();
    virtual ~LoRaGateway();

    //virtual void receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj, cObject* details) override;
    //virtual void receiveSignal(cComponent* source, simsignal_t signalID, long l, cObject* details) override;

    unsigned getPosX();
    unsigned getPosY();
    const char* getLoRaGateIn();
    const char* getLoRaGateOut();
    const char* getLoRaGateBasename();
    //simsignal_t getSignalInterference();

    //void addNeighborDevice(int neighborId);
    void addNeighborDevice(cModule* neighbor);
    void addNeighborGateway(LoRaGateway* neighbor);
    void receiveNotification(
            cPacket* msg, cModule* sender, simtime_t sendingTime, simtime_t arrivalPreamble, simtime_t arrivalFrame);

    //void handleInterference(cModule* source, cMessage* msg);
    //void handleInterference(cPacket* msgInterference, std::list<std::tuple<cPacket*, bool>>& interferedMessages);
    //void handleInterference(cPacket* msgInterference, std::list<std::tuple<cPacket*, int, bool>>& interferedMessages);
    void handleInterference(
            cPacket* msgInterference, std::list<std::tuple<cPacket*, cModule*, bool>>& interferedMessages,
            int rssiInterference, bool isUplink);
    void handleInterferenceUplink(
            cPacket* msgInterference, std::list<std::tuple<cPacket*, cModule*, bool>>& interferedMessages);
    void handleInterferenceDownlink(
            cPacket* msgInterference, cModule* interferer,
            std::list<std::tuple<cPacket*, cModule*, bool>>& interferedMessages);

    void deviceFinish();
};
