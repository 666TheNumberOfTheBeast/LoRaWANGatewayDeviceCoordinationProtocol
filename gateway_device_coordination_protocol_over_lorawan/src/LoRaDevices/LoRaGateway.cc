#include "LoRaGateway.h"
#include "LoRaEndDevice.h"

#include <random>

//namespace masterthesis {


// The module class needs to be registered with OMNeT++
Define_Module(LoRaGateway);


LoRaGateway::LoRaGateway() {
    // Set the pointer to nullptr, so that the destructor won't crash
    // even if initialize() doesn't get called because of a runtime
    // error or user cancellation during the startup process.

    /*for (std::map<cMessage*, std::array<uint8_t, IPv4_ADDRESS_SIZE>>::iterator it=eventTimeouts.begin();
            it!=eventTimeouts.end(); ++it) {
        it->first = nullptr;
    }*/

    /*for (auto it=map.begin(); it!=map.end(); ++it) {
        std::get<0>(it->second) = nullptr;
        std::get<1>(it->second) = nullptr;
    }*/

    eventTimeoutResources = eventTimeoutNearbyGateways = nullptr;
}

LoRaGateway::~LoRaGateway() {
    // Dispose of dynamically allocated the objects
    cancelAndDelete(eventTimeoutResources);

    /*for (auto it : map) {
        auto msg1 = std::get<0>(it.second);
        auto msg2 = std::get<1>(it.second);

        if (msg1 && msg1->getOwner() == this)
            delete msg1;

        cancelAndDelete(msg2);
    }

    for (auto it : mapTcp) {
        auto msg1 = std::get<0>(it.second);
        auto msg2 = std::get<1>(it.second);

        if (msg1 && msg1->getOwner() == this)
            delete msg1;

        cancelAndDelete(msg2);
    }

    //for (auto it : eventTimeoutRetransmissions)
    //    cancelAndDelete(it.first);

    for (auto it : eventTimeoutChannelTransmissions) {
        //cancelAndDelete(it.first);

        auto msg = std::get<0>(it.second);
        if (msg && msg->getOwner() == this)
            delete msg;
    }*/

    commonEndDeviceSKeys.clear();
    neighborGatewayAddresses.clear();
    clusters.clear();
    map.clear();
    associations.clear();
    mapTcp.clear();
    routingTable.clear();
    eventTimeoutRetransmissions.clear();
    eventTimeoutChannelTransmissions.clear();
    interferences.clear();
}

void LoRaGateway::initialize() {
    // Initialize is called at the beginning of the simulation
    EV << "Initializing gateway...\n";

    // Initialize class variables
    srand(time(NULL));

    // Set network server address
    networkServerAddress = {
        IPv4_ADDRESS_NETWORK_SERVER_0,
        IPv4_ADDRESS_NETWORK_SERVER_1,
        IPv4_ADDRESS_NETWORK_SERVER_2,
        IPv4_ADDRESS_NETWORK_SERVER_3
    };

    // Set gateway address.
    // Use the unique component's ID (it's >= 2) instead of the non-unique module vector index (it's >= 0)
    int id = getId() - 2;
    int j = id/254;
    address[0] = IPv4_ADDRESS_NETWORK_SERVER_0;
    address[1] = IPv4_ADDRESS_NETWORK_SERVER_1;
    address[2] = (IPv4_ADDRESS_NETWORK_SERVER_2 + j) % 256;
    address[3] = 1 + id%254;

    EV << "Gateway IP address: "
        << (int) address[0] << "."
        << (int) address[1] << "."
        << (int) address[2] << "."
        << (int) address[3]
        << "\n";


    // Set Device EUI
    /*for (int i=0; i<EUI_SIZE; i++) {
        devEUI[i]  = (rand() + (int) uniform(0, 100)) % 256;
    }*/
    memset(devEUI, 0, sizeof(devEUI));
    uintToBytes(id, &devEUI[4]);

    // Set network server ID
    memset(networkId, 0, sizeof(networkId));

    memcpy(commonGatewaysKey, "Common_GW_root_key", sizeof(commonGatewaysKey));
    memcpy(commonEndDeviceKey, "Common_root_key", sizeof(commonEndDeviceKey));
    memcpy(associationKey, "Association_root_key", sizeof(associationKey));

    memset(nwkSKey, 0, sizeof(nwkSKey));

    printKey(devEUI, sizeof(devEUI), "devEUI", EV);
    printKey(networkId, sizeof(networkId), "networkId", EV);

    printKey(commonGatewaysKey, sizeof(commonGatewaysKey), "commonGatewaysKey", EV);
    printKey(commonEndDeviceKey, sizeof(commonEndDeviceKey), "commonEndDeviceKey", EV);
    printKey(associationKey, sizeof(associationKey), "associationKey", EV);


    // Resources O.S. default values
    cpuLoad         = uniform(0, 0.08);
    gpuLoad         = uniform(0, 0.05);
    ramLoad         = 0.05;
    storageOccupied = 866e6; // 866 MB
    networkIn       = 0;
    networkOut      = 0;

    stage = STAGE_ACTIVATED;

    // Set network server out gate ID in routing table
    routingTable[networkServerAddress] = 0;

    // Set timers
    timeoutResources = 1;
    // The message is received when the end device approximately closes the transmission window (time on air).
    // After airtime the end device awaits RX_DELAY_1, so, the response must be delayed only if
    // response airtime < RX_DELAY_1, otherwise must be immediately transmitted.
    // For a retransmission attempt, await this roughly identical moment in the future (TX_DELAY) and apply the delay
    // (no airtime here, be confident on half RX_DELAY_1 but it is only an attempt)
    timeoutLoRa = RX_DELAY_1/2 + TX_DELAY;
    timeoutTcp  = 3;

    eventTimeoutResources = new cMessage("timeoutResources");
    scheduleAt(simTime() + timeoutResources, eventTimeoutResources);

    eventTimeoutNearbyGateways = new cMessage("timeoutNearbyGateways");


    // For statistics
    messagesSentCount         = 0;
    messagesReceivedCount     = 0;
    messagesSentLoRaCount     = 0;
    messagesReceivedLoRaCount = 0;
    messagesSentIpCount       = 0;
    messagesReceivedIpCount   = 0;
    messagesLostCount         = 0;
    messagesLostLoRaCount     = 0;
    messagesLostIpCount       = 0;
    interferencesCount         = 0;
    interferencesPossibleCount = 0;


    // Set gateway position
    cModule* parent = getParentModule();
    int bgX = parent->par("bgX").intValue();
    int bgY = parent->par("bgY").intValue();
    bool realDeployment = parent->par("realDeployment").boolValue();

    //========================
    // Set random position
    /*posX = (rand() + (int) uniform(0, bgX)) % bgX;
    posY = (rand() + (int) uniform(0, bgY)) % bgY;
    //posX = (rand() % bgX + (int) uniform(500, bgX)) % bgX;
    //posY = (rand() % bgY + (int) uniform(500, bgY)) % bgY;

    if (posY <= 300 && posX <= bgX*0.6)
        posY += 500;*/
    //========================

    // Get the index of the gateway in the vector
    int index = getIndex();

    if (index == 0) {
        // Set gateway position
        posX = bgX / 2;
        posY = bgY / 2;
    }
    else {
        if (realDeployment) {
            // 1. calculate min total distance of an end device from the two gateways
            // 2. place the gateway between minDistance e max = calculated min distance

            // Get all RSSIs
            cValueArray* RSSIs_ = (cValueArray*) getParentModule()->par("RSSIs").objectValue();

            // Convert RSSIs to array of strings
            std::vector<std::string> rssiStrings = RSSIs_->asStringVector();

            // Min distance between the two gateways equal to the max difference of distances of
            // an end device from the two gateways.
            // Max distance between the two gateways equal to the min total distance of
            // an end device from the two gateways
            double minDistance = 0;
            double maxDistance = LORA_RANGE;

            // Iterate RSSIs per end device
            for (auto rssiString : rssiStrings) {
                // Split the strings of RSSIs to obtain one RSSI per gateway
                stringstream stream(rssiString);
                string rssi;
                std::vector<double> RSSIs {};

                int i = 0;
                while (stream >> rssi) {
                    // Convert string to double
                    RSSIs.push_back(stod(rssi));

                    //EV << rssi << "\n";
                    i++;
                }

                // In real deployment dataset, each device is connected with the same two gateways
                std::vector<double> distances {};

                // Calculate distances of the end device from the gateways
                for (int j=0; j<i; j++) {
                    // Calculate distance from RSSI collected at the gateway
                    // rssi = -10 * PATH_LOSS_EXP * log10(d) + MEASURED_POWER
                    // rssi - MEASURED_POWER = -10 * PATH_LOSS_EXP * log10(d)
                    // 10^((rssi - MEASURED_POWER) / (-10 * PATH_LOSS_EXP)) = d
                    distances.push_back(pow(10, (RSSIs[j] - MEASURED_POWER) / (-10 * PATH_LOSS_EXP)));

                    EV << "RSSI: " << RSSIs[j] << " => Distance: " << distances[j] << "\n";
                    //EV << "Distance: " << distances[i] << " => RSSI: " << -10 * PATH_LOSS_EXP * log10(distances[i]) + MEASURED_POWER << "\n";
                }

                if (distances.size() < 2)
                    continue;

                // Sum the distances of the end device from the two gateways
                double sum  = distances[0] + distances[1];
                double diff = abs(distances[0] - distances[1]);

                // Update the min distance between gateways if needed
                if (diff > minDistance) {
                    minDistance = diff;
                    EV << "max difference updated with " << minDistance << "!\n";
                }
                // Update the max distance between gateways if needed
                if (sum < maxDistance) {
                    maxDistance = sum;
                    EV << "min total distance updated with " << maxDistance << "!\n";
                }
            }

            EV << "Max difference distance of an end device from the two gateways = min total distance between gateways: " << minDistance << "\n";
            EV << "Min total distance of an end device from the two gateways = max total distance between gateways: " << maxDistance << "\n";

            // To obtain the intersection between two circumferences
            // we need both r1+r2 >= d and |r1-r2| <= d <=>
            // d >= |r1-r2| and d <= r1+r2 <=>
            // d in [|r1-r2|, r1+r2] = [min, max]
            // (= to include tangent circumferences)

            unsigned diffDistance = maxDistance - minDistance;
            double d = minDistance + (rand() + (int) uniform(0, diffDistance)) % diffDistance;

            // Locate the gateway on a point on the circumference of radius d
            // (x1 - x2)^2 + (y1 - y2)^2 = d^2 =>
            // x2 = x1 + d * cos alfa
            // y2 = x2 + d * sin alfa
            LoRaGateway* otherGateway = dynamic_cast<LoRaGateway*>(parent->getSubmodule("gateways", 0));

            // Locate on the circumference with positive coordinates
            do {
                // Get a random angle in radians
                double angle = ((rand() + (int) uniform(0, 360)) % 360) * 3.14 / 180;
                posX = otherGateway->getPosX() + d * cos(angle);
                posY = otherGateway->getPosY() + d * sin(angle);
            } while (posX < 0 || posY < 0);
        }
        else {
            // ============================
            // Set gateway position
            /*posX = (rand() + (int) uniform(0, bgX)) % bgX;
            posY = (rand() + (int) uniform(0, bgY)) % bgY;
            //posX = (rand() % bgX + (int) uniform(500, bgX)) % bgX;
            //posY = (rand() % bgY + (int) uniform(500, bgY)) % bgY;

            // Check if it is a valid position
            if (posY <= 300 && posX < bgX*0.7)
                posY += 1000;

            unsigned lastPosX;
            unsigned lastPosY;

            // Check if the gateway is in the range of at least another gateway (among the previous in the vector)
            if (!isInGatewayRange(index, &lastPosX, &lastPosY)) {
                // Try to place the gateway in the radio range of the previous one
                posX = rand()%2 ? (lastPosX + (int) uniform(500, 4500)) % bgX : (lastPosX - (int) uniform(500, 4500)) % bgX;
                posY = rand()%2 ? (lastPosY + (int) uniform(500, 4500)) % bgY : (lastPosY - (int) uniform(500, 4500)) % bgY;
            }*/
            // ============================

            // ============================
            // Get a random gateway among the previous in the vector
            int i = (rand() + (int) uniform(0, 100)) % index;
            LoRaGateway* otherGateway = dynamic_cast<LoRaGateway*>(parent->getSubmodule("gateways", i));

            const unsigned minDx = 500;
            const unsigned maxDx = LORA_RANGE / sqrt(2);
            const unsigned diffDx = maxDx- minDx;

            // Try to set gateway position in the other gateway range
            //posX = rand()%2 ? (otherGateway->getPosX() + (int) uniform(minDx, maxDx)) % bgX : (otherGateway->getPosX() - (int) uniform(minDx, maxDx)) % bgX;
            //posY = rand()%2 ? (otherGateway->getPosY() + (int) uniform(minDx, maxDx)) % bgY : (otherGateway->getPosY() - (int) uniform(minDx, maxDx)) % bgY;
            posX = rand()%2 ? (otherGateway->getPosX() + minDx + (rand() + (int) uniform(0, diffDx)) % diffDx) % bgX :
                              (otherGateway->getPosX() - minDx + (rand() + (int) uniform(0, diffDx)) % diffDx) % bgX;
            posY = rand()%2 ? (otherGateway->getPosY() + minDx + (rand() + (int) uniform(0, diffDx)) % diffDx) % bgY :
                              (otherGateway->getPosY() - minDx + (rand() + (int) uniform(0, diffDx)) % diffDx) % bgY;
            // ============================

            // Create connections over LoRa with other gateways based on location
            /*int gateIndex = 0;

            for (int i=0; i<index; i++) {
                // Get the other gateway in the vector
                otherGateway = dynamic_cast<LoRaGateway*>(parent->getSubmodule("gateways", i));
                double distance = 0;

                // Work by addition
                // Check if the gateway is in the range of at least another gateway (among the previous in the vector)
                if (isInLoRaRange(posX, posY, otherGateway->getPosX(), otherGateway->getPosY(), &distance, LORA_RANGE)) {
                    // Get gateway vector gate size (IN & OUT sizes are equal)
                    int otherGatewayGateSize = otherGateway->gateSize(otherGateway->getLoRaGateIn());

                    // Increment gate vector sizes
                    setGateSize(LORA_GATE_BASENAME, gateIndex+1);
                    otherGateway->setGateSize(otherGateway->getLoRaGateBasename(), otherGatewayGateSize+1);

                    // Get device output gate & gateway input gate
                    cGate* gatewayGate  = gate(LORA_GATE_OUT, gateIndex);
                    cGate* otherGatewayGate = otherGateway->gate(otherGateway->getLoRaGateIn(), otherGatewayGateSize);

                    EV << "Device gate OUT vector size: " << gatewayGate->getVectorSize() << "\n";
                    EV << "Gateway gate IN vector size: " << otherGatewayGate->getVectorSize() << "\n";
                    //EV << "Gateway gate IN vector size: " << gateway->gateSize(gateway->getLoRaGateIn()) << "\n";

                    // Define channel
                    double delay = distance / SPEED_LIGHT_ON_AIR;
                    cDatarateChannel* channel = cDatarateChannel::create("channel");
                    channel->setDelay(delay);
                    channel->setDatarate(5e3); // 5 Kb/s

                    // Create OUT->IN connection
                    gatewayGate->connectTo(otherGatewayGate, channel);
                    channel->callInitialize();

                    // Get device input gate & gateway output gate
                    gatewayGate  = gate(LORA_GATE_IN, gateIndex++);
                    otherGatewayGate = otherGateway->gate(otherGateway->getLoRaGateOut(), otherGatewayGateSize);

                    EV << "Device gate IN vector size: " << gatewayGate->getVectorSize() << "\n";
                    EV << "Gateway gate OUT vector size: " << otherGatewayGate->getVectorSize() << "\n";
                    //EV << "Gateway gate OUT vector size: " << gateway->gateSize(gateway->getLoRaGateOut()) << "\n";

                    // Define channel
                    channel = cDatarateChannel::create("channel");
                    channel->setDelay(delay);
                    channel->setDatarate(5e3); // 5 Kb/s

                    // Create IN<-OUT connection
                    otherGatewayGate->connectTo(gatewayGate, channel);
                    channel->callInitialize();
                }
            }*/
        }

        // Create connections over LoRa with other gateways based on location
        //int gateIndex = 0;
        int gateIndex = realDeployment ? gateSize(LORA_GATE_BASENAME) : 0;

        for (int i=0; i<index; i++) {
            // Get the other gateway in the vector
            LoRaGateway* otherGateway = dynamic_cast<LoRaGateway*>(parent->getSubmodule("gateways", i));
            double distance = 0;

            // Work by addition
            // Check if the gateway is in the range of at least another gateway (among the previous in the vector)
            if (isInLoRaRange(posX, posY, otherGateway->getPosX(), otherGateway->getPosY(), &distance, LORA_RANGE)) {
                // Get gateway vector gate size (IN & OUT sizes are equal)
                int otherGatewayGateSize = otherGateway->gateSize(otherGateway->getLoRaGateIn());

                // Increment gate vector sizes
                setGateSize(LORA_GATE_BASENAME, gateIndex+1);
                otherGateway->setGateSize(otherGateway->getLoRaGateBasename(), otherGatewayGateSize+1);

                // Get device output gate & gateway input gate
                cGate* gatewayGate  = gate(LORA_GATE_OUT, gateIndex);
                cGate* otherGatewayGate = otherGateway->gate(otherGateway->getLoRaGateIn(), otherGatewayGateSize);

                EV << "Device gate OUT vector size: " << gatewayGate->getVectorSize() << "\n";
                EV << "Gateway gate IN vector size: " << otherGatewayGate->getVectorSize() << "\n";
                //EV << "Gateway gate IN vector size: " << gateway->gateSize(gateway->getLoRaGateIn()) << "\n";

                // Define channel
                double delay = distance / SPEED_LIGHT_ON_AIR;
                cDatarateChannel* channel = cDatarateChannel::create("channel");
                channel->setDelay(delay);
                channel->setDatarate(5e3); // 5 Kb/s

                // Create OUT->IN connection
                gatewayGate->connectTo(otherGatewayGate, channel);
                channel->callInitialize();

                // Get device input gate & gateway output gate
                gatewayGate  = gate(LORA_GATE_IN, gateIndex++);
                otherGatewayGate = otherGateway->gate(otherGateway->getLoRaGateOut(), otherGatewayGateSize);

                EV << "Device gate IN vector size: " << gatewayGate->getVectorSize() << "\n";
                EV << "Gateway gate OUT vector size: " << otherGatewayGate->getVectorSize() << "\n";
                //EV << "Gateway gate OUT vector size: " << gateway->gateSize(gateway->getLoRaGateOut()) << "\n";

                // Define channel
                channel = cDatarateChannel::create("channel");
                channel->setDelay(delay);
                channel->setDatarate(5e3); // 5 Kb/s

                // Create IN<-OUT connection
                otherGatewayGate->connectTo(gatewayGate, channel);
                channel->callInitialize();
            }
        }
    }

    cDisplayString& ds = getDisplayString();
    ds.setTagArg("p", 0, posX);
    ds.setTagArg("p", 1, posY);

    EV << "posX: " << posX << "\n";
    EV << "posY: " << posY << "\n";


    // Convert component ID to string
    const size_t intLen = 4 * sizeof(int);
    char id_[intLen] = {};
    snprintf(id_, intLen, "%d", id);

    // Register signal for handling interferences
    /*char signalInterferenceName[9+intLen] = "gateway-";
    strncat(signalInterferenceName, id_, sizeof(signalInterferenceName)-1);

    EV << "Gateway signal for interferences name: " << signalInterferenceName << "\n";

    signalInterference = registerSignal(signalInterferenceName);
    subscribe(signalInterference, this);

    EV << "Gateway signal for interferences: " << signalInterference << "\n";*/

    // To automatically stop the simulation
    endDeviceTerminations = 0;


    // The WATCH() statement below will let you examine the variables under Tkenv
    WATCH(cpuLoad);
    WATCH(gpuLoad);
    WATCH(ramLoad);
    WATCH(storageOccupied);
    WATCH(networkIn);
    WATCH(networkOut);


    // Register signals for statistic collection
    signalSent         = registerSignal("sent");
    signalReceived     = registerSignal("received");
    signalSentLoRa     = registerSignal("sentLoRa");
    signalReceivedLoRa = registerSignal("receivedLoRa");
    signalSentIp       = registerSignal("sentIp");
    signalReceivedIp   = registerSignal("receivedIp");
    signalLost         = registerSignal("lost");
    signalLostLoRa     = registerSignal("lostLoRa");
    signalLostIp       = registerSignal("lostIp");
    signalSentCount         = registerSignal("sentCount");
    signalReceivedCount     = registerSignal("receivedCount");
    signalSentLoRaCount     = registerSignal("sentLoRaCount");
    signalReceivedLoRaCount = registerSignal("receivedLoRaCount");
    signalSentIpCount       = registerSignal("sentIpCount");
    signalReceivedIpCount   = registerSignal("receivedIpCount");
    signalLostCount         = registerSignal("lostCount");
    signalLostLoRaCount     = registerSignal("lostLoRaCount");
    signalLostIpCount       = registerSignal("lostIpCount");
    signalInterference              = registerSignal("interference");
    signalInterferencePossible      = registerSignal("interferencePossible");
    signalInterferenceCount         = registerSignal("interferenceCount");
    signalInterferencePossibleCount = registerSignal("interferencePossibleCount");
    signalRSSI         = registerSignal("rssi");
    signalConnected    = registerSignal("connected");
    signalCpu          = registerSignal("cpu");
    signalGpu          = registerSignal("gpu");
    signalRam          = registerSignal("ram");
    signalStorage      = registerSignal("storage");
    signalNetworkIn    = registerSignal("networkIn");
    signalNetworkOut   = registerSignal("networkOut");

    EV << "Gateway initialized\n";
}

void LoRaGateway::handleMessage(cMessage *msgIn) {
    // The handleMessage() method is called whenever a message arrives
    // at the module.

    // For analyzing MICs
    std::cout << "*****************\nGateway\n";

    EV << "============= GATEWAY =============\n";
    EV << "Received message: " << msgIn->getName() << "\n";
    EV << "stage: " << (unsigned) stage << "\n";

    // Check if the message is the timeout for updating gateway resources
    if (msgIn == eventTimeoutResources) {
        // Send signals for statistic collection
        emit(signalCpu, cpuLoad);
        emit(signalGpu, gpuLoad);
        emit(signalRam, ramLoad);
        emit(signalStorage, (storageOccupied * 100.0)/STORAGE_SIZE);
        emit(signalNetworkIn, networkIn);
        emit(signalNetworkOut, networkOut);

        EV << "Cpu load in the last second: " << cpuLoad << "\n";
        EV << "Gpu load in the last second: " << gpuLoad << "\n";
        EV << "RAM load in the last second: " << ramLoad << "\n";
        EV << "Bytes of storage occupied: " << storageOccupied << " = " << ((storageOccupied * 100.0)/STORAGE_SIZE) << "%\n";
        EV << "Bytes received in the last second: " << networkIn << "\n";
        EV << "Bytes sent in the last second: " << networkOut << "\n";

        updateResources();

        scheduleAt(simTime() + timeoutResources, eventTimeoutResources);
        return;
    }

    // Check if the message is the timeout for sending NEARBY_GATEWAYS message
    if (msgIn == eventTimeoutNearbyGateways) {
        EV << "Sending NEARBY_GATEWAYS messages...\n";
        sendMessageNearbyGateway();

        networkOut += (MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_TCP_HEADER) * neighborGatewayAddresses.size();
        return;
    }

    // Check if the message is a timeout for handling transmissions on the channels
    auto itTX = eventTimeoutChannelTransmissions.find(msgIn);
    if (itTX != eventTimeoutChannelTransmissions.end()) {
        // Check if at least a HELLO_GATEWAY message was received
        if (neighborGatewayAddresses.empty())
            return;

        // Get the entry from the map
        auto tuple = itTX->second;

        EV << "gatename: " << std::get<1>(tuple) << "\n";
        EV << "index: " << std::get<2>(tuple) << "\n";

        // Get the message to retransmit
        cPacket* msgOut = std::get<0>(tuple);

        // Retry to send
        //sendSecurely(std::get<0>(tuple), std::get<1>(tuple).c_str(), std::get<2>(tuple));
        sendSecurely(this, eventTimeoutChannelTransmissions, msgOut, std::get<1>(tuple).c_str(), std::get<2>(tuple));

        // Remove the entry from the map
        eventTimeoutChannelTransmissions.erase(itTX);

        return;
    }

    // Check if the message is a retransmission timeout through the inverted index
    std::map<cMessage*, std::array<uint8_t, IPv4_ADDRESS_SIZE>>::iterator it = eventTimeoutRetransmissions.find(msgIn);
    if (it != eventTimeoutRetransmissions.end()) {
        // The message is a timeout.
        // Handle retransmissions over LoRa and over TCP/IP
        EV << "The timeout for a retransmission is expired\n";

        std::array<uint8_t, IPv4_ADDRESS_SIZE> destAddress = it->second;

        EV << "Destination address: "
        << (unsigned) destAddress[0] << "."
        << (unsigned) destAddress[1] << "."
        << (unsigned) destAddress[2] << "."
        << (unsigned) destAddress[3]
        << "\n";

        //================
        /*cPacket* msgOut = std::get<0>(mapTcp[destAddress]);
        // Check if the message must be retransmitted over TCP/IP
        TCPSegment* traMsg = dynamic_cast<TCPSegment*>(msgOut);
        if (traMsg) {
            EV << "Retransmit over TCP/IP\n";

            std::list<cPacket*> messagesOut = std::get<0>(mapTcp[destAddress]);

            // Resend the message to the destination
            sendMessageIp(msgOut, destAddress);

            EV << "Restart the timeout\n";
            scheduleAt(simTime() + timeoutTcp, it->first);

            // Send signal for statistic collection
            emit(signalSent, 1u);
            emit(signalSentIp, 1u);

            networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_TCP_HEADER;
        }*/
        //================

        //================
        // Check if the message must be retransmitted over TCP/IP
        auto itTcp = mapTcp.find(destAddress);
        if (itTcp != mapTcp.end()) {
            EV << "Retransmit over TCP/IP\n";

            std::list<cPacket*> messagesOut = std::get<0>(mapTcp[destAddress]);
            for (auto msgOut : messagesOut) {
                // Resend the message to the destination
                sendMessageIp(msgOut, destAddress);

                // Send signal for statistic collection
                emit(signalSent, 1u);
                emit(signalSentIp, 1u);
                emit(signalSentCount, ++messagesSentCount);
                emit(signalSentIpCount, ++messagesSentIpCount);
            }

            EV << "Restart the timeout\n";
            scheduleAt(simTime() + timeoutTcp, it->first);

            networkOut += (MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_TCP_HEADER) * messagesOut.size();
        }
        //================
        else {
            // The timeout is not about a TCP/IP retransmission.
            // Get the last message sent on LoRa to the device address
            auto tuple = map[destAddress];
            /*auto itMap = map.find(address);
            if (!itMap)
                return;

            auto tuple = itMap->second;*/
            //msgOut = std::get<0>(tuple);
            cPacket* msgOut = std::get<0>(tuple);

            // Check if the message is a LoRa frame (should be)
            LoRaPhysicalFrame* phyMsg = dynamic_cast<LoRaPhysicalFrame*>(msgOut);
            if (!phyMsg)
                return;

            LoRaDatalinkFrame* dlMsg = dynamic_cast<LoRaDatalinkFrame*>(phyMsg->getEncapsulatedPacket());
            if (!dlMsg)
                return;

            // Decapsulate LoRa App layer
            LoRaAppDownlinkFrame* appMsg = dynamic_cast<LoRaAppDownlinkFrame*>(dlMsg->decapsulate());
            if (!appMsg)
                return;

            // Only HELLO_GATEWAY, HELLO and FORWARD are expected to be retransmitted.
            // Use the right key
            uint8_t keyMIC[KEY_LORAWAN_SIZE];
            uint8_t port = appMsg->getPort();

            switch (port) {
                case MSG_PORT_HELLO_GATEWAY: {
                    // Get common gateways root key
                    memcpy(keyMIC, commonGatewaysKey, KEY_LORAWAN_SIZE);

                    // Create new payload because the expiration time is no longer valid.
                    // Calculate preamble and frame airtime
                    auto tupleAirtime = calculateTimeOnAir(msgOut);

                    // Get frame time on air in ms
                    double airtimeFrame = std::get<1>(tupleAirtime);

                    // Check if computation succeeded
                    if (airtimeFrame < 0)
                        return;

                    // Get current time in ms
                    simtime_t time = simTime() * 1000;

                    // Calculate expiration time with a tolerance of 50 ms
                    simtime_t expirationTime = time + airtimeFrame + 50;

                    EV << "Time: " << time << " ms\n";
                    EV << "Airtime frame: " << airtimeFrame << " ms\n";
                    EV << "Time expiration: " << time + airtimeFrame << " ms\n";
                    EV << "ceil(Time expiration): " << ceil(time + airtimeFrame) << " ms\n";
                    EV << "ceil(Time expiration): " << ceil(time + airtimeFrame).dbl() << " ms\n";

                    // Convert time to an array of bytes
                    uint8_t timestampBytes[4] = {};
                    //uint8_t timestampBytes[8] = {};
                    uintToBytes(ceil(expirationTime).dbl(), timestampBytes);

                    // Build payload
                    uint8_t payload[8] = {};
                    memcpy(payload, timestampBytes, 4);
                    memcpy(&payload[4], address, IPv4_ADDRESS_SIZE);

                    setArrayInMessageDownlink(appMsg, &LoRaAppDownlinkFrame::setPayload, payload, sizeof(payload));

                    break; }
                case MSG_PORT_STATS: {
                    // Get common session key shared with the end device
                    /*auto itCommonSKeys = commonEndDeviceSKeys.find(address);
                    if (itCommonSKeys == commonEndDeviceSKeys.end())
                        return;

                    keyMIC = itCommonSKeys->second.data();*/
                    //memcpy(keyMIC, commonEndDeviceSKeys[destAddress].data(), KEY_LORAWAN_SIZE);

                    // Get the address of the peer who forwarded the message
                    std::array<uint8_t, IPv4_ADDRESS_SIZE> forwarderAddress = std::get<8>(tuple);

                    if (neighborGatewayAddresses.find(forwarderAddress) == neighborGatewayAddresses.end())
                        // Get common session key shared with the end device
                        memcpy(keyMIC, commonEndDeviceSKeys[destAddress].data(), KEY_LORAWAN_SIZE);
                    else {
                        // Get common session key shared with the gateways in the cluster.
                        // Iterate clusters (subsets of the previous set) to find the session key
                        for (auto itClusters : clusters) {
                            auto commonSKey = itClusters.first;
                            auto cluster    = itClusters.second;

                            if (cluster.find(forwarderAddress) != cluster.end()) {
                                memcpy(keyMIC, commonSKey.data(), KEY_LORAWAN_SIZE);
                                break;
                            }
                        }
                    }

                    break; }
                default:
                    return;
            }


            EV << "Retransmit over LoRa\n";

            // Resend the message associated with the timeout (using the map indexed by addresses)
            // updating the frame counter as the message can be refused
            // if another downlink has been received with the same counter

            // Increment the frame counter
            appMsg->setCounter(appMsg->getCounter() + 1);

            // Encapsulate LoRa App layer
            dlMsg->encapsulate(appMsg);

            // Recalculate MIC
            calculateMIC(dlMsg, appMsg, keyMIC);

            // Do not need to update stored message as the physical pointer is still valid
            //std::get<0>(tuple) = phyMsg;

            // Retransmit
            //sendBroadcast(this, phyMsg->dup(), LORA_GATE_OUT);
            sendBroadcastSecurely(this, eventTimeoutChannelTransmissions, phyMsg->dup(), LORA_GATE_OUT);

            // Send signal for statistic collection
            emit(signalSent, 1u);
            emit(signalSentLoRa, 1u);
            emit(signalSentCount, ++messagesSentCount);
            emit(signalSentLoRaCount, ++messagesSentLoRaCount);

            networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC +
                          LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;

            bool restartTimer = true;

            // Check if the retransmission involves a HELLO_GATEWAY message
            if (appMsg->getPort() == MSG_PORT_HELLO_GATEWAY) {
                // Decrease the number of retransmissions
                std::get<3>(tuple) -= 1;

                // Check if repetitions are exhausted
                if (std::get<3>(tuple) <= 0) {
                    restartTimer = false;

                    // Schedule timer for allowing to possibly send the NEARBY_GATEWAYS messages
                    // if no HELLO_GATEWAY message was received so far and at least one is received during the timer
                    scheduleAt(simTime() + 10, eventTimeoutNearbyGateways);
                }
            }
            // Try to retransmit once otherwise if the end device terminates, this msg is repeated infinitely
            else
                restartTimer = false;

            if (restartTimer) {
                EV << "Restart the timeout\n";
                scheduleAt(simTime() + timeoutLoRa, it->first);
            }
            else {
                EV << "Cancel the timeout\n";

                // Remove the entry from the map
                eventTimeoutRetransmissions.erase(it);

                // Delete the timeout and the message
                cancelAndDelete(it->first);
                delete std::get<0>(tuple);

                std::get<0>(tuple) = nullptr;
                std::get<1>(tuple) = nullptr;

                storageOccupied -= sizeof(cMessage*) + IPv4_ADDRESS_SIZE;
            }

            // Store the updated entry
            map[destAddress] = tuple;
        }
    }
    else {
        // The message is not a timeout.
        // Lose the message with a certain probability
        //if (uniform(0, 1) < 2) {
        /*if (uniform(0, 1) < MSG_LOSS_PROBABILITY) {
            EV << "Message lost!\n";

            // Make animation more informative
            bubble("Message lost!");

            // Delete the message as in OMNeT++ once sent out,
            // a message no longer belongs to the sender module and
            // it is taken over by the simulation kernel, and will eventually be delivered to the destination module.
            // Once the message arrives in the destination module, that module will have full authority over it
            delete msgIn;

            // Send signal for statistic collection
            emit(signalLost, 1);

            return;
        }*/


        // Get the gate name and index from which the message is arrived
        /*char* gateName = const_cast<char*> (msgIn->getArrivalGate()->getName());
        int gateIndex = msgIn->getArrivalGate()->getIndex();

        EV << "Gate name: " << gateName << "\n";
        EV << "Gate index: " << gateIndex << "\n";*/


        // Process the message based on the arrival gate
        if (msgIn->arrivedOn(IP_GATE_IN))
            processMessageFromTransportLayer(msgIn);
        else {
            if (checkLoRaInterference(msgIn))
                processMessageFromLoRaLayer(msgIn);
        }
    }
}

void LoRaGateway::refreshDisplay() const {
    char buf[50] = {};
    snprintf(buf, sizeof(buf), "Connected devices: %lu", associations.size());
    getDisplayString().setTagArg("t", 0, buf);
}



//bool LoRaGateway::isValidPort(unsigned port, bool isUplinkFrame) {
bool LoRaGateway::isValidPort(unsigned port, uint8_t msgType) {
    switch (msgType) {
        /*case MSG_LORA:
            return port == MSG_PORT_MAC_CMD                  ||
                   port == MSG_PORT_HELLO_GATEWAY            ||
                   port == MSG_PORT_GENERATE_COMMON_KEY      ||
                   port == MSG_PORT_HELLO                    ||
                   port == MSG_PORT_FORWARD                  ||
                   port == MSG_PORT_STATS                    ||
                   port == MSG_PORT_PAIRING_REQUEST          ||
                   port == MSG_PORT_PAIRING_ACCEPT           ||
                   //port == MSG_PORT_PAIRING_VERIFY           ||
                   //port == MSG_PORT_PAIRING_VERIFIED         ||
                   port == MSG_PORT_CONNECTION               ||
                   port == MSG_PORT_GENERATE_ASSOCIATION_KEY ||
                   port == MSG_PORT_DATA_PROFILE             ||
                   port == MSG_PORT_DATA;
            break;*/
        case MSG_LORA_UPLINK:
            return port == MSG_PORT_MAC_CMD                  ||
                   port == MSG_PORT_GENERATE_COMMON_KEY      ||
                   port == MSG_PORT_HELLO                    ||
                   port == MSG_PORT_FORWARD                  ||
                   port == MSG_PORT_PAIRING_REQUEST          ||
                   //port == MSG_PORT_PAIRING_VERIFIED         ||
                   port == MSG_PORT_CONNECTION               ||
                   port == MSG_PORT_GENERATE_ASSOCIATION_KEY ||
                   port == MSG_PORT_DATA_PROFILE             ||
                   port == MSG_PORT_DATA;
            break;
        case MSG_LORA_DOWNLINK:
            return port == MSG_PORT_MAC_CMD                  ||
                   port == MSG_PORT_HELLO_GATEWAY            ||
                   port == MSG_PORT_STATS                    ||
                   port == MSG_PORT_PAIRING_ACCEPT           ||
                   //port == MSG_PORT_PAIRING_VERIFY           ||
                   port == MSG_PORT_CONNECTION               ||
                   port == MSG_PORT_GENERATE_ASSOCIATION_KEY ||
                   port == MSG_PORT_DATA_PROFILE             ||
                   port == MSG_PORT_DATA;
            break;
        case MSG_TCP:
            return port == MSG_PORT_NEARBY_GATEWAYS     ||
                   port == MSG_PORT_SYNC_COUNTER       ||
                   port == MSG_PORT_CONNECTION_GATEWAY ||
                   //port == MSG_PORT_SHARE_DATA_PROFILE ||
                   port == MSG_PORT_PROCESSED_DATA;
            break;
        case MSG_UDP:
            // Open just a port
            /*return port == MSG_PORT_FORWARD_JOIN            ||
                   port == MSG_PORT_FORWARD_MAC_CMD         ||
                   port == MSG_PORT_FORWARD_PAIRING_REQUEST ||
                   port == MSG_PORT_FORWARD_CONNECTION;*/
            return port == MSG_PORT_FORWARD_OVER_IP;
            break;
        default:
            throw std::invalid_argument("Invalid message type");
    }
}

uint8_t LoRaGateway::isValidLoRaFrame(
        cMessage* msg, uint8_t* frameType,
        uint8_t* deviceAddress, uint16_t* counter, unsigned* port, uint8_t* payload,
        uint8_t* spreadingFactor, uint8_t* transmissionPower,
        float* bandwidth, float* channelFrequency) {
    if (!msg || !frameType || !deviceAddress || !counter ||
        !port || !payload || !spreadingFactor ||
        !transmissionPower || !bandwidth || !channelFrequency)
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

    // The frame may be sent by an end device, gateway or network server.
    // Check if it sent over LoRa
    LoRaDatalinkFrame* datalinkMsg = dynamic_cast<LoRaDatalinkFrame*>(physicalMsg->getEncapsulatedPacket());
    if (!datalinkMsg)
        return BAD_FRAME;

    // Get the frame type
    *frameType = datalinkMsg->getFType();

    if (*frameType == JOIN_REQUEST) {
        LoRaJoinRequestFrame* joinRequestMsg = dynamic_cast<LoRaJoinRequestFrame*>(datalinkMsg->getEncapsulatedPacket());
        if (!joinRequestMsg)
            return BAD_FRAME;

        return 0;
    }
    else if (*frameType == JOIN_ACCEPT) {
        LoRaJoinAcceptFrame* joinAcceptMsg = dynamic_cast<LoRaJoinAcceptFrame*>(datalinkMsg->getEncapsulatedPacket());
        if (!joinAcceptMsg)
            return BAD_FRAME;

        return 0;
    }

    std::array<uint8_t, IPv4_ADDRESS_SIZE> deviceAddr;
    bool isUplinkFrame;

    if (*frameType == UPLINK_UNCONFIRMED || *frameType == UPLINK_CONFIRMED) {
        // Check if it is an uplink frame
        LoRaAppUplinkFrame* appMsgUplink = dynamic_cast<LoRaAppUplinkFrame*>(datalinkMsg->getEncapsulatedPacket());
        if (!appMsgUplink)
            return BAD_FRAME;

        isUplinkFrame = true;

        // Get the message port, counter, address and payload
        *port    = appMsgUplink->getPort();
        *counter = appMsgUplink->getCounter();
        getArrayInMessageUplink(appMsgUplink, &LoRaAppUplinkFrame::getDeviceAddress, deviceAddress, IPv4_ADDRESS_SIZE);
        getArrayInMessageUplink(appMsgUplink, &LoRaAppUplinkFrame::getPayload, payload, LORA_FRAME_SIZE_APP_PAYLOAD);

        memcpy(deviceAddr.data(), deviceAddress, IPv4_ADDRESS_SIZE);

        // The MIC can be verified only if the key used is shared with the gateway, so,
        // when the end device communicates with the network server, application server or associated gateway,
        // the session keys used are not shared with the gateway, so, it cannot verify the MIC.
        // Check if the end device is communicating with a server
        if (!(*port == MSG_PORT_MAC_CMD) && !(*port == MSG_PORT_CONNECTION)) {
            // No, check if the end device is communicating with the associated gateway
            if (*port == MSG_PORT_DATA_PROFILE ||
                *port == MSG_PORT_DATA) {
                // Yes, check the MIC of the message for data integrity & authentication
                // only if the gateway is associated with the device
                auto it = associations.find(deviceAddr);
                if (it != associations.end()) {
                    // The MIC must be verified with the association session key
                    int numKeys = 1;
                    uint8_t* keys[numKeys] = { std::get<0>(it->second).data() };

                    if(!isValidMIC(datalinkMsg, appMsgUplink, keys, numKeys))
                        return BAD_MIC;
                }
            }
            // Check if the end device and the gateway are generating a common session key
            else if (*port == MSG_PORT_GENERATE_COMMON_KEY) {
                // Use the root key
                int numKeys = 1;
                uint8_t* keys[numKeys] = { commonEndDeviceKey };

                printKey(keys[0], KEY_LORAWAN_SIZE, "keys[0]", EV);

                // Check the MIC of the message for data integrity & authentication
                if (!isValidMIC(datalinkMsg, appMsgUplink, keys, numKeys))
                    return BAD_MIC;
            }
            else {
                // The MIC can be verified with the commonEndDeviceSKeys[deviceAddr] or with the commonEndDeviceKey
                // for gateways in the radio range of the end device.
                // The MIC is verified with the cluster session keys for gateways out the radio range of the end device

                // A common session key should have been generated
                int numKeys = 1 + clusters.size();
                uint8_t* keys[numKeys] = {};

                // Check if the common session key has been generated for the end device
                auto it = commonEndDeviceSKeys.find(deviceAddr);
                if (it != commonEndDeviceSKeys.end())
                    keys[0] = it->second.data();
                else {
                    // The gateway is probably out of range of end device
                    //keys[0] = commonEndDeviceKey;

                    int i = 0;
                    for (auto it : clusters)
                        keys[i++] = (uint8_t*) it.first.data();
                    keys[numKeys-1] = commonGatewaysKey;
                }

                printKey(keys[0], KEY_LORAWAN_SIZE, "keys[0]", EV);

                // Check the MIC of the message for data integrity & authentication
                if (!isValidMIC(datalinkMsg, appMsgUplink, keys, numKeys))
                    return BAD_MIC;
            }
        }
    }
    else if (*frameType == DOWNLINK_UNCONFIRMED || *frameType == DOWNLINK_CONFIRMED) {
        // Check if it is a downlink frame
        LoRaAppDownlinkFrame* downlinkAppMsg = dynamic_cast<LoRaAppDownlinkFrame*>(datalinkMsg->getEncapsulatedPacket());
        if (!downlinkAppMsg)
            return BAD_FRAME;

        isUplinkFrame = false;

        // Get the message port, counter, address and payload
        *port    = downlinkAppMsg->getPort();
        *counter = downlinkAppMsg->getCounter();
        getArrayInMessageDownlink(downlinkAppMsg, &LoRaAppDownlinkFrame::getDeviceAddress, deviceAddress, IPv4_ADDRESS_SIZE);
        getArrayInMessageDownlink(downlinkAppMsg, &LoRaAppDownlinkFrame::getPayload, payload, LORA_FRAME_SIZE_APP_PAYLOAD);

        memcpy(deviceAddr.data(), deviceAddress, IPv4_ADDRESS_SIZE);

        // The MIC can be verified only if the key used is shared with the gateway, so,
        // when the network server, application server or associated gateway communicate with the end device,
        // the session keys used are not shared with the gateway, so, it cannot verify the MIC.
        // Check if a server or the associated gateway is communicating with the end device
        if (!(*port == MSG_PORT_MAC_CMD)                  &&
            !(*port == MSG_PORT_CONNECTION)               &&
            //!(*port == MSG_PORT_GENERATE_ASSOCIATION_KEY) &&
            !(*port == MSG_PORT_DATA_PROFILE)             &&
            !(*port == MSG_PORT_DATA)) {
            // No, the MIC can be verified with a cluster session key or with the commonGatewaysKey
            int numKeys = 1 + clusters.size();
            uint8_t* keys[numKeys];

            int i = 0;
            for (auto it : clusters)
                keys[i++] = (uint8_t*) it.first.data();
            keys[numKeys-1] = commonGatewaysKey;

            // Check the MIC of the message for data integrity & authentication
            if (!isValidMIC(datalinkMsg, downlinkAppMsg, keys, numKeys))
                return BAD_MIC;
        }
    }
    else
        return BAD_FRAME;

    // Check if the message port is valid
    //if (!isValidPort(*port, *isUplinkFrame))
    //if (!isValidPort(*port, MSG_LORA))
    if (!isValidPort(*port, isUplinkFrame ? MSG_LORA_UPLINK : MSG_LORA_DOWNLINK))
        return BAD_PORT;

    // Check if the address is in the map
    auto it = map.find(deviceAddr);
    if (it == map.end())
        // The address is not in the map
        return 0;

    // The address is in the map.
    // Check if the frame is a retransmission
    auto tuple = it->second;

    EV << "Received frame counter: " << (int) *counter << "\n";

    if (isUplinkFrame) {
        EV << "Stored uplink frame counter: " << (int) std::get<4>(tuple) << "\n";
        if (*counter <= std::get<4>(tuple))
            return BAD_COUNTER;

        // Update frame counter for uplinks
        // Set uplink & downlink frame counters
        // (downlink based on uplink to ensure sending not an "old" frame)
        EV << "Updating uplink & downlink counters...\n";
        std::get<4>(tuple) = *counter;
        std::get<5>(tuple) = *counter;
        EV << "Uplink counter: " << (int) *counter << "\n";
        EV << "Downlink counter: " << (int) *counter << "\n";
    }
    else {
        // Use < instead of <= because there is no guarantee that a frame is delivered to the destination.
        // Furthermore, it is useful to receive multiple HELLO_GATEWAY messages with the same counter
        // in order to decrease traffic
        EV << "Stored downlink frame counter: " << (int) std::get<5>(tuple) << "\n";

        //if (*counter <= std::get<5>(tuple))
        if (*counter < std::get<5>(tuple))
            return BAD_COUNTER;

        EV << "Updating downlink counters...\n";

        // Update frame counter for downlinks
        std::get<5>(tuple) = *counter;

        EV << "Downlink counter: " << (int) *counter << "\n";
    }

    // Update the entry in the map
    map[deviceAddr] = tuple;
    return 0;
}

uint8_t LoRaGateway::isValidMessageIp(cMessage* msg,
        uint8_t* srcAddress, uint8_t* destAddress, bool isTcp, uint32_t sequenceNumber, uint32_t ackNumber, uint8_t* payload) {
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
            //std::tuple<cPacket*, cMessage*, uint32_t, uint32_t, uint32_t> tuple = it->second;
            std::tuple<std::list<cPacket*>, cMessage*, uint32_t, uint32_t, uint32_t> tuple = it->second;

            //EV << "Received TCP/IP packet with seq num: " << sequenceNumber << "\n";
            //EV << "Stored TCP/IP packet sequence number IN: " << std::get<2>(tuple) << "\n";
            if (sequenceNumber < std::get<2>(tuple))
                return BAD_COUNTER;

            // Simplified version of TCP/IP control without packet ordering
        }
        // Check if no message is received by the IP address but the sequence number is not 1
        else if (sequenceNumber != 1)
            return BAD_COUNTER;
    }

    // Get encapsulated packet
    cPacket* encapsulatedPacket = ipMsg->getEncapsulatedPacket();

    // Check if the packet has an encapsulated packet
    if (encapsulatedPacket == nullptr)
        // No, get the IP packet payload
        getArrayInMessageIp(ipMsg, &IPv4Packet::getPayload, payload, LORA_FRAME_SIZE_APP_PAYLOAD);

    return 0;
}

uint8_t LoRaGateway::isValidMessageTcp(cMessage* msg, unsigned* port, uint32_t* sequenceNumber, uint32_t* ackNumber) {
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

    return 0;
}

uint8_t LoRaGateway::isValidMessageUdp(cMessage* msg, unsigned* port) {
    UDPSegment* udpMsg = dynamic_cast<UDPSegment*>(msg);
    if (!udpMsg)
        return BAD_FRAME;

    // The frame has been sent over UDP.
    // Check if the message port is valid
    *port = udpMsg->getDestPort();
    if (!isValidPort(*port, MSG_UDP))
        return BAD_PORT;

    return 0;
}

void LoRaGateway::printError(uint8_t error) {
    EV << "Error: " << (int) error << "\n";
    switch (error) {
        case BAD_ADDRESS:
            EV << "Received IP packet for a different IP address\n";
            break;
        case BAD_PORT:
            EV << "Received invalid port in the message\n";
            // Only a message can be received in a receive window
            //message_received = MSG_MAC_CMD;
            break;
        default:
            EV << "Received invalid frame\n";
    }
}

void LoRaGateway::processMessageFromTransportLayer(cMessage* msgIn) {
    // Lose the message with a certain probability
    //if (uniform(0, 1) < 2) {
    //if (uniform(0, 1) < MSG_LOSS_PROBABILITY_IP) {
    if (((rand() + (int) uniform(0,100)) % 101) < MSG_LOSS_PROBABILITY_IP) {
        EV << "Message lost!\n";

        // Make animation more informative
        bubble("Message lost!");

        // Send signal for statistic collection
        emit(signalLost, 1u);
        emit(signalLostCount, ++messagesLostCount);
        emit(signalLostIp, 1u);
        emit(signalLostIpCount, ++messagesLostIpCount);

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

    // Check if the message is a valid UDP segment and get useful info
    uint8_t error = isValidMessageUdp(msgIn, &port);
    if (error) {
        if (error != BAD_FRAME) {
            printError(error);
            delete msgIn;
            return;
        }

        //EV << "The message is not a UDP segment!\n";

        // The message is not a UDP segment.
        // Check if the encapsulated message is a valid TCP packet and get useful info
        error = isValidMessageTcp(msgIn, &port, &sequenceNumber, &ackNumber);
        if (error) {
            printError(error);
            delete msgIn;
            return;
        }

        isTcp = true;

        networkIn += MESSAGE_SIZE_TCP_HEADER;
    }
    else
        networkIn += MESSAGE_SIZE_UDP_HEADER;

    // The message is a valid TCP or UDP segment
    cPacket* traMsg = dynamic_cast<cPacket*>(msgIn);
    cPacket* ipMsg  = traMsg->getEncapsulatedPacket();

    std::array<uint8_t, IPv4_ADDRESS_SIZE> srcAddress;
    uint8_t destAddress[IPv4_ADDRESS_SIZE];
    uint8_t payloadIn[LORA_FRAME_SIZE_APP_PAYLOAD];

    // Check if the encapsulated message is a valid IP packet and get useful info
    error = isValidMessageIp(ipMsg, srcAddress.data(), destAddress, isTcp, sequenceNumber, ackNumber, payloadIn);
    if (error) {
        //EV << "Received message is not a valid IP datagram\n";
        printError(error);
        delete msgIn;
        return;
    }

    // The encapsulated message is a valid IP packet
    networkIn += MESSAGE_SIZE_IPV4_HEADER;

    // Get the gate index from which the message is arrived
    int gateIndex = msgIn->getArrivalGate()->getIndex();

    // In OMNeT++ the gateway does not know which is the gate index associated to the IP address.
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
        //std::tuple<cPacket*, cMessage*, uint32_t, uint32_t, uint32_t> tuple;
        std::tuple<std::list<cPacket*>, cMessage*, uint32_t, uint32_t, uint32_t> tuple;
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
            // Init the entry (skipping three-ways handshake)
            //std::get<0>(tuple) = nullptr;
            std::get<0>(tuple) = std::list<cPacket*> {};
            std::get<1>(tuple) = nullptr;
            std::get<2>(tuple) = sequenceNumber;
            std::get<3>(tuple) = 1;
            std::get<4>(tuple) = sequenceNumber+1;

            mapTcp[srcAddress] = tuple;
        }

        // Check if the message has no payload
        if (!memcmp(msgIn->getName(), "tcpACKMsg", strlen("tcpACKMsg"))) {
            // Send signal for statistic collection
            emit(signalReceived, 1u);
            emit(signalReceivedIp, 1u);
            emit(signalReceivedCount, ++messagesReceivedCount);
            emit(signalReceivedIpCount, ++messagesReceivedIpCount);

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

        //send(msgOut, IP_GATE_OUT, gateIndex);
        //sendSecurely(msgOut, IP_GATE_OUT, gateIndex);
        sendSecurely(this, eventTimeoutChannelTransmissions, msgOut, IP_GATE_OUT, gateIndex);

        // Do not schedule retransmission as it is an ACK

        // Send signal for statistic collection
        emit(signalSent, 1u);
        emit(signalSentIp, 1u);
        emit(signalSentCount, ++messagesSentCount);
        emit(signalSentIpCount, ++messagesSentIpCount);

        networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_TCP_HEADER;
    }

    cPacket* dlMsg = ipMsg->decapsulate();

    // Send signal for statistic collection
    emit(signalReceived, 1u);
    emit(signalReceivedIp, 1u);
    emit(signalReceivedCount, ++messagesReceivedCount);
    emit(signalReceivedIpCount, ++messagesReceivedIpCount);

    // Check if the packet has an encapsulated message
    if (!dlMsg) {
        // The IP packet has not an encapsulated message.
        // Process IP packet
        if (port == MSG_PORT_NEARBY_GATEWAYS) {
            // G1 has {G2, G3},
            // G2 has {G1, G3},
            // G3 has {G1, G2, G4},
            // G4 has {G3}
            // =>
            // cluster_1: {G1, G2, G3}
            // cluster_2: {G3, G4}

            // Check if the sender is in the collected set (i.e. each other are in the radio range)
            if (neighborGatewayAddresses.find(srcAddress) == neighborGatewayAddresses.end()) {
                delete msgIn;
                return;
            }

            std::array<uint8_t, IPv4_ADDRESS_SIZE> endDeviceAddress {0, 0, 0, 0};
            auto it = map.find(endDeviceAddress);
            auto tuple = it->second;

            // Update the number of received NEARBY_GATEWAYS messages
            std::get<4>(tuple) += 1;
            map[endDeviceAddress] = tuple;

            // Calculate intersection of received neighbor set and collected set
            // (including the sender and the destination)
            int receivedAddresses = payloadIn[0];
            std::set<std::array<uint8_t, IPv4_ADDRESS_SIZE>> setIntersection {srcAddress};

            for (int i=1; i < receivedAddresses*IPv4_ADDRESS_SIZE; i+=IPv4_ADDRESS_SIZE) {
                std::array<uint8_t, IPv4_ADDRESS_SIZE> neighborAddress;
                memcpy(neighborAddress.data(), &payloadIn[i], IPv4_ADDRESS_SIZE);

                // Check if the received address is equal to the current gateway one or has been collected,
                // before inserting it in the intersection
                if (!memcmp(address, neighborAddress.data(), IPv4_ADDRESS_SIZE) ||
                        neighborGatewayAddresses.find(neighborAddress) != neighborGatewayAddresses.end())
                    setIntersection.insert(neighborAddress);
            }

            // G1 has intersection {G2, G1, G3} with G2 and {G3, G1, G2} with G3,
            // G2 has intersection {G1, G2, G3} with G1 and {G3, G1, G2} with G3,
            // G3 has intersection {G1, G2, G3} with G1 and {G2, G1, G3} with G2 and {G4, G3} with G4,
            // G4 has intersection {G3, G4} with G3

            EV << "Set intersection: {\n";
            for (auto address : setIntersection)
                EV << (unsigned) address[0] << "."
                   << (unsigned) address[1] << "."
                   << (unsigned) address[2] << "."
                   << (unsigned) address[3] << "\n";
            EV << "}\n";

            // Check if the intersection is already stored as a cluster
            for (auto const& it : clusters) {
                auto setStored = it.second;
                if (setIntersection == setStored)
                    return;
            }

            // Temporary store the intersection as a cluster
            int size = clusters.size();
            char key_[32];
            snprintf(key_, 32, "%d",  size);
            std::array<uint8_t, KEY_LORAWAN_SIZE> key;
            memcpy(key.data(), key_, key.size());
            clusters[key] = setIntersection;

            // Check if all NEARBY_GATEWAYS messages have been received
            if (std::get<4>(tuple) == neighborGatewayAddresses.size()) {
                // Print clusters
                EV << "Clusters: {\n";

                for (auto it : clusters) {
                    auto key     = it.first.data();
                    auto cluster = it.second;

                    printKey((uint8_t*) key, sizeof(key), "key", EV);

                    EV << "{ ";
                    for (auto address : cluster)
                        EV << (unsigned) address[0] << "."
                           << (unsigned) address[1] << "."
                           << (unsigned) address[2] << "."
                           << (unsigned) address[3];
                    EV << " },\n";
                }

                EV << "}\n";

                // TODO: Key agreement over IP for each cluster
                EV << "Start a key agreement per cluster with nearby peers\n";

                // Create a fake cluster with all neighbors only for the simulation to skip a real key agreement
                // phase per cluster.
                // This is needed otherwise MICs will not work because different keys used
                // to calculate it in the same cluster.
                // e.g.: a bridge gw between 2 clusters and another including the other bridge (chain)
                // GW1 - GW2 - GW3 - G4
                // GW2 generates key with index 0 with GW1 and index 1 with GW3
                // GW3 generates key with index 0 con GW1 and index 1 with GW4
                std::array<uint8_t, KEY_LORAWAN_SIZE> key {""};
                clusters[key] = neighborGatewayAddresses;

                // TODO: Key agreement with network server
                EV << "Start key agreement with network server\n";

                // Once ended it is available for ACK the GENERATE_COMMON_KEY message of the end device
                stage = STAGE_ACTIVATED;
            }
        }
        //if (port == MSG_PORT_CONNECTION) {
        else if (port == MSG_PORT_CONNECTION_GATEWAY) {
            EV << "Received response for MSG_PORT_CONNECTION_GATEWAY message\n";

            // Get end device address
            std::array<uint8_t, IPv4_ADDRESS_SIZE> deviceAddress;
            memcpy(deviceAddress.data(), payloadIn, IPv4_ADDRESS_SIZE);

            // Check if a message from the end device address has already been received
            auto it = map.find(deviceAddress);
            if (it == map.end()) {
                delete msgIn;
                return;
            }

            // The end device is in the map, get the entry
            auto tuple = it->second;

            // Get message requestId
            uint8_t requestId = payloadIn[4];

            // Check if the request ID in the message matches the ID stored in the map
            if (requestId != std::get<2>(tuple) || associations.find(deviceAddress) == associations.end()) {
                delete msgIn;
                return;
            }

            // The request ID matches and the end device is associating with the gateway.
            // Get message response
            uint8_t response  = payloadIn[5];
            if (response == ACK) {
                EV << "Association with end device confirmed\n";
            }
            else {
                // Countermeasures?
                EV << "Association with end device not confirmed\n";
            }
        }
        /*else if (port == MSG_PORT_SHARE_DATA_PROFILE) {
            EV << "Received response for SHARE_DATA_PROFILE message\n";

            // Get end device address
            std::array<uint8_t, IPv4_ADDRESS_SIZE> deviceAddress;
            memcpy(deviceAddress.data(), payloadIn, IPv4_ADDRESS_SIZE);

            // Check if a message from the end device address has already been received
            auto it = map.find(deviceAddress);
            if (it == map.end() || associations.find(deviceAddress) == associations.end()) {
                delete msgIn;
                return;
            }

            // The end device is in the map, get the entry
            auto tuple = it->second;

            // The data profile has been received, do not send again
            std::get<7>(tuple) = true;

            // Update the entry
            map[deviceAddress] = tuple;
        }*/

        delete msgIn;
        return;
    }

    // The IP packet has an encapsulated message
    delete msgIn;
    processMessageFromLoRaLayer(dlMsg, srcAddress);
}

void LoRaGateway::processMessageFromLoRaLayer(
        cMessage* msgIn, std::array<uint8_t, IPv4_ADDRESS_SIZE> srcAddress/*={0,0,0,0}*/) {
    std::array<uint8_t, IPv4_ADDRESS_SIZE> endDeviceAddress;
    uint8_t frameType;
    uint16_t counter = 0;
    unsigned port;
    uint8_t payloadIn[LORA_FRAME_SIZE_APP_PAYLOAD] = {};
    uint8_t spreadingFactor;
    uint8_t transmissionPower;
    float bandwidth;
    float channelFrequency;

    // Check if the message (also encapsulated) is a valid LoRa frame and get useful info
    uint8_t error = isValidLoRaFrame(
            msgIn, &frameType, endDeviceAddress.data(), &counter, &port, payloadIn,
            &spreadingFactor, &transmissionPower, &bandwidth, &channelFrequency);
    if (error) {
        //EV << "The message is not a valid LoRa frame!\n";
        printError(error);
        delete msgIn;
        return;
    }

    // Check if the message is received via LoRa
    if (srcAddress == std::array<uint8_t, IPv4_ADDRESS_SIZE> {0,0,0,0}) {
        // Send signal for statistic collection
        emit(signalReceived, 1u);
        emit(signalReceivedLoRa, 1u);
        emit(signalReceivedCount, ++messagesReceivedCount);
        emit(signalReceivedLoRaCount, ++messagesReceivedLoRaCount);

        // Get message RSSI
        LoRaEndDevice* device = dynamic_cast<LoRaEndDevice*>(msgIn->getSenderModule());
        if (device) {
            // Collect RSSI about the end device
            int rssi = calculateRSSI(this, device->getPosX(), device->getPosY(), posX, posY, EV);
            EV << "Message RSSI: " << rssi << "\n";

            // Send signal for statistic collection
            emit(signalRSSI, rssi);
        }
    }

    if (frameType == JOIN_REQUEST) {
        EV << "Forwarding JOIN_REQUEST message over IP...\n";
        //forwardMessageIp(msgIn, "forwardJoinRequestMsg", networkServerAddress, MSG_PORT_FORWARD_JOIN);
        forwardMessageIp(msgIn, "forwardJoinRequestMsg", networkServerAddress, MSG_PORT_FORWARD_OVER_IP, false);

        // Send signal for statistic collection
        emit(signalSent, 1u);
        emit(signalSentIp, 1u);
        emit(signalSentCount, ++messagesSentCount);
        emit(signalSentIpCount, ++messagesSentIpCount);

        networkIn  += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC + LORA_FRAME_SIZE_JOIN_REQUEST;
        networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_UDP_HEADER;
        networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC + LORA_FRAME_SIZE_JOIN_REQUEST;


        // Define a fake end device address (never assigned to a real device) to include in the LoRa frame
        // for gateway activation
        std::array<uint8_t, IPv4_ADDRESS_SIZE> voidAddress {0, 0, 0, 0};

        // Check if a gateway activation is already running or there exist valid cluster session keys
        if (map.find(voidAddress) != map.end() || !clusters.empty())
            return;

        EV << "Start gateway activation\n";

        // TODO: Synchronize clock with network server

        // Send HELLO_GATEWAY message
        sendMessageHelloGateway(voidAddress, counter, spreadingFactor, transmissionPower, bandwidth, channelFrequency);

        // Send signal for statistic collection
        emit(signalSent, 1u);
        emit(signalSentLoRa, 1u);
        emit(signalSentCount, ++messagesSentCount);
        emit(signalSentLoRaCount, ++messagesSentLoRaCount);

        networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC +
                      LORA_FRAME_SIZE_APP_HEADER + IPv4_ADDRESS_SIZE;

        return;
    }
    else if (frameType == JOIN_ACCEPT) {
        // Check if the message is a forward by a peer
        if (memcmp(srcAddress.data(), networkServerAddress.data(), IPv4_ADDRESS_SIZE)) {
            // Yes, ignore it to avoid cycles and
            // because only Join Accept directly received from the network server must be forwarded
            EV << "Ignore the message as it is not directly received by the network server but a peer\n";
            delete msgIn;
            return;
        }

        EV << "Forwarding JOIN_ACCEPT message over LoRa...\n";
        //sendBroadcast(this, msgIn, LORA_GATE_OUT);
        sendBroadcastSecurely(this, eventTimeoutChannelTransmissions, dynamic_cast<cPacket*> (msgIn), LORA_GATE_OUT);

        // Send signal for statistic collection
        emit(signalSent, 1u);
        emit(signalSentLoRa, 1u);
        emit(signalSentCount, ++messagesSentCount);
        emit(signalSentLoRaCount, ++messagesSentLoRaCount);

        networkIn  += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC + LORA_FRAME_SIZE_JOIN_ACCEPT;
        networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC + LORA_FRAME_SIZE_JOIN_ACCEPT;

        return;
    }

    networkIn += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC +
                 LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;

    // The message is a valid LoRa frame
    EV << "Received message for the gateway with port: " << (int) port << "\n";
    EV << "Received message for the gateway with end device address: "
        << (int) endDeviceAddress[0] << "."
        << (int) endDeviceAddress[1] << "."
        << (int) endDeviceAddress[2] << "."
        << (int) endDeviceAddress[3]
        << "\n";

    // Check if a message from this address has already been received
    //std::tuple<cPacket*, cMessage*, uint8_t, uint8_t, uint16_t, uint16_t, std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::array<uint8_t, IPv4_ADDRESS_SIZE>> tuple;
    std::tuple<cPacket*, cMessage*, uint8_t, uint8_t, uint16_t, uint16_t, std::array<uint8_t, IPv4_ADDRESS_SIZE>, bool, std::array<uint8_t, IPv4_ADDRESS_SIZE>> tuple;
    auto it = map.find(endDeviceAddress);


    //*************************************
    // TODO: Merge STATS response to HELLO and FORWARD message
    //*************************************


    if (port == MSG_PORT_HELLO_GATEWAY) {
        EV << "Received MSG_PORT_HELLO_GATEWAY message\n";

        // Get the timestamp
        uint8_t timestampBytes[4];
        //uint8_t timestampBytes[8];
        memcpy(timestampBytes, payloadIn, sizeof(timestampBytes));

        // Convert from array of bytes to unsigned
        unsigned timestamp = bytesToUint(timestampBytes);

        //simtime_t time = simTime();
        simtime_t time = simTime() * 1000; // convert to ms
        //std::time_t time = std::time(nullptr);

        EV << "Time: " << time << "\n";
        EV << "Time expiration: " << timestamp << "\n";

        // Verify timestamp validity
        if (time >= timestamp) {
            EV << "Message is expired!\n";
            delete msgIn;
            return;

            // TODO: Synchronize clock with network server only once every X days (e.g. week)
        }

        // Get the peer address
        std::array<uint8_t, IPv4_ADDRESS_SIZE> neighborAddress;
        memcpy(neighborAddress.data(), &payloadIn[4], IPv4_ADDRESS_SIZE);

        EV << "Neighbor gateway IP address: "
           << (int) neighborAddress[0] << "."
           << (int) neighborAddress[1] << "."
           << (int) neighborAddress[2] << "."
           << (int) neighborAddress[3]
           << "\n";

        // Insert the received IP address to the set of neighbors
        neighborGatewayAddresses.insert(neighborAddress);

        // Go to activation stage only if at least an HELLO_GATEWAY message is received
        // as makes no sense to activate a gateway if it participates alone to a single cluster
        stage = STAGE_ACTIVATION;

        storageOccupied += IPv4_ADDRESS_SIZE;

        // Define a fake end device address (never assigned to a real device) to include in the LoRa frame
        // for gateway activation
        std::array<uint8_t, IPv4_ADDRESS_SIZE> voidAddress {0, 0, 0, 0};

        // Check if a gateway activation is already running or there exist valid cluster session keys
        if (map.find(voidAddress) != map.end() || !clusters.empty())
            return;

        // Send HELLO_GATEWAY message as it belongs to the same graph connected component where an end device activates
        // but no Join Request message has been received
        sendMessageHelloGateway(voidAddress, counter, spreadingFactor, transmissionPower, bandwidth, channelFrequency);

        // Send signal for statistic collection
        emit(signalSent, 1u);
        emit(signalSentLoRa, 1u);
        emit(signalSentCount, ++messagesSentCount);
        emit(signalSentLoRaCount, ++messagesSentLoRaCount);

        networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC +
                      LORA_FRAME_SIZE_APP_HEADER + IPv4_ADDRESS_SIZE;
    }
    else if (port == MSG_PORT_GENERATE_COMMON_KEY) {
        EV << "Received GENERATE_COMMON_KEY message\n";

        EV << "Decrypting GENERATE_COMMON_KEY message...\n";

        // Remember the key used for decryption
        // (here the message is never forwarded so the key to use is just one)
        std::array<uint8_t, KEY_LORAWAN_SIZE> decryptKey;
        decryptUplink(payloadIn, payloadIn, decryptKey.data(), endDeviceAddress, srcAddress);

        // Check if the common session key for the end device has already been generated
        // (if MIC is not implemented, multiple end devices accept the same Join Accept and
        // therefore obtain the same address)
        if (commonEndDeviceSKeys.find(endDeviceAddress) == commonEndDeviceSKeys.end()) {
            // This is the first message received from the end device.
            // Get the nonces & net ID
            uint8_t nonce1_[4] = {
                payloadIn[0], payloadIn[1], payloadIn[2], payloadIn[3]
            };
            uint8_t nonce2_[4] = {
                payloadIn[4], payloadIn[5], payloadIn[6], payloadIn[7]
            };
            unsigned nonce1 = bytesToUint(nonce1_);
            unsigned nonce2 = bytesToUint(nonce2_);
            memcpy(networkId, &payloadIn[8], sizeof(networkId));

            EV << "nonce1: " << nonce1 << "\n";
            EV << "nonce2: " << nonce2 << "\n";

            // Derive and store the session key exclusively shared with the end device and gateways in its radio range
            std::array<uint8_t, KEY_LORAWAN_SIZE> commonSKey;
            memcpy(commonSKey.data(),
                   generateKey(commonEndDeviceKey, nonce1, nonce2, networkId),
                   commonSKey.size());
            commonEndDeviceSKeys[endDeviceAddress] = commonSKey;

            printKey(commonSKey.data(), KEY_LORAWAN_SIZE, "commonSKey", EV);

            // Init the entry in the map of messages received from an end device
            std::get<3>(tuple) = 0; // Use level number as counter for received GENERATE_COMMON_KEY
            std::get<7>(tuple) = false;

            // Increment occupied storage
            storageOccupied += IPv4_ADDRESS_SIZE + sizeof(commonEndDeviceKey);
            storageOccupied += IPv4_ADDRESS_SIZE + sizeof(tuple);
        }
        else if (it != map.end())
            // The end device is in the map, get the entry
            tuple = it->second;


        // Update number of received GENERATE_COMMON_KEY messages and frame counters
        std::get<3>(tuple) += 1;
        std::get<4>(tuple) = counter;
        std::get<5>(tuple) = counter;
        map[endDeviceAddress] = tuple;

        EV << "std::get<0>(tuple): " << std::get<0>(tuple) << "\n";
        EV << "std::get<1>(tuple): " << std::get<1>(tuple) << "\n";
        EV << "std::get<2>(tuple): " << (unsigned) std::get<2>(tuple) << "\n";
        EV << "std::get<3>(tuple): " << (unsigned) std::get<3>(tuple) << "\n";
        EV << "std::get<4>(tuple): " << (unsigned) std::get<4>(tuple) << "\n";
        EV << "std::get<5>(tuple): " << (unsigned) std::get<5>(tuple) << "\n";
        //EV << "std::get<6>(tuple): " << std::get<6>(tuple) << "\n";

        // Check if the gateway activation is finished
        if (stage == STAGE_ACTIVATION) {
            // Send NACK to warn the end device to wait
            EV << "Sending NACK for GENERATE_COMMON_KEY message...\n";
            sendMessageCommonKeyAck(
                    endDeviceAddress.data(), counter, false,
                    spreadingFactor, transmissionPower, bandwidth, channelFrequency);

            // Send signal for statistic collection
            emit(signalSent, 1u);
            emit(signalSentLoRa, 1u);
            emit(signalSentCount, ++messagesSentCount);
            emit(signalSentLoRaCount, ++messagesSentLoRaCount);

            networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC + LORA_FRAME_SIZE_APP_HEADER + 1;
        }
        else if (stage == STAGE_ACTIVATED and std::get<3>(tuple) >= RETRANSMISSIONS) {
            // Send ACK only after n retransmissions of end device message to permit receiving NACK
            // in LoRaWAN class A
            EV << "Sending ACK for GENERATE_COMMON_KEY message...\n";
            sendMessageCommonKeyAck(
                    endDeviceAddress.data(), counter, true,
                    spreadingFactor, transmissionPower, bandwidth, channelFrequency);

            // Send signal for statistic collection
            emit(signalSent, 1u);
            emit(signalSentLoRa, 1u);
            emit(signalSentCount, ++messagesSentCount);
            emit(signalSentLoRaCount, ++messagesSentLoRaCount);

            networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC + LORA_FRAME_SIZE_APP_HEADER + 1;
        }

        // Do not set the last message sent of the tuple as this message does not need to be retransmitted

        // Propagate the downlink counter to the network server to enable it to send MAC commands
        // (shared downlink counter but also uplink counter as it is the same in this moment)
        EV << "Propagating downlink counter from gateway to network server...\n";
        sendMessageShareCounter(endDeviceAddress.data(), counter);

        //=======================
        // Send signal for statistic collection
        /*emit(signalSent, 2u);
        emit(signalSentLoRa, 1u);
        emit(signalSentIp, 1u);

        storageOccupied += IPv4_ADDRESS_SIZE + sizeof(commonEndDeviceKey);
        networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_APP_HEADER;
        networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_TCP_HEADER;*/
        //=======================

        //=======================
        // Send signal for statistic collection
        emit(signalSent, 1u);
        emit(signalSentIp, 1u);
        emit(signalSentCount, ++messagesSentCount);
        emit(signalSentIpCount, ++messagesSentIpCount);

        networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_TCP_HEADER;
        //=======================

        delete msgIn;
    }
    if (port == MSG_PORT_HELLO) {
        EV << "Received HELLO message\n";

        // Get message requestId & sequence number
        uint8_t requestId   = payloadIn[0];
        uint8_t levelNumber = payloadIn[1];


        // Check if a GENERATE_COMMON_KEY message from the end device address has already been received
        if (it == map.end()) {
            // No, but a gateway cannot receive an HELLO message without having received a GENERATE_COMMON_KEY
            delete msgIn;
            return;
        }

        // The end device is in the map, get the entry
        tuple = it->second;

        // Check if the common session key with the end device has been created (should be)
        auto itCommonSKeys = commonEndDeviceSKeys.find(endDeviceAddress);
        if (itCommonSKeys == commonEndDeviceSKeys.end()) {
            // No, but a gateway cannot receive an HELLO message without having received a GENERATE_COMMON_KEY
            delete msgIn;
            return;
        }

        // Get the common session key shared with the end device
        auto commonSKey = itCommonSKeys->second;

        EV << "Decrypting HELLO message...\n";

        // Remember the key used for decryption
        std::array<uint8_t, KEY_LORAWAN_SIZE> decryptKey;
        decryptUplink(payloadIn, payloadIn, decryptKey.data(), endDeviceAddress, srcAddress);

        // Check if a STATS message was sent and the request ID in the message matches the ID stored in the map
        if (std::get<0>(tuple) != nullptr && requestId == std::get<2>(tuple)) {
           // Get the associated timer to the previously STATS sent
           cMessage* timer = std::get<1>(tuple);

           // Check if the ACK for the STATS message is already arrived
           if (timer == nullptr) {
               EV << "ACK already received for STATS message, ignore the message\n";
               delete msgIn;
               return;
           }

           // Iterate message list for ACKs
           int i;
           for (i=2; i<11-IPv4_ADDRESS_SIZE; i+=IPv4_ADDRESS_SIZE) {
               uint8_t gatewayIpAddress[IPv4_ADDRESS_SIZE] = {};
               memcpy(gatewayIpAddress, &payloadIn[i], IPv4_ADDRESS_SIZE);

               // Check if the IP address acknowledged matches the gateway one
               if (!memcmp(address, gatewayIpAddress, IPv4_ADDRESS_SIZE)) {
                   // ACK received, cancel timeout event and remove the pointer from the tuple
                   EV << "ACK received for STATS message, canceling timer for resending the frame...\n";
                   eventTimeoutRetransmissions.erase(timer);

                   delete std::get<0>(tuple);
                   cancelAndDelete(timer);

                   // Do not set to null otherwise a new STATS message can be sent although no necessary.
                   // It is replaced when a PAIRING_REQUEST arrives
                   //std::get<0>(tuple) = nullptr;
                   std::get<1>(tuple) = nullptr;
                   map[endDeviceAddress] = tuple;

                   // Decrease occupied storage
                   storageOccupied -= sizeof(cMessage*) + IPv4_ADDRESS_SIZE;

                   delete msgIn;
                   return;
               }
           }

           // Check if ellipsis at the end of payload
           /*if ('.' == payloadIn[i] && '.' == payloadIn[i+1]) {
               // Reset timeout
               EV << "No ACK received for STATS message but found ellipsis, resetting timer\n";
               cancelEvent(std::get<1>(tuple));
               scheduleAt(simTime()+timeout, std::get<1>(tuple));

               return;
           }*/

           EV << "Canceling timer for resending the frame...\n";

           // The gateway has sent a STATS but no ACK has been received.
           // Delete the timeout to create a new one
           eventTimeoutRetransmissions.erase(timer);

           delete std::get<0>(tuple);
           cancelAndDelete(timer);

           storageOccupied -= sizeof(cMessage*) + IPv4_ADDRESS_SIZE;

           // RIUTILIZZO CODICE: INVECE DI AGGIORNARE VECCHI MESSAGGI, NE CREO NUOVI
           // QUINDI TIMER INUTILE? NO, PERCHÈ SE FRAME HELLO PERSA, RIMANDA IL VECCHIO MSG
       }

        // The request ID does not match (i.e. new algorithm run) or
        // no ACK has been received for the previous STATS message.
        // Build payload
        uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {
            requestId,
            getCpuLoad(),
            getGpuLoad(),
            getRamLoad(),
            getStorageLoad(),
            getNetworkIn(),
            getNetworkOut(),
            address[0],
            address[1],
            address[2],
            address[3]
        };

        // Create the STATS message and the corresponding timeout
        cPacket* msgOut = createMessageDownlink(
                "statsMsg", true,
                endDeviceAddress.data(), IPv4_ADDRESS_SIZE, std::get<5>(tuple), MSG_PORT_STATS,
                payload, LORA_FRAME_SIZE_APP_PAYLOAD,
                //commonEndDeviceKey,
                commonSKey.data(),
                spreadingFactor, transmissionPower, bandwidth, channelFrequency);
        cMessage* eventTimeout = new cMessage("timeoutGW");

        // Store the entry in the table (include the timeout reference to stop retransmissions)
        std::get<0>(tuple) = msgOut;
        std::get<1>(tuple) = eventTimeout;
        std::get<2>(tuple) = requestId;
        std::get<3>(tuple) = levelNumber; // 0 because HELLO

        // Set uplink & downlink frame counters
        // (downlink based on uplink to ensure sending not an "old" frame)
        EV << "Updating downlink & uplink counters...\n";
        std::get<4>(tuple) = counter;
        std::get<5>(tuple) = counter;
        EV << "Uplink counter: " << (int) counter << "\n";
        EV << "Downlink counter: " << (int) counter << "\n";
        map[endDeviceAddress] = tuple;

        // Add entry in the inverted index
        eventTimeoutRetransmissions[eventTimeout] = endDeviceAddress;


        //===================
        // Calculate response airtime to calculate the delay of the response
        EV << "Calculate Time On Air\n";

        // Calculate preamble and frame airtime
        auto tuple = calculateTimeOnAir(msgOut);

        // Convert frame time on air from ms to s
        double airtimeFrame = std::get<1>(tuple) / 1000;

        // Check if computation succeeded
        if (airtimeFrame < 0)
            return;

        // Calculate arrival time
        /*simtime_t simtime = simTime();
        simtime_t arrivalFrame = simtime + airtimeFrame;

        EV << "Simulation time: " << simtime << "\n";
        EV << "Expected arrival frame time: " << arrivalFrame << "\n";*/

        EV << "Sending STATS message...\n";

        // Calculate the delay (with tolerance) to send the message in the receive window of the end device
        double delay = RX_DELAY_1 - airtimeFrame + 0.1;
        if (delay > 0)
            sendDelayedBroadcastSecurely(this, eventTimeoutChannelTransmissions, msgOut->dup(), delay, LORA_GATE_OUT);
        else
            sendBroadcastSecurely(this, eventTimeoutChannelTransmissions, msgOut->dup(), LORA_GATE_OUT);

        // For a retransmission attempt, await this roughly identical moment in the future (TX_DELAY) and apply the delay
        scheduleAt(simTime() + TX_DELAY + delay, eventTimeout);
        //===================

        //===================
        /*EV << "Sending STATS message...\n";
        // Send the message in the receive window of the end device
        //sendDelayedBroadcast(this, msgOut, RX_DELAY_1 + 0.1, LORA_GATE_OUT);
        //sendDelayedBroadcastSecurely(this, eventTimeoutChannelTransmissions, msgOut, RX_DELAY_1 + 0.1, LORA_GATE_OUT);
        sendDelayedBroadcastSecurely(this, eventTimeoutChannelTransmissions, msgOut->dup(), RX_DELAY_1 + 0.1, LORA_GATE_OUT);
        scheduleAt(simTime() + timeoutLoRa, eventTimeout);*/
        //===================

        // Propagate the downlink counter to the network server to enable it to send MAC commands
        // (shared downlink counter but also uplink counter as it is the same in this moment)
        EV << "Propagating downlink counter from gateway to network server...\n";
        sendMessageShareCounter(endDeviceAddress.data(), counter);

        // Send signal for statistic collection
        emit(signalSent, 2u);
        emit(signalSentLoRa, 1u);
        emit(signalSentIp, 1u);

        messagesSentCount += 2;
        emit(signalSentCount, messagesSentCount);
        emit(signalSentLoRaCount, ++messagesSentLoRaCount);
        emit(signalSentIpCount, ++messagesSentIpCount);

        //EV << "sizeof(tuple): " << sizeof(tuple) << "\n";
        //storageOccupied += IPv4_ADDRESS_SIZE + sizeof(tuple);
        storageOccupied += sizeof(cMessage*) + IPv4_ADDRESS_SIZE;
        networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC +
                      LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;
        networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_TCP_HEADER;

        delete msgIn;
    }
    else if (port == MSG_PORT_FORWARD) {
        EV << "Received FORWARD message\n";

        EV << "Decrypting FORWARD message...\n";

        // Remember the key used for decryption
        std::array<uint8_t, KEY_LORAWAN_SIZE> decryptKey;
        decryptUplink(payloadIn, payloadIn, decryptKey.data(), endDeviceAddress, srcAddress);

        // Get message requestId & sequence number
        uint8_t requestId   = payloadIn[0];
        uint8_t levelNumber = payloadIn[1];

        // Check if a HELLO/FORWARD message from the end device address has already been received
        if (it != map.end()) {
            // The end device is in the map, get the entry
            tuple = it->second;

            // Check if the request ID in the message matches the ID stored in the map
            if (requestId == std::get<2>(tuple)) {
                // Get the associated timer to the previously STATS sent
                cMessage* timer = std::get<1>(tuple);

                // Check if a STATS message has been sent in the current round and
                // the gateway is awaiting the ACK
                if (levelNumber == std::get<3>(tuple)) {
                    // Check if the ACK for the STATS message is already arrived
                    if (timer == nullptr) {
                        EV << "ACK already received for STATS message, ignore the message\n";
                        delete msgIn;
                        return;
                    }

                    // Iterate message list for ACKs
                    for (int i=2; i<11-IPv4_ADDRESS_SIZE; i+=IPv4_ADDRESS_SIZE) {
                        uint8_t gatewayIpAddress[IPv4_ADDRESS_SIZE] = {};
                        memcpy(gatewayIpAddress, &payloadIn[i], IPv4_ADDRESS_SIZE);

                        // Check if the IP address acknowledged matches the gateway one
                        if (!memcmp(address, gatewayIpAddress, IPv4_ADDRESS_SIZE)) {
                            // ACK received, cancel timeout event and remove the pointer from the tuple
                            EV << "ACK received for STATS message, canceling timer for resending the frame...\n";
                            eventTimeoutRetransmissions.erase(timer);

                            delete std::get<0>(tuple);
                            cancelAndDelete(timer);

                            std::get<0>(tuple) = nullptr;
                            std::get<1>(tuple) = nullptr;
                            map[endDeviceAddress] = tuple;

                            storageOccupied -= sizeof(cMessage*) + IPv4_ADDRESS_SIZE;

                            delete msgIn;
                            return;
                        }
                    }

                    // The gateway has sent a STATS but no ACK has been received.
                    // Delete the timeout to create a new one
                    eventTimeoutRetransmissions.erase(timer);

                    delete std::get<0>(tuple);
                    cancelAndDelete(timer);

                    // Decrease occupied storage
                    storageOccupied -= sizeof(cMessage*) + IPv4_ADDRESS_SIZE;
                    storageOccupied -= IPv4_ADDRESS_SIZE + sizeof(tuple);

                    // Reuse code: instead of updating existing message, create new
                }
                // Else check if the message has to be forwarded to neighbors (i.e. next level)
                else if (levelNumber > std::get<3>(tuple)) {
                    // The level number is greater than the stored one,
                    // update the entry in the table to level - 1
                    // to avoid forwarding old frames and to continue forwarding in the current round
                    std::get<3>(tuple) = levelNumber - 1;
                    map[endDeviceAddress] = tuple;

                    // Check if the ACK for the STATS message is arrived in the previous level
                    if (timer != nullptr) {
                        // No, but the message is lost forever as the end device is accessing the next level.
                        // Cancel timeout event and remove the pointer from the tuple
                        EV << "No ACK received for STATS message but FORWARD,\ncanceling timer for resending the frame...\n";
                        eventTimeoutRetransmissions.erase(timer);

                        delete std::get<0>(tuple);
                        cancelAndDelete(timer);

                        std::get<0>(tuple) = nullptr;
                        std::get<1>(tuple) = nullptr;
                        map[endDeviceAddress] = tuple;

                        storageOccupied -= sizeof(cMessage*) + IPv4_ADDRESS_SIZE;
                    }

                    EV << "Forwarding FORWARD message to neighbors...\n";

                    //==========================================
                    // Forward the message to nearby gateways over LoRa
                    // and do not schedule retransmission because the end device handles it
                    /*sendBroadcast(this, msgIn, LORA_GATE_OUT);

                    // Send signal for statistic collection
                    emit(signalSent, 1u);
                    emit(signalSentLoRa, 1u);

                    networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;*/
                    //==========================================

                    //==========================================
                    // Forward the message to nearby gateways over IP
                    forwardMessageToNeighbors(msgIn, decryptKey.data());

                    // Send signal for statistic collection
                    emit(signalSent, 1u);
                    emit(signalSentIp, 1u);
                    emit(signalSentCount, ++messagesSentCount);
                    emit(signalSentIpCount, ++messagesSentIpCount);

                    networkOut += (LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC +
                                   LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD) * neighborGatewayAddresses.size();
                    networkOut += (MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_UDP_HEADER) * neighborGatewayAddresses.size();
                    //==========================================

                    return;
                }
                else {
                    // Ignore invalid level numbers
                    delete msgIn;
                    return;
                }
            }
        }

        // The end device is not in the map or
        // there is an old value of the request ID (i.e. new algorithm run) or
        // no ACK has been received for the previous STATS message.
        // Build payload
        uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {
            requestId,
            getCpuLoad(),
            getGpuLoad(),
            getRamLoad(),
            getStorageLoad(),
            getNetworkIn(),
            getNetworkOut(),
            address[0],
            address[1],
            address[2],
            address[3]
        };

        // Create the STATS message and the corresponding timeout
        cPacket* msgOut = createMessageDownlink(
                "statsMsg", true,
                endDeviceAddress.data(), IPv4_ADDRESS_SIZE, std::get<5>(tuple), MSG_PORT_STATS,
                payload, LORA_FRAME_SIZE_APP_PAYLOAD,
                //commonEndDeviceKey,
                // Reuse the same cluster session key used to decrypt incoming message
                decryptKey.data(),
                spreadingFactor, transmissionPower, bandwidth, channelFrequency);
        cMessage* eventTimeout = new cMessage("timeoutGW");

        // Store the entry in the table (include the timeout reference to stop retransmissions)
        std::get<0>(tuple) = msgOut;
        std::get<1>(tuple) = eventTimeout;
        std::get<2>(tuple) = requestId;
        std::get<3>(tuple) = levelNumber;
        std::get<8>(tuple) = srcAddress;
        map[endDeviceAddress] = tuple;

        // Add entry in the inverted index
        eventTimeoutRetransmissions[eventTimeout] = endDeviceAddress;


        //EV << "Sending STATS message...\n";

        //====================
        // Send the message in the receive window of the end device
        //sendDelayedBroadcast(this, msgOut, RX_DELAY_1 + 0.1, LORA_GATE_OUT);
        //====================

        //====================
        // Send the message in the receive window of the end device through the sender (peer)
        //forwardMessageIp(msgOut, "statsMsg",
                //selectedGatewayAddress, MSG_PORT_FORWARD);
        //        srcAddress, MSG_PORT_FORWARD_OVER_IP, true, RX_DELAY_1 + 0.1);
        //====================

        //scheduleAt(simTime() + timeoutLoRa, eventTimeout);


        //===================
        // Calculate response airtime to calculate the delay of the response
        EV << "Calculate Time On Air\n";

        // Calculate preamble and frame airtime
        auto tuple = calculateTimeOnAir(msgOut);

        // Convert frame time on air from ms to s
        double airtimeFrame = std::get<1>(tuple) / 1000;

        // Check if computation succeeded
        if (airtimeFrame < 0)
            return;

        // Calculate arrival time
        /*simtime_t simtime = simTime();
        simtime_t arrivalFrame = simtime + airtimeFrame;

        EV << "Simulation time: " << simtime << "\n";
        EV << "Expected arrival frame time: " << arrivalFrame << "\n";*/

        EV << "Sending STATS message...\n";

        // Calculate the delay (with tolerance) to send the message in the receive window of the end device
        double delay = RX_DELAY_1 - airtimeFrame + 0.1;

        // Send the message in the receive window of the end device through the sender (peer)
        if (delay > 0)
            forwardMessageIp(msgOut, "statsMsg",
                //selectedGatewayAddress, MSG_PORT_FORWARD);
                srcAddress, MSG_PORT_FORWARD_OVER_IP, true, delay);
        else
            forwardMessageIp(msgOut, "statsMsg",
                //selectedGatewayAddress, MSG_PORT_FORWARD);
                srcAddress, MSG_PORT_FORWARD_OVER_IP, true);

        // For a retransmission attempt, await this roughly identical moment in the future (TX_DELAY) and apply the delay
        scheduleAt(simTime() + TX_DELAY + delay, eventTimeout);
        //===================


        // Propagate the downlink counter to the network server to enable it to send MAC commands
        // (shared downlink counter but also uplink counter as it is the same in this moment)
        EV << "Propagating downlink counter from gateway to network server...\n";
        sendMessageShareCounter(endDeviceAddress.data(), counter);

        // Send signal for statistic collection
        emit(signalSent, 2u);
        emit(signalSentIp, 2u);

        messagesSentCount   += 2;
        messagesSentIpCount += 2;
        emit(signalSentCount, messagesSentCount);
        emit(signalSentIpCount, messagesSentIpCount);

        storageOccupied += IPv4_ADDRESS_SIZE + sizeof(tuple);
        storageOccupied += sizeof(cMessage*) + IPv4_ADDRESS_SIZE;
        networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC +
                      LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;
        networkOut += (MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_TCP_HEADER) * 2;

        delete msgIn;
    }
    else if (port == MSG_PORT_STATS) {
        EV << "Received STATS message\n";

        // Check if a message from the end device address has already been received
        if (it == map.end()) {
            // No, but a gateway cannot receive a STATS message without having forwarded a FORWARD
            delete msgIn;
            return;
        }

        // The end device is in the map, get the entry
        tuple = it->second;

        // Check if a FORWARD message from the end device address has been received
        if (std::get<3>(tuple) <= 1) {
           // No, but a gateway cannot receive a STATS message without having forwarded a FORWARD
           delete msgIn;
           return;
        }

        // Check if the common session key with the end device has been created (should be)
        auto itCommonSKeys = commonEndDeviceSKeys.find(endDeviceAddress);
        if (itCommonSKeys == commonEndDeviceSKeys.end()) {
            // No, but a gateway cannot receive a STATS message without having received a GENERATE_COMMON_KEY
            delete msgIn;
            return;
        }

        // Get the common session key shared with the end device
        auto commonSKey = itCommonSKeys->second;

        EV << "Decrypting STATS message...\n";

        // Remember the cluster key used for decryption
        uint8_t decryptKey[KEY_LORAWAN_SIZE];
        decryptDownlink(payloadIn, payloadIn, decryptKey, srcAddress);

        // Check if the message is a LoRa frame (should be)
        LoRaPhysicalFrame* phyMsg = dynamic_cast<LoRaPhysicalFrame*>(msgIn);
        if (!phyMsg)
          return;

        LoRaDatalinkFrame* dlMsg = dynamic_cast<LoRaDatalinkFrame*>(phyMsg->getEncapsulatedPacket());
        if (!dlMsg)
          return;

        // Decapsulate LoRa App layer
        LoRaAppUplinkFrame* appMsg = dynamic_cast<LoRaAppUplinkFrame*>(dlMsg->decapsulate());
        if (!appMsg)
          return;

        EV << "Encrypting message...\n";

        // Encrypt
        // ...

        EV << "Recalculating MIC...\n";

        // Recalculate MIC
        calculateMIC(dlMsg, appMsg, commonSKey.data());


        // Check custom seq number or
        // check default frame counter (but the gateway checks uplinks and not downlinks because downlinks are
        // checked by the end device. However if a frame with an old downlink arrives, it will be discarded
        // by the end device, so instead of consuming bandwidth for a bad message, drop here)
        // So, check the downlink counter when the frame arrives to the gateway during validation.
        // Update also the counter? If it is really shared, it should be updated
        //std::get<5>(tuple) = counter;
        //map[endDeviceAddress] = tuple;

        // Do not propagate the downlink counter to the network server
        // because it is a redundant message, already done when STATS message is sent

        // Propagate back the STATS message in response to the FORWARD message
        // forwarded to nearby gateways over LoRa
        EV << "Forwarding STATS message over LoRa...\n";
        //sendBroadcast(this, msgIn, LORA_GATE_OUT);
        sendBroadcastSecurely(this, eventTimeoutChannelTransmissions, dynamic_cast<cPacket*> (msgIn), LORA_GATE_OUT);

        // Do not schedule retransmission because it is burden of the original sender

        // Send signal for statistic collection
        emit(signalSent, 1u);
        emit(signalSentLoRa, 1u);
        emit(signalSentCount, ++messagesSentCount);
        emit(signalSentLoRaCount, ++messagesSentLoRaCount);

        networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC +
                      LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;
    }
    else if (port == MSG_PORT_PAIRING_REQUEST) {
        EV << "Received PAIRING_REQUEST message\n";

        // Check if a HELLO/FORWARD message from the end device address has already been received
        if (it == map.end()) {
            // No, but a gateway cannot receive a PAIRING_REQUEST message without having sent a STATS
            delete msgIn;
            return;
        }

        // The end device is in the map, get the entry
        tuple = it->second;

        EV << "Decrypting PAIRING_REQUEST message...\n";

        // Remember the key used for decryption
        std::array<uint8_t, KEY_LORAWAN_SIZE> decryptKey;
        decryptUplink(payloadIn, payloadIn, decryptKey.data(), endDeviceAddress, srcAddress);

        //printKey(decryptKey.data(), KEY_LORAWAN_SIZE, "decryptKey", EV);

        // Get message requestId
        uint8_t requestId = payloadIn[0];

        // Check if the request ID in the message matches the ID stored in the map
        if (requestId != std::get<2>(tuple)) {
            delete msgIn;
            return;
        }

        // The request ID matches.
        // Check if the ACK for the STATS message is already arrived
        if (std::get<1>(tuple) != nullptr) {
            // No, but the message is lost forever as the end device selected a gateway.
            // Cancel timeout event and remove the pointer from the tuple
            EV << "No ACK received for STATS message but PAIRING_REQUEST,\ncanceling timer for resending the frame...\n";
            eventTimeoutRetransmissions.erase(std::get<1>(tuple));

            delete std::get<0>(tuple);
            cancelAndDelete(std::get<1>(tuple));

            std::get<0>(tuple) = nullptr;
            std::get<1>(tuple) = nullptr;
            map[endDeviceAddress] = tuple;

            storageOccupied -= sizeof(cMessage*) + IPv4_ADDRESS_SIZE;
        }

        // Get the IP address of the selected RP from the message
        //uint8_t selectedGatewayAddress[IPv4_ADDRESS_SIZE];
        std::array<uint8_t, IPv4_ADDRESS_SIZE> selectedGatewayAddress;
        memcpy(selectedGatewayAddress.data(), &payloadIn[1], IPv4_ADDRESS_SIZE);

        // Check if the IP address of the selected gateway matches the gateway one
        if (memcmp(address, selectedGatewayAddress.data(), IPv4_ADDRESS_SIZE)) {
            // No, forward the message to the proper destination over IP
            // after having encapsulated the LoRa frame in upper layers packets
            EV << "Forwarding PAIRING_REQUEST message to selected gateway over IP...\n";

            forwardMessageIp(msgIn, "forwardPairingRequestMsg",
                    //selectedGatewayAddress, MSG_PORT_FORWARD_PAIRING_REQUEST);
                    selectedGatewayAddress, MSG_PORT_FORWARD_OVER_IP, true);
            // Do not schedule retransmission because it is burden of the original sender

            // Remember that the PAIRING_REQUEST message has been forwarded
            //memcpy(std::get<6>(tuple).data(), &payloadIn[1], IPv4_ADDRESS_SIZE);
            std::get<6>(tuple) = selectedGatewayAddress;

            EV << "Selected gw address: "
               << (unsigned) std::get<6>(tuple)[0] << "."
               << (unsigned) std::get<6>(tuple)[1] << "."
               << (unsigned) std::get<6>(tuple)[2] << "."
               << (unsigned) std::get<6>(tuple)[3] << "\n";

            // Update the entry
            map[endDeviceAddress] = tuple;

            // Send signal for statistic collection
            emit(signalSent, 1u);
            emit(signalSentIp, 1u);
            emit(signalSentCount, ++messagesSentCount);
            emit(signalSentIpCount, ++messagesSentIpCount);

            networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_UDP_HEADER;
            networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC +
                          LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;
            return;
        }

        // The IP addresses match.
        // Create and send the PAIRING_ACCEPT message
        uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {
            requestId
        };

        // Check if the end device is already associating with the gateway
        // and this is a retransmission due to lost response
        auto itAssociation = associations.find(endDeviceAddress);

        // Check if gateway resources are sufficient
        bool areResourcesOk = checkResources();
        if (areResourcesOk) {
            if (itAssociation == associations.end()) {
                // Get the timestamp about the STATS message reception (on which the decision was made)
                uint8_t timestampBytes[4];
                memcpy(timestampBytes, &payloadIn[5], sizeof(timestampBytes));

                // Convert from array of bytes to unsigned
                //unsigned timestamp = bytesToUint(timestampBytes);
                // Convert from array of bytes to unsigned and from ms to s
                unsigned timestamp = bytesToUint(timestampBytes) / 1000;

                // Check if a new association occurs in the interval of time [timestamp, now]
                if (isStateChanged(timestamp)) {
                    // Notify the end device about the current state
                    // (different from the one on which the selection was made)

                    payload[1] = STATS_UPDATE;
                    payload[2] = getCpuLoad();
                    payload[3] = getGpuLoad();
                    payload[4] = getRamLoad();
                    payload[5] = getStorageLoad();
                    payload[6] = getNetworkIn();
                    payload[7] = getNetworkOut();

                    EV << "Sending STATS_UPDATE response for pairing request...\n";

                    // Message for end device
                    cPacket* msgOut = createMessageDownlink(
                            "statsUpdateMsg", true,
                            endDeviceAddress.data(), IPv4_ADDRESS_SIZE, counter, MSG_PORT_PAIRING_ACCEPT,
                            payload, LORA_FRAME_SIZE_APP_PAYLOAD,
                            //commonEndDeviceKey,
                            decryptKey.data(),
                            spreadingFactor, transmissionPower, bandwidth, channelFrequency);

                    //std::get<0>(tuple) = msgOut;

                    // Update the entry
                    map[endDeviceAddress] = tuple;

                    //====================
                    // Send the message in the receive window of the end device
                    //sendDelayedBroadcast(this, std::get<0>(tuple), RX_DELAY_1 + 0.1, LORA_GATE_OUT);
                    //====================

                    //===================
                    // Calculate response airtime to calculate the delay of the response
                    EV << "Calculate Time On Air\n";

                    // Calculate preamble and frame airtime
                    auto tuple = calculateTimeOnAir(msgOut);

                    // Convert frame time on air from ms to s
                    double airtimeFrame = std::get<1>(tuple) / 1000;

                    // Check if computation succeeded
                    if (airtimeFrame < 0)
                        return;

                    // Calculate arrival time
                    /*simtime_t simtime = simTime();
                    simtime_t arrivalFrame = simtime + airtimeFrame;

                    EV << "Simulation time: " << simtime << "\n";
                    EV << "Expected arrival frame time: " << arrivalFrame << "\n";*/

                    // Calculate the delay (with tolerance) to send the message in the receive window of the end device
                    double delay = RX_DELAY_1 - airtimeFrame + 0.1;
                    //===================


                    //====================
                    // Check if the common session key with the end device has been generated
                    // (i.e. the end device is in the range of the gateway)
                    if (commonEndDeviceSKeys.find(endDeviceAddress) != commonEndDeviceSKeys.end()) {
                        // Send the message in the receive window of the end device
                        //sendDelayedBroadcast(this, std::get<0>(tuple), RX_DELAY_1 + 0.1, LORA_GATE_OUT);
                        //sendDelayedBroadcastSecurely(this, eventTimeoutChannelTransmissions, msgOut, RX_DELAY_1 + 0.1, LORA_GATE_OUT);

                        if (delay > 0)
                            sendDelayedBroadcastSecurely(this, eventTimeoutChannelTransmissions, msgOut, delay, LORA_GATE_OUT);
                        else
                            sendBroadcastSecurely(this, eventTimeoutChannelTransmissions, msgOut, LORA_GATE_OUT);

                        // Send signal for statistic collection
                        emit(signalSentLoRa, 1u);
                        emit(signalSentLoRaCount, ++messagesSentLoRaCount);
                    }
                    else {
                        // Send the message in the receive window of the end device through the sender (peer)
                        //forwardMessageIp(msgOut, "statsUpdateMsg",
                                //selectedGatewayAddress, MSG_PORT_FORWARD_PAIRING_REQUEST);
                                //srcAddress, MSG_PORT_FORWARD_OVER_IP, true, RX_DELAY_1 + 0.1);

                        // Send the message in the receive window of the end device through the sender (peer)
                        if (delay > 0)
                            forwardMessageIp(msgOut, "statsUpdateMsg",
                                //selectedGatewayAddress, MSG_PORT_FORWARD);
                                srcAddress, MSG_PORT_FORWARD_OVER_IP, true, delay);
                        else
                            forwardMessageIp(msgOut, "statsUpdateMsg",
                                //selectedGatewayAddress, MSG_PORT_FORWARD);
                                srcAddress, MSG_PORT_FORWARD_OVER_IP, true);

                        // Send signal for statistic collection
                        emit(signalSentIp, 1u);
                        emit(signalSentIpCount, ++messagesSentIpCount);

                        networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_UDP_HEADER;
                    }
                    //====================

                    // Do not reschedule the message as it is an ACK

                    // Send signal for statistic collection
                    emit(signalSent, 1u);
                    emit(signalSentCount, ++messagesSentCount);

                    networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC +
                                  LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;

                    delete msgIn;
                    return;
                }

                // This is the first request from the end device
                EV << "Adding the new association in the map...\n";

                // Store the entry in the map of associations
                //std::tuple<std::string, std::string, std::time_t> tupleAssociations {"", "",  std::time(nullptr)};
                //std::tuple<std::array<uint8_t, KEY_LORAWAN_SIZE>, std::string, std::time_t> tupleAssociations
                    //{std::array<uint8_t, KEY_LORAWAN_SIZE> {}, "",  std::time(nullptr)};
                std::tuple<std::array<uint8_t, KEY_LORAWAN_SIZE>, std::string, simtime_t> tupleAssociations
                    {std::array<uint8_t, KEY_LORAWAN_SIZE> {}, "",  simTime()};
                associations[endDeviceAddress] = tupleAssociations;

                EV << "Devices connected to the gateway: " << associations.size() << "\n";

                // Message for network server
                EV << "Sending CONNECTION_GW message to network server...\n";
                sendMessageConnection(endDeviceAddress.data(), requestId, counter, ACK);

                // Send signals for statistic collection
                //emit(signalConnected, 1u);
                emit(signalConnected, associations.size());
                emit(signalSent, 1u);
                emit(signalSentIp, 1u);
                emit(signalSentCount, ++messagesSentCount);
                emit(signalSentIpCount, ++messagesSentIpCount);

                //EV << "sizeof(tupleAssociations): " << sizeof(tupleAssociations) << "\n";
                storageOccupied += IPv4_ADDRESS_SIZE + sizeof(tupleAssociations);
                networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_TCP_HEADER;
            }
            else
                EV << "Already received PAIRING_REQUEST message from the end device but the response could be lost!\n";

            // Build payload
            payload[1] = ACK;

            EV << "Sending ACK response for pairing request...\n";

            // Message for end device
            std::get<0>(tuple) = createMessageDownlink(
                    "pairingAcceptACKMsg", true,
                    endDeviceAddress.data(), IPv4_ADDRESS_SIZE, counter, MSG_PORT_PAIRING_ACCEPT,
                    payload, LORA_FRAME_SIZE_APP_PAYLOAD,
                    //commonEndDeviceKey,
                    decryptKey.data(),
                    spreadingFactor, transmissionPower, bandwidth, channelFrequency);
        }
        else {
            if (itAssociation != associations.end()) {
                // The PAIRING_REQUEST is a repetition but if before it was accepted,
                // now it is refused, so notify the network server to delete the previous CONNECTION_GW message
                EV << "Deleting the association from the map...\n";

                // Get the tuple
                auto tupleAssociations = itAssociation->second;

                // Delete the association
                associations.erase(itAssociation);

                EV << "Devices connected to the gateway: " << associations.size() << "\n";

                // Message for network server
                EV << "Sending CONNECTION_GW message to network server...\n";
                sendMessageConnection(endDeviceAddress.data(), requestId, counter, NACK);

                // Send signals for statistic collection
                //emit(signalConnected, -1u);
                emit(signalConnected, associations.size());
                emit(signalSent, 1u);
                emit(signalSentIp, 1u);
                emit(signalSentCount, ++messagesSentCount);
                emit(signalSentIpCount, ++messagesSentIpCount);

                //EV << "sizeof(tupleAssociations): " << sizeof(tupleAssociations) << "\n";
                storageOccupied -= IPv4_ADDRESS_SIZE + sizeof(tupleAssociations);
                networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_TCP_HEADER;
            }

            // Build payload
            payload[1] = NACK;

            EV << "Sending NACK response for pairing request...\n";
            std::get<0>(tuple) = createMessageDownlink(
                    "pairingAcceptNACKMsg", true,
                    endDeviceAddress.data(), IPv4_ADDRESS_SIZE, counter, MSG_PORT_PAIRING_ACCEPT,
                    payload, LORA_FRAME_SIZE_APP_PAYLOAD,
                    //commonEndDeviceKey,
                    decryptKey.data(),
                    spreadingFactor, transmissionPower, bandwidth, channelFrequency);

            // Propagate the downlink counter to the network server to enable it to send MAC commands
            // (shared downlink counter but also uplink counter as it is the same in this moment)
            EV << "Propagating downlink counter from gateway to network server...\n";
            sendMessageShareCounter(endDeviceAddress.data(), counter);

            // Send signal for statistic collection
            emit(signalSent, 1u);
            emit(signalSentIp, 1u);
            emit(signalSentCount, ++messagesSentCount);
            emit(signalSentIpCount, ++messagesSentIpCount);

            networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_TCP_HEADER;
        }

        // Update the entry
        map[endDeviceAddress] = tuple;

        //====================
        // Send the message in the receive window of the end device
        //sendDelayedBroadcast(this, std::get<0>(tuple), RX_DELAY_1 + 0.1, LORA_GATE_OUT);
        //====================

        //===================
        // Calculate response airtime to calculate the delay of the response
        EV << "Calculate Time On Air\n";

        // Calculate preamble and frame airtime
        auto tupleAirtime = calculateTimeOnAir(std::get<0>(tuple));

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

        // Calculate the delay (with tolerance) to send the message in the receive window of the end device
        double delay = RX_DELAY_1 - airtimeFrame + 0.1;
        //===================

        //====================
        // Check if the common session key with the end device has been generated
        // (i.e. the end device is in the range of the gateway)
        if (commonEndDeviceSKeys.find(endDeviceAddress) != commonEndDeviceSKeys.end()) {
            // Send the message in the receive window of the end device
            //sendDelayedBroadcast(this, std::get<0>(tuple), RX_DELAY_1 + 0.1, LORA_GATE_OUT);
            //sendDelayedBroadcastSecurely(this, eventTimeoutChannelTransmissions, std::get<0>(tuple), RX_DELAY_1 + 0.1, LORA_GATE_OUT);

            if (delay > 0)
                sendDelayedBroadcastSecurely(this, eventTimeoutChannelTransmissions, std::get<0>(tuple), delay, LORA_GATE_OUT);
            else
                sendBroadcastSecurely(this, eventTimeoutChannelTransmissions, std::get<0>(tuple), LORA_GATE_OUT);

            // Send signal for statistic collection
            emit(signalSentLoRa, 1u);
            emit(signalSentLoRaCount, ++messagesSentLoRaCount);
        }
        else {
            // Send the message in the receive window of the end device through the sender (peer)
            //forwardMessageIp(std::get<0>(tuple), "pairingAcceptMsg",
                    //selectedGatewayAddress, MSG_PORT_FORWARD_PAIRING_REQUEST);
                    //srcAddress, MSG_PORT_FORWARD_OVER_IP, true, RX_DELAY_1 + 0.1);

            // Send the message in the receive window of the end device through the sender (peer)
            if (delay > 0)
                forwardMessageIp(std::get<0>(tuple), "pairingAcceptMsg",
                    //selectedGatewayAddress, MSG_PORT_FORWARD);
                    srcAddress, MSG_PORT_FORWARD_OVER_IP, true, delay);
            else
                forwardMessageIp(std::get<0>(tuple), "pairingAcceptMsg",
                    //selectedGatewayAddress, MSG_PORT_FORWARD);
                    srcAddress, MSG_PORT_FORWARD_OVER_IP, true);

            // Send signal for statistic collection
            emit(signalSentIp, 1u);
            emit(signalSentIpCount, ++messagesSentIpCount);

            networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_UDP_HEADER;
        }
        //====================

        emit(signalSent, 1u);
        emit(signalSentCount, ++messagesSentCount);

        // Do not reschedule the message as it is an ACK

        networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC +
                      LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;

        delete msgIn;
    }
    else if (port == MSG_PORT_PAIRING_ACCEPT) {
        EV << "Received PAIRING_ACCEPT message\n";

        // Check if a message from the end device address has already been received
        if (it == map.end()) {
            // No, but a gateway cannot receive a PAIRING_ACCEPT message without having forwarded a PAIRING_REQUEST
            delete msgIn;
            return;
        }

        // The end device is in the map, get the entry
        tuple = it->second;

        // Check if the PAIRING_REQUEST has been forwarded and the sender address matches
        if (memcmp(std::get<6>(tuple).data(), srcAddress.data(), IPv4_ADDRESS_SIZE)) {
            // No, but a gateway cannot receive a PAIRING_ACCEPT (downlink) message
            // without having forwarded a PAIRING_REQUEST (uplink)
            EV << "Ignore the message as it is not directly received by the selected gateway over IP\n";
            delete msgIn;
            return;
        }

        // Check if the common session key with the end device has been created (should be)
        auto itCommonSKeys = commonEndDeviceSKeys.find(endDeviceAddress);
        if (itCommonSKeys == commonEndDeviceSKeys.end()) {
            // No, but a gateway cannot receive a PAIRING_ACCEPT message without having received a GENERATE_COMMON_KEY
            delete msgIn;
            return;
        }

        // Get the common session key shared with the end device
        auto commonSKey = itCommonSKeys->second;

        EV << "Decrypting PAIRING_ACCEPT message...\n";

        // Remember the cluster key used for decryption
        uint8_t decryptKey[KEY_LORAWAN_SIZE];
        decryptDownlink(payloadIn, payloadIn, decryptKey, srcAddress);

        // Check if the message is a LoRa frame (should be)
        LoRaPhysicalFrame* phyMsg = dynamic_cast<LoRaPhysicalFrame*>(msgIn);
        if (!phyMsg)
          return;

        LoRaDatalinkFrame* dlMsg = dynamic_cast<LoRaDatalinkFrame*>(phyMsg->getEncapsulatedPacket());
        if (!dlMsg)
          return;

        // Decapsulate LoRa App layer
        LoRaAppUplinkFrame* appMsg = dynamic_cast<LoRaAppUplinkFrame*>(dlMsg->decapsulate());
        if (!appMsg)
          return;

        EV << "Encrypting message...\n";

        // Encrypt
        // ...

        EV << "Recalculating MIC...\n";

        // Recalculate MIC
        calculateMIC(dlMsg, appMsg, commonSKey.data());

        // Propagate back the PAIRING_ACCEPT message in response to the PAIRING_REQUEST message
        // forwarded to nearby gateways over IP
        EV << "Forwarding PAIRING_ACCEPT message over LoRa...\n";
        //sendBroadcast(this, msgIn, LORA_GATE_OUT);
        sendBroadcastSecurely(this, eventTimeoutChannelTransmissions, dynamic_cast<cPacket*> (msgIn), LORA_GATE_OUT);

        // Do not schedule retransmission because it is burden of the original sender

        // Send signal for statistic collection
        emit(signalSent, 1u);
        emit(signalSentLoRa, 1u);
        emit(signalSentCount, ++messagesSentCount);
        emit(signalSentLoRaCount, ++messagesSentLoRaCount);

        networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC +
                      LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;
    }

    //else if (port == MSG_PORT_PAIRING_VERIFY || port == MSG_PORT_MAC_CMD) {
    else if (port == MSG_PORT_CONNECTION || port == MSG_PORT_MAC_CMD) {
        // Propagate back the PAIRING_VERIFY or MAC_CMD message sent by the network server.
        // Do not schedule retransmission because it is burden of the original sender

        // Check if the frame is an uplink
        if (frameType == UPLINK_UNCONFIRMED || frameType == UPLINK_CONFIRMED) {
            EV << "Forwarding CONNECTION message over IP\n";
            //forwardMessageIp(msgIn, "forwardConnectionMsg", networkServerAddress, MSG_PORT_FORWARD_CONNECTION);
            forwardMessageIp(msgIn, "forwardConnectionMsg", networkServerAddress, MSG_PORT_FORWARD_OVER_IP, false);

            emit(signalSentIp, 1u);
            emit(signalSentIpCount, ++messagesSentIpCount);

            networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_UDP_HEADER;
            networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC +
                          LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;
        }
        else {
            // Check if the message is a forward by a peer
            if (memcmp(srcAddress.data(), networkServerAddress.data(), IPv4_ADDRESS_SIZE)) {
                // Yes, ignore it to avoid cycles and
                // because only messages directly received from the network server must be forwarded

                // Since the downlink counter is not updated, refuse the frame here
                EV << "Ignore the message as it is not directly received by the network server but a peer\n";
                delete msgIn;
                return;
            }

            EV << "Forwarding CONNECTION or MAC_CMD message over LoRa\n";
            //sendBroadcast(this, msgIn->dup(), LORA_GATE_OUT);
            //sendBroadcastSecurely(this, eventTimeoutChannelTransmissions, dynamic_cast<cPacket*> (msgIn->dup()), LORA_GATE_OUT);
            sendBroadcastSecurely(this, eventTimeoutChannelTransmissions, dynamic_cast<cPacket*> (msgIn), LORA_GATE_OUT);

            emit(signalSentLoRa, 1u);
            emit(signalSentLoRaCount, ++messagesSentLoRaCount);

            networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC +
                          LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;
        }

        // Send signal for statistic collection
        emit(signalSent, 1u);
        emit(signalSentCount, ++messagesSentCount);
    }
    /*else if (port == MSG_PORT_PAIRING_VERIFIED) {
        // Propagate the PAIRING_VERIFIED message TO the network server over LoRa
        EV << "Forwarding PAIRING_VERIFIED message to network server...\n";

        // Do not schedule retransmission because it is burden of the original sender.
        // Send over UDP

        // Create UDP/IP packet
        cPacket* msgOut = createMessageIp(
            //"forwardPairingVerifiedMsg",
            "forwardConnectionMsg",
            address, networkServerAddress.data(), IPv4_ADDRESS_SIZE,
            nullptr, 0,
            dynamic_cast<LoRaDatalinkFrame*>(msgIn->dup()));
        msgOut = createMessageUdp(
            //"forwardPairingVerifiedMsg",
            "forwardConnectionMsg",
            MSG_PORT_CONNECTION, MSG_PORT_CONNECTION,
            nullptr, 0,
            msgOut);

        // Send the message to the network server
        send(msgOut, IP_GATE_OUT, 0);

        // Send signal for statistic collection
        emit(signalSent, 1);

        networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_TCP_HEADER;
    }*/
    else if (port == MSG_PORT_GENERATE_ASSOCIATION_KEY) {
        EV << "Received GENERATE_ASSOCIATION_KEY message\n";

        // Reuse OTAA procedure: a nonce from the end device and one from the gateway

        // Check if a HELLO/FORWARD message from the end device address has already been received
        if (it == map.end()) {
            // No, but a gateway cannot receive a PAIRING_REQUEST message without having sent a STATS
            delete msgIn;
            return;
        }

        // The end device is in the map, get the entry
        tuple = it->second;

        // Check if the frame is a downlink
        if (frameType == DOWNLINK_UNCONFIRMED || frameType == DOWNLINK_CONFIRMED) {
            EV << "Selected gw address: "
               << (unsigned) std::get<6>(tuple)[0] << "."
               << (unsigned) std::get<6>(tuple)[1] << "."
               << (unsigned) std::get<6>(tuple)[2] << "."
               << (unsigned) std::get<6>(tuple)[3] << "\n";

            // Check if the GENERATE_ASSOCIATION_KEY has been forwarded and the sender address matches
            if (memcmp(std::get<6>(tuple).data(), srcAddress.data(), IPv4_ADDRESS_SIZE)) {
                // No, but a gateway cannot receive a GENERATE_ASSOCIATION_KEY (downlink) message
                // without having forwarded a GENERATE_ASSOCIATION_KEY (uplink)
                EV << "Ignore the message as it is not directly received by the selected gateway over IP\n";
                delete msgIn;
                return;
            }

            // Check if the common session key with the end device has been created (should be)
            auto itCommonSKeys = commonEndDeviceSKeys.find(endDeviceAddress);
            if (itCommonSKeys == commonEndDeviceSKeys.end()) {
                // No, but a gateway cannot receive a GENERATE_ASSOCIATION_KEY message
                // without having received a GENERATE_COMMON_KEY
                delete msgIn;
                return;
            }

            // Get the common session key shared with the end device
            auto commonSKey = itCommonSKeys->second;

            EV << "Decrypting GENERATE_ASSOCIATION_KEY message...\n";

            // Remember the cluster key used for decryption
            uint8_t decryptKey[KEY_LORAWAN_SIZE];
            decryptDownlink(payloadIn, payloadIn, decryptKey, srcAddress);

            // Check if the message is a LoRa frame (should be)
            LoRaPhysicalFrame* phyMsg = dynamic_cast<LoRaPhysicalFrame*>(msgIn);
            if (!phyMsg)
              return;

            LoRaDatalinkFrame* dlMsg = dynamic_cast<LoRaDatalinkFrame*>(phyMsg->getEncapsulatedPacket());
            if (!dlMsg)
              return;

            // Decapsulate LoRa App layer
            LoRaAppUplinkFrame* appMsg = dynamic_cast<LoRaAppUplinkFrame*>(dlMsg->decapsulate());
            if (!appMsg)
              return;

            EV << "Encrypting message...\n";

            // Encrypt
            // ...

            EV << "Recalculating MIC...\n";

            // Recalculate MIC
            calculateMIC(dlMsg, appMsg, commonSKey.data());

            // Propagate back the GENERATE_ASSOCIATION_KEY message in response to the GENERATE_ASSOCIATION_KEY message
            // forwarded to nearby gateways over IP
            EV << "Forwarding GENERATE_ASSOCIATION_KEY message over LoRa...\n";
            //sendBroadcast(this, msgIn, LORA_GATE_OUT);
            sendBroadcastSecurely(this, eventTimeoutChannelTransmissions, dynamic_cast<cPacket*> (msgIn), LORA_GATE_OUT);

            // Do not schedule retransmission because it is burden of the original sender

            // Send signal for statistic collection
            emit(signalSent, 1u);
            emit(signalSentLoRa, 1u);
            emit(signalSentCount, ++messagesSentCount);
            emit(signalSentLoRaCount, ++messagesSentLoRaCount);

            networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC +
                          LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;
            return;
        }

        // Decrypt the message payload
        EV << "Decrypting GENERATE_ASSOCIATION_KEY message...\n";

        // Remember the key used for decryption
        std::array<uint8_t, KEY_LORAWAN_SIZE> decryptKey;
        decryptUplink(payloadIn, payloadIn, decryptKey.data(), endDeviceAddress, srcAddress);

        // Get message requestId
        uint8_t requestId = payloadIn[0];

        // Check if the request ID in the message matches the ID stored in the map
        if (requestId != std::get<2>(tuple)) {
            delete msgIn;
            return;
        }

        // Check if the end device is associated with the gateway
        auto itAssociation = associations.find(endDeviceAddress);
        if (itAssociation == associations.end()) {
            // Add in the entry the IP address of the gateway to which the end device is associated
            memcpy(std::get<6>(tuple).data(), &payloadIn[1], IPv4_ADDRESS_SIZE);

            EV << "Selected gw address: "
               << (unsigned) std::get<6>(tuple)[0] << "."
               << (unsigned) std::get<6>(tuple)[1] << "."
               << (unsigned) std::get<6>(tuple)[2] << "."
               << (unsigned) std::get<6>(tuple)[3] << "\n";

            // Update the entry
            map[endDeviceAddress] = tuple;

            EV << "Forwarding GENERATE_ASSOCIATION_KEY message to selected gateway over IP...\n";

            // Forward the message to selected gateway
            forwardMessageIp(msgIn, "forwardGenerateAssociationKeyMsg",
                    //networkServerAddress, MSG_PORT_FORWARD_PAIRING_REQUEST);
                    std::get<6>(tuple), MSG_PORT_FORWARD_OVER_IP, true);

            // Send signal for statistic collection
            emit(signalSent, 1u);
            emit(signalSentIp, 1u);
            emit(signalSentCount, ++messagesSentCount);
            emit(signalSentIpCount, ++messagesSentIpCount);

            networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_UDP_HEADER;
            networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC +
                          LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;
            return;
        }

        // Check if the IP address of the selected gateway matches the gateway one
        // (should be as it is in the association)
        if (memcmp(address, &payloadIn[1], IPv4_ADDRESS_SIZE)) {
            delete msgIn;
            return;
        }

        // The end device is associated with the gateway.
        // Generate a nonce for deriving a symmetric key
        unsigned nonceGw = generateNonce(0xFFFFFF);

        // Convert nonce to array of bytes
        uint8_t nonceGw_[4];
        uintToBytes(nonceGw, nonceGw_);

        uint8_t nonceDev_[4] = {
            payloadIn[5], payloadIn[6], payloadIn[7], payloadIn[8]
        };

        unsigned nonceDev = bytesToUint(nonceDev_);

        EV << "nonceGw: " << nonceGw << "\n";
        EV << "nonceDev: " << nonceDev << "\n";

        uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {
            requestId,
            nonceGw_[0],
            nonceGw_[1],
            nonceGw_[2],
            nonceGw_[3]
        };

        // Get the entry
        auto tupleAssociations = itAssociation->second;

        // Derive session key exclusively shared with the associated gateway
        //std::get<0>(tupleAssociations) = generateKey(associationKey, nonceGw, nonceDev, networkId);
        memcpy(std::get<0>(tupleAssociations).data(),
               generateKey(associationKey, nonceGw, nonceDev, networkId),
               std::get<0>(tupleAssociations).size());

        printKey(std::get<0>(tupleAssociations).data(), KEY_LORAWAN_SIZE, "associationSKey", EV);

        // Update the entry
        associations[endDeviceAddress] = tupleAssociations;

        // The end device is in the map, get the entry
        tuple = it->second;

        EV << "Sending ACK response for GENERATE_ASSOCIATION_KEY message...\n";

        // Message for end device
        cPacket* msgOut = createMessageDownlink(
                "generateAssociationKeyACKMsg", true,
                endDeviceAddress.data(), IPv4_ADDRESS_SIZE, counter, MSG_PORT_GENERATE_ASSOCIATION_KEY,
                payload, LORA_FRAME_SIZE_APP_PAYLOAD,
                //commonEndDeviceKey,
                decryptKey.data(),
                spreadingFactor, transmissionPower, bandwidth, channelFrequency);

        std::get<0>(tuple) = msgOut;

        // Update the entry
        map[endDeviceAddress] = tuple;

        //====================
        // Send the message in the receive window of the end device
        //sendDelayedBroadcast(this, std::get<0>(tuple), RX_DELAY_1 + 0.1, LORA_GATE_OUT);
        //====================

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

        // Calculate the delay (with tolerance) to send the message in the receive window of the end device
        double delay = RX_DELAY_1 - airtimeFrame + 0.1;
        //===================

        //====================
        // Check if the common session key with the end device has been generated
        // (i.e. the end device is in the range of the gateway)
        if (commonEndDeviceSKeys.find(endDeviceAddress) != commonEndDeviceSKeys.end()) {
            // Send the message in the receive window of the end device
            //sendDelayedBroadcast(this, std::get<0>(tuple), RX_DELAY_1 + 0.1, LORA_GATE_OUT);
            //sendDelayedBroadcastSecurely(this, eventTimeoutChannelTransmissions, msgOut, RX_DELAY_1 + 0.1, LORA_GATE_OUT);

            if (delay > 0)
                sendDelayedBroadcastSecurely(this, eventTimeoutChannelTransmissions, msgOut, delay, LORA_GATE_OUT);
            else
                sendBroadcastSecurely(this, eventTimeoutChannelTransmissions, msgOut, LORA_GATE_OUT);

            emit(signalSentLoRa, 1u);
            emit(signalSentLoRaCount, ++messagesSentLoRaCount);
        }
        else {
            // Send the message in the receive window of the end device through the sender (peer)
            //forwardMessageIp(msgOut, "generateAssociationKeyACKMsg",
                    //selectedGatewayAddress, MSG_PORT_FORWARD_PAIRING_REQUEST);
                    //srcAddress, MSG_PORT_FORWARD_OVER_IP, true, RX_DELAY_1 + 0.1);

            // Send the message in the receive window of the end device through the sender (peer)
            if (delay > 0)
                forwardMessageIp(msgOut, "generateAssociationKeyACKMsg",
                    //selectedGatewayAddress, MSG_PORT_FORWARD);
                    srcAddress, MSG_PORT_FORWARD_OVER_IP, true, delay);
            else
                forwardMessageIp(msgOut, "generateAssociationKeyACKMsg",
                    //selectedGatewayAddress, MSG_PORT_FORWARD);
                    srcAddress, MSG_PORT_FORWARD_OVER_IP, true);

            emit(signalSentIp, 1u);
            emit(signalSentIpCount, ++messagesSentIpCount);

            networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_UDP_HEADER;
        }
        //====================

        // Do not reschedule the message as it is an ACK

        // Propagate the downlink counter to the network server to enable it to send MAC commands
        // (shared downlink counter but also uplink counter as it is the same in this moment)
        EV << "Propagating downlink counter from gateway to network server...\n";
        sendMessageShareCounter(endDeviceAddress.data(), counter);

        // Send signal for statistic collection
        emit(signalSent, 2u);
        messagesSentCount += 2;
        emit(signalSentCount, messagesSentCount);
        emit(signalSentIp, 1u);
        emit(signalSentIpCount, ++messagesSentIpCount);

        networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC +
                      LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;
        networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_TCP_HEADER;

        delete msgIn;
    }
    else if (port == MSG_PORT_DATA_PROFILE) {
        EV << "Received DATA_PROFILE message\n";

        // Check if a HELLO/FORWARD message from the end device address has already been received
        if (it == map.end()) {
            delete msgIn;
            return;
        }

        // The end device is in the map, get the entry
        tuple = it->second;

        // Check if the end device is associated with the gateway
        //std::tuple<std::string, std::string, std::time_t> tupleAssociations;
        //std::tuple<std::array<uint8_t, KEY_LORAWAN_SIZE>, std::string, std::time_t> tupleAssociations;
        std::tuple<std::array<uint8_t, KEY_LORAWAN_SIZE>, std::string, simtime_t> tupleAssociations;

        auto itAssociation = associations.find(endDeviceAddress);
        if (itAssociation == associations.end()) {
            // This is the first message that is not encrypted with a common session key
            // but the association key.
            // To forward to selected gateway its address must be stored through the previous messages

            // Check if the frame is an uplink
            if (frameType == UPLINK_UNCONFIRMED || frameType == UPLINK_CONFIRMED) {
                // Check if the end device has an associated gateway (should be)
                if (std::get<6>(tuple).empty()) {
                    delete msgIn;
                    return;
                }

                EV << "Forwarding DATA_PROFILE message to selected gateway over IP...\n";

                // Forward the message to selected gateway
                forwardMessageIp(msgIn, "forwardDataProfileMsg",
                        //std::get<6>(tuple), MSG_PORT_FORWARD_PAIRING_REQUEST);
                        std::get<6>(tuple), MSG_PORT_FORWARD_OVER_IP, false);

                // Send signal for statistic collection
                emit(signalSentIp, 1u);
                emit(signalSentIpCount, ++messagesSentIpCount);

                networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_UDP_HEADER;
                networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC +
                              LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;
            }
            else {
                // Check if the DATA_PROFILE has been forwarded and the sender address matches
                if (memcmp(std::get<6>(tuple).data(), srcAddress.data(), IPv4_ADDRESS_SIZE)) {
                    // No, but a gateway cannot receive a DATA_PROFILE (downlink) message
                    // without having forwarded a DATA_PROFILE (uplink)
                    EV << "Ignore the message as it is not directly received by the selected gateway over IP\n";
                    delete msgIn;
                    return;
                }

                // Forward the message back to the end device
                EV << "Forwarding DATA_PROFILE message over LoRa\n";
                //sendBroadcast(this, msgIn->dup(), LORA_GATE_OUT);
                sendBroadcastSecurely(
                        this, eventTimeoutChannelTransmissions, dynamic_cast<cPacket*> (msgIn->dup()), LORA_GATE_OUT);

                // Send signal for statistic collection
                emit(signalSentLoRa, 1u);
                emit(signalSentLoRaCount, ++messagesSentLoRaCount);

                networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC +
                              LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;
            }

            // Send signal for statistic collection
            emit(signalSent, 1u);
            emit(signalSentCount, ++messagesSentCount);

            return;
        }

        // The end device is associated with the gateway.
        // Get the entry
        tupleAssociations = itAssociation->second;

        // Decrypt the message payload with the session key exclusively shared with the end device
        EV << "Decrypting DATA_PROFILE message...\n";

        // Remember the key used for decryption
        std::array<uint8_t, KEY_LORAWAN_SIZE> decryptKey;
        decryptUplink(payloadIn, payloadIn, decryptKey.data(), endDeviceAddress, srcAddress);

        // In our deployment, the data profile is represented only by the location string.
        // Check if the message is a retransmission due to lost response
        // using the starting index from which the string has being copied
        if (payloadIn[0] == std::get<1>(tupleAssociations).length()) {
            // Append the location string to the corresponding string in the tuple.
            std::get<1>(tupleAssociations).append((char*) &payloadIn[1], LORA_FRAME_SIZE_APP_PAYLOAD-1);

            // Update the entry
            associations[endDeviceAddress] = tupleAssociations;
        }

        // Payload is not a string because does not use the last byte for null terminator
        //EV << "payload: " << &payloadIn[1] << "\n";

        EV << "payload (location string fragment): ";
        for (int i=1; i < LORA_FRAME_SIZE_APP_PAYLOAD; i++)
            EV << payloadIn[i];
        EV << "\n";
        EV << "locationString: " << std::get<1>(tupleAssociations) << "\n";

        // Propagate the downlink counter to the network server to enable it to send MAC commands
        // (shared downlink counter but also uplink counter as it is the same in this moment)
        EV << "Propagating downlink counter from gateway to network server...\n";
        sendMessageShareCounter(endDeviceAddress.data(), counter);

        // Create and send the response to the DATA PROFILE message
        uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {
            std::get<2>(tuple) // Request ID
        };

        EV << "Sending ACK for data profile message\n";

        // Message for end device
        cPacket* msgOut = createMessageDownlink(
                "dataProfileACKMsg", true,
                endDeviceAddress.data(), IPv4_ADDRESS_SIZE, counter, MSG_PORT_DATA_PROFILE,
                payload, LORA_FRAME_SIZE_APP_PAYLOAD,
                //commonEndDeviceKey,
                std::get<0>(tupleAssociations).data(),
                spreadingFactor, transmissionPower, bandwidth, channelFrequency);

        std::get<0>(tuple) = msgOut;

        // Update the entry
        map[endDeviceAddress] = tuple;

        //====================
        // Send the message in the receive window of the end device
        //sendDelayedBroadcast(this, std::get<0>(tuple), RX_DELAY_1 + 0.1, LORA_GATE_OUT);
        //====================

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

        // Calculate the delay (with tolerance) to send the message in the receive window of the end device
        double delay = RX_DELAY_1 - airtimeFrame + 0.1;
        //===================

        //====================
        // Check if the common session key with the end device has been generated
        // (i.e. the end device is in the range of the gateway)
        if (commonEndDeviceSKeys.find(endDeviceAddress) != commonEndDeviceSKeys.end()) {
            // Send the message in the receive window of the end device
            //sendDelayedBroadcast(this, std::get<0>(tuple), RX_DELAY_1 + 0.1, LORA_GATE_OUT);
            //sendDelayedBroadcastSecurely(this, eventTimeoutChannelTransmissions, msgOut, RX_DELAY_1 + 0.1, LORA_GATE_OUT);

            if (delay > 0)
                sendDelayedBroadcastSecurely(this, eventTimeoutChannelTransmissions, msgOut, delay, LORA_GATE_OUT);
            else
                sendBroadcastSecurely(this, eventTimeoutChannelTransmissions, msgOut, LORA_GATE_OUT);

            emit(signalSentLoRa, 1u);
            emit(signalSentLoRaCount, ++messagesSentLoRaCount);
        }
        else {
            // Send the message in the receive window of the end device through the sender (peer)
            //forwardMessageIp(msgOut, "dataProfileACKMsg",
                    //selectedGatewayAddress, MSG_PORT_FORWARD_PAIRING_REQUEST);
                    //srcAddress, MSG_PORT_FORWARD_OVER_IP, true, RX_DELAY_1 + 0.1);

            // Send the message in the receive window of the end device through the sender (peer)
            if (delay > 0)
                forwardMessageIp(msgOut, "dataProfileACKMsg",
                    //selectedGatewayAddress, MSG_PORT_FORWARD);
                    srcAddress, MSG_PORT_FORWARD_OVER_IP, true, delay);
            else
                forwardMessageIp(msgOut, "dataProfileACKMsg",
                    //selectedGatewayAddress, MSG_PORT_FORWARD);
                    srcAddress, MSG_PORT_FORWARD_OVER_IP, true);

            emit(signalSentIp, 1u);
            emit(signalSentIpCount, ++messagesSentIpCount);

            networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_UDP_HEADER;
        }
        //====================

        // Do not reschedule the message as it is an ACK

        // Send signal for statistic collection
        emit(signalSent, 2u);
        messagesSentCount += 2;
        emit(signalSentCount, messagesSentCount);
        emit(signalSentIp, 1u);
        emit(signalSentIpCount, ++messagesSentIpCount);

        networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC +
                      LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;
        networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_TCP_HEADER;

        // To send data profile to network server once received completely,
        // use either a bool in the message payload to denote termination or exploit first DATA message.
        // Choice depends on the needs of the IoT app, the second approach minimizes fragments while
        // the first make the data profile immediately available on the network server

        delete msgIn;
    }
    else if (port == MSG_PORT_DATA) {
        EV << "Received DATA message\n";

        // Check if a HELLO/FORWARD message from the end device address has already been received
        if (it == map.end()) {
            delete msgIn;
            return;
        }

        // The end device is in the map, get the entry
        tuple = it->second;

        // Check if the end device is associated with the gateway
        auto itAssociation = associations.find(endDeviceAddress);
        if (itAssociation == associations.end()) {
            // Check if the end device has an associated gateway (should be)
            if (std::get<6>(tuple).empty()) {
                delete msgIn;
                return;
            }

            EV << "Forwarding DATA message to selected gateway over IP...\n";

            // Forward the message to selected gateway
            forwardMessageIp(msgIn, "forwardDataMsg",
                    //std::get<6>(tuple), MSG_PORT_FORWARD_PAIRING_REQUEST);
                    std::get<6>(tuple), MSG_PORT_FORWARD_OVER_IP, false);

            // Send signal for statistic collection
            emit(signalSent, 1u);
            emit(signalSentIp, 1u);
            emit(signalSentCount, ++messagesSentCount);
            emit(signalSentIpCount, ++messagesSentIpCount);

            networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_UDP_HEADER;
            networkOut += LORA_FRAME_SIZE_DATALINK_HEADER + LORA_FRAME_SIZE_DATALINK_MIC +
                          LORA_FRAME_SIZE_APP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;
            return;
        }

        // The end device is associated with the gateway.
        // Get the entry
        auto tupleAssociations = itAssociation->second;

        std::string dataProfile = "";
        int dataProfileSize = 0;

        // Check if the data profile has been propagated to the network server
        if (!std::get<7>(tuple)) {
            // The data profile has been sent and will be retransmitted until received once
            std::get<7>(tuple) = true;

            // Update the entry
            map[endDeviceAddress] = tuple;

            // Get the data profile
            //std::string dataProfile = std::get<1>(tupleAssociations);
            dataProfile = std::get<1>(tupleAssociations);
            dataProfileSize = dataProfile.length();

            EV << "dataProfile: " << dataProfile << "\n";
            EV << "dataProfileSize: " << dataProfileSize << "\n";
            EV << "Including data profile in the payload\n";
            //EV << "Sending SHARE_DATA_PROFILE message to network server...\n";

            //sendMessageDataProfile(endDeviceAddress.data(), dataProfile.c_str(), dataProfile.length());
        }

        // Decrypt the message payload with the session key exclusively shared with the end device
        EV << "Decrypting DATA message...\n";

        // Remember the key used for decryption
        std::array<uint8_t, KEY_LORAWAN_SIZE> decryptKey;
        decryptUplink(payloadIn, payloadIn, decryptKey.data(), endDeviceAddress, srcAddress);

        // Get message data
        uint8_t data = payloadIn[0];
        uint8_t timestamp[4] = {
                payloadIn[1],
                payloadIn[2],
                payloadIn[3],
                payloadIn[4]
        };

        // Forward to network server
        EV << "Sending PROCESSED_DATA message to network server...\n";
        //sendMessageProcessedData(endDeviceAddress.data(), processData(data), timestamp);
        sendMessageProcessedData(
                endDeviceAddress.data(), processData(data), timestamp, dataProfile.c_str(), dataProfileSize);

        // Send signal for statistic collection
        emit(signalSent, 1u);
        emit(signalSentIp, 1u);
        emit(signalSentCount, ++messagesSentCount);
        emit(signalSentIpCount, ++messagesSentIpCount);

        networkOut += MESSAGE_SIZE_IPV4_HEADER + MESSAGE_SIZE_TCP_HEADER + LORA_FRAME_SIZE_APP_PAYLOAD;

        delete msgIn;
    }
}

// Decrypt src into dest and store the used key (end device or cluster key) into decryptKey.
// Use the end device address to know if the end device is in its radio range
void LoRaGateway::decryptUplink(
        uint8_t* src, uint8_t* dest, uint8_t* decryptKey,
        std::array<uint8_t, IPv4_ADDRESS_SIZE>& endDeviceAddress, std::array<uint8_t, IPv4_ADDRESS_SIZE>& srcAddress) {
    if (!src || !dest || ! decryptKey)
        return;

    uint8_t voidAddress[IPv4_ADDRESS_SIZE] = {0,0,0,0};

    // Check if the message is a forward by a peer
    if (memcmp(srcAddress.data(), voidAddress, IPv4_ADDRESS_SIZE)) {
        // Check if the source gateway is a neighbor
        if (neighborGatewayAddresses.find(srcAddress) != neighborGatewayAddresses.end()) {
            // Iterate clusters (subsets of the previous set) to find the session key
            for (auto itClusters : clusters) {
                auto commonSKey = itClusters.first;
                auto cluster    = itClusters.second;

                if (cluster.find(srcAddress) != cluster.end()) {
                    memcpy(decryptKey, commonSKey.data(), KEY_LORAWAN_SIZE);
                    break;
                }
            }
        }
        else
            // Use the root key (useless multi-hop to not employ the root key)
            memcpy(decryptKey, commonGatewaysKey, KEY_LORAWAN_SIZE);
    }
    else {
        // The message has been sent by a sensor.
        // Check if the common session key with the end device has been created
        // (i.e. this is a first level gateway)
        auto itCommonSKeys = commonEndDeviceSKeys.find(endDeviceAddress);
        if (itCommonSKeys != commonEndDeviceSKeys.end()) {
            // Get the common session key shared with the end device
            auto commonSKey = itCommonSKeys->second;
            memcpy(decryptKey, commonSKey.data(), KEY_LORAWAN_SIZE);
        }
        else {
            // Check if the association session key with the end device has been created
            auto itAssociations = associations.find(endDeviceAddress);
            if (itAssociations != associations.end()) {
                // Get the association session key shared with the end device
                auto associationSKey = std::get<0>(itAssociations->second);
                memcpy(decryptKey, associationSKey.data(), KEY_LORAWAN_SIZE);
            }
            else
                // Use the root key
                memcpy(decryptKey, commonEndDeviceKey, KEY_LORAWAN_SIZE);
        }
    }

    // TODO: Decrypt payload
    // decryptKey = ...
}

// Decrypt src into dest and store the used cluster key into decryptKey
void LoRaGateway::decryptDownlink(
        uint8_t* src, uint8_t* dest, uint8_t* decryptKey, std::array<uint8_t, IPv4_ADDRESS_SIZE>& srcAddress) {
    // Check if the destination gateway is a neighbor
    if (neighborGatewayAddresses.find(srcAddress) != neighborGatewayAddresses.end()) {
        // Iterate clusters (subsets of the previous set) to find the session key
        for (auto itClusters : clusters) {
            auto commonSKey = itClusters.first;
            auto cluster    = itClusters.second;

            if (cluster.find(srcAddress) != cluster.end()) {
                memcpy(decryptKey, commonSKey.data(), KEY_LORAWAN_SIZE);
                break;
            }
        }
    }
    else
        // Use the root key (useless multi-hop to not employ the root key)
        memcpy(decryptKey, commonGatewaysKey, KEY_LORAWAN_SIZE);

    // TODO: Decrypt payload with cluster session key
    // decryptKey = ...
}

uint8_t LoRaGateway::processData(uint8_t data) {
    // TODO: Perform stream processing
    return data;
}

// Return true if a new association was performed in a moment next or equal the timestamp
bool LoRaGateway::isStateChanged(unsigned timestamp) {
    EV << "Input timestamp: " << timestamp << "\n";

    for (auto it=associations.begin(); it != associations.end(); ++it) {
        auto tuple = it->second;

        EV << "Association timestamp: " << std::get<2>(tuple) << "\n";

        if (std::get<2>(tuple) >= timestamp)
            return true;
    }

    return false;
}

// Send gateway IP address to neighbors for next creating clusters
void LoRaGateway::sendMessageHelloGateway(
        //uint8_t* endDeviceAddress, uint16_t counter,
        std::array<uint8_t, IPv4_ADDRESS_SIZE>& endDeviceAddress, uint16_t counter,
        uint8_t spreadingFactor, uint8_t transmissionPower, float bandwidth, float channelFrequency) {
    // Build payload
    uint8_t payload[8] = {};

    cPacket* msgOut = createMessageDownlink(
            "helloGatewayMsg", false,
            endDeviceAddress.data(), IPv4_ADDRESS_SIZE,
            counter, MSG_PORT_HELLO_GATEWAY, payload, sizeof(payload),
            commonGatewaysKey,
            spreadingFactor, transmissionPower, bandwidth, channelFrequency);

    EV << "Calculate Time On Air\n";

    // Calculate preamble and frame airtime
    auto tupleAirtime = calculateTimeOnAir(msgOut);

    // Convert frame time on air from ms to s
    //double airtimeFrame = std::get<1>(tupleAirtime) / 1000;

    // Get frame time on air in ms
    double airtimeFrame = std::get<1>(tupleAirtime);

    // Check if computation succeeded
    if (airtimeFrame < 0)
        return;


    // Get current time in s
    //simtime_t time = simTime();
    //std::time_t time = std::time(nullptr);

    /*EV << "Time: " << time << " s\n";
    EV << "Airtime frame: " << airtimeFrame << " s\n";
    EV << "ceil(Airtime frame): " << ceil(airtimeFrame) << " s\n";
    EV << "Time expiration: " << time + airtimeFrame << " s\n";
    EV << "ceil(Time expiration): " << ceil(time + airtimeFrame) << " s\n";
    EV << "ceil(Time expiration): " << ceil(time + airtimeFrame).dbl() << " s\n";
    //EV << "ceil(Time expiration): " << (long) ceil(time + airtimeFrame). << " s\n";
    EV << "sizeof(ceil(Time expiration)): " << sizeof(ceil(time + airtimeFrame)) << "\n";*/

    // Sum time and airtime to calculate expiration of the message.
    // Ceiling to seconds is less reliable than ceiling to ms but
    // for ms in a real deployment use std::chrono instead of std::time

    // Use simulation time instead of real time otherwise simulation stops result in invalid messages.
    // Get current time in ms
    simtime_t time = simTime() * 1000;

    // Calculate expiration time with a tolerance of 50 ms
    simtime_t expirationTime = time + airtimeFrame + 50;

    EV << "Time: " << time << " ms\n";
    EV << "Airtime frame: " << airtimeFrame << " ms\n";
    EV << "Time expiration: " << time + airtimeFrame << " ms\n";
    EV << "ceil(Time expiration): " << ceil(time + airtimeFrame) << " ms\n";
    EV << "ceil(Time expiration): " << ceil(time + airtimeFrame).dbl() << " ms\n";
    EV << "ceil(Time expiration with tolerance): " << ceil(expirationTime).dbl() << " ms\n";

    // Convert time to an array of bytes
    uint8_t timestampBytes[4] = {};
    //uint8_t timestampBytes[8] = {};
    uintToBytes(ceil(expirationTime).dbl(), timestampBytes);
    //uintToBytes((long) ceil(time + airtimeFrame), timestampBytes);

    // Build payload
    memcpy(payload, timestampBytes, 4);
    memcpy(&payload[4], address, IPv4_ADDRESS_SIZE);

    // Delete temporary message used for airtime calculation and
    // create the definitive with correct payload (same size) and MIC
    delete msgOut;
    msgOut = createMessageDownlink(
            "helloGatewayMsg", false,
            endDeviceAddress.data(), IPv4_ADDRESS_SIZE,
            counter, MSG_PORT_HELLO_GATEWAY, payload, LORA_FRAME_SIZE_APP_PAYLOAD,
            commonGatewaysKey,
            spreadingFactor, transmissionPower, bandwidth, channelFrequency);

    // Schedule retransmissions
    cMessage* eventTimeout = new cMessage("timeoutGW");

    // Store the entry in the table (message, repetitions, uplink and downlink frame counter)
    //std::tuple<cPacket*, cMessage*, uint8_t, uint8_t, uint16_t, uint16_t, std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::array<uint8_t, IPv4_ADDRESS_SIZE>> tuple;
    std::tuple<cPacket*, cMessage*, uint8_t, uint8_t, uint16_t, uint16_t, std::array<uint8_t, IPv4_ADDRESS_SIZE>, bool, std::array<uint8_t, IPv4_ADDRESS_SIZE>> tuple;
    std::get<0>(tuple) = msgOut;
    std::get<3>(tuple) = RETRANSMISSIONS - 1; // -1 because it immediately sends a message
    std::get<4>(tuple) = 0; // Use for receive all NEARBY_GATEWAYS messages
    std::get<5>(tuple) = 0; // Use for downlinks
    std::get<7>(tuple) = false;
    map[endDeviceAddress] = tuple;

    // Add entry in the inverted index
    eventTimeoutRetransmissions[eventTimeout] = endDeviceAddress;

    EV << "Sending HELLO_GATEWAY message...\n";
    //sendBroadcast(this, msgOut->dup(), LORA_GATE_OUT);
    sendBroadcastSecurely(this, eventTimeoutChannelTransmissions, msgOut->dup(), LORA_GATE_OUT);
    scheduleAt(simTime() + 5, eventTimeout);
}

// Share set of collected IP addresses with such addresses
void LoRaGateway::sendMessageNearbyGateway() {
    uint8_t payloadSize = sizeof(std::size_t) + IPv4_ADDRESS_SIZE * neighborGatewayAddresses.size();

    /*EV << "sizeof(std::size_t): " << sizeof(std::size_t) << "\n";
    EV << "sizeof(unsigned): " << sizeof(unsigned) << "\n";
    EV << "sizeof(long unsigned): " << sizeof(long unsigned) << "\n";

    unsigned a = rand() % 0xffffffff;
    uint8_t d[4] = {};

    EV << "a (before):\t" << std::hex << a << "\n";
    EV << "a (before):\t" << std::dec << a << "\n";

    uintToBytes(a, d);
    a = bytesToUint(d);

    EV << "a (after):\t" << std::hex << a << "\n";
    EV << "a (after):\t" << std::dec << a << "\n";

    long unsigned aa = rand() % 0xffffffffffffffff;
    uint8_t dd[8] = {};

    EV << "aa (before):\t" << std::hex << aa << "\n";
    EV << "aa (before):\t" << std::dec << aa << "\n";

    luintToBytes(aa, dd);
    aa = bytesToLuint(dd);

    EV << "aa (before):\t" << std::hex << aa << "\n";
    EV << "aa (before):\t" << std::dec << aa << "\n";*/

    uint8_t sizeBytes[sizeof(std::size_t)];
    luintToBytes(neighborGatewayAddresses.size(), sizeBytes);

    // Build payload
    uint8_t payload[payloadSize] = {};
    memcpy(payload, sizeBytes, sizeof(sizeBytes));

    int i = 1;
    for (auto neighborAddress : neighborGatewayAddresses) {
        EV << "Neighbor gateway IP address: "
           << (int) neighborAddress[0] << "."
           << (int) neighborAddress[1] << "."
           << (int) neighborAddress[2] << "."
           << (int) neighborAddress[3]
           << "\n";

        memcpy(&payload[i], neighborAddress.data(), IPv4_ADDRESS_SIZE);
        i += IPv4_ADDRESS_SIZE;
    }

    // Send collected set to collected neighbors
    for (auto neighborAddress : neighborGatewayAddresses)
        sendMessageTcp("nearbyGatewaysMsg", "timeoutNearbyGateways",
                neighborAddress, MSG_PORT_NEARBY_GATEWAYS, payload, payloadSize, nullptr);
}

// Share downlink counter of a given end device with network server
void LoRaGateway::sendMessageCommonKeyAck(
        uint8_t* endDeviceAddress, uint16_t counter, bool ack,
        uint8_t spreadingFactor, uint8_t transmissionPower, float bandwidth, float channelFrequency) {
    uint8_t payloadSize = 1;

    // Build payload
    uint8_t payload[payloadSize] = {
        ack ? ACK : NACK
    };

    const char* name = ack ? "generateCommonKeyACKMsg" : "generateCommonKeyNACKMsg";

    // Create response
    cPacket* msgOut = createMessageDownlink(
            name, false,
            endDeviceAddress, IPv4_ADDRESS_SIZE,
            counter, MSG_PORT_GENERATE_COMMON_KEY, payload, payloadSize,
            commonEndDeviceKey,
            spreadingFactor, transmissionPower, bandwidth, channelFrequency);

    // Send the message in the receive window of the end device
    //sendDelayedBroadcast(this, msgOut, RX_DELAY_1 + 0.1, LORA_GATE_OUT);
    sendDelayedBroadcastSecurely(this, eventTimeoutChannelTransmissions, msgOut, RX_DELAY_1 + 0.1, LORA_GATE_OUT);

    // Do not reschedule the message as it is an ACK
}

// Share downlink counter of a given end device with network server
void LoRaGateway::sendMessageShareCounter(uint8_t* endDeviceAddress, uint16_t counter) {
    uint8_t payloadSize = IPv4_ADDRESS_SIZE + 4;

    uint8_t counter_[4];
    uintToBytes(counter, counter_);

    // Build payload
    uint8_t payload[payloadSize] = {
            endDeviceAddress[0],
            endDeviceAddress[1],
            endDeviceAddress[2],
            endDeviceAddress[3],
            counter_[0],
            counter_[1],
            counter_[2],
            counter_[3]
    };

    sendMessageTcp("shareCounterMsg", "timeoutShareCounter",
            networkServerAddress, MSG_PORT_SYNC_COUNTER, payload, payloadSize, nullptr);
}

// Include the counter in the CONNECTION message instead of sending the share counter message
// because if share counter message is lost but the CONNECTION message is received
// then the subsequent server message may be sent with the old counter
void LoRaGateway::sendMessageConnection(uint8_t* endDeviceAddress, uint8_t requestId, uint16_t counter, uint8_t res) {
    uint8_t payloadSize = IPv4_ADDRESS_SIZE + 6;

    uint8_t counter_[4];
    uintToBytes(counter, counter_);

    // Build payload
    uint8_t payload[payloadSize] = {
            endDeviceAddress[0],
            endDeviceAddress[1],
            endDeviceAddress[2],
            endDeviceAddress[3],
            requestId,
            counter_[0],
            counter_[1],
            counter_[2],
            counter_[3],
            res
    };

    sendMessageTcp("connectionMsg", "timeoutConnection",
        //networkServerAddress, MSG_PORT_CONNECTION, payload, payloadSize, nullptr);
        networkServerAddress, MSG_PORT_CONNECTION_GATEWAY, payload, payloadSize, nullptr);
}

void LoRaGateway::sendMessageDataProfile(uint8_t* endDeviceAddress, const char* dataProfile, int dataProfileSize) {
    if (!endDeviceAddress || !dataProfile)
        return;

    uint8_t payloadSize = IPv4_ADDRESS_SIZE + 5;

    // Build payload
    uint8_t payload[payloadSize] = {
            endDeviceAddress[0],
            endDeviceAddress[1],
            endDeviceAddress[2],
            endDeviceAddress[3]
    };
    memcpy(&payload[4], dataProfile, dataProfileSize);

    sendMessageTcp("shareDataProfileMsg", "timeoutShareDataProfile",
            networkServerAddress, MSG_PORT_SHARE_DATA_PROFILE, payload, payloadSize, nullptr);
}

// Send processed data of a given end device to network server
void LoRaGateway::sendMessageProcessedData(
        uint8_t* endDeviceAddress, uint8_t processedData, uint8_t* timestamp,
        const char* dataProfile, int dataProfileSize) {
    if (!endDeviceAddress || !timestamp)
        return;

    uint8_t payloadSize = IPv4_ADDRESS_SIZE + 5 + dataProfileSize;

    // Build payload
    uint8_t payload[payloadSize] = {
            endDeviceAddress[0],
            endDeviceAddress[1],
            endDeviceAddress[2],
            endDeviceAddress[3],
            processedData,
            timestamp[0],
            timestamp[1],
            timestamp[2],
            timestamp[3]
    };
    memcpy(&payload[9], dataProfile, dataProfileSize);

    // UDP vs TCP
    sendMessageTcp("processedDataMsg", "timeoutProcessedData",
            networkServerAddress, MSG_PORT_PROCESSED_DATA, payload, payloadSize, nullptr);
}

void LoRaGateway::sendMessageTcp(
        const char* messageName, const char* timeoutName,
        std::array<uint8_t, IPv4_ADDRESS_SIZE>& destAddress, unsigned port,
        uint8_t* payload, unsigned payloadSize, cPacket* encapsulatedPacket) {
    // Check if the network server is already in the map for TCP/IP connections
    //std::tuple<cPacket*, cMessage*, uint32_t, uint32_t, uint32_t> tuple;
    std::tuple<std::list<cPacket*>, cMessage*, uint32_t, uint32_t, uint32_t> tuple;

    auto it = mapTcp.find(destAddress);
    if (it != mapTcp.end()) {
        // The network server is in the map, get the entry
        tuple = it->second;

        // Check if a timeout exists for a previous transmission
        if (std::get<1>(tuple)) {
            // Cancel old timeout
            cancelEvent(std::get<1>(tuple));

            // Do not remove the entry from the table or delete the timer to reuse it (session)

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

    // Send message to destination
    sendMessageIp(msgOut, destAddress);

    scheduleAt(simTime() + timeoutTcp, std::get<1>(tuple));
}

void LoRaGateway::sendMessageIp(cPacket* msg, std::array<uint8_t, IPv4_ADDRESS_SIZE>& destAddress, simtime_t delay/*=0*/) {
    //cPacket* msgOut = sendDuplicate ? msg->dup() : msg;

    // In OMNeT++ the gateway does not know which is the gate index associated to the IP address.
    // Check if the address is in the routing table to retrieve the gate index
    auto itRT = routingTable.find(destAddress);
    if (itRT != routingTable.end())
        delay > 0 ? //sendDelayed(msg->dup(), delay, IP_GATE_OUT, itRT->second) :
                    sendDelayedSecurely(this, eventTimeoutChannelTransmissions, msg->dup(), delay, IP_GATE_OUT, itRT->second) :
                    //sendDelayedSecurely(this, eventTimeoutChannelTransmissions, msgOut, delay, IP_GATE_OUT, itRT->second) :
                    //send(msg->dup(), IP_GATE_OUT, itRT->second);
                    sendSecurely(this, eventTimeoutChannelTransmissions, msg->dup(), IP_GATE_OUT, itRT->second);
                    //sendSecurely(this, eventTimeoutChannelTransmissions, msgOut, IP_GATE_OUT, itRT->second);
    else
        // The address is not in the routing table, send in broadcast
        delay > 0 ? //sendDelayedBroadcast(this, msg->dup(), delay, IP_GATE_OUT) :
                    sendDelayedBroadcastSecurely(this, eventTimeoutChannelTransmissions, msg->dup(), delay, IP_GATE_OUT) :
                    //sendDelayedBroadcastSecurely(this, eventTimeoutChannelTransmissions, msgOut, delay, IP_GATE_OUT) :
                    //sendBroadcast(this, msg->dup(), IP_GATE_OUT);
                    sendBroadcastSecurely(this, eventTimeoutChannelTransmissions, msg->dup(), IP_GATE_OUT);
                    //sendBroadcastSecurely(this, eventTimeoutChannelTransmissions, msgOut, IP_GATE_OUT);
}

// Forward LoRa message to destination over IP
void LoRaGateway::forwardMessageIp(
        cMessage* msg, const char* name,
        std::array<uint8_t, IPv4_ADDRESS_SIZE>& destAddress, unsigned port, bool reencrypt, simtime_t delay/*=0*/) {
    // Check if the message must be decrypted and re-encrypted with a different key
    if (reencrypt) {
        LoRaPhysicalFrame* phyMsg = dynamic_cast<LoRaPhysicalFrame*>(msg);
        if (!phyMsg)
           return;

        LoRaDatalinkFrame* dlMsg = dynamic_cast<LoRaDatalinkFrame*>(phyMsg->getEncapsulatedPacket());
        if (!dlMsg)
           return;

        // Decapsulate LoRa App layer
        LoRaAppUplinkFrame* appMsg = dynamic_cast<LoRaAppUplinkFrame*>(dlMsg->decapsulate());
        if (!appMsg)
           return;

        //uint8_t key[KEY_LORAWAN_SIZE];
        uint8_t* key;

        // Check if the destination gateway is a neighbor
        if (neighborGatewayAddresses.find(destAddress) != neighborGatewayAddresses.end()) {
            // Iterate clusters (subsets of the previous set) to find the session key
            for (auto itClusters : clusters) {
                auto commonSKey = itClusters.first;
                auto cluster    = itClusters.second;

                if (cluster.find(destAddress) != cluster.end()) {
                    key = commonSKey.data();
                    break;
                }
            }
        }
        else
            // Use the root key (useless multi-hop to not employ the root key)
            key = commonGatewaysKey;

        EV << "Encrypting message...\n";

        // Encrypt
        // ...

        EV << "Recalculating MIC...\n";

        // Recalculate MIC
        calculateMIC(dlMsg, appMsg, key);
    }

    // Create UDP/IP packet
    cPacket* msgOut = createMessageIp(
        name,
        address, destAddress.data(), IPv4_ADDRESS_SIZE,
        nullptr, 0,
        dynamic_cast<cPacket*> (msg));
    msgOut = createMessageUdp(
        name,
        port, port,
        nullptr, 0,
        msgOut);

    // Send the message to the destination
    sendMessageIp(msgOut, destAddress, delay);
    // TODO: messages without reencrypt do not need duplicates
}

// Forward LoRa message to destination over IP
/*void LoRaGateway::forwardDelayedMessageIp(
        cMessage* msg, const char* name,
        std::array<uint8_t, IPv4_ADDRESS_SIZE>& destAddress, unsigned port, bool reencrypt, simtime_t delay) {

}*/

// Forward LoRa message to neighbors over IP
void LoRaGateway::forwardMessageToNeighbors(cMessage* msg, uint8_t* decryptKey) {
    // Check if the message is a LoRa frame (should be)
    LoRaPhysicalFrame* phyMsg = dynamic_cast<LoRaPhysicalFrame*>(msg);
    if (!phyMsg)
       return;

    LoRaDatalinkFrame* dlMsg = dynamic_cast<LoRaDatalinkFrame*>(phyMsg->getEncapsulatedPacket());
    if (!dlMsg)
       return;

    // Decapsulate LoRa App layer
    LoRaAppUplinkFrame* appMsg = dynamic_cast<LoRaAppUplinkFrame*>(dlMsg->decapsulate());
    if (!appMsg)
       return;

    // Forward to neighbors
    for (auto itClusters : clusters) {
       auto commonSKey = itClusters.first;

       // Check if the common session key is the same used for decrypting the incoming message
       if (!memcmp(decryptKey, commonSKey.data(), commonSKey.size()))
           // Avoid to send to cluster from which the message was received
           continue;

       EV << "Encrypting message...\n";

       // Encrypt
       // ...

       EV << "Recalculating MIC...\n";

       // Recalculate MIC
       calculateMIC(dlMsg, appMsg, commonSKey.data());

       // Forward to peers in the cluster
       for (auto neighborAddress : itClusters.second)
           forwardMessageIp(msg, "forwardMsg", neighborAddress, MSG_PORT_FORWARD_OVER_IP, false);
    }
}

uint8_t LoRaGateway::getCpuLoad() {
    return (uint8_t) (cpuLoad * 100);
    //return 100; // XXX: for testing no gateway metrics requirements
}

uint8_t LoRaGateway::getGpuLoad() {
    return (uint8_t) (gpuLoad * 100);
}

uint8_t LoRaGateway::getRamLoad() {
    return (uint8_t) (ramLoad * 100);
}

uint8_t LoRaGateway::getStorageLoad() {
    return (uint8_t) ((double)storageOccupied/STORAGE_SIZE * 100.0);
}

uint8_t LoRaGateway::getNetworkIn() {
    return (uint8_t) networkIn / 1000;
}

uint8_t LoRaGateway::getNetworkOut() {
    return (uint8_t) networkOut / 1000;
}

bool LoRaGateway::checkResources() {
    return getCpuLoad() < CPU_LOAD_THRESHOLD &&
           getGpuLoad() < GPU_LOAD_THRESHOLD &&
           getRamLoad() < RAM_LOAD_THRESHOLD &&
           getStorageLoad() < STORAGE_LOAD_THRESHOLD &&
           getNetworkIn() < NETWORK_IN_THRESHOLD &&
           getNetworkOut() < NETWORK_OUT_THRESHOLD;
}

void LoRaGateway::updateResources() {
    // xxx: uniform vs normal
    cpuLoad = uniform(0, 0.03); // 0.08
    gpuLoad = uniform(0, 0.02); // 0.05
    ramLoad = uniform(0.04, 0.06); //0.05;

    for (unsigned i=1; i <= associations.size(); i++) {
        //cpuLoad += uniform(0.01, 0.15);
        //gpuLoad += uniform(0.01, 0.1);
        //ramLoad += uniform(0.02, 0.04);

        cpuLoad += uniform(0.01, 0.03);
        gpuLoad += uniform(0.01, 0.02);
        ramLoad += uniform(0.01, 0.02);
    }

    networkIn  = 0;
    networkOut = 0;
}


// Return true if the message survives to interference, false otherwise
bool LoRaGateway::checkLoRaInterference(cMessage* msg) {
    if (!msg)
        return false;

    //std::tuple<int, float> tuple;
    std::tuple<double, float> tuple;

    // Check if the message is subjected to interference
    //auto it = interferences.find(msg);
    long msgId = msg->getTreeId();
    auto it = interferences.find(msgId);
    if (it == interferences.end()) {
        // No interference has been applied to the message because no other nearby end device sent a message during
        // the time on air. However, the external noise must always be applied

        // Get message RSSI
        LoRaEndDevice* device = dynamic_cast<LoRaEndDevice*>(msg->getSenderModule());
        if (!device)
            return false;

        int rssi = calculateRSSI(this, device->getPosX(), device->getPosY(), posX, posY, EV);
        EV << "Message RSSI: " << rssi << "\n";

        // Apply external noise and insert the message in interferences
        tuple = applyExternalNoise(msg, rssi);
        if (std::get<1>(tuple) < 0)
            return false;
    }
    else
        tuple = it->second;

    // Remove the message from interferences as it is arrived and is no longer over the air
    // otherwise the interference is overestimated because probabilities wrongly sum between
    // retransmitted messages
    interferences.erase(msgId);

    EV << "Drop probability: " << std::get<1>(tuple) << "\n";

    if (uniform(0, 1) <= std::get<1>(tuple)) {
        EV << "Message lost!\n";

        // Make animation more informative
        bubble("Message lost!");

        // Delete the message as in OMNeT++ once sent out,
        // a message no longer belongs to the sender module and
        // it is taken over by the simulation kernel, and will eventually be delivered to the destination module.
        // Once the message arrives in the destination module, that module will have full authority over it
        delete msg;

        // Send signal for statistic collection
        emit(signalLost, 1u);
        emit(signalLostCount, ++messagesLostCount);
        emit(signalLostLoRa, 1u);
        emit(signalLostLoRaCount, ++messagesLostLoRaCount);

        return false;
    }

    return true;
}

// Calculate BER probability according to the experimental BER curves
float LoRaGateway::calculateProbabilityBER(int maxBer, int snrThreshold, int snr) {
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
//std::tuple<int, float> LoRaGateway::applyExternalNoise(cMessage* msg, int rssi, uint8_t sf, float bw) {
std::tuple<double, float> LoRaGateway::applyExternalNoise(cMessage* msg, int rssi, uint8_t sf, float bw) {
    if (!msg)
        return std::tuple<int, float> {1, -1};

    EV << "Applying external noise...\n";

    // Thermal noise
    double thermalNoisePower = THERMAL_NOISE_POWER_SPECTRAL_DENSITY + NOISE_FIGURE_RX + 10 * log10(bw*1000);
    EV << "Thermal noise: " << thermalNoisePower << "\n";

    // Additive White Gaussian Noise (AWGN)
    //thermalNoisePower = normal(0, thermalNoisePower);
    //EV << "Thermal noise (AWGN): " << thermalNoisePower << "\n";

    // AWGN disabled because has zero mean and is not suitable to compare with RSSI that is negative (too influence)


    // External interference due to other technologies operating on the same unlicensed frequencies
    // (e.g. Wi-Fi, Bluetooth)
    float externalInterferenceProbability = 1 - pow(
            (1 - PROBABILITY_OCCURRENCE_EXTERNAL_INTERFERENCE),
            ceil(bw/EXTERNAL_INTERFERENCE_BANDWIDTH));

    EV << "externalInterferenceProbability: " << externalInterferenceProbability << "\n";

    double externalInterferencePower = 0;

    // Check if the external probability occurs
    if (((rand() + (int) uniform(0,100)) % 101) / 100.0 < externalInterferenceProbability) {
        // Interval of interference power in dBm considered for external interference is [-105, -65] almost [-105, -80]
        std::random_device rd {};
        std::mt19937 generator {rd()};

        // Con questi parametri la curva non è totalmente corretta perchè l'intervallo è [-108, -98]
        // ma ci va vicino. Penso che per essere più corretta serve GEV type II e non type I ma
        // non è definita in c++ quindi ok così
        float location = -105;
        float scale    = 1.65;
        std::extreme_value_distribution<float> distribution(location, scale);

        //for (int i=0; i<20; i++)
        //    EV << "distribution(generator): " << distribution(generator) << "\n";

        externalInterferencePower = distribution(generator) + 10 * log10(PROBABILITY_OCCURRENCE_EXTERNAL_INTERFERENCE);
        EV << "External Interference Power: " << externalInterferencePower << "\n";

        //EV << "10 * log10(PROBABILITY_OCCURRENCE_EXTERNAL_INTERFERENCE): " << 10 * log10(PROBABILITY_OCCURRENCE_EXTERNAL_INTERFERENCE) << "\n";
    }

    // Calculate Signal to Interference plus Noise Ratio for the signal.
    // To sum Noise and Interference, convert them from dBm to mW
    double noiseAndInterferencePower = (pow(10, thermalNoisePower/10) + pow(10, externalInterferencePower/10));
    EV << "NoiseAndInterference power: " << noiseAndInterferencePower << " mW\n";
    // Then convert the sum from mW to dBm
    //int sinr = rssi - 10 * log10(pow(10, thermalNoisePower/10) + pow(10, externalInterferencePower/10));
    int sinr = rssi - 10 * log10(noiseAndInterferencePower);
    int sinrThreshold = 0;
    int maxBer        = 0;
    EV << "SINR: " << sinr << "\n";

    //EV << "thermalNoisePower (mW): " << pow(10, thermalNoisePower/10) << "\n";
    //EV << "externalInterferencePower (mW): " << pow(10, externalInterferencePower/10) << "\n";

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
        EV << "The message is affected by external interference!\n";

        // Calculate the probability the message is dropped
        probDrop = calculateProbabilityBER(maxBer, sinrThreshold, sinr);
        EV << "The message is dropped with a probability of " << probDrop <<"\n";
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
//std::tuple<int, float> LoRaGateway::applyExternalNoise(cMessage* msg, int rssi) {
std::tuple<double, float> LoRaGateway::applyExternalNoise(cMessage* msg, int rssi) {
    if (!msg)
        return std::tuple<int, float> {-1, -1};

    // Get message parameters (sent by source module)
    LoRaPhysicalFrame* msg_ = dynamic_cast<LoRaPhysicalFrame*>(msg);
    if (!msg_)
        return std::tuple<int, float> {-1, -1};

    return applyExternalNoise(msg, rssi, msg_->getSpreadingFactor(), msg_->getBandwidth());
}


// ************** PUBLIC METHODS **************
unsigned LoRaGateway::getPosX() {
    return posX;
}
unsigned LoRaGateway::getPosY() {
    return posY;
}

const char* LoRaGateway::getLoRaGateIn() {
    return LORA_GATE_IN;
}

const char* LoRaGateway::getLoRaGateOut() {
    return LORA_GATE_OUT;
}

const char* LoRaGateway::getLoRaGateBasename() {
    return LORA_GATE_BASENAME;
}


void LoRaGateway::addNeighborDevice(int neighborId) {
    if (neighborId < 0)
        return;

    EV << "============= addNeighborDevice =============\n";

    EV << "neighborId: " << neighborId << "\n";
    neighborDevicesInterferences.insert(neighborId);

    EV << "=====================================\n";
}


void LoRaGateway::handleInterference(
        //cPacket* msgInterference, std::list<std::tuple<cPacket*, bool>>& interferedMessages) {
        cPacket* msgInterference, std::list<std::tuple<cPacket*, int, bool>>& interferedMessages) {
    if (!msgInterference || interferedMessages.size() == 0)
        return;

    //EV << "handleInterference - msgInterference->getSenderModuleId(): " << msgInterference->getSenderModuleId() << "\n";

    // Check if the end device is in the radio range (should be)
    if (neighborDevicesInterferences.find(msgInterference->getSenderModuleId()) == neighborDevicesInterferences.end())
        return;

    EV << "============= handleInterference =============\n";

    // Get possible interferer message parameters (sent by source module)
    LoRaPhysicalFrame* msgInterference_ = dynamic_cast<LoRaPhysicalFrame*>(msgInterference);
    if (!msgInterference_)
        return;

    uint8_t sfInterference = msgInterference_->getSpreadingFactor();
    float bwInterference   = msgInterference_->getBandwidth();
    float cfInterference   = msgInterference_->getChannelFrequency();

    EV << "Possible interference spreading factor: " << (int) sfInterference << "\n";
    EV << "Possible interference bandwidth: " << bwInterference << "\n";
    EV << "Possible interference ch frequency: " << cfInterference << "\n";

    // Apply external noise to the possible interferer message.
    // This cannot be applied twice or more because only this time it is the interferer

    // Get possible interferer message RSSI
    LoRaEndDevice* deviceInterference = dynamic_cast<LoRaEndDevice*>(msgInterference_->getSenderModule());
    if (!deviceInterference)
        return;

    int rssiInterference = calculateRSSI(this, deviceInterference->getPosX(), deviceInterference->getPosY(), posX, posY, EV);
    EV << "Possible interference RSSI: " << rssiInterference << "\n";

    auto tupleInterference = applyExternalNoise(msgInterference_, rssiInterference, sfInterference, bwInterference);
    if (std::get<1>(tupleInterference) < 0)
        return;

    // Get possible interference shared ID between original message and duplicates
    long interferenceId = msgInterference->getTreeId();

    // Compare possible interference message with possible interfered messages
    // (collected via notifications by the end device source before it sent its message and
    // other time on airs are not ended)
    for (auto tuple : interferedMessages) {
        //EV << "Interfered device ID: " << std::get<1>(tuple) << "\n";

        // Check if the end device is in the radio range
        if (neighborDevicesInterferences.find(std::get<1>(tuple)) == neighborDevicesInterferences.end())
            continue;

        // Get possible interfered message parameters
        LoRaPhysicalFrame* msgSignal = dynamic_cast<LoRaPhysicalFrame*>(std::get<0>(tuple));
        if (!msgSignal)
            return;

        uint8_t sfSignal = msgSignal->getSpreadingFactor();
        float bwSignal   = msgSignal->getBandwidth();
        float cfSignal   = msgSignal->getChannelFrequency();
        bool isInterferingPreamble = std::get<2>(tuple);

        EV << "Signal spreading factor: " << (int) sfSignal << "\n";
        EV << "Signal bandwidth: " << bwSignal << "\n";
        EV << "Signal ch frequency: " << cfSignal << "\n";

        //===================================
        /*LoRaEndDevice* deviceSignal = dynamic_cast<LoRaEndDevice*>(msgSignal->getSenderModule());
        if (!deviceSignal)
            return;

        int rssiSignal = calculateRSSI(this, deviceSignal->getPosX(), deviceSignal->getPosY(), posX, posY, EV);
        EV << "Signal RSSI: " << rssiSignal << "\n";

        //int snrSignal = applyExternalNoise(msgSignal, rssiSignal, sfSignal, bwSignal);

        // Check if noise has already been applied to this signal
        auto it = interferences.find(msgSignal);
        auto tupleSignal = it == interferences.end() ?
                applyExternalNoise(msgSignal, rssiSignal, sfSignal, bwSignal) : it->second;//*/
        //===================================

        //===================================
        /*std::tuple<int, float> tupleSignal;

        // Check if noise has already been applied to this signal
        //auto it = interferences.find(msgSignal);
        long signalId = msgSignal->getTreeId();
        auto it = interferences.find(signalId);

        if (it == interferences.end()) {
            // Get possible interfered message RSSI
            LoRaEndDevice* deviceSignal = dynamic_cast<LoRaEndDevice*>(msgSignal->getSenderModule());
            if (!deviceSignal)
                return;

            int rssiSignal = calculateRSSI(this, deviceSignal->getPosX(), deviceSignal->getPosY(), posX, posY, EV);
            EV << "Signal RSSI: " << rssiSignal << "\n";

            tupleSignal = applyExternalNoise(msgSignal, rssiSignal, sfSignal, bwSignal);
            if (std::get<1>(tupleSignal) < 0)
                continue;
        }
        else {
            tupleSignal = it->second;
            EV << "Signal SINR: " << std::get<0>(tupleSignal) << "\n";
        }*/
        //===================================

        //===================================
        // Get possible interfered message RSSI
        LoRaEndDevice* deviceSignal = dynamic_cast<LoRaEndDevice*>(msgSignal->getSenderModule());
        if (!deviceSignal)
            return;

        int rssiSignal = calculateRSSI(this, deviceSignal->getPosX(), deviceSignal->getPosY(), posX, posY, EV);
        EV << "Signal RSSI: " << rssiSignal << "\n";

        std::tuple<double, float> tupleSignal;

        // Check if noise has already been applied to this signal
        //auto it = interferences.find(msgSignal);
        long signalId = msgSignal->getTreeId();
        auto it = interferences.find(signalId);

        if (it == interferences.end()) {
            tupleSignal = applyExternalNoise(msgSignal, rssiSignal, sfSignal, bwSignal);
            if (std::get<1>(tupleSignal) < 0)
                continue;
        }
        else {
            tupleSignal = it->second;
            EV << "NoiseAndInterference power: " << std::get<0>(tupleSignal) << " mW\n";
        }
        //===================================

        // Check if the two messages are transmitted using the same bandwidth
        if (bwInterference == bwSignal) {
            EV << "The signal and the possible interference are transmitted in the same bandwidth!\n";

            // Check if the two messages are transmitted using the same channel
            if (cfInterference == cfSignal) {
                EV << "The signal and the possible interference are transmitted in the same ch frequency!\n";

                //handleIntraChannelInterference();

                // Signal Interference Ratio calculated in dB
                //const int sir = std::get<0>(tupleSignal) - std::get<0>(tupleInterference);
                // Sum the interference in mW to other interferences and noise in mW
                double noiseAndInterferencePower = std::get<0>(tupleSignal) + pow(10, rssiInterference/10);                EV << "NoiseAndInterferences power: " << noiseAndInterferencePower << " mW\n";
                const int sir = rssiSignal - 10*log10(noiseAndInterferencePower);
                EV << "SIR: " << sir << "\n";

                // Update the SNIR of the interfered message to sum up the effect of multiple interferer messages
                //std::get<0>(tupleSignal) = sir;

                // Update the sum of noise and interferences in mW
                std::get<0>(tupleSignal) = noiseAndInterferencePower;

                int sirThreshold = INTERFERENCE_SIR_THRESHOLD;

                // Check if the two messages are transmitted using the same spreading factor
                if (sfInterference == sfSignal) {
                    // Strong Interference
                    EV << "The signal and the possible interference are transmitted with the same spreading factor! STRONG INTERFERENCE\n";

                    if (isInterferingPreamble) {
                        if (sir >= sirThreshold) {
                            // Interference is dropped,
                            // Signal is dropped with a probability of 8-15%

                            //interferences[msgInterference] = 1;
                            //interferences[msgSignal]       += 0.1;

                            std::get<1>(tupleSignal)       += 0.1;
                            std::get<1>(tupleInterference) = 1;
                        }
                        else {
                            // Signal and Interference are dropped
                            //interferences[msgInterference] = 1;
                            //interferences[msgSignal]       = 1;

                            std::get<1>(tupleSignal)       = 1;
                            std::get<1>(tupleInterference) = 1;
                        }
                    }
                    else {
                        if (sir >= sirThreshold) {
                            // Signal is dropped with a probability of 0-5%,
                            // Interference is dropped
                            //interferences[msgInterference] = 1;
                            //interferences[msgSignal]       += 0.03;

                            std::get<1>(tupleSignal)      += 0.03;
                            std::get<1>(tupleInterference) = 1;
                        }
                        else {
                            // Signal and Interference are dropped
                            //interferences[msgInterference] = 1;
                            //interferences[msgSignal]       = 1;

                            std::get<1>(tupleSignal)       = 1;
                            std::get<1>(tupleInterference) = 1;
                        }
                    }

                    // Send signal for statistic collection
                    emit(signalInterference, 1u);
                    emit(signalInterferenceCount, ++interferencesCount);
                }

                // Else the two messages are transmitted using different spreading factors.
                // Check if the second message is transmitted during the preamble of the first
                else if (isInterferingPreamble) {
                    EV << "The signal and the possible interference are transmitted with different spreading factors! WEAK INTERFERENCE\n";
                    // Noise, imperfect orthogonality, weak interference
                    int maxBer   = 0;

                    switch (sfSignal) {
                        case 7:
                            sirThreshold = -9; // (-8-9-9-9-9)/5
                            maxBer = 44;       // (41+42+43+45+49)/5
                            break;
                        case 8:
                            sirThreshold = -12; // (-11-11-12-13-13)/5
                            maxBer = 44;
                            break;
                        case 9:
                            sirThreshold = -14; // (-15-13-13-14-15)/5
                            maxBer = 43;        // (33+38+45+50+50)/5
                            break;
                        case 10:
                            sirThreshold = -18; // (-19-18-17-17-18)/5
                            maxBer = 42;
                            break;
                        case 11:
                            sirThreshold = -21; // (-22-22-21-20-20)/5
                            maxBer = 42;
                            break;
                        case 12:
                            sirThreshold = -24; // (-25-25-25-24-23)/5
                            maxBer = 41;        // (38+40+42+43+43)/5
                            break;
                        default:
                            throw std::invalid_argument("Invalid spreading factor");
                    }

                    if (sir >= sirThreshold) {
                        // Signal survives, Interference?
                        // Based on same SF study, interferer is always dropped
                        //interferences[msgInterference] = 1;
                        //interferences[msgSignal]       += 0;

                        //std::get<1>(tupleSignal)       += 0;
                        std::get<1>(tupleInterference) = 1;
                    }
                    else {
                        // Signal is dropped, Interference?
                        // Based on same SF study, interferer is always dropped
                        //interferences[msgInterference] = 1;
                        //interferences[msgSignal]       = 1;

                        std::get<1>(tupleSignal)       = calculateProbabilityBER(maxBer, sirThreshold, sir);
                        std::get<1>(tupleInterference) = 1;
                    }

                    // Send signal for statistic collection
                    emit(signalInterference, 1u);
                    emit(signalInterferenceCount, ++interferencesCount);
                }

                // There is no experimental case with different SFs
                // and collision after the end of the preamble of interfered message
            }
            else {
                // The two messages are transmitted using different channels.
                // Check if the interferer message is transmitted during preamble of the signal message and
                // using the same spreading factor

                EV << "The signal and the possible interference are transmitted in different ch frequencies!\n";

                //handleInterChannelInterference();

                if (isInterferingPreamble && sfInterference == sfSignal) {
                    EV << "The signal and the possible interference are transmitted with the same spreading factor!\n";

                    if (sfInterference >= 10) {
                        // Strong interference
                        float chDistance = abs(cfInterference - cfSignal);

                        if (chDistance <= 0.2) {
                            EV << "INTERFERENCE\n";

                            // Interference between 95% and 99%
                            // SF12 and bw 500 KHz -> interference 85%
                            if (bwInterference == BANDWIDTH_500) {
                                //interferences[msgInterference] += 0.85;
                                //interferences[msgSignal] += 0.85;

                                std::get<1>(tupleSignal)       += 0.85;
                                std::get<1>(tupleInterference) += 0.85;
                            }
                            else {
                                //interferences[msgInterference] += 0.97;
                                //interferences[msgSignal] += 0.97;

                                std::get<1>(tupleSignal)       += 0.97;
                                std::get<1>(tupleInterference) += 0.97;
                            }

                            // Send signal for statistic collection
                            emit(signalInterference, 1u);
                            emit(signalInterferenceCount, ++interferencesCount);
                        }
                        else if (chDistance <= 0.4) {
                            // SF12 and bw 125 KHz -> Interference 30%
                            // SF11 and bw 125 KHz -> Interference 20%
                            // SF10 and bw 125 KHz -> Interference 5%
                            // SF12 and bw 250 KHz -> Interference 1%
                            // bw 500 KHz -> interference 0%
                            if (bwInterference == BANDWIDTH_125) {
                                EV << "INTERFERENCE\n";

                                switch (sfInterference) {
                                    case 12:
                                        //interferences[msgInterference] += 0.3;
                                        //interferences[msgSignal]       += 0.3;
                                        std::get<1>(tupleSignal)       += 0.3;
                                        std::get<1>(tupleInterference) += 0.3;
                                        break;
                                    case 11:
                                        //interferences[msgInterference] += 0.2;
                                        //interferences[msgSignal]       += 0.2;
                                        std::get<1>(tupleSignal)       += 0.2;
                                        std::get<1>(tupleInterference) += 0.2;
                                        break;
                                    case 10:
                                        //interferences[msgInterference] += 0.05;
                                        //interferences[msgSignal]       += 0.05;
                                        std::get<1>(tupleSignal)       += 0.05;
                                        std::get<1>(tupleInterference) += 0.05;
                                        break;
                                    default:
                                        throw std::invalid_argument("Invalid spreading factor");
                                }

                                // Send signal for statistic collection
                                emit(signalInterference, 1u);
                                emit(signalInterferenceCount, ++interferencesCount);
                            }
                            else if (bwInterference == BANDWIDTH_250) {
                                EV << "INTERFERENCE\n";

                                //interferences[msgInterference] += 0.01;
                                //interferences[msgSignal]       += 0.01;
                                std::get<1>(tupleSignal)       += 0.01;
                                std::get<1>(tupleInterference) += 0.01;

                                // Send signal for statistic collection
                                emit(signalInterference, 1u);
                                emit(signalInterferenceCount, ++interferencesCount);
                            }
                        }
                        else if (chDistance <= 0.6) {
                            // SF12 and bw 125 KHz -> Interference 5%
                            // SF11 and bw 125 KHz -> Interference 0%
                            // SF10 and bw 125 KHz -> Interference 0%
                            // bw 250 KHz -> Interference 0%
                            if (bwInterference == BANDWIDTH_125) {
                                EV << "INTERFERENCE\n";

                                //interferences[msgInterference] += 0.05;
                                //interferences[msgSignal]       += 0.05;
                                std::get<1>(tupleSignal)       += 0.05;
                                std::get<1>(tupleInterference) += 0.05;

                                // Send signal for statistic collection
                                emit(signalInterference, 1u);
                                emit(signalInterferenceCount, ++interferencesCount);
                            }
                        }
                    }
                    /*else {
                        // Weak interference
                        continue;
                    }*/
                }
            }

            // Store updated entries
            //interferences[msgSignal]       = tupleSignal;
            //interferences[msgInterference] = tupleInterference;
            interferences[signalId]       = tupleSignal;
            interferences[interferenceId] = tupleInterference;
        }

        // Send signal for statistic collection
        emit(signalInterferencePossible, 1u);
        emit(signalInterferencePossibleCount, ++interferencesPossibleCount);
    }


    EV << "===========================================\n";
}

// Receive notification from end device that completed the protocol run to stop the simulation when all have finished
void LoRaGateway::deviceFinish() {
    cModule* parent = getParentModule();
    const int nEndDevices = parent->par("nEndDevices").intValue();

    if (++endDeviceTerminations == nEndDevices) {
        //endSimulation();
        cancelAndDelete(eventTimeoutResources);
        eventTimeoutResources = nullptr;
    }
}

//}
