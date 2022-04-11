#include <string.h>
#include <omnetpp.h>

#include "Utils.h"

//namespace masterthesis {

using namespace omnetpp;
using namespace std;

class LoRaNetworkServer : public cSimpleModule {
  private:
    // ============ CLASS CONSTANTS ==============
    // $o and $i suffix is used to identify the input/output part of a two way gate
    const char* IP_GATEWAYS_GATE_OUT = "ethg$o";
    const char* IP_INTERNET_GATE_OUT = "pppg$o";
    // ============ CLASS CONSTANTS ==============

    // ============ CLASS VARIABLES ==============
    // Identifiers (simplification without join server)
    uint8_t joinEUI[EUI_SIZE];
    uint8_t networkId[NETWORK_ID_SIZE];

    // Root keys for OTAA, one key per end device EUI
    std::map<std::array<uint8_t, EUI_SIZE>, std::array<uint8_t, KEY_LORAWAN_SIZE>> nwkKeys;
    std::map<std::array<uint8_t, EUI_SIZE>, std::array<uint8_t, KEY_LORAWAN_SIZE>> appKeys;


    // Network server IP address
    uint8_t address[IPv4_ADDRESS_SIZE];

    // Variables for assigning end devices addresses
    long endDeviceActivations;
    long numGateways;

    // Map indexed by end device and gateway EUIs
    // and value a tuple composed of:
    // - the corresponding address
    // - devNonce included in the Join Request
    std::map<std::array<uint8_t, EUI_SIZE>, std::tuple<std::array<uint8_t, IPv4_ADDRESS_SIZE>, uint16_t>> addressAssignments;

    // Map indexed by gateway IP addresses
    // and value the corresponding session keys (securely stored)
    std::map<std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::array<uint8_t, KEY_LORAWAN_SIZE>> gatewaySessionKeys;

    // Map indexed by end device addresses
    // and value the corresponding session keys (securely stored)
    std::map<std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::array<uint8_t, KEY_LORAWAN_SIZE>> endDeviceSessionKeys;

    // Map indexed by end device addresses
    // and value a tuple composed of:
    // - request ID,
    // - fCntUp,
    // - fCntDown,
    // - is CONNECTION message from gateway received?,
    // - is CONNECTION message from end device received?,
    // - IP address of the gateway to associate with
    //std::map<std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::tuple<std::array<uint8_t, KEY_LORAWAN_SIZE>, uint8_t, uint16_t, uint16_t, bool, bool, std::array<uint8_t, IPv4_ADDRESS_SIZE>>> map;
    std::map<std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::tuple<uint8_t, uint16_t, uint16_t, bool, bool, std::array<uint8_t, IPv4_ADDRESS_SIZE>>> map;

    // Map indexed by end device addresses associated with the gateway
    // and value the gateway addresses
    std::map<std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::array<uint8_t, IPv4_ADDRESS_SIZE>> associations;

    // Map indexed by data profiles of end devices
    // and value the corresponding end device addresses
    std::map<std::string, std::array<uint8_t, IPv4_ADDRESS_SIZE>> dataProfiles;

    // Map indexed by TCP/IP addresses
    // and value a tuple composed of:
    // - last message sent to the address,
    // - corresponding timeout,
    // - sequence number IN,
    // - sequence number OUT,
    // - ACK sequence number
    // - is internal network?
    //std::map<std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::tuple<cPacket*, cMessage*, uint32_t, uint32_t, uint32_t, bool>> mapTcp;

    // replace last message sent to the address with last messages sent to the address
    std::map<std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::tuple<std::list<cPacket*>, cMessage*, uint32_t, uint32_t, uint32_t, bool>> mapTcp;

    // Routing table for associating a gateway with a gate index
    std::map<std::array<uint8_t, IPv4_ADDRESS_SIZE>, int> routingTable;


    // Timers duration
    float timeoutLoRa;
    float timeoutTcp;

    // Inverted index with timeouts for retransmissions and corresponding address
    std::map<cMessage*, std::array<uint8_t, IPv4_ADDRESS_SIZE>> eventTimeoutRetransmissions;

    // Timeout to handle the transmissions on the channels to avoid busy channel error
    std::map<cMessage*, std::tuple<cPacket*, std::string, int>> eventTimeoutChannelTransmissions;


    // For display strings during animation
    unsigned long messagesIn;
    unsigned long messagesOut;
    // ============ CLASS VARIABLES ==============

    // ============ CLASS FUNCTIONS ==============
    bool isValidPort(unsigned port, uint8_t msgType);
    uint8_t isValidLoRaFrame(
            cMessage* msg, uint8_t* frameType,
            uint8_t* deviceAddress, uint16_t* counter, unsigned* port, uint8_t* payload,
            uint8_t* spreadingFactor, uint8_t* transmissionPower,
            float* bandwidth, float* channelFrequency);
    uint8_t isValidMessageIp(
            cMessage* msg,
            uint8_t* srcAddress, uint8_t* destAddress,
            bool isTcp, uint32_t sequenceNumber, uint32_t ackNumber,
            uint8_t* payload, cPacket** encapsulatedPacket);
    uint8_t isValidMessageTcp(
            cMessage* msg, unsigned* port,
            uint32_t* sequenceNumber, uint32_t* ackNumber, cPacket** encapsulatedPacket);
    uint8_t isValidMessageUdp(cMessage* msg, unsigned* port, cPacket** encapsulatedPacket);
    void printError(uint8_t error);
    void sendMessageTcp(
            const char* messageName, const char* timeoutName,
            std::array<uint8_t, IPv4_ADDRESS_SIZE>& destAddress, unsigned port,
            uint8_t* payload, unsigned payloadSize,
            cPacket* encapsulatedPacket, const char* gateName);
    // ============ CLASS FUNCTIONS ==============

  protected:
      // The following redefined virtual function holds the algorithm.
      virtual void initialize() override;
      virtual void handleMessage(cMessage *msg) override;
      virtual void refreshDisplay() const override;

  public:
    LoRaNetworkServer();
    virtual ~LoRaNetworkServer();
};

// The module class needs to be registered with OMNeT++
Define_Module(LoRaNetworkServer);

LoRaNetworkServer::LoRaNetworkServer() {
    // Set the pointer to nullptr, so that the destructor won't crash
    // even if initialize() doesn't get called because of a runtime
    // error or user cancellation during the startup process.
}

LoRaNetworkServer::~LoRaNetworkServer() {
    // Dispose of dynamically allocated the objects
    /*for (auto it : mapTcp) {
        auto msg1 = std::get<0>(it.second);
        auto msg2 = std::get<1>(it.second);

        if (msg1 && msg1->getOwner() == this)
            delete msg1;

        cancelAndDelete(msg2);
    }*/

    /*for (auto it : eventTimeoutRetransmissions) {
        eventTimeoutRetransmissions.erase(it.first);
        cancelAndDelete(it.first);
    }*/

    /*for (auto it : eventTimeoutChannelTransmissions) {
        //eventTimeoutRetransmissions.erase(it.first);
        //cancelAndDelete(it.first);

        auto msg = std::get<0>(it.second);
        if (msg && msg->getOwner() == this)
            delete msg;
    }*/

    gatewaySessionKeys.clear();
    endDeviceSessionKeys.clear();
    map.clear();
    associations.clear();
    dataProfiles.clear();
    mapTcp.clear();
    routingTable.clear();
    eventTimeoutRetransmissions.clear();
    eventTimeoutChannelTransmissions.clear();
}

void LoRaNetworkServer::initialize() {
    // Initialize is called at the beginning of the simulation.
    EV << "Initializing network server...\n";

    // Initialize class variables.
    // Set network server address
    address[0] = IPv4_ADDRESS_NETWORK_SERVER_0;
    address[1] = IPv4_ADDRESS_NETWORK_SERVER_1;
    address[2] = IPv4_ADDRESS_NETWORK_SERVER_2;
    address[3] = IPv4_ADDRESS_NETWORK_SERVER_3;

    EV << "Network server IP address: "
        << (int) address[0] << "."
        << (int) address[1] << "."
        << (int) address[2] << "."
        << (int) address[3]
        << "\n";

    // Set Join EUI
    for (int i=0; i < EUI_SIZE; i++)
        joinEUI[i] = i+10;

    // Set network server ID
    networkId[0] = 1;
    networkId[1] = 2;
    networkId[2] = 3;
    /*networkId[0] = (rand() + (int) uniform(0, 255)) % 256;
    networkId[1] = (rand() + (int) uniform(0, 255)) % 256;
    networkId[2] = (rand() + (int) uniform(0, 255)) % 256;*/


    printKey(joinEUI, sizeof(joinEUI), "joinEUI", EV);
    printKey(networkId, sizeof(networkId), "networkId", EV);

    EV << "\nRoot keys of end devices:\n";

    // Different nwkKey per device to activate multiple device at the same time
    // and the same must be stored on the network server
    // (limited to 2^(sizeof(int)*8) = 2^32 end devices)
    int numEndDevices = getParentModule()->par("nEndDevices").intValue();
    numGateways       = getParentModule()->par("nGateways").intValue();

    for (int i=0; i < numEndDevices; i++) {
        std::array<uint8_t, EUI_SIZE> devEUI {};
        std::array<uint8_t, KEY_LORAWAN_SIZE> nwkKey {};

        //for (int j=0; j < EUI_SIZE; j++)
            //devEUI[j]  = (rand() + (int) uniform(0, 100)) % 256;

        int devId = numGateways + 1 + i;
        uintToBytes(devId, &devEUI[4]);
        uintToBytes(devId, nwkKey.data());

        nwkKeys[devEUI] = nwkKey;

        printKey(devEUI.data(), EUI_SIZE, "devEUI", EV);
        printKey(nwkKey.data(), KEY_LORAWAN_SIZE, "nwkKey", EV);
        EV << "\n";
    }

    endDeviceActivations = 0;

    // Set timers.
    // Wait the receive window of the end device approx begins (RX_DELAY_1 + 0.1) and repeat in the same moment (TX_DELAY)
    timeoutLoRa = RX_DELAY_1 + 0.1 + TX_DELAY;
    timeoutTcp  = 3;


    messagesIn  = 0;
    messagesOut = 0;

    EV << "Network server initialized\n";
}

void LoRaNetworkServer::handleMessage(cMessage *msgIn) {
    // The handleMessage() method is called whenever a message arrives at the module

    // For analyzing MICs
    std::cout << "*****************\nServer\n";

    EV << "============= NETWORK SERVER =============\n";
    EV << "Received message: " << msgIn->getName() << "\n";

    // Check if the message is a timeout for handling transmissions on the channels
    auto itTX = eventTimeoutChannelTransmissions.find(msgIn);
    if (itTX != eventTimeoutChannelTransmissions.end()) {
        // Get the entry from the map
        auto tuple = itTX->second;

        EV << "gatename: " << std::get<1>(tuple) << "\n";
        EV << "index: " << std::get<2>(tuple) << "\n";

        // Retry to send
        sendSecurely(this, eventTimeoutChannelTransmissions, std::get<0>(tuple), std::get<1>(tuple).c_str(), std::get<2>(tuple));

        // Remove the entry from the map
        eventTimeoutChannelTransmissions.erase(itTX);

        // Delete the timeout message
        delete msgIn;

        return;
    }

    // Check if the message is a timeout through the inverted index
    std::map<cMessage*, std::array<uint8_t, IPv4_ADDRESS_SIZE>>::iterator it = eventTimeoutRetransmissions.find(msgIn);
    if (it != eventTimeoutRetransmissions.end()) {
        // The message is a timeout.
        // Handle retransmissions on the internal network (LoRaWAN) and on the external network (Internet)
        EV << "Timeout expired, resending message and restarting timer\n";

        // Get the entry
        std::array<uint8_t, IPv4_ADDRESS_SIZE> destAddress = it->second;
        auto tuple = mapTcp[destAddress];

        //=================
        // Check if the message must be retransmitted over TCP/IP
        /*TCPSegment* traMsg = dynamic_cast<TCPSegment*>( std::get<0>(tuple) );
        if (!traMsg)
            return;

        // Decapsulate Network layer
        cPacket* netMsg = traMsg->getEncapsulatedPacket();
        if (!netMsg)
            return;

        float timeout;

        // Decapsulate LoRa Physical layer
        cPacket* phyMsg = netMsg->getEncapsulatedPacket();
        if (phyMsg) {
            // The frame has to be retransmitted on the internal network.
            // Resend the message associated with the timeout (using the map indexed by addresses)
            // updating the frame counter as the message can be refused
            // if another downlink has been received with the same counter

            // Decapsulate Datalink layer
            LoRaDatalinkFrame* dlMsg = dynamic_cast<LoRaDatalinkFrame*>(netMsg->getEncapsulatedPacket());
            if (!dlMsg)
                return;

            // Decapsulate LoRa App layer
            LoRaAppDownlinkFrame* appMsg = dynamic_cast<LoRaAppDownlinkFrame*>(dlMsg->decapsulate());
            if (!appMsg)
                return;

            // Get the end device address
            std::array<uint8_t, IPv4_ADDRESS_SIZE> endDeviceAddress;
            getArrayInMessageDownlink(
                    appMsg, &LoRaAppDownlinkFrame::getDeviceAddress, endDeviceAddress.data(), IPv4_ADDRESS_SIZE);

            // Get the corresponding session key
            uint8_t keyMIC[KEY_LORAWAN_SIZE];
            memcpy(keyMIC, endDeviceSessionKeys[endDeviceAddress].data(), KEY_LORAWAN_SIZE);

            // Increment the frame counter
            appMsg->setCounter(appMsg->getCounter() + 1);

            // Recalculate MIC
            calculateMIC(dlMsg, appMsg, keyMIC);

            // Encapsulate layers
            //phyMsg->encapsulate(dlMsg);
            dlMsg->encapsulate(appMsg);
            //netMsg->encapsulate(dlMsg);
            //traMsg->encapsulate(netMsg);

            // Update message stored in the map
            std::get<0>(tuple) = traMsg;

            timeout = timeoutLoRa;
        }
        else
            timeout = timeoutTcp;

        // Get the gate name where the frame has to be retransmitted
        const char* gateName = std::get<5>(tuple) ? IP_GATEWAYS_GATE_OUT : IP_INTERNET_GATE_OUT;

        // Send in broadcast or through a given gateway using routing.
        // In OMNeT++ the gateway does not know which is the gate index associated to the IP address.
        // Check if the address is in the routing table to retrieve the gate index
        auto itRT = routingTable.find(address);
        if (itRT != routingTable.end())
            //send(std::get<0>(tuple)->dup(), gateName, itRT->second);
            sendSecurely(this, eventTimeoutChannelTransmissions, std::get<0>(tuple)->dup(), gateName, itRT->second);
        else
            // The address is not in the routing table, send in broadcast
            //sendBroadcast(this, std::get<0>(tuple)->dup(), gateName);
            sendBroadcastSecurely(this, eventTimeoutChannelTransmissions, std::get<0>(tuple)->dup(), gateName);

        scheduleAt(simTime() + timeout, it->first);*/
        //=================

        //=================
        std::list<cPacket*> messagesOut = std::get<0>(tuple);
        for (auto msgOut : messagesOut) {
            // Check if the message must be retransmitted over TCP/IP
            TCPSegment* traMsg = dynamic_cast<TCPSegment*>(msgOut);
            if (!traMsg)
                return;

            // Decapsulate Network layer
            cPacket* netMsg = traMsg->getEncapsulatedPacket();
            if (!netMsg)
                return;

            float timeout;

            // Decapsulate LoRa Physical layer
            cPacket* phyMsg = netMsg->getEncapsulatedPacket();
            if (phyMsg) {
                // The frame has to be retransmitted on the internal network.
                // Resend the message associated with the timeout (using the map indexed by addresses)
                // updating the frame counter as the message can be refused
                // if another downlink has been received with the same counter

                // Decapsulate Datalink layer
                LoRaDatalinkFrame* dlMsg = dynamic_cast<LoRaDatalinkFrame*>(netMsg->getEncapsulatedPacket());
                if (!dlMsg)
                    return;

                // Decapsulate LoRa App layer
                LoRaAppDownlinkFrame* appMsg = dynamic_cast<LoRaAppDownlinkFrame*>(dlMsg->decapsulate());
                if (!appMsg)
                    return;

                // Get the end device address
                std::array<uint8_t, IPv4_ADDRESS_SIZE> endDeviceAddress;
                getArrayInMessageDownlink(
                        appMsg, &LoRaAppDownlinkFrame::getDeviceAddress, endDeviceAddress.data(), IPv4_ADDRESS_SIZE);

                // Get the corresponding session key
                uint8_t keyMIC[KEY_LORAWAN_SIZE];
                memcpy(keyMIC, endDeviceSessionKeys[endDeviceAddress].data(), KEY_LORAWAN_SIZE);

                // Increment the frame counter
                appMsg->setCounter(appMsg->getCounter() + 1);

                // Recalculate MIC
                //calculateMIC(dlMsg, appMsg, keyMIC);
                calculateMIC(dlMsg, keyMIC);

                // Encapsulate layers
                //phyMsg->encapsulate(dlMsg);
                dlMsg->encapsulate(appMsg);
                //netMsg->encapsulate(dlMsg);
                //traMsg->encapsulate(netMsg);

                // Update message stored in the map
                //std::get<0>(tuple) = traMsg;

                timeout = timeoutLoRa;
            }
            else
                timeout = timeoutTcp;

            // Get the gate name where the frame has to be retransmitted
            const char* gateName = std::get<5>(tuple) ? IP_GATEWAYS_GATE_OUT : IP_INTERNET_GATE_OUT;

            // Send in broadcast or through a given gateway using routing.
            // In OMNeT++ the gateway does not know which is the gate index associated to the IP address.
            // Check if the address is in the routing table to retrieve the gate index
            auto itRT = routingTable.find(destAddress);
            if (itRT != routingTable.end())
                //send(std::get<0>(tuple)->dup(), gateName, itRT->second);
                sendSecurely(this, eventTimeoutChannelTransmissions, msgOut->dup(), gateName, itRT->second);
            else
                // The address is not in the routing table, send in broadcast
                //sendBroadcast(this, std::get<0>(tuple)->dup(), gateName);
                sendBroadcastSecurely(this, eventTimeoutChannelTransmissions, msgOut->dup(), gateName);

            scheduleAt(simTime() + timeout, it->first);
        }
        //=================
    }
    else {
        // The message is not a timeout.
        // Lose the message with a certain probability
        //if (uniform(0, 1) < 2) {
        //if (uniform(0, 1) < MSG_LOSS_PROBABILITY_IP) {
        if (((rand() + (int) uniform(0,100)) % 101) < MSG_LOSS_PROBABILITY_IP) {
            EV << "Message lost!\n";

            // Make animation more informative
            bubble("Message lost!");

            // Delete the message as in OMNeT++ once sent out,
            // a message no longer belongs to the sender module and
            // it is taken over by the simulation kernel, and will eventually be delivered to the destination module.
            // Once the message arrives in the destination module, that module will have full authority over it
            delete msgIn;
            return;
        }


        unsigned port;
        uint32_t sequenceNumber;
        uint32_t ackNumber;
        bool isTcp = false;
        cPacket* encPacket;

        // Check if the message is a valid UDP segment and get useful info
        uint8_t error = isValidMessageUdp(msgIn, &port, &encPacket);
        if (error) {
            if (error != BAD_FRAME) {
                printError(error);
                delete msgIn;
                return;
            }

            // The message is not a UDP segment.
            // Check if the encapsulated message is a valid TCP packet and get useful info
            error = isValidMessageTcp(msgIn, &port, &sequenceNumber, &ackNumber, &encPacket);
            if (error) {
                printError(error);
                delete msgIn;
                return;
            }

            isTcp = true;
        }

        // The message is a valid TCP segment
        std::array<uint8_t, IPv4_ADDRESS_SIZE> srcAddress;
        uint8_t destAddress[IPv4_ADDRESS_SIZE];
        //uint8_t payloadIn[LORA_FRAME_SIZE_APP_PAYLOAD] = {};
        uint8_t payloadIn[MESSAGE_SIZE_IPV4_PAYLOAD] = {};

        // Check if the encapsulated message is a valid IP packet and get useful info
        error = isValidMessageIp(
                encPacket, srcAddress.data(), destAddress, isTcp, sequenceNumber, ackNumber, payloadIn, &encPacket);
        if (error) {
            printError(error);
            delete msgIn;
            return;
        }

        // Get the gate name and index from which the message is arrived
        char gateName[10];
        strncpy(gateName, msgIn->getArrivalGate()->getName(), sizeof(gateName));
        int gateIndex = msgIn->getArrivalGate()->getIndex();

        // Replace input with output
        gateName[strlen(gateName)-1] = 'o';

        //EV << "Gate name: " << gateName << "\n";
        //EV << "Gate index: " << gateIndex << "\n";

        // In OMNeT++ the network server does not know which is the gate index associated to the IP address.
        // Check if the source address is in the routing table
        if (routingTable.find(srcAddress) == routingTable.end())
            // Associate the gateway IP address with the gate index in the routing table
            // in order to directly contact the proper gateway instead of broadcasting the message in the future
            routingTable[srcAddress] = gateIndex;

        // Check if the message is TCP
        if (isTcp) {
            EV << "Received TCP/IP packet with seq num: " << sequenceNumber << "\n";
            EV << "Received TCP/IP packet with ACK seq num: " << ackNumber << "\n";

            // Check if the source address is in the map of TCP/IP connections
            //std::tuple<cPacket*, cMessage*, uint32_t, uint32_t, uint32_t, bool> tuple;
            std::tuple<std::list<cPacket*>, cMessage*, uint32_t, uint32_t, uint32_t, bool> tuple;
            auto it = mapTcp.find(srcAddress);

            if (it != mapTcp.end()) {
                // Get the entry
                tuple = it->second;

                if (ackNumber > std::get<3>(tuple) + 1) {
                    // Invalid ACK number
                    delete msgIn;
                    return;
                }

                // Remove the subset of last messages that have been confirmed
                int removals = 0;

                for (auto msg : std::get<0>(tuple)) {
                    TCPSegment* tcpMsg = dynamic_cast<TCPSegment*> (msg);
                    if (tcpMsg->getSequenceNumber() < ackNumber) {
                        removals++;
                        delete msg;
                    }
                }

                EV << "Items to remove from unconfirmed messages: " << removals << "\n";

                // TODO: Integrate the removal into above iteration
                for (int i=0; i < removals; i++)
                    std::get<0>(tuple).pop_front();

                // Checks if all last messages sent have been confirmed
                if (ackNumber == std::get<3>(tuple) + 1) {
                    EV << "Canceling timeout for TCP/IP packet retransmission...\n";
                    cancelEvent(std::get<1>(tuple));

                    // Do not remove the entry from the table or delete the timer to reuse it (session)
                }

                EV << "Number of messages for given address awaiting for ACKs: " << std::get<0>(tuple).size() << "\n";
                mapTcp[srcAddress] = tuple;
            }
            else {
                // Init the entry (skipping three-ways handshake for simplicity)
                //std::get<0>(tuple) = nullptr;
                std::get<0>(tuple) = std::list<cPacket*> {};
                std::get<1>(tuple) = nullptr;
                std::get<2>(tuple) = sequenceNumber;
                std::get<3>(tuple) = 1;
                std::get<4>(tuple) = sequenceNumber+1;
                std::get<5>(tuple) = memcmp(gateName, IP_GATEWAYS_GATE_OUT, strlen(IP_GATEWAYS_GATE_OUT)) ? false : true;

                mapTcp[srcAddress] = tuple;
            }

            // Check if the message has no payload
            if (!memcmp(msgIn->getName(), "tcpACKMsg", strlen("tcpACKMsg"))) {
                delete msgIn;
                return;
            }

            // The message has a payload.
            // Send TCP/IP ACK
            EV << "Sending TCP/IP ACK...\n";

            // Update IN and ACK sequence numbers
            std::get<2>(tuple) = sequenceNumber;
            std::get<4>(tuple) = sequenceNumber+1;

            mapTcp[srcAddress] = tuple;

            EV << "Src address: "
                << (int) srcAddress[0] << "."
                << (int) srcAddress[1] << "."
                << (int) srcAddress[2] << "."
                << (int) srcAddress[3]
                << "\n";

            // Create and send message to sender
            cPacket* msgOut = createMessageIp(
                    "tcpACKMsg",
                    address, srcAddress.data(), IPv4_ADDRESS_SIZE,
                    nullptr, 0,
                    nullptr);
            msgOut = createMessageTcp(
                    "tcpACKMsg",
                    port, port,
                    std::get<3>(tuple), std::get<4>(tuple), true,
                    nullptr, 0,
                    msgOut);

            //send(msgOut, gateName, gateIndex);
            sendSecurely(this, eventTimeoutChannelTransmissions, msgOut, gateName, gateIndex);

            // Do not schedule retransmission as it is an ACK
        }

        // The message is a valid IP packet.
        std::array<uint8_t, IPv4_ADDRESS_SIZE> endDeviceAddress;

        // Increase the number of messages received
        messagesIn++;

        // Check if the packet has an encapsulated message
        if (encPacket != nullptr) {
            uint16_t counter;
            uint8_t frameType;
            uint8_t spreadingFactor;
            uint8_t transmissionPower;
            float bandwidth;
            float channelFrequency;

            // Check if the encapsulated message is a valid LoRa frame and get useful info
            error = isValidLoRaFrame(
                    encPacket, &frameType, endDeviceAddress.data(), &counter, &port, payloadIn,
                    &spreadingFactor, &transmissionPower, &bandwidth, &channelFrequency);
            if (error) {
                printError(error);
                delete msgIn;
                return;
            }

            // The message is a valid LoRa frame.
            // Process the LoRa frame

            if (frameType == JOIN_REQUEST) {
                // Elaborate JOIN_REQUEST from end device (simplification without join server)
                EV << "Elaborating JOIN_REQUEST from LoRa end device...\n";

                // Retrieve information from the message
                LoRaPhysicalFrame* phyMsg            = dynamic_cast<LoRaPhysicalFrame*>(encPacket);
                LoRaDatalinkFrame* dlMsg             = dynamic_cast<LoRaDatalinkFrame*>(phyMsg->getEncapsulatedPacket());
                LoRaJoinRequestFrame* joinRequestMsg = dynamic_cast<LoRaJoinRequestFrame*>(dlMsg->getEncapsulatedPacket());

                uint8_t joinEUIMsg[EUI_SIZE];
                std::array<uint8_t, EUI_SIZE> devEUI;
                getArrayInMessageJoinRequest(joinRequestMsg, &LoRaJoinRequestFrame::getJoinEUI, joinEUIMsg, EUI_SIZE);
                getArrayInMessageJoinRequest(joinRequestMsg, &LoRaJoinRequestFrame::getDevEUI, devEUI.data(), EUI_SIZE);
                uint16_t devNonce = joinRequestMsg->getDevNonce();

                auto it = nwkKeys.find(devEUI);

                // Check if the joinEUI is valid or the device EUI is not present in the map of root keys
                if (memcmp(joinEUI, joinEUIMsg, EUI_SIZE) || it == nwkKeys.end()) {
                    EV << "Received invalid Join Request!\n";
                    delete msgIn;
                    return;
                }

                std::array<uint8_t, IPv4_ADDRESS_SIZE> endDeviceAddress;

                // Check if the devEUI is already present in the address assignments
                // to elaborate the same Join Request once
                auto itAddresses = addressAssignments.find(devEUI);
                if (itAddresses != addressAssignments.end()) {
                    auto tuple = itAddresses->second;

                    // Check if the Join Request contains an already processed device nonce for the device EUI
                    // (i.e. the message is a duplicate sent by a further gateway in the radio range of the end device)
                    if (devNonce <= std::get<1>(tuple)) {
                        EV << "Received invalid Join Request!\n";
                        delete msgIn;
                        return;
                    }

                    // Retrieve the assigned address
                    endDeviceAddress = std::get<0>(tuple);
                }
                else {
                    // No address has been assigned to the device EUI, assign it

                    // Do not use ID as in the end device module because
                    // the sender here is the gateway that forwards the frame
                    //int i = msgIn->getSenderModule()->getId() - 2;
                    long i = numGateways + ++endDeviceActivations;
                    int j = i/254;

                    endDeviceAddress[0] = IPv4_ADDRESS_NETWORK_SERVER_0;
                    endDeviceAddress[1] = IPv4_ADDRESS_NETWORK_SERVER_1;
                    endDeviceAddress[2] = static_cast<uint8_t> ((IPv4_ADDRESS_NETWORK_SERVER_2 + j) % 256);
                    endDeviceAddress[3] = static_cast<uint8_t> (1 + i%254);
                }

                addressAssignments[devEUI] =
                        std::tuple<std::array<uint8_t, IPv4_ADDRESS_SIZE>, uint16_t> {endDeviceAddress, devNonce};

                // Generate other parameters to include in the Join Accept message
                uint32_t joinNonce = generateNonce(0xFFFFFF);
                uint8_t DLSettings = 1;
                uint8_t RXDelay    = 1;
                uint8_t CFList[16] = {};

                //EV << std::hex << "joinNonce: "  << joinNonce << std::dec << "\n";
                EV << "joinNonce: "  << joinNonce << "\n";

                EV << "End device IP address: "
                    << (int) endDeviceAddress[0] << "."
                    << (int) endDeviceAddress[1] << "."
                    << (int) endDeviceAddress[2] << "."
                    << (int) endDeviceAddress[3]
                    << "\n";

                std::array<uint8_t, KEY_LORAWAN_SIZE> nwkKey { it->second };
                std::array<uint8_t, KEY_LORAWAN_SIZE> nwkSKey;
                //std::array<uint8_t, KEY_LORAWAN_SIZE> appSKey;

                // Derive session key exclusively shared between the network server and end device
                memcpy(nwkSKey.data(), generateKey(nwkKey.data(), joinNonce, devNonce, networkId), nwkSKey.size());

                // Derive session key exclusively shared between the application server and end device
                //memcpy(appSKey.data(), generateKey(appKey, joinNonce, devNonce, networkId), sizeof(appSKey));

                printKey(nwkSKey.data(), KEY_LORAWAN_SIZE, "nwkSKey", EV);
                //printKey(appSKey.data(), KEY_LORAWAN_SIZE, "appSKey", EV);

                // Store the entry in the map
                endDeviceSessionKeys[endDeviceAddress] = nwkSKey;

                // Init the entry
                std::tuple<uint8_t, uint16_t, uint16_t, bool, bool, std::array<uint8_t, IPv4_ADDRESS_SIZE>> tuple;
                std::get<1>(tuple) = 0;
                std::get<2>(tuple) = 0;
                std::get<3>(tuple) = false;
                std::get<4>(tuple) = false;

                // Store the entry in the map
                map[endDeviceAddress] = tuple;

                EV << "Sending JOIN_ACCEPT message...\n";
                cPacket* msgOut = createMessageJoinAccept(
                        "joinAccept",
                        joinNonce, networkId, endDeviceAddress.data(),
                        DLSettings, RXDelay, CFList,
                        nwkKey.data(),
                        spreadingFactor, transmissionPower,
                        bandwidth, channelFrequency);
                /*msgOut = createMessageIp(
                    "joinAccept",
                    address, srcAddress.data(), IPv4_ADDRESS_SIZE,
                    nullptr, 0,
                    msgOut);
                msgOut = createMessageUdp(
                    "joinAccept",
                    //MSG_PORT_FORWARD_JOIN, MSG_PORT_FORWARD_JOIN,
                    MSG_PORT_FORWARD_OVER_IP, MSG_PORT_FORWARD_OVER_IP,
                    nullptr, 0,
                    msgOut);*/

                //sendDelayed(msgOut->dup(), RX_DELAY_1 + 0.1, IP_GATEWAYS_GATE_OUT, msgIn->getArrivalGate()->getIndex());
                //sendDelayedSecurely(this, eventTimeoutChannelTransmissions, msgOut, RX_DELAY_1 + 0.1, IP_GATEWAYS_GATE_OUT, msgIn->getArrivalGate()->getIndex());

                //===================
                // Calculate response airtime to calculate the delay of the response
                EV << "Calculate Time On Air\n";

                // Calculate preamble and frame airtime
                auto tupleAirtime = calculateTimeOnAir(msgOut);

                // Convert frame time on air from ms to s
                double airtimeFrame = std::get<1>(tupleAirtime) / 1000;

                // Check if computation succeeded
                if (airtimeFrame < 0)
                    return;

                // Calculate arrival time
                /*simtime_t simtime = simTime();
                simtime_t arrivalFrame = simtime + airtimeFrame;

                EV << "Simulation time: " << simtime << "\n";
                EV << "Expected arrival frame time: " << arrivalFrame << "\n";//*/

                msgOut = createMessageIp(
                    "joinAccept",
                    address, srcAddress.data(), IPv4_ADDRESS_SIZE,
                    nullptr, 0,
                    msgOut);
                msgOut = createMessageUdp(
                    "joinAccept",
                    //MSG_PORT_FORWARD_JOIN, MSG_PORT_FORWARD_JOIN,
                    MSG_PORT_FORWARD_OVER_IP, MSG_PORT_FORWARD_OVER_IP,
                    nullptr, 0,
                    msgOut);

                // Calculate the delay (with tolerance) to send the message in the receive window of the end device
                double delay = RX_DELAY_1 - airtimeFrame + 0.1;
                EV << "Delay: " << delay << "\n";

                if (delay > 0)
                    sendDelayedSecurely(this, eventTimeoutChannelTransmissions, msgOut, delay, IP_GATEWAYS_GATE_OUT, msgIn->getArrivalGate()->getIndex());
                else
                    sendSecurely(this, eventTimeoutChannelTransmissions, msgOut, IP_GATEWAYS_GATE_OUT, msgIn->getArrivalGate()->getIndex());
                //===================

                // Increase the number of messages sent
                messagesOut++;

                delete msgIn;
                return;
            }

            EV << "Received message for the network server with port: " << (int) port << "\n";
            EV << "Received message for the network server with end device address: "
                << (int) endDeviceAddress[0] << "."
                << (int) endDeviceAddress[1] << "."
                << (int) endDeviceAddress[2] << "."
                << (int) endDeviceAddress[3]
                << "\n";

            // Check if a message from/about this address has already been received
            std::tuple<uint8_t, uint16_t, uint16_t, bool, bool, std::array<uint8_t, IPv4_ADDRESS_SIZE>> tuple;
            auto it = map.find(endDeviceAddress);

            /*if (port == MSG_PORT_PAIRING_VERIFIED) {
                // Check if the end device is already associated with a gateway or
                // if a message from/about the end device address has already been received
                if (associations.find(endDeviceAddress) != associations.end() || it == map.end()) {
                    delete msgIn;
                    return;
                }

                // The end device is in the map, get the entry
                tuple = it->second;

                // Get message requestId & verify response
                uint8_t requestId      = payloadIn[0];
                uint8_t verifyResponse = payloadIn[1];

                // Check if the request ID in the message matches the ID stored in the map
                if (requestId != std::get<2>(tuple)) {
                    delete msgIn;
                    return;
                }

                // Response received, cancel timeout event and remove the pointer from the tuple
                EV << "Response received for PAIRING_VERIFY message, canceling timer for resending the frame\n";
                cancelAndDelete(std::get<1>(tuple));
                std::get<1>(tuple) = nullptr;
                map[endDeviceAddress] = tuple;

                // Build payload
                uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {
                    requestId,
                    verifyResponse
                };

                // Get the gateway address from the stored tuple
                std::array<uint8_t, IPv4_ADDRESS_SIZE> gatewayAddress = std::get<6>(tuple);
                const char* name;

                // Check if the pairing is confirmed
                if (verifyResponse == ACK) {
                    // The association is confirmed,
                    // add the pair (end device address, gateway address)
                    // among the associations
                    associations[endDeviceAddress] = gatewayAddress;

                    name = "connectionACKMsg";
                    EV << "Sending ACK for CONNECTION message to gateway...\n";
                }
                else {
                    // The association is not confirmed
                    name = "connectionNACKMsg";
                    EV << "Sending NACK for CONNECTION message to gateway...\n";
                }

                sendMessageTcp(name, "timeoutConnectionACK",
                        gatewayAddress, MSG_PORT_CONNECTION, payload, sizeof(payload), nullptr);
            }*/
            if (port == MSG_PORT_CONNECTION) {
                EV << "Received CONNECTION message\n";

                // Check if the end device is already associated with a gateway or
                // if a message from/about the end device address has already been received (should be)
                /*if (associations.find(endDeviceAddress) != associations.end() || it == map.end()) {
                    delete msgIn;
                    return;
                }*/

                // Check if a message from/about the end device address has already been received (should be)
                if (it == map.end()) {
                    // Ignore the message but the session key between end device and network server could be compromised
                    delete msgIn;
                    return;
                }

                // The end device is in the map, get the entry
                tuple = it->second;

                // Update the uplink & downlink counters
                EV << "Updating downlink & uplink counters...\n";
                std::get<1>(tuple) = counter;
                std::get<2>(tuple) = counter;
                EV << "Uplink counter: " << (int) counter << "\n";
                EV << "Downlink counter: " << (int) counter << "\n";

                // Store the updated entry in the map
                map[endDeviceAddress] = tuple;

                EV << "std::get<0>(tuple): " << (unsigned) std::get<0>(tuple) << "\n";
                EV << "std::get<1>(tuple): " << (unsigned) std::get<1>(tuple) << "\n";
                EV << "std::get<2>(tuple): " << (unsigned) std::get<2>(tuple) << "\n";
                EV << "std::get<3>(tuple): " << std::get<3>(tuple) << "\n";
                EV << "std::get<4>(tuple): " << std::get<4>(tuple) << "\n";
                //EV << "std::get<5>(tuple): " << std::get<6>(tuple) << "\n";

                // Get message requestId
                uint8_t requestId = payloadIn[0];

                // Get gateway IP address
                std::array<uint8_t, IPv4_ADDRESS_SIZE> gatewayAddress;
                memcpy(gatewayAddress.data(), &payloadIn[1], IPv4_ADDRESS_SIZE);

                // Check if the CONNECTION message from the gateway is already arrived
                if (std::get<3>(tuple)) {
                    // It is arrived.
                    // Check if the request ID in the message matches the ID stored in the map
                    if (requestId != std::get<0>(tuple)) {
                        // Ignore the message but the session key
                        // between end device and network server could be compromised
                        delete msgIn;
                        return;
                    }

                    // Get the network server session key shared with the end device
                    /*auto itSessionKeys = endDeviceSessionKeys.find(endDeviceAddress);
                    if (it == endDeviceSessionKeys.end()) {
                        // Ignore the message but the session key
                        // between end device and network server could be compromised
                        delete msgIn;
                        return;
                    }

                    std::array<uint8_t, KEY_LORAWAN_SIZE> nwkSKey = itSessionKeys->second;*/
                    std::array<uint8_t, KEY_LORAWAN_SIZE> nwkSKey = endDeviceSessionKeys[endDeviceAddress];

                    // Check if the end device is already associated with a gateway
                    if (associations.find(endDeviceAddress) != associations.end()) {
                        // Check if this is a retransmission due to lost response
                        if (gatewayAddress != std::get<5>(tuple)) {
                            // Ignore the message but the session key
                            // between end device and network server could be compromised
                            delete msgIn;
                            return;
                        }

                        EV << "Already received CONNECTION message to associate the end device with the selected gateway but the response could be lost!\n";

                        // It is a retransmission, resend only to the end device.
                        // Build payload
                        uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {
                            requestId,
                            ACK
                        };

                        // Create the response to CONNECTION message
                        cPacket* msgOut = createMessageDownlink(
                            "connectionACKMsg", true,
                            endDeviceAddress.data(), IPv4_ADDRESS_SIZE,
                            counter, MSG_PORT_CONNECTION, payload, LORA_FRAME_SIZE_APP_PAYLOAD,
                            nwkSKey.data(),
                            spreadingFactor, transmissionPower,
                            bandwidth, channelFrequency);

                        //===================
                        // Calculate response airtime to calculate the delay of the response
                        EV << "Calculate Time On Air\n";

                        // Calculate preamble and frame airtime
                        auto tupleAirtime = calculateTimeOnAir(msgOut);

                        // Convert frame time on air from ms to s
                        double airtimeFrame = std::get<1>(tupleAirtime) / 1000;

                        // Check if computation succeeded
                        if (airtimeFrame < 0)
                            return;

                        // Calculate arrival time
                        /*simtime_t simtime = simTime();
                        simtime_t arrivalFrame = simtime + airtimeFrame;

                        EV << "Simulation time: " << simtime << "\n";
                        EV << "Expected arrival frame time: " << arrivalFrame << "\n";*/

                        EV << "Resending response to CONNECTION message to end device...\n";

                        msgOut = createMessageIp(
                            "connectionACKMsg",
                            address, srcAddress.data(), IPv4_ADDRESS_SIZE,
                            nullptr, 0,
                            msgOut);
                        msgOut = createMessageUdp(
                            "connectionACKMsg",
                            //MSG_PORT_FORWARD_CONNECTION, MSG_PORT_FORWARD_CONNECTION,
                            MSG_PORT_FORWARD_OVER_IP, MSG_PORT_FORWARD_OVER_IP,
                            nullptr, 0,
                            msgOut);

                        // Calculate the delay (with tolerance) to send the message in the receive window of the end device
                        double delay = RX_DELAY_1 - airtimeFrame + 0.1;
                        EV << "Delay: " << delay << "\n";

                        if (delay > 0)
                            sendDelayedSecurely(this, eventTimeoutChannelTransmissions, msgOut, delay, IP_GATEWAYS_GATE_OUT, msgIn->getArrivalGate()->getIndex());
                        else
                            sendSecurely(this, eventTimeoutChannelTransmissions, msgOut, IP_GATEWAYS_GATE_OUT, msgIn->getArrivalGate()->getIndex());
                        //===================

                        // Send response to end device
                        //EV << "Resending response to CONNECTION message to end device...\n";
                        // TODO: send back to gateway, in broadcast or know how to reach the end device
                        // Send the message in the receive window of the end device
                        //sendDelayed(msgOut->dup(), RX_DELAY_1 + 0.1, IP_GATEWAYS_GATE_OUT, msgIn->getArrivalGate()->getIndex());
                        //sendDelayedSecurely(this, eventTimeoutChannelTransmissions, msgOut, RX_DELAY_1 + 0.1, IP_GATEWAYS_GATE_OUT, msgIn->getArrivalGate()->getIndex());
                        //sendBroadcast(this, msgOut, IP_GATEWAYS_GATE_OUT);

                        // Increase the number of messages sent
                        messagesOut++;

                        delete msgIn;
                        return;
                    }

                    // Build payload
                    uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {
                        endDeviceAddress[0],
                        endDeviceAddress[1],
                        endDeviceAddress[2],
                        endDeviceAddress[3],
                        requestId
                    };

                    const char* name;

                    EV << "Selected gateway address from end device: "
                        << (int) gatewayAddress[0] << "."
                        << (int) gatewayAddress[1] << "."
                        << (int) gatewayAddress[2] << "."
                        << (int) gatewayAddress[3]
                        << "\n";
                    EV << "Selected gateway address from gateway (stored): "
                        << (int) std::get<5>(tuple)[0] << "."
                        << (int) std::get<5>(tuple)[1] << "."
                        << (int) std::get<5>(tuple)[2] << "."
                        << (int) std::get<5>(tuple)[3]
                        << "\n";//*/

                    // Check if the IP addresses match
                    if (gatewayAddress == std::get<5>(tuple)) {
                        // The association is confirmed,
                        // add the pair (end device address, gateway address)
                        // among the associations
                        associations[endDeviceAddress] = gatewayAddress;

                        payload[5] = ACK;
                        name = "connectionACKMsg";
                        EV << "Sending ACK for CONNECTION message to gateway...\n";
                    }
                    else {
                        // The association is not confirmed
                        payload[5] = NACK;
                        name = "connectionNACKMsg";
                        EV << "Sending NACK for CONNECTION message to gateway...\n";
                    }

                    // Send response to gateway over TCP
                    sendMessageTcp(name, "timeoutConnectionACK",
                            gatewayAddress, MSG_PORT_CONNECTION_GATEWAY,
                            payload, sizeof(payload), nullptr, IP_GATEWAYS_GATE_OUT);

                    uint8_t res = payload[5];

                    // Clean payload
                    memset(payload, 0, LORA_FRAME_SIZE_APP_PAYLOAD);

                    payload[0] = requestId;
                    payload[1] = res;

                    // Create the response to CONNECTION message over UDP
                    cPacket* msgOut = createMessageDownlink(
                        name, true,
                        endDeviceAddress.data(), IPv4_ADDRESS_SIZE,
                        counter, MSG_PORT_CONNECTION, payload, LORA_FRAME_SIZE_APP_PAYLOAD,
                        nwkSKey.data(),
                        spreadingFactor, transmissionPower,
                        bandwidth, channelFrequency);


                    //===================
                    // Calculate response airtime to calculate the delay of the response
                    EV << "Calculate Time On Air\n";

                    // Calculate preamble and frame airtime
                    auto tupleAirtime = calculateTimeOnAir(msgOut);

                    // Convert frame time on air from ms to s
                    double airtimeFrame = std::get<1>(tupleAirtime) / 1000;

                    // Check if computation succeeded
                    if (airtimeFrame < 0)
                        return;

                    // Calculate arrival time
                    /*simtime_t simtime = simTime();
                    simtime_t arrivalFrame = simtime + airtimeFrame;

                    EV << "Simulation time: " << simtime << "\n";
                    EV << "Expected arrival frame time: " << arrivalFrame << "\n";*/

                    EV << "Sending response to CONNECTION message to end device...\n";

                    msgOut = createMessageIp(
                        name,
                        address, srcAddress.data(), IPv4_ADDRESS_SIZE,
                        nullptr, 0,
                        msgOut);
                    msgOut = createMessageUdp(
                        name,
                        //MSG_PORT_FORWARD_CONNECTION, MSG_PORT_FORWARD_CONNECTION,
                        MSG_PORT_FORWARD_OVER_IP, MSG_PORT_FORWARD_OVER_IP,
                        nullptr, 0,
                        msgOut);

                    // Calculate the delay (with tolerance) to send the message in the receive window of the end device
                    double delay = RX_DELAY_1 - airtimeFrame + 0.1;
                    EV << "Delay: " << delay << "\n";

                    if (delay > 0)
                        sendDelayedSecurely(this, eventTimeoutChannelTransmissions, msgOut, delay, IP_GATEWAYS_GATE_OUT, msgIn->getArrivalGate()->getIndex());
                    else
                        sendSecurely(this, eventTimeoutChannelTransmissions, msgOut, IP_GATEWAYS_GATE_OUT, msgIn->getArrivalGate()->getIndex());
                    //===================

                    // Send response to end device
                    //EV << "Sending response to CONNECTION message to end device...\n";
                    // TODO: send back to gateway, in broadcast or know how to reach the end device
                    // Send the message in the receive window of the end device
                    //sendDelayed(msgOut->dup(), RX_DELAY_1 + 0.1, IP_GATEWAYS_GATE_OUT, msgIn->getArrivalGate()->getIndex());
                    //sendDelayedSecurely(this, eventTimeoutChannelTransmissions, msgOut->dup(), RX_DELAY_1 + 0.1, IP_GATEWAYS_GATE_OUT, msgIn->getArrivalGate()->getIndex());
                    //sendDelayedSecurely(this, eventTimeoutChannelTransmissions, msgOut, RX_DELAY_1 + 0.1, IP_GATEWAYS_GATE_OUT, msgIn->getArrivalGate()->getIndex());
                    //sendBroadcast(this, msgOut, IP_GATEWAYS_GATE_OUT);

                    // Increase the number of messages sent
                    messagesOut+= 2;
                }
                else {
                    // It is not arrived.
                    EV << "Init the entry\n";
                    // Update the entry
                    std::get<0>(tuple) = requestId;
                    std::get<4>(tuple) = true;
                    std::get<5>(tuple) = gatewayAddress;
                    map[endDeviceAddress] = tuple;
                }
            }

            delete msgIn;
            return;
        }

        // The IP packet does not have an encapsulated message.
        // Process the TCP/IP packet

        // Copy the end device address from the IP payload
        memcpy(endDeviceAddress.data(), payloadIn, IPv4_ADDRESS_SIZE);

        EV << "Received message for the network server with port: " << (int) port << "\n";
        EV << "Received message for the network server with end device address: "
            << (int) endDeviceAddress[0] << "."
            << (int) endDeviceAddress[1] << "."
            << (int) endDeviceAddress[2] << "."
            << (int) endDeviceAddress[3]
            << "\n";

        // Check if a message from/about this address has already been received
        std::tuple<uint8_t, uint16_t, uint16_t, bool, bool, std::array<uint8_t, IPv4_ADDRESS_SIZE>> tuple;
        auto it = map.find(endDeviceAddress);

        /*if (port == MSG_PORT_CONNECTION) {
            EV << "Received CONNECTION message\n";

            // Check if a CONNECTION message arrives while the end device is already paired with a gateway
            if (associations.find(endDeviceAddress) != associations.end()) {
                delete msgIn;
                return;
            }

            // Get message requestId & counter
            uint8_t requestId = payload[4];
            uint8_t counter_[4] = {
                payload[5], payload[6], payload[7], payload[8]
            };
            unsigned counter = bytesToUint(counter_);

            //EV << "requestId received: " << (int) requestId << "\n";

            // Check if a message from/about the end device address has already been received
            if (it != map.end()) {
                // The end device is in the map, get the entry
                tuple = it->second;
            }

            // Update the uplink & downlink counters
            EV << "Updating downlink & uplink counters...\n";
            std::get<4>(tuple) = counter;
            std::get<5>(tuple) = counter;
            EV << "Uplink counter: " << (int) counter << "\n";
            EV << "Downlink counter: " << (int) counter << "\n";

            // Build payload
            uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {
                requestId,
                srcAddress[0],
                srcAddress[1],
                srcAddress[2],
                srcAddress[3]
            };

            // ===============================================================
            // Create the PAIRING_VERIFY message and the corresponding timeout
            cPacket* msgOut = createMessageDownlink("pairingVerifyMsg", true,
                    endDeviceAddress.data(), IPv4_ADDRESS_SIZE, counter, MSG_PORT_PAIRING_VERIFY, payload, LORA_FRAME_SIZE_APP_PAYLOAD);
            msgOut = createMessageIp(
                "pairingVerifyMsg",
                address, srcAddress.data(), IPv4_ADDRESS_SIZE,
                nullptr, 0,
                msgOut);
            msgOut = createMessageUdp(
                "pairingVerifyMsg",
                MSG_PORT_CONNECTION, MSG_PORT_CONNECTION,
                nullptr, 0,
                msgOut);
            cMessage* eventTimeout = new cMessage("timeoutNS");

            // Store the entry in the table
            std::get<0>(tuple) = msgOut;
            std::get<1>(tuple) = eventTimeout;
            std::get<2>(tuple) = requestId;
            std::get<6>(tuple) = std::array<uint8_t, IPv4_ADDRESS_SIZE> {srcAddress[0], srcAddress[1], srcAddress[2], srcAddress[3]};
            map[endDeviceAddress] = tuple;

            // Add entry in the inverted index
            eventTimeoutRetransmissions[eventTimeout] = endDeviceAddress;
            // ===============================================================


            // ===============================================================
            // Check if the dest IP address is already in the map for TCP/IP connections
            /*std::tuple<cMessage*, cMessage*, uint32_t, uint32_t, uint32_t> tupleTcp;
            auto itTcp = mapTcp.find(srcAddress);
            if (itTcp != mapTcp.end()) {
                // The network server is in the map, get the entry
                tupleTcp = itTcp->second;

                // Check if a timeout exists for a previous transmission
                if (std::get<1>(tupleTcp)) {
                    // Cancel old timeout
                    cancelEvent(std::get<1>(tupleTcp));

                    // Rename timeout
                    std::get<1>(tupleTcp)->setName("timeoutNS");
                }
                else {
                    // Create timeout
                    cMessage* eventTimeout = new cMessage("timeoutNS");
                    std::get<1>(tupleTcp) = eventTimeout;

                    // Add timeout in the inverted index
                    eventTimeoutRetransmissions[eventTimeout] = srcAddress;
                }

                // Update TCP/IP sequence number
                std::get<2>(tupleTcp) += 1;
            }
            else {
                // Create timeout
                cMessage* eventTimeout = new cMessage("timeoutNS");
                std::get<1>(tupleTcp) = eventTimeout;

                // Add timeout in the inverted index
                eventTimeoutRetransmissions[eventTimeout] = srcAddress;

                // Init counters (sequence number should be random).
                // Simplified skipping three-way handshake
                std::get<2>(tuple) = 1;
                std::get<3>(tuple) = 1;
                std::get<4>(tuple) = 1;
            }

            // Create the PAIRING_VERIFY message and the corresponding timeout
            cPacket* msgOut = createMessageDownlink("pairingVerifyMsg", true,
                    endDeviceAddress.data(), IPv4_ADDRESS_SIZE, counter, MSG_PORT_PAIRING_VERIFY, payload, LORA_FRAME_SIZE_APP_PAYLOAD);
            msgOut = createMessageIp(
                "pairingVerifyMsg",
                address, srcAddress.data(), IPv4_ADDRESS_SIZE,
                nullptr, 0,
                msgOut);
            msgOut = createMessageTcp(
                "pairingVerifyMsg",
                MSG_PORT_CONNECTION, MSG_PORT_CONNECTION,
                std::get<3>(tupleTcp), std::get<4>(tupleTcp), false,
                nullptr, 0,
                msgOut);
            cMessage* eventTimeout = new cMessage("timeoutNS");

            // Store the entry in the table
            std::get<0>(tupleTcp) = msgOut;
            mapTcp[srcAddress] = tupleTcp;

            std::get<0>(tuple) = msgOut;
            std::get<1>(tuple) = eventTimeout;
            std::get<2>(tuple) = requestId;
            std::get<6>(tuple) = std::array<uint8_t, IPv4_ADDRESS_SIZE> {srcAddress[0], srcAddress[1], srcAddress[2], srcAddress[3]};
            map[endDeviceAddress] = tuple;*/

            // Here it is better to use UDP instead of TCP because it is important to receive
            // a response from the sensor and not an ACK from the gateway.
            // So, retransmission must not be based on TCP ACKs but sensor response
            // ===============================================================

            /*EV << "Sending PAIRING_VERIFY message...\n";
            // TODO: send back to gateway, in broadcast or know how to reach the end device
            //send(msgOut, IP_GATEWAYS_GATE_OUT, msgIn->getArrivalGate()->getIndex());
            // Send the message in the receive window of the end device
            sendDelayed(msgOut->dup(), RX_DELAY_1 + 0.1, IP_GATEWAYS_GATE_OUT, msgIn->getArrivalGate()->getIndex());
            //sendBroadcast(this, msgOut, IP_GATEWAYS_GATE_OUT);

            scheduleAt(simTime()+timeout, eventTimeout);
        }*/
        if (port == MSG_PORT_CONNECTION_GATEWAY) {
            EV << "Received MSG_PORT_CONNECTION_GATEWAY message\n";

            // Check if the end device is already associated with a gateway or
            // if a message from/about the end device address has already been received (should be)
            if (associations.find(endDeviceAddress) != associations.end() || it == map.end()) {
                delete msgIn;
                return;
            }

            // The end device is in the map, get the entry
            tuple = it->second;

            // Get message requestId, counter & result
            uint8_t requestId = payloadIn[4];
            uint8_t counter_[4] = {
                    payloadIn[5], payloadIn[6], payloadIn[7], payloadIn[8]
            };
            unsigned counter = bytesToUint(counter_);
            uint8_t res = payloadIn[9];

            EV << "requestId received: " << (int) requestId << "\n";

            // Update the uplink & downlink counters
            EV << "Updating downlink & uplink counters...\n";
            std::get<1>(tuple) = counter;
            std::get<2>(tuple) = counter;
            EV << "Uplink counter: " << (int) counter << "\n";
            EV << "Downlink counter: " << (int) counter << "\n";

            // Check if the result is a NACK
            if (res == NACK) {
                // Check if a CONNECTION_GW message about the end device association has been received,
                // the request ID matches and the gateway addresses match
                if (!std::get<3>(tuple) || !(requestId == std::get<0>(tuple)) || !(srcAddress == std::get<5>(tuple))) {
                    delete msgIn;
                    return;
                }

                EV << "Removing the reception of the previous CONNECTION_GW message about the end device...\n";

                // Remove the reception of the CONNECTION_GW message to overwrite the gateway address
                std::get<3>(tuple) = false;
                return;
            }

            // Check if the CONNECTION message from the end device is already arrived
            /*if (std::get<1>(tuple)) {
                // It is arrived.
                // Check if the request ID in the message matches the ID stored in the map
                if (requestId != std::get<2>(tuple)) {
                    delete msgIn;
                    return;
                }

                // Build payload
                uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {
                    requestId
                };

                const char* name;

                // Check if the IP addresses match
                if (srcAddress == std::get<6>(tuple)) {
                    // The association is confirmed,
                    // add the pair (end device address, gateway address)
                    // among the associations
                    associations[endDeviceAddress] = srcAddress;

                    payload[1] = ACK;
                    name = "connectionACKMsg";
                    EV << "Sending ACK for CONNECTION message to gateway...\n";
                }
                else {
                    // The association is not confirmed
                    payload[1] = NACK;
                    name = "connectionNACKMsg";
                    EV << "Sending NACK for CONNECTION message to gateway...\n";
                }

                sendMessageTcp(name, "timeoutConnectionACK",
                        srcAddress, MSG_PORT_CONNECTION_GW, payload, sizeof(payload), nullptr, IP_GATEWAYS_GATE_OUT);
            }
            else {
                // It is not arrived.
                // Update the entry
                std::get<0>(tuple) = true;
                std::get<2>(tuple) = requestId;
                std::get<6>(tuple) = srcAddress;
                map[endDeviceAddress] = tuple;
            }*/

            // Check if a CONNECTION_GW message about the end device is already arrived
            if (!std::get<3>(tuple)) {
                EV << "Init the entry\n";

                // Init the entry
                std::get<0>(tuple) = requestId;
                std::get<3>(tuple) = true;
                std::get<5>(tuple) = srcAddress;
                map[endDeviceAddress] = tuple;

                EV << "std::get<0>(tuple): " << (unsigned) std::get<0>(tuple) << "\n";
                EV << "std::get<1>(tuple): " << (unsigned) std::get<1>(tuple) << "\n";
                EV << "std::get<2>(tuple): " << (unsigned) std::get<2>(tuple) << "\n";
                EV << "std::get<3>(tuple): " << std::get<3>(tuple) << "\n";
                EV << "std::get<4>(tuple): " << std::get<4>(tuple) << "\n";
                //EV << "std::get<5>(tuple): " << std::get<5>(tuple) << "\n";
            }
        }
        else if (port == MSG_PORT_SYNC_COUNTER) {
            EV << "Received SYNC_COUNTER message\n";

            // Check if a message from/about the end device address has already been received (should be)
            if (it == map.end()) {
                // Ignore the message but the session key between end device and network server could be compromised
                delete msgIn;
                return;
            }

            // Get the shared downlink counter (till now equal to uplink counter)
            uint8_t counter_[4] = {
                    payloadIn[4], payloadIn[5], payloadIn[6], payloadIn[7]
            };
            unsigned counter = bytesToUint(counter_);

            // Check if a message from/about the end device address has already been received
            /*if (it != map.end()) {
                // The end device is in the map, get the entry
                tuple = it->second;

                // Check if the received downlink counter is outdated
                if (counter <= std::get<3>(tuple)) {
                    EV << "Received outdated counter, ignore it\n";
                    delete msgIn;
                    return;
                }
            }
            else {
                // Init the entry
                std::get<4>(tuple) = false;
                std::get<5>(tuple) = false;
                std::get<2>(tuple) = 0;
                std::get<3>(tuple) = 0;
            }*/

            // The end device is in the map, get the entry
            tuple = it->second;

            // Check if the received downlink counter is outdated
            if (counter <= std::get<2>(tuple)) {
                EV << "Received outdated counter, ignore it\n";
                delete msgIn;
                return;
            }

            // Update the uplink & downlink counters
            EV << "Updating downlink & uplink counters...\n";
            std::get<1>(tuple) = counter;
            std::get<2>(tuple) = counter;
            EV << "Uplink counter: " << (int) counter << "\n";
            EV << "Downlink counter: " << (int) counter << "\n";

            // Store the updated entry in the map
            map[endDeviceAddress] = tuple;
        }
        /*else if (port == MSG_PORT_SHARE_DATA_PROFILE) {
            EV << "Received SHARE_DATA_PROFILE message\n";

            // Check if the end device is associated with a gateway or
            // if a message from/about the end device address has already been received (should be)
            if (it == map.end() || associations.find(endDeviceAddress) == associations.end()) {
                delete msgIn;
                return;
            }

            // Get the data profile
            std::string dataProfile = (char*) &payloadIn[4];
            EV << "Data profile: " << dataProfile << "\n";

            // Store it in the corresponding map
            dataProfiles[dataProfile] = endDeviceAddress;
        }*/
        else if (port == MSG_PORT_PROCESSED_DATA) {
            EV << "Received PROCESSED_DATA message\n";

            // Check if the end device is already associated with a gateway or
            // if a message from/about the end device address has already been received (should be)
            if (it == map.end() || associations.find(endDeviceAddress) == associations.end()) {
                delete msgIn;
                return;
            }

            // Check if the message includes a data profile
            if (payloadIn[9] != 0) {
                // Get the data profile
                std::string dataProfile = (char*) &payloadIn[9];
                EV << "Data profile: " << dataProfile << "\n";

                // Store it in the corresponding map
                dataProfiles[dataProfile] = endDeviceAddress;
            }

            EV << "Forwarding processed data to the cloud...\n";
            // TODO: Forward to cloud
        }

        delete msgIn;
    }
}

void LoRaNetworkServer::refreshDisplay() const {
    char buf[50] = {};
    snprintf(buf, sizeof(buf), "in: %lu out: %lu", messagesIn, messagesOut);
    getDisplayString().setTagArg("t", 0, buf);

    // Do not consider TCP ACK for in & out messages
}


// Check if the port is valid
bool LoRaNetworkServer::isValidPort(unsigned port, uint8_t msgType) {
    switch (msgType) {
        //case MSG_LORA:
        case MSG_LORA_UPLINK:
            //return port == MSG_PORT_PAIRING_VERIFIED ||
            return port == MSG_PORT_CONNECTION ||
                   port == MSG_PORT_DATA       ||
                   port == MSG_PORT_MAC_CMD;
            break;
        case MSG_TCP:
            return port == MSG_PORT_SYNC_COUNTER       ||
                   port == MSG_PORT_CONNECTION_GATEWAY ||
                   //port == MSG_PORT_SHARE_DATA_PROFILE ||
                   port == MSG_PORT_PROCESSED_DATA;
            break;
        case MSG_UDP:
            /*return port == MSG_PORT_FORWARD_JOIN    ||
                   port == MSG_PORT_FORWARD_MAC_CMD ||
                   //port == MSG_PORT_CONNECTION   ||
                   port == MSG_PORT_FORWARD_CONNECTION;*/
            return port == MSG_PORT_FORWARD_OVER_IP;
            break;
        default:
            throw std::invalid_argument("Invalid message type");
    }
}

uint8_t LoRaNetworkServer::isValidLoRaFrame(
        cMessage* msg, uint8_t* frameType,
        uint8_t* deviceAddress, uint16_t* counter, unsigned* port, uint8_t* payload,
        uint8_t* spreadingFactor, uint8_t* transmissionPower,
        float* bandwidth, float* channelFrequency) {
    if (!msg || !frameType || !deviceAddress || !counter || !port || !payload)
        return BAD_PARAMS;

    // Check if the message is sent over LoRa
    LoRaPhysicalFrame* physicalMsg = dynamic_cast<LoRaPhysicalFrame*>(msg);
    if (!physicalMsg)
        return BAD_FRAME;

    // Get physical parameter
    *spreadingFactor   = physicalMsg->getSpreadingFactor();
    *transmissionPower = physicalMsg->getTransmissionPower();
    *bandwidth         = physicalMsg->getBandwidth();
    *channelFrequency  = physicalMsg->getChannelFrequency();

    LoRaDatalinkFrame* datalinkMsg = dynamic_cast<LoRaDatalinkFrame*>(physicalMsg->getEncapsulatedPacket());
    if (!datalinkMsg)
        return BAD_FRAME;

    *frameType = datalinkMsg->getFType();
    if (*frameType == JOIN_REQUEST) {
        LoRaJoinRequestFrame* joinReqMsg = dynamic_cast<LoRaJoinRequestFrame*>(datalinkMsg->getEncapsulatedPacket());
        if (!joinReqMsg)
            return BAD_FRAME;

        // Get the device EUI
        std::array<uint8_t, EUI_SIZE> devEUI;
        getArrayInMessageJoinRequest(joinReqMsg, &LoRaJoinRequestFrame::getDevEUI, devEUI.data(), EUI_SIZE);

        // Check if the device EUI is included in the map of root keys
        auto it = nwkKeys.find(devEUI);
        if (it == nwkKeys.end())
            return BAD_FRAME;

        // Retrieve the corresponding root key
        std::array<uint8_t, KEY_LORAWAN_SIZE> nwkKey { it->second };
        int numKeys = 1;
        uint8_t* keys[numKeys] = { nwkKey.data() };

        // Check the MIC of the message for data integrity & authentication
        if (!isValidMIC(datalinkMsg, joinReqMsg, keys, numKeys))
            return BAD_MIC;

        return 0;
    }

    LoRaAppUplinkFrame* endDeviceAppMsg = dynamic_cast<LoRaAppUplinkFrame*>(datalinkMsg->getEncapsulatedPacket());
    if (!endDeviceAppMsg)
        return BAD_FRAME;

    // Get the message counter
    *counter = endDeviceAppMsg->getCounter();

    // Get the device address
    getArrayInMessageUplink(endDeviceAppMsg, &LoRaAppUplinkFrame::getDeviceAddress, deviceAddress, IPv4_ADDRESS_SIZE);

    std::array<uint8_t, IPv4_ADDRESS_SIZE> deviceAddr;
    memcpy(deviceAddr.data(), deviceAddress, IPv4_ADDRESS_SIZE);

    // Check if the address is in the map
    auto it = map.find(deviceAddr);
    if (it != map.end()) {
        // The address is in the map.
        // Check if the frame is a retransmission
        auto tuple = it->second;

        // Retrieve the corresponding session key

        // Get the network server session key shared with the end device
        auto itSessionKeys = endDeviceSessionKeys.find(deviceAddr);
        if (itSessionKeys == endDeviceSessionKeys.end())
            // Ignore the message if no session key is found (should not be)
            return BAD_FRAME;

        std::array<uint8_t, KEY_LORAWAN_SIZE> nwkSKey = itSessionKeys->second;
        uint8_t* keys[1] = { nwkSKey.data() };

        // Check the MIC of the message for data integrity & authentication
        if (!isValidMIC(datalinkMsg, endDeviceAppMsg, keys, sizeof(keys)))
            return BAD_MIC;

        if (*counter <= std::get<1>(tuple))
            return BAD_COUNTER;

        // Update frame counter for uplinks
        //std::get<1>(tuple) = *counter;
    }

    // Get the message port
    *port = endDeviceAppMsg->getPort();

    // Check if the message port is valid
    //if (!isValidPort(*port, MSG_LORA))
    if (!isValidPort(*port, MSG_LORA_UPLINK))
        return BAD_PORT;

    // Get the payload
    getArrayInMessageUplink(endDeviceAppMsg, &LoRaAppUplinkFrame::getPayload, payload, LORA_FRAME_SIZE_APP_PAYLOAD);

    return 0;
}

uint8_t LoRaNetworkServer::isValidMessageIp(
        cMessage* msg,
        uint8_t* srcAddress, uint8_t* destAddress,
        bool isTcp, uint32_t sequenceNumber, uint32_t ackNumber,
        uint8_t* payload, cPacket** encapsulatedPacket) {
    IPv4Packet* ipMsg = dynamic_cast<IPv4Packet*>(msg);
    if (!ipMsg)
        return BAD_FRAME;

    // The frame has been sent over IP.
    getArrayInMessageIp(ipMsg, &IPv4Packet::getDestAddress, destAddress, IPv4_ADDRESS_SIZE);

    // Check if the packet is intended for the current network server IP address
    if (memcmp(address, destAddress, IPv4_ADDRESS_SIZE))
        return BAD_ADDRESS;

    getArrayInMessageIp(ipMsg, &IPv4Packet::getSrcAddress, srcAddress, IPv4_ADDRESS_SIZE);

    // Check if the source address is in the map of TCP/IP connections
    std::array<uint8_t, IPv4_ADDRESS_SIZE> srcAddr;
    memcpy(srcAddr.data(), srcAddress, IPv4_ADDRESS_SIZE);

    if (isTcp) {
        auto it = mapTcp.find(srcAddr);
        if (it != mapTcp.end()) {
            // The address is in the map.
            // Check if the frame is a retransmission
            //std::tuple<cPacket*, cMessage*, uint32_t, uint32_t, uint32_t, bool> tuple = it->second;
            std::tuple<std::list<cPacket*>, cMessage*, uint32_t, uint32_t, uint32_t, bool> tuple = it->second;

            EV << "Received TCP/IP packet with seq num: " << sequenceNumber << "\n";
            EV << "Stored TCP/IP packet sequence number IN: " << std::get<2>(tuple) << "\n";
            if (sequenceNumber < std::get<2>(tuple))
                return BAD_COUNTER;

            // Simplified version of TCP/IP control without packet ordering
        }
        // Check if no message is received by the IP address but the sequence number is not 1
        else if (sequenceNumber != 1)
            return BAD_COUNTER;
    }

    // Get encapsulated packet
    *encapsulatedPacket = ipMsg->getEncapsulatedPacket();

    // Check if the packet has an encapsulated packet
    if (*encapsulatedPacket == nullptr)
        // No, get the IP packet payload
        //getArrayInMessageIp(ipMsg, &IPv4Packet::getPayload, payload, LORA_FRAME_SIZE_APP_PAYLOAD);
        getArrayInMessageIp(ipMsg, &IPv4Packet::getPayload, payload, MESSAGE_SIZE_IPV4_PAYLOAD);

    return 0;
}

uint8_t LoRaNetworkServer::isValidMessageTcp(
        cMessage* msg, unsigned* port, uint32_t* sequenceNumber, uint32_t* ackNumber, cPacket** encapsulatedPacket) {
    TCPSegment* tcpMsg = dynamic_cast<TCPSegment*>(msg);
    if (!tcpMsg)
        return BAD_FRAME;

    // The frame has been sent over TCP.
    // Check if the message port is valid
    *port = tcpMsg->getDestPort();
    if (!isValidPort(*port, MSG_TCP))
        return BAD_PORT;

    // Get sequence and ACK numbers
    *sequenceNumber = tcpMsg->getSequenceNumber();
    *ackNumber      = tcpMsg->getACKnumber();

    // Get encapsulated packet
    *encapsulatedPacket = tcpMsg->getEncapsulatedPacket();

    return 0;
}

uint8_t LoRaNetworkServer::isValidMessageUdp(cMessage* msg, unsigned* port, cPacket** encapsulatedPacket) {
    UDPSegment* udpMsg = dynamic_cast<UDPSegment*>(msg);
    if (!udpMsg)
        return BAD_FRAME;

    // The frame has been sent over UDP.
    // Check if the message port is valid
    *port = udpMsg->getDestPort();
    if (!isValidPort(*port, MSG_UDP))
        return BAD_PORT;

    // Get encapsulated packet
    *encapsulatedPacket = udpMsg->getEncapsulatedPacket();

    return 0;
}

void LoRaNetworkServer::printError(uint8_t error) {
    switch (error) {
        case BAD_ADDRESS:
            EV << "Received packet for a different IP address\n";
            break;
        case BAD_PORT:
            EV << "Received invalid port in the packet or frame\n";
            // Only a message can be received in a receive window
            //message_received = MSG_MAC_CMD;
            break;
        default:
            EV << "Received invalid frame\n";
    }
}

void LoRaNetworkServer::sendMessageTcp(
        const char* messageName, const char* timeoutName,
        std::array<uint8_t, IPv4_ADDRESS_SIZE>& destAddress, unsigned port,
        uint8_t* payload, unsigned payloadSize,
        cPacket* encapsulatedPacket, const char* gateName) {
    // Check if the network server is already in the map for TCP/IP connections
    //std::tuple<cPacket*, cMessage*, uint32_t, uint32_t, uint32_t, bool> tuple;
    std::tuple<std::list<cPacket*>, cMessage*, uint32_t, uint32_t, uint32_t, bool> tuple;
    auto it = mapTcp.find(destAddress);
    if (it != mapTcp.end()) {
        // The network server is in the map, get the entry
        tuple = it->second;

        // Use a list of messages to perfectly handle TCP retransmissions because
        // ACKs remove the first n items from the list or all of them based on its value.
        // In this way, there are no overriding of messages to retransmitted and a set
        // is retransmitted instead of just the last (without list)


        // Check if a timeout exists for a previous transmission
        if (std::get<1>(tuple)) {
            // Cancel old timeout
            cancelEvent(std::get<1>(tuple));

            // Rename timeout
            std::get<1>(tuple)->setName(timeoutName);
        }
        else {
            // Create timeout
            cMessage* eventTimeout = new cMessage(timeoutName);
            std::get<1>(tuple) = eventTimeout;

            // Add timeout in the inverted index
            eventTimeoutRetransmissions[eventTimeout] = destAddress;
        }

        // Update TCP/IP sequence number OUT
        std::get<3>(tuple) += 1;
    }
    else {
        // Init list of last messages sent
        std::get<0>(tuple) = std::list<cPacket*> {};

        // Create timeout
        cMessage* eventTimeout = new cMessage(timeoutName);
        std::get<1>(tuple) = eventTimeout;

        // Add timeout in the inverted index
        eventTimeoutRetransmissions[eventTimeout] = destAddress;

        // Init counters (sequence number should be random).
        // Simplified skipping three-way handshake
        std::get<2>(tuple) = 1;
        std::get<3>(tuple) = 1;
        std::get<4>(tuple) = 1;
        std::get<5>(tuple) = memcmp(gateName, IP_GATEWAYS_GATE_OUT, strlen(IP_GATEWAYS_GATE_OUT)) ? false : true;
    }

    EV << "Sending TCP/IP packet with seq num: " << std::get<3>(tuple) << "\n";
    EV << "Sending TCP/IP packet with ACK seq num: " << std::get<4>(tuple) << "\n";

    // Create TCP/IP packet
    cPacket* msgOut = createMessageIp(
            messageName,
            address, destAddress.data(), IPv4_ADDRESS_SIZE,
            payload, payloadSize,
            encapsulatedPacket);
    msgOut = createMessageTcp(
            messageName,
            port, port,
            std::get<3>(tuple), std::get<4>(tuple), true,
            nullptr, 0,
            msgOut);

    // Store the entry in the table
    //std::get<0>(tuple) = msgOut;
    std::get<0>(tuple).push_back(msgOut);
    mapTcp[destAddress] = tuple;

    EV << "Printing routing table:\n";
    for(auto it = routingTable.begin(); it != routingTable.end(); ++it){
        auto ipAddress = it->first;
        EV << "IP address: "
            << (int) ipAddress[0] << "."
            << (int) ipAddress[1] << "."
            << (int) ipAddress[2] << "."
            << (int) ipAddress[3]
            << "\t| gate ID: " << it->second << "\n";
    }

    // In OMNeT++ the gateway does not know which is the gate index associated to the IP address.
    // Check if the address is in the routing table to retrieve the gate index
    auto itRT = routingTable.find(destAddress);
    if (itRT != routingTable.end()) {
        EV << "Sending to gate: " << itRT->second << "\n";
        //send(msgOut->dup(), gateName, itRT->second);
        //sendSecurely(msgOut->dup(), gateName, itRT->second);
        sendSecurely(this, eventTimeoutChannelTransmissions, msgOut->dup(), gateName, itRT->second);
    }
    else
        // The address is not in the routing table, send in broadcast
        //sendBroadcast(this, msgOut->dup(), gateName);
        sendBroadcastSecurely(this, eventTimeoutChannelTransmissions, msgOut->dup(), gateName);

    scheduleAt(simTime() + timeoutTcp, std::get<1>(tuple));
}


//}
