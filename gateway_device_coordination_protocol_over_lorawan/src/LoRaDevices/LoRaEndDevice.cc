#include "LoRaGateway.h"
#include "LoRaEndDevice.h"


//namespace masterthesis {


// The module class needs to be registered with OMNeT++
Define_Module(LoRaEndDevice);


LoRaEndDevice::LoRaEndDevice() {
    // Set the pointer to nullptr, so that the destructor won't crash
    // even if initialize() doesn't get called because of a runtime
    // error or user cancellation during the startup process.

    eventTimeoutTX = eventTimeoutRX1 = eventTimeoutRX2 = eventTimeoutDutyCycle = nullptr;
    msgOut = nullptr;
}

LoRaEndDevice::~LoRaEndDevice() {
    // Dispose of dynamically allocated the objects
    cancelAndDelete(eventTimeoutTX);
    cancelAndDelete(eventTimeoutRX1);
    cancelAndDelete(eventTimeoutRX2);
    cancelAndDelete(eventTimeoutDutyCycle);

    /*if (msgOut && msgOut->getOwner() == this)
        delete msgOut;

    for (auto it : statsMessages)
        if (it.second && it.second->getOwner() == this)
            delete it.second;

    for (auto it : neighborMessages) {
        auto msg = std::get<0>(it);

        if (msg && msg->getOwner() == this)
            delete msg;
    }*/

    statsMessages.clear();
    gateways2ACK.clear();
    sortedGateways.clear();
    bandwidths.clear();
    channelFrequencies.clear();
    neighborDevicesInterferences.clear();
    neighborGatewaysInterferences.clear();
    neighborMessages.clear();
}

void LoRaEndDevice::initialize() {
    // Initialize is called at the beginning of the simulation
    EV << "Initializing end device...\n";

    // Initialize class variables
    srand(time(NULL));

    // Set device address (now it is assigned by the network server).
    // Use the unique component's ID (it's > 2 because 0 is the simulation, 1 the network server, 2..m the gateways) instead of the non-unique module vector index (it's >= 0)
    int id = getId() - 2;
    /*int j = id/254;
    address[0] = IPv4_ADDRESS_NETWORK_SERVER_0;
    address[1] = IPv4_ADDRESS_NETWORK_SERVER_1;
    address[2] = (IPv4_ADDRESS_NETWORK_SERVER_2 + j) % 256;
    address[3] = 1 + id%254;

    EV << "End device IP address: "
        << (int) address[0] << "."
        << (int) address[1] << "."
        << (int) address[2] << "."
        << (int) address[3]
        << "\n";*/

    // Set Join EUI
    for (int i=0; i < EUI_SIZE; i++) {
        joinEUI[i] = i+10;
        //devEUI[i]  = (rand() + (int) uniform(0, 100)) % 256;
        devEUI[i]  = 0;
    }

    // Set Device EUI
    uintToBytes(id, &devEUI[4]);

    // Set network server ID
    memset(networkId, 0, sizeof(networkId));

    // Different nwkKey per device to activate multiple device at the same time
    // and the same must be stored on the network server
    // (limited to 2^(sizeof(int)*8) = 2^32 end devices since getId() returns an int)
    memset(nwkKey, 0, sizeof(nwkKey));
    uintToBytes(id, nwkKey);

    /*for (unsigned i=0; i<1e3; i++) {
        uint8_t k[KEY_LORAWAN_SIZE] = {};

        uintToBytes(i, k);
        printKey(k, sizeof(k), "k", EV);
    }*/

    //memcpy(nwkKey, "Network_root_key", sizeof(nwkKey));
    memcpy(appKey, "Application_root_key", sizeof(appKey));

    memset(nwkSKey, 0, sizeof(nwkSKey));
    memset(appSKey, 0, sizeof(appSKey));

    memcpy(commonKey, "Common_root_key", sizeof(commonKey));
    memset(commonSKey, 0, sizeof(commonSKey));

    memcpy(associationKey, "Association_root_key", sizeof(associationKey));
    memset(associationSKey, 0, sizeof(associationSKey));

    printKey(joinEUI, sizeof(joinEUI), "joinEUI", EV);
    printKey(devEUI, sizeof(devEUI), "devEUI", EV);
    printKey(networkId, sizeof(networkId), "networkId", EV);

    printKey(nwkKey, sizeof(nwkKey), "nwkKey", EV);
    printKey(appKey, sizeof(appKey), "appKey", EV);
    printKey(commonKey, sizeof(commonKey), "commonKey", EV);
    printKey(associationKey, sizeof(associationKey), "associationKey", EV);


    fCntUp   = 0;
    fCntDown = 0;
    nonceDev = 0;

    transmissionWindowState = WINDOW_TX;

    //messageReceived  = MSG_NULL;
    messageReceived  = false;
    stage            = STAGE_ACTIVATION;
    retransmissions  = RETRANSMISSIONS;
    requestId        = 0;
    levelNumber      = 0;
    //isPairingAccepted = false;
    //isPairingVerified = false;
    searchNewGateway = false;

    locationStringIndex = 0;

    // Convert component ID to string
    const size_t intLen = 4 * sizeof(int);
    char id_[intLen] = {};
    snprintf(id_, intLen, "%d", id);

    strncpy(locationString, "Sapienza/Building/", LOCATION_STRING_LENGTH);
    strncat(locationString, id_, sizeof(locationString)-1);
    EV << "location string: " << locationString << "\n";

    // Set timers
    eventTimeoutTX  = new cMessage("timeoutTX");
    eventTimeoutRX1 = new cMessage("timeoutRX1");
    eventTimeoutRX2 = new cMessage("timeoutRX2");

    // Init last message sent expected arrival time
    msgOutArrivalTime = 0;


    // In the protocol NbTrans is 1 (default) so the following variables are not necessary.
    // However, in general, it is better to use them to support also NbTrans > 1
    messagesIn            = 0;
    messagesOut           = 0;
    messagesLost          = 0;
    messagesInvalid       = 0;
    messagesRetransmitted = 0;
    interferencesCount         = 0;
    interferencesPossibleCount = 0;


    cModule* parent = getParentModule();
    realDeployment = parent->par("realDeployment").boolValue();
    timeLimit      = 0;
    const int nGateways = parent->par("nGateways").intValue();

    // Check if the configuration consists of a real deployment
    if (realDeployment) {
        timeLimit = parent->par("timeLimit").intValue();

        // Locate the end device according to the RSSIs of in range gateways retrieved from the dataset.
        // Get all RSSIs and spreading factors values
        cValueArray* RSSIs_ = (cValueArray*) parent->par("RSSIs").objectValue();
        cValueArray* SFs    = (cValueArray*) parent->par("spreadingFactors").objectValue();

        int deviceIndex = getIndex();
        if (RSSIs_->size() <= deviceIndex || SFs->size() <= deviceIndex)
            return;

        // Get device RSSIs and spreading factor
        std::string rssiString = RSSIs_->asStringVector()[deviceIndex];
        spreadingFactor = SFs->asIntVector()[deviceIndex];
        EV << "Spreading Factor: " << (unsigned) spreadingFactor << "\n";

        // Split the string of RSSIs to obtain one RSSI per gateway
        //EV << rssiString << "\n";
        stringstream stream(rssiString);
        string rssi;
        int nGatewaysInRange = gateSize(LORA_GATE_OUT);
        double RSSIs[nGatewaysInRange] = {};

        int i = 0;
        while (stream >> rssi) {
            // Convert string to double
            RSSIs[i] = stod(rssi);

            //EV << rssi << "\n";
            i++;
        }

        EV << "nGatewaysInRange: " << nGatewaysInRange << "\n";
        //return;

        // In real deployment dataset, each device is connected with the same two gateways
        if (nGatewaysInRange < 2)
            return;

        double distances[nGatewaysInRange] = {};
        unsigned gatewayPositions[nGatewaysInRange][2] = {};

        // Calculate distances of the end device from the gateways
        for (int i=0; i<nGatewaysInRange; i++) {
            // Get the gateway in the vector at index i
            LoRaGateway* gateway = dynamic_cast<LoRaGateway*>(parent->getSubmodule("gateways", i));

            // Calculate distance from RSSI collected at the gateway
            // rssi = -10 * PATH_LOSS_EXP * log10(d) + MEASURED_POWER
            // rssi - MEASURED_POWER = -10 * PATH_LOSS_EXP * log10(d)
            // 10^((rssi - MEASURED_POWER) / (-10 * PATH_LOSS_EXP)) = d
            distances[i] = pow(10, (RSSIs[i] - MEASURED_POWER) / (-10 * PATH_LOSS_EXP));

            EV << "RSSI: " << RSSIs[i] << " => Distance: " << distances[i] << "\n";
            //EV << "Distance: " << distances[i] << " => RSSI: " << -10 * PATH_LOSS_EXP * log10(distances[i]) + MEASURED_POWER << "\n";

            // Store gateway position
            gatewayPositions[i][0] = gateway->getPosX();
            gatewayPositions[i][1] = gateway->getPosY();
        }

        // Circumference equation:
        // (x-x_c)^2 + (y-y_c)^2 = r^2 <=> x^2 + y^2 - 2 x_c x - 2 y_c y + x_c^2 + y_c^2 - r^2 = 0
        // Intersect the circumference equations of the two gateways
        // Circumference1: x^2 + y^2 - 2 x_c1 x - 2 y_c1 y + x_c1^2 + y_c1^2 - r1^2 = 0
        // Circumference2: x^2 + y^2 - 2 x_c2 x - 2 y_c2 y + x_c2^2 + y_c2^2 - r2^2 = 0
        // Difference: -2 x_c1 x - 2 y_c1 y + x_c1^2 + y_c1^2 - r1^2 - (-2 x_c2 x - 2 y_c2 y + x_c2^2 + y_c2^2 - r2^2) = 0
        //             -2 x_c1 x - 2 y_c1 y + x_c1^2 + y_c1^2 - r1^2 + 2 x_c2 x + 2 y_c2 y - x_c2^2 - y_c2^2 + r2^2) = 0
        //             (-2 x_c1 + 2 x_c2) x + (-2 y_c1 + 2 y_c2) y + (x_c1^2 + y_c1^2 - r1^2 - x_c2^2 - y_c2^2 + r2^2) = 0
        //             y = (-(-2 x_c1 + 2 x_c2) x - (x_c1^2 + y_c1^2 - r1^2 - x_c2^2 - y_c2^2 + r2^2)) / (-2 y_c1 + 2 y_c2)
        double Xc1 = gatewayPositions[0][0];
        double Yc1 = gatewayPositions[0][1];
        double Xc2 = gatewayPositions[1][0];
        double Yc2 = gatewayPositions[1][1];
        double r1  = distances[0];
        double r2  = distances[1];

        EV << "Xc1: " << Xc1 << ", Yc1: " << Yc1 << ", r1: " << r1 << "\n";
        EV << "Xc2: " << Xc2 << ", Yc2: " << Yc2 << ", r2: " << r2 << "\n";

        // Calculate radical axis
        double diffX = -2 * Xc1 + 2 * Xc2;
        double diffY = -2 * Yc1 + 2 * Yc2;
        double diffConst = Xc1 * Xc1 + Yc1 * Yc1 - r1 * r1 - (Xc2 * Xc2 + Yc2 * Yc2 - r2 * r2);

        double m = -diffX / diffY;
        double k = -diffConst / diffY;
        EV << "y = " << m << "x " << (k >= 0 ? "+ " : "") << k << "\n";

        //EV << "diffX: " << diffX << "\n";
        //EV << "diffY: " << diffY << "\n";
        //EV << "diffConst: " << diffConst << "\n";
        //EV << "" << << "\n";

        // Replace the radical axis in the first circumference equation
        // circumference1: x^2 + y^2 - 2 x_c1 x - 2 y_c1 y + x_c1^2 + y_c1^2 - r1^2 = 0
        // radical axis: y = (-(-2 x_c1 + 2 x_c2) x - (x_c1^2 + y_c1^2 - r1^2 - x_c2^2 - y_c2^2 + r2^2)) / (-2 y_c1 + 2 y_c2)
        //               y = ((2 x_c1 - 2 x_c2) x - x_c1^2 - y_c1^2 + r1^2 + x_c2^2 + y_c2^2 - r2^2) / (-2 y_c1 + 2 y_c2)
        //               y = ((2 x_c1 - 2 x_c2) / (-2 y_c1 + 2 y_c2) x + (-x_c1^2 - y_c1^2 + r1^2 + x_c2^2 + y_c2^2 - r2^2) / (-2 y_c1 + 2 y_c2)
        //               y = m x + k
        // replacement: x^2 + ((2 x_c1 - 2 x_c2) / (-2 y_c1 + 2 y_c2) x + (-x_c1^2 - y_c1^2 + r1^2 + x_c2^2 + y_c2^2 - r2^2) / (-2 y_c1 + 2 y_c2))^2
        //              - 2 x_c1 x - (2 y_c1 ((2 x_c1 - 2 x_c2) / (-2 y_c1 + 2 y_c2) x + (-x_c1^2 - y_c1^2 + r1^2 + x_c2^2 + y_c2^2 - r2^2) / (-2 y_c1 + 2 y_c2)) + x_c1^2 + y_c1^2 - r1^2 = 0

        //              x^2 + (mx + k)^2 - 2 x_c1 x - 2 y_c1 (mx + k) + x_c1^2 + y_c1^2 - r1^2 = 0
        //              x^2 + m^2 x^2 + k^2 + 2mk x - 2 x_c1 x - 2 y_c1 (mx + k) + x_c1^2 + y_c1^2 - r1^2 = 0
        //              (1 + m^2) x^2 + (2mk - 2 x_c1 - 2 y_c1 m) x + k^2 - 2 y_c1 k + x_c1^2 + y_c1^2 - r1^2 = 0
        //              a x^2 + bx + c = 0
        //              delta = b^2 - 4 ac => x1,2 = (-b +- sqrt(delta)) / (2a)
        double a = 1 + m * m;
        //double b = 2 * m * k - 2 * Xc1 - 2 * Yc1 * m;
        double b = 2 * (m * (k - Yc1) - Xc1);
        //double c = k * k - 2 * Yc1 * k + Xc1 * Xc1 + Yc1 * Yc1 - r1 * r1;
        double c = k * (k - 2 * Yc1) + Xc1 * Xc1 + Yc1 * Yc1 - r1 * r1;
        EV << a << "x^2 " << (b >= 0 ? "+ " : "") << b << "x " << (c >= 0 ? "+ " : "") << c << " = 0\n";

        double delta = b * b - 4 * a * c;
        if (delta < 0) {
            EV << "Negative delta!\n";
            // No real solutions, the gateway is not in the radio range of one of the two gateways
            // but this should not occur with data from dataset
            return;
        }
        else if (delta > 0)
            // Two distinct solutions
            delta = sqrt(delta);

        double x1 = (-b - delta) / (2*a);
        double x2 = (-b + delta) / (2*a);

        double y1 = m * x1 + k;
        double y2 = m * x2 + k;

        EV << "Solution 1: (" << x1 << ", " << y1 << ")\n";
        EV << "Solution 2: (" << x2 << ", " << y2 << ")\n";

        // Check if solution 1 values are positive and solution 2 values are negative, otherwise choose randomly
        if ((x1 > 0 && y1 > 0) && (x2 < 0 || y2 < 0 || (rand() + (int) uniform(0, 100)) % 101 < 50)) {
            // Choose sol 1
            posX = x1;
            posY = y1;
        }
        else {
            // Choose sol 2
            posX = x2;
            posY = y2;
        }
    }
    else {
        // Locate the end device randomly in a gateway radio range.
        // Set end device position
        int bgX = parent->par("bgX").intValue();
        int bgY = parent->par("bgY").intValue();

        bool fullCoverage = parent->par("fullCoverage").boolValue();

        // Gateway index
        int i = 0;

        // Set min and max distance from a gateway
        // sqrt(dx^2 + dy^2) < LORA_RANGE => sqrt(2dx^2) < LORA_RANGE => sqrt(2)dx < LORA_RANGE
        // dx < LORA_RANGE / sqrt(2)
        unsigned minDx = 300;
        unsigned maxDx = LORA_RANGE / sqrt(2);

        // Check if all end devices should be located in the radio range of all gateways
        if (fullCoverage) {
            // Get the first gateway
            maxDx -= 300;
        }
        else {
            // Get a random gateway
            i = (rand() + (int) uniform(0, 100)) % nGateways;
        }

        // Get the gateway
        LoRaGateway* gateway = dynamic_cast<LoRaGateway*>(parent->getSubmodule("gateways", i));

        // Get a random distance in [min, max]
        const unsigned diffDx = maxDx - minDx;
        double d = minDx + (rand() + (int) uniform(0, diffDx)) % diffDx;

        // Try to set device position in the gateway range
        //posX = rand()%2 ? (gateway->getPosX() + minDx + (rand() + (int) uniform(0, diffDx)) % diffDx) % bgX :
        //                  (gateway->getPosX() - minDx + (rand() + (int) uniform(0, diffDx)) % diffDx) % bgX;
        //posY = rand()%2 ? (gateway->getPosY() + minDx + (rand() + (int) uniform(0, diffDx)) % diffDx) % bgY :
        //                  (gateway->getPosY() - minDx + (rand() + (int) uniform(0, diffDx)) % diffDx) % bgY;

        // Locate in the gateway range with positive coordinates
        do {
            // Get a random angle in radians
            double angle = ((rand() + (int) uniform(0, 360)) % 360) * 3.14 / 180;
            posX = gateway->getPosX() + d * cos(angle);
            posY = gateway->getPosY() + d * sin(angle);
        } while (posX < 0 || posY < 0 || posX > bgX || posY > bgY);

        // Create connections with gateways based on device location
        int gateIndex = 0;
        std::vector<uint8_t> spreadingFactors;

        for (int i=0; i<nGateways; i++) {
            // Get the gateway in the vector at index i
            LoRaGateway* gateway = dynamic_cast<LoRaGateway*>(parent->getSubmodule("gateways", i));
            double distance = 0;

            // Check if the device is in the range of the gateway
            if (isInLoRaRange(posX, posY, gateway->getPosX(), gateway->getPosY(), &distance, LORA_RANGE)) {
                // Get gateway vector gate size (IN & OUT sizes are equal)
                int gatewayGateSize = gateway->gateSize(gateway->getLoRaGateIn()); // qui si blocca

                // Increment gate vector sizes
                setGateSize(LORA_GATE_BASENAME, gateIndex+1);
                gateway->setGateSize(gateway->getLoRaGateBasename(), gatewayGateSize+1);

                // Get device output gate & gateway input gate
                cGate* deviceGate  = gate(LORA_GATE_OUT, gateIndex++);
                cGate* gatewayGate = gateway->gate(gateway->getLoRaGateIn(), gatewayGateSize);

                //EV << "Device gate OUT vector size: " << deviceGate->getVectorSize() << "\n";
                //EV << "Gateway gate IN vector size: " << gatewayGate->getVectorSize() << "\n";


                // Calculate communication delay
                double delay = distance / SPEED_LIGHT_ON_AIR;

                // Calculate appropriate spreading factor [7, 12] for this distance
                uint8_t sf = (int) (distance / (LORA_RANGE / 6)) + 7;
                spreadingFactors.push_back(sf);

                // Derive data rate
                //double datarate = sf * (4 / (4 + CODING_RATE)) / (pow(2, sf) / BANDWIDTH) * 1000;

                // Define channel
                cDatarateChannel* channel = cDatarateChannel::create("channel");
                channel->setDelay(delay);
                //channel->setDatarate(1e6); // 1 Mb/s = 1000 Kb/s
                //channel->setDatarate(5e3); // 5 Kb/s
                //channel->setDatarate(datarate);

                // Create OUT->IN connection
                deviceGate->connectTo(gatewayGate, channel);
                //channel->callInitialize();

                //EV << "deviceGate:" << deviceGate << "\n";
                //EV << "gatewayGate:" << gatewayGate << "\n";

                // Get device input gate & gateway output gate
                deviceGate  = deviceGate->getOtherHalf();
                gatewayGate = gatewayGate->getOtherHalf();

                //EV << "Device gate IN vector size: " << deviceGate->getVectorSize() << "\n";
                //EV << "Gateway gate OUT vector size: " << gatewayGate->getVectorSize() << "\n";

                // Define channel
                channel = cDatarateChannel::create("channel");
                channel->setDelay(delay);
                //channel->setDatarate(datarate);

                // Create IN<-OUT connection
                gatewayGate->connectTo(deviceGate, channel);
                //channel->callInitialize();

                //EV << "deviceGate:" << deviceGate << "\n";
                //EV << "gatewayGate:" << gatewayGate << "\n";


                //============================
                // Get neighbour gateway signal for handling interferences
                /*simsignal_t gatewaySignalInterference = gateway->getSignalInterference();

                // Append it to the list of neighbor gateways
                neighborGatewaysSignalInterferences.push_back(
                        std::tuple<cModule*, simsignal_t> {gateway, gatewaySignalInterference});

                EV << "Gateway signal for interferences: " << gatewaySignalInterference << "\n";*/
                //============================

                //============================
                // Share references with neighbor gateways for handling interferences
                addNeighborGateway(gateway);
                //gateway->addNeighborDevice(getId());
                gateway->addNeighborDevice(this);
                //============================
            }
        }

        // Verify
        for (int i=0; i<gateSize(LORA_GATE_OUT); i++) {
            try {
                cGate* deviceGate = gate(LORA_GATE_OUT, i);
                EV << "Is device gate OUT [" << i << "] connected ? " << deviceGate->isConnected() << "\n";

                deviceGate = gate(LORA_GATE_IN, i);
                EV << "Is device gate IN [" << i << "] connected ? " << deviceGate->isConnected() << "\n";
            } catch (const std::exception& e) {
                break;
            }
        }

        // Calculate median SF (assuming ADR already chose it)
        const int spreadingFactorsSize = spreadingFactors.size();
        spreadingFactor = spreadingFactorsSize % 2 ?
                spreadingFactors[spreadingFactorsSize/2] :
                (spreadingFactors[spreadingFactorsSize/2 - 1] + spreadingFactors[spreadingFactorsSize/2]) / 2;
        EV << "Spreading Factor: " << (unsigned) spreadingFactor << "\n";
    }

    cDisplayString& ds = getDisplayString();
    ds.setTagArg("p", 0, posX);
    ds.setTagArg("p", 1, posY);

    EV << "posX: " << posX << "\n";
    EV << "posY: " << posY << "\n";


    // Get current deployment region
    int region = parent->par("region").intValue();

    // Get LoRa regional parameters
    std::vector<float> bandwidths_;
    std::vector<std::tuple<uint8_t, float, float, uint8_t, uint8_t>> frequencies;
    uint8_t transmissionPowerMax = 0;
    std::map<uint8_t, std::vector<uint8_t>> transmissionPowers;

    dutyCycleUsed          = 0;
    dutyCycleStartInterval = 0;
    eventTimeoutDutyCycle = new cMessage("timeoutDutyCycle");

    /*switch(region) {
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
            dutyCycle            = DUTY_CYCLE_EU_868;
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
            dutyCycle            = DUTY_CYCLE_EU_433;
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
            dutyCycle            = DUTY_CYCLE_US_915;
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

            if (spreadingFactorNum > 0) {
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
    }*/

    getPhysicalParameters(region, &dutyCycle, bandwidths, channelFrequencies);

    // Print the map indexed by spreading factors and values the list of corresponding tuples of (bandwidth, tx power)
    for (const auto& k : bandwidths) {
        EV << "SF: " << (int) k.first << " -> ";
        for (auto tuple : k.second)
            EV << "BW: " << std::get<0>(tuple) << ", TP: " << (int) std::get<1>(tuple) << " | ";
        EV << "\n";
    }

    // Print the map indexed by bandwidths and values the list of corresponding channel frequencies
    for (const auto& k : channelFrequencies) {
        EV << "BW: " << k.first << " -> ";
        for (float ch : k.second)
            EV << "Channel frequency: " << ch << ", ";
        EV << "\n";
    }//*/

    // Calculate median SF (assuming ADR already chose it)
    /*const int spreadingFactorsSize = spreadingFactors.size();
    spreadingFactor = spreadingFactorsSize % 2 ?
            spreadingFactors[spreadingFactorsSize/2] :
            (spreadingFactors[spreadingFactorsSize/2 - 1] + spreadingFactors[spreadingFactorsSize/2]) / 2;
    EV << "Spreading Factor: " << (int) spreadingFactor << "\n";*/

    // Get corresponding list of bandwidths
    auto bandwidths__ = bandwidths[spreadingFactor];

    // Select a random one
    auto tuple = bandwidths__[(rand() + (int) uniform(0, 10)) % bandwidths__.size()];
    float bandwidth = std::get<0>(tuple);
    EV << "Bandwidth: " << bandwidth << " KHz\n";

    // Calculate data rate (multiply by 1000 to convert from bits/ms to bits/s)
    //double datarate = spreadingFactor * (4.0 / (4 + CODING_RATE)) / (pow(2, spreadingFactor) / bandwidth) * 1000;
    double datarate = calculateDatarate(spreadingFactor, bandwidth, CODING_RATE);
    EV << "Data rate: " << datarate << " bits/s\n";

    // Set channels data rate
    for (int i=0; i<gateSize(LORA_GATE_OUT); i++) {
        // OUT->IN connection
        cGate* deviceGate  = gate(LORA_GATE_OUT, i);
        cDatarateChannel* channel = dynamic_cast<cDatarateChannel*>(deviceGate->getChannel());
        channel->setDatarate(datarate);
        channel->callInitialize();

        //EV << "deviceGate OUT:" << deviceGate << "\n";

        // IN<-OUT connection
        cGate* gatewayGate = deviceGate->getNextGate();
        gatewayGate = gatewayGate->getOtherHalf();
        channel = dynamic_cast<cDatarateChannel*>(gatewayGate->getChannel());
        channel->setDatarate(datarate);
        channel->callInitialize();

        //EV << "gatewayGate OUT:" << gatewayGate << "\n";
    }


    // Register signal for handling interferences
    /*char signalInterferenceName[12 + intLen] = "endDevice-"; // 12+16 bytes
    strncat(signalInterferenceName, id_, sizeof(signalInterferenceName)-1);

    EV << "End device signal for interferences name: " << signalInterferenceName << "\n";

    signalInterference = registerSignal(signalInterferenceName);
    subscribe(signalInterference, this);

    EV << "End device signal for interferences: " << signalInterference << "\n";*/

    // Collect module IDs of nearby end devices for handling interferences in the simulation
    int index = getIndex();

    for (int i=0; i<index; i++) {
        // Get the other device in the vector at index i
        LoRaEndDevice* other = dynamic_cast<LoRaEndDevice*>(parent->getSubmodule("endDevices", i));
        double distance = 0;

        // Check if the end device is in the interference range of the other device
        if (isInLoRaRange(posX, posY, other->getPosX(), other->getPosY(), &distance, INTERFERENCE_RANGE)) {
            addNeighborDevice(other);
            other->addNeighborDevice(this);
        }
    }


    // The WATCH() statement below will let you examine the variables under Tkenv
    WATCH(fCntUp);
    WATCH(fCntDown);
    WATCH(retransmissions);
    WATCH(requestId);
    WATCH(levelNumber);

    // Register signals for statistic collection
    signalSent               = registerSignal("sent");
    signalReceived           = registerSignal("received");
    signalLost               = registerSignal("lost");
    signalInvalid            = registerSignal("invalid");
    signalRetransmitted      = registerSignal("retransmitted");
    signalSentCount          = registerSignal("sentCount");
    signalReceivedCount      = registerSignal("receivedCount");
    signalLostCount          = registerSignal("lostCount");
    signalInvalidCount       = registerSignal("invalidCount");
    signalRetransmittedCount = registerSignal("retransmittedCount");
    signalInterference              = registerSignal("interference");
    signalInterferencePossible      = registerSignal("interferencePossible");
    signalInterferenceCount         = registerSignal("interferenceCount");
    signalInterferencePossibleCount = registerSignal("interferencePossibleCount");


    // The 'ev' object works like 'cout' in C++
    EV << "End device initialized\n";

    // Start with a transmission window
    EV << "Starting timeout for opening the transmission window...\n";
    //scheduleAt(simTime(), eventTimeoutTX);  // XXX: temp for heavy testing
    // Wake up end devices in different moments in time ([0, 600] seconds)
    // to better simulate the OTAA beginnings of the end devices and reduce collisions.
    // (600 seconds interval supports all tests till 320-3 and 100-16)
    //int time = (rand() + (int) uniform(0, 600)) % 600;
    std::random_device rd {};
    std::mt19937 gen { rd() };
    std::uniform_int_distribution<> distribution(0, 600);
    //std::uniform_int_distribution<> distribution(0, 1800);
    //std::uniform_int_distribution<> distribution(0, 2400);
    int time = distribution(gen);

    EV << "Activation time: " << time << "\n";
    scheduleAt(simTime() + time, eventTimeoutTX);//*/


    // t0 => TRANSMISSION WINDOW TILL t1
    // t1 => RECEIVING WINDOW 1 TILL t2
    // t2 => POSSIBLE RECEIVING WINDOW 2 OR TRANSMISSION WINDOW TILL t3
    // t3 => RECEIVING WINDOW 1 TILL t4
}

void LoRaEndDevice::handleMessage(cMessage *msgIn) {
    // The handleMessage() method is called whenever a message arrives at the module

    // For analyzing the MICs
    std::cout << "*****************\nDevice\n";

    EV << "============= END DEVICE =============\n";
    EV << "Received message: " << msgIn->getName() << "\n";
    EV << "Transmission window state: " << (int) transmissionWindowState << "\n";
    EV << "Stage: " << (int) stage << "\n";
    EV << "Uplink counter: " << (int) fCntUp << "\n";
    EV << "Downlink counter: " << (int) fCntDown << "\n";

    //EV << "neighbourSignalInterferences size: " << neighbourDevicesSignalInterferences.size() << "\n";
    //for (std::tuple<cModule*, simsignal_t> t : neighbourDevicesSignalInterferences)
    //    EV << "Neighbour signal: " << std::get<1>(t) << "\n";

    // Check if the message is a timeout
    if (msgIn == eventTimeoutTX) {
        // Previous window is ended, update the transmission window state of the end device
        transmissionWindowState = WINDOW_TX;
        EV << "Timeout expired. TX window begins\n";

        // Check if duty cycle limit is reached
        if (dutyCycle > 0 && dutyCycleUsed/DUTY_CYCLE_INTERVAL > dutyCycle) {
            EV << "Duty cycle limit!\n";

            // Set a random wake up after the duty cycle is reset
            simtime_t waitTime = dutyCycleStartInterval + DUTY_CYCLE_INTERVAL + (rand() + (int) uniform(0, 600)) % 600;

            // Schedule next transmission window
            EV << "Starting timeout for opening the next transmission window...\n";
            //simtime_t simtime = simTime();
            //scheduleAt(simtime + TX_DELAY, eventTimeoutTX);
            scheduleAt(waitTime, eventTimeoutTX);

            return;
        }

        // Get a random bandwidth on which transmit (based on the SF and current deployment region)
        auto bandwidths_ = bandwidths[spreadingFactor];
        auto tuple = bandwidths_[(rand() + (int) uniform(0, 10)) % bandwidths_.size()];
        float bandwidth           = std::get<0>(tuple);
        uint8_t transmissionPower = std::get<1>(tuple);

        // Get a random channel frequency on which transmit (based on the SF and current deployment region)
        auto channelFrequencies_ = channelFrequencies[bandwidth];
        //float channelFrequency   = channelFrequencies_[(rand() + (int) uniform(0, 40)) % channelFrequencies_.size()];
        std::random_device rd {};
        std::mt19937 gen { rd() };
        std::uniform_int_distribution<> distribution(0, channelFrequencies_.size()-1);
        float channelFrequency = channelFrequencies_[distribution(gen)];

        EV << "Spreading Factor: " << (int) spreadingFactor << "\n";
        EV << "Bandwidth: " << bandwidth << " KHz\n";
        EV << "Transmission Power: " << (int) transmissionPower << " dBm\n";
        EV << "Channel Frequency: " << channelFrequency << " MHz\n";

        // Transmit!
        if (stage == STAGE_ACTIVATION) {
            EV << "End device OTAA\n";

            EV << "Sending JOIN_REQUEST message...\n";
            msgOut = createMessageJoinRequest(
                    "joinRequest",
                    joinEUI, devEUI, nonceDev++,
                    nwkKey,
                    spreadingFactor, transmissionPower,
                    bandwidth, channelFrequency);
            sendMessage(false);

            // Check if the message is a retransmission due to a miss gateway receive
            if (nonceDev > 1) {
                // Send signal for statistic collection
                emit(signalRetransmitted, 1u);
                emit(signalRetransmittedCount, ++messagesRetransmitted);
            }
        }
        // Check if the end device is executing the pairing algorithm
        else if (stage == STAGE_GENERATE_COMMON_KEY) {
            EV << "retransmissions: " << (int) retransmissions << "\n";

            if (retransmissions || statsMessages.empty()) {
                // Check if this is the first GENERATE_COMMON_KEY message
                if (retransmissions == RETRANSMISSIONS) {
                    // In OTAA, the end device sends joinEUI, devEUI, nonce.
                    // Generate two nonces for deriving a symmetric key
                    // to share with gateways in the radio range
                    unsigned nonce1 = generateNonce(0xFFFFFF);
                    unsigned nonce2 = generateNonce(0xFFFF);

                    // Convert nonces to array of bytes
                    uint8_t nonce1_[4];
                    uintToBytes(nonce1, nonce1_);
                    uint8_t nonce2_[4];
                    uintToBytes(nonce2, nonce2_);


                    std::time_t time = std::time(nullptr);
                    EV << "Timestamp: " << time << "\n";
                    EV << "sizeof(time): " << sizeof(time) << "\n";

                    // Convert timestamp to array of bytes
                    uint8_t timestampBytes[4];
                    uintToBytes(time, timestampBytes);

                    unsigned t = bytesToUint(timestampBytes);
                    EV << "Timestamp (after): " << t << "\n";
                    EV << "sizeof(t): " << sizeof(t) << "\n";

                    // TODO: Add timestamp in the payload

                    // 4 bytes int => year 2038 limit
                    // (2^31-1) / (60 * 60 * 24 * 365) = 68 years since 1970.
                    // 4 bytes unsigned => year 2106
                    // (2^32-1) / (60 * 60 * 24 * 365) = 136 years since 1970.

                    // std::time_t uses 8 bytes

                    // Timestamp requires clock synchronization among end devices and gateways.
                    // OTAA can include time synchronization with network server.
                    // Gateways synchronize during activation too.
                    // Constant time updates against clock deviations.

                    // Session keys of gateways should be valid for an amount of time
                    // such that clock drift are below a threshold and are not suitable to be compromised

                    // Network ID should be avoided here to save space,
                    // gateway should obtain it during activation or join

                    // Build payload
                    uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {
                        nonce1_[0],
                        nonce1_[1],
                        nonce1_[2],
                        nonce1_[3],
                        nonce2_[0],
                        nonce2_[1],
                        nonce2_[2],
                        nonce2_[3],
                        networkId[0],
                        networkId[1],
                        networkId[2]
                    };

                    //EV << "nonce1: " << std::hex << nonce1 << "\n";
                    //EV << "nonce2: " << std::hex << nonce2 << std::dec << "\n";
                    EV << "nonce1: " << nonce1 << "\n";
                    EV << "nonce2: " << nonce2 << "\n";

                    // Derive session key exclusively shared with the gateways in the radio range
                    memcpy(commonSKey, generateKey(commonKey, nonce1, nonce2, networkId), sizeof(commonSKey));

                    printKey(commonSKey, sizeof(commonSKey), "commonSKey", EV);

                    // Send message (in OMNeT++, for retransmission send copies and retain the original)
                    EV << "Sending GENERATE_COMMON_KEY message...\n";
                    msgOut = createMessageUplink(
                            "generateCommonKeyMsg", true,
                            address, IPv4_ADDRESS_SIZE,
                            ++fCntUp, MSG_PORT_GENERATE_COMMON_KEY, payload, LORA_FRAME_SIZE_APP_PAYLOAD,
                            commonKey,
                            spreadingFactor, transmissionPower,
                            bandwidth, channelFrequency);
                    sendMessage(true);

                    retransmissions--;
                }
                else {
                    // Repeat GENERATE_COMMON_KEY msg until the repetitions parameter is exhausted
                    // or no response has been received
                    EV << "Resending GENERATE_COMMON_KEY message...\n";

                    resendMessage(commonKey);

                    // Check if the repetitions are exhausted to avoid an overflow
                    if (retransmissions)
                        retransmissions--;
                }
            }
            else {
                // The message has been sent n times and at least a confirmation has been received
                EV << "At least a GENERATE_COMMON_KEY message has been received, go to next algorithm stage!\n";

                // Go to HELLO stage and reset variables for handling STATS messages
                stage           = STAGE_HELLO;
                retransmissions = RETRANSMISSIONS;
                statsMessages.clear();

                delete msgOut;

                // Build payload
                uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {
                        requestId,
                        levelNumber
                };

                EV << "Sending HELLO message...\n";

                // Create and send HELLO message
                msgOut = createMessageUplink(
                        "helloMsg", false,
                        address, IPv4_ADDRESS_SIZE,
                        ++fCntUp, MSG_PORT_HELLO, payload, LORA_FRAME_SIZE_APP_PAYLOAD,
                        commonSKey,
                        spreadingFactor, transmissionPower,
                        bandwidth, channelFrequency);
                sendMessage(false);
            }
        }
        else if (stage <= STAGE_FORWARD) {
            EV << "retransmissions: " << (int) retransmissions << "\n";

            // Repeat HELLO/FORWARD msg with ACKs until the repetitions parameter is exhausted
            // or there are still gateways to ACK or no response has been received
            //if (retransmissions || !gateways2ACK.empty() || statsMessages.empty()) {
            // Remove no response because if all levels are explored no response is received
            if (retransmissions || !gateways2ACK.empty()) {
                // Build payload
                uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {
                        requestId,
                        levelNumber
                };

                //int i;

                // Insert in the payload ACKs for possible received gateways STATS responses
                // (if class A, then a single message is received in each window and
                // a single gateway is acknowledged per message => increased latency)
                for (int i=2; i<LORA_FRAME_SIZE_APP_PAYLOAD-IPv4_ADDRESS_SIZE; i+=IPv4_ADDRESS_SIZE) {
                    if (gateways2ACK.empty())
                        break;
                    else {
                        // Get the IP address of the gateway to ACK
                        std::array<uint8_t, IPv4_ADDRESS_SIZE> gatewayIpAddress = gateways2ACK.front();

                        // Copy the address in the payload
                        memcpy(&payload[i], gatewayIpAddress.data(), IPv4_ADDRESS_SIZE);

                        // Remove the gateway from the list
                        gateways2ACK.pop_front();
                    }
                }

                // Check if there are other gateways to be acknowledged but their IPs do not fit the
                // payload size (if class A, only a message is received per window so this condition is never true)
                /*if (statsMessagesAckSize < statsMessagesSize) {
                    // Add ellipsis
                    EV << "Adding 'ellipsis' to the payload\n";
                    memset(&payload[i], '.', 2);
                }*/

                // Create and send the message
                const char* name;
                uint8_t port;
                if (stage == STAGE_HELLO) {
                    EV << "Sending HELLO message (possibly with ACK)...\n";
                    name = "helloMsg";
                    port = MSG_PORT_HELLO;
                }
                else {
                    EV << "Sending FORWARD message (possibly with ACK)...\n";
                    name = "forwardMsg";
                    port = MSG_PORT_FORWARD;
                }

                // Only gateways in the radio range have the commonSKey and can verify and decrypt the message.
                // To make the message available to out of range gateways,
                // it must be decrypted and rencrypted with the cluster session keys and
                // MIC must be recalculated by in range gateways.

                // Increment always the counter so that uplink counter >= downlink counter
                // to synchronize downlink counters among gateways (and network server indirectly)
                msgOut = createMessageUplink(
                        name, true,
                        address, IPv4_ADDRESS_SIZE,
                        ++fCntUp, port, payload, LORA_FRAME_SIZE_APP_PAYLOAD,
                        commonSKey,
                        spreadingFactor, transmissionPower,
                        bandwidth, channelFrequency);
                sendMessage(false);

                // Check if the repetitions are exhausted to avoid an overflow
                if (retransmissions)
                    retransmissions--;
            }
            // The message has been sent n times,
            // check if a STATS message has been received in the previous receive window
            // or a new gateway must be selected
            else if (!messageReceived || searchNewGateway) {
                // No message has been received or a NACK from the selected gateway, so,
                // analyze responses sorting them by ascending order of a score
                // based on RSSI, CPU-load, ... to select the best one (multiple if same score)
                searchNewGateway = false;

                // Get the list of best gateways (min score) if any
                auto it = sortedGateways.begin();
                if (it == sortedGateways.end()) {
                    EV << "No gateway in the current level satisfies metrics requirements\n";

                    // Check if at least a STATS message has been received in the current level
                    if (statsMessages.empty()) {
                        EV << "No STATS message received in the current level, terminate exploration\n";

                        // Terminate and notify gateways to stop the simulation when all end devices have finished the protocol run
                        cModule* parent = getParentModule();
                        const int nGateways = parent->par("nGateways").intValue();

                        for (int i=0; i<nGateways; i++) {
                            // Get the gateway in the vector at index i
                            LoRaGateway* gateway = dynamic_cast<LoRaGateway*>(parent->getSubmodule("gateways", i));
                            gateway->deviceFinish();
                        }

                        // Cancel the duty cycle timeout
                        cancelAndDelete(eventTimeoutDutyCycle);
                        eventTimeoutDutyCycle = nullptr;

                        return;
                    }

                    // Go to next level and reset variables for handling STATS messages
                    stage           = STAGE_FORWARD;
                    retransmissions = RETRANSMISSIONS;
                    levelNumber++;
                    statsMessages.clear();
                    gateways2ACK.clear();
                    sortedGateways.clear();

                    // Send forward msg
                    EV << "Sending FORWARD message...\n";

                    uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {
                            requestId,
                            levelNumber
                    };

                    msgOut = createMessageUplink(
                            "forwardMsg", false,
                            address, IPv4_ADDRESS_SIZE,
                            ++fCntUp, MSG_PORT_FORWARD, payload, LORA_FRAME_SIZE_APP_PAYLOAD,
                            commonSKey,
                            spreadingFactor, transmissionPower,
                            bandwidth, channelFrequency);
                    sendMessage(true);
                }
                else {
                    auto listSelectedGateways = it->second;

                    EV << "Printing the list of best gateways:\n";
                    for (auto tuple : listSelectedGateways) {
                        EV << "Gateway IP: "
                            << (int) std::get<0>(tuple)[0] << "."
                            << (int) std::get<0>(tuple)[1] << "."
                            << (int) std::get<0>(tuple)[2] << "."
                            << (int) std::get<0>(tuple)[3]
                            //<< "\ttimestamp: " << bytesToUint(std::get<1>(tuple).data()) << "\n";
                            << "\ttimestamp: " << std::get<1>(tuple) << "\n";
                    }

                    // Get the IP address of the best gateway.
                    // Select the last item as it is closer in time (it should be pushed to front when inserted)
                    auto tuple = listSelectedGateways.front();
                    std::array<uint8_t, IPv4_ADDRESS_SIZE> gatewayIpAddress = std::get<0>(tuple);

                    // Check if the IP address is valid
                    if (gatewayIpAddress[0] > 0) {
                        // Go to pairing stage and send pairing message
                        stage = STAGE_PAIRING;

                        EV << "Selected gateway IP: "
                            << (int) gatewayIpAddress[0] << "."
                            << (int) gatewayIpAddress[1] << "."
                            << (int) gatewayIpAddress[2] << "."
                            << (int) gatewayIpAddress[3]
                            << "\n";

                        // Get the associated timestamp
                        //std::array<uint8_t, 4> timestampBytes = std::get<1>(tuple);
                        //std::time_t timestamp = std::get<1>(tuple);
                        simtime_t timestamp = std::get<1>(tuple);

                        // Convert timestamp to array of bytes
                        std::array<uint8_t, 4> timestampBytes;
                        //std::array<uint8_t, 8> timestampBytes;
                        //uintToBytes(timestamp, timestampBytes.data());
                        // Send time in ms instead of s
                        uintToBytes(ceil(timestamp * 1000).dbl(), timestampBytes.data());

                        uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {
                                requestId,
                                gatewayIpAddress[0],
                                gatewayIpAddress[1],
                                gatewayIpAddress[2],
                                gatewayIpAddress[3],
                                timestampBytes[0],
                                timestampBytes[1],
                                timestampBytes[2],
                                timestampBytes[3]
                        };

                        // Copy the IP address in the selected gateway address
                        memcpy(selectedGatewayAddress.data(), gatewayIpAddress.data(), IPv4_ADDRESS_SIZE);

                        EV << "Sending PAIRING_REQUEST message...\n";
                        msgOut = createMessageUplink(
                                "pairingMsg", false,
                                address, IPv4_ADDRESS_SIZE,
                                ++fCntUp, MSG_PORT_PAIRING_REQUEST, payload, LORA_FRAME_SIZE_APP_PAYLOAD,
                                commonSKey,
                                spreadingFactor, transmissionPower,
                                bandwidth, channelFrequency);
                        sendMessage(true);
                    }

                    // Remove the gateway from the list
                    listSelectedGateways.pop_front();

                    // Update the map
                    if (!listSelectedGateways.empty())
                        sortedGateways[it->first] = listSelectedGateways;
                    else
                        sortedGateways.erase(it);
                }
            }
            else {
                // Go to forward stage and reset variables for handling STATS messages
                stage           = STAGE_FORWARD;
                retransmissions = RETRANSMISSIONS;
                levelNumber++;
                statsMessages.clear();
                gateways2ACK.clear();
                sortedGateways.clear();

                // Send forward msg
                EV << "Sending FORWARD message...\n";

                uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {
                        requestId,
                        levelNumber
                };

                msgOut = createMessageUplink(
                        "forwardMsg", false,
                        address, IPv4_ADDRESS_SIZE,
                        ++fCntUp, MSG_PORT_FORWARD, payload, LORA_FRAME_SIZE_APP_PAYLOAD,
                        commonSKey,
                        spreadingFactor, transmissionPower,
                        bandwidth, channelFrequency);
                sendMessage(true);
            }
        }

        /*else if (stage == STAGE_PAIRING) {
            // Check if the network server has sent a verification for the PAIRING message in the last receive window
            if (messageReceived == MSG_PAIRING_VERIFY) {
                // Yes, build payload to acknowledge the network server
                const char* name;
                uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = { requestId };

                if (isPairingVerified) {
                    payload[1] = ACK;
                    name = "pairingVerifyACKMsg";
                    EV << "Sending ACK message to network server for pairing verification...\n";
                }
                else {
                    payload[1] = NACK;
                    name = "pairingVerifyNACKMsg";
                    EV << "Sending NACK message to network server for pairing verification...\n";
                }

                // Send ACK or NACK msg to network server
                msgOut = createMessageUplink(name, true,
                        address, IPv4_ADDRESS_SIZE, ++fCntUp, MSG_PORT_PAIRING_VERIFIED, payload, LORA_FRAME_SIZE_APP_PAYLOAD);
                sendBroadcast(this, msgOut, LORA_GATE_OUT);
            }
            // Check if the selected gateway has sent the PAIRING_ACCEPT message
            // in response to the PAIRING_REQUEST in a previous receive window
            else if (!isPairingAccepted) {
                // No because isPairingAccepted variable can be either true and the pairing is accepted or
                // false but the state is downgraded to FORWARD_STAGE.
                // So, here false means the PAIRING_REQUEST message is not received,
                // repeat the sending
                EV << "Resending PAIRING_REQUEST message...\n";
                sendBroadcast(this, msgOut->dup(), LORA_GATE_OUT);
            }
            // Else check if no frame has been received in the last receive window and
            // the pairing has been verified (to await the verification)
            else if (!messageReceived && isPairingVerified) {
                // Association completed and verified.
                // Negotiate symmetric key

                stage = STAGE_GENERATE_ASSOCIATION_KEY;

                // Send joinEUI, devEUI, nonce (in OTAA)

                // Generate two nonces for deriving a symmetric key
                // to share with gateways in the radio range
                unsigned nonce1 = generateNonce(0xFFFFFF);
                unsigned nonce2 = generateNonce(0xFFFF);

                // Convert nonces to array of bytes
                uint8_t nonce1_[4];
                uintToBytes(nonce1, nonce1_);
                uint8_t nonce2_[4];
                uintToBytes(nonce2, nonce2_);

                // Build payload
                uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {
                    nonce1_[0],
                    nonce1_[1],
                    nonce1_[2],
                    nonce1_[3],
                    nonce2_[0],
                    nonce2_[1],
                    nonce2_[2],
                    nonce2_[3],
                    networkId[0],
                    networkId[1],
                    networkId[2]
                };

                // Derive session key exclusively shared with the associated gateway
                strncpy(associationSKey, generateKey(associationKey, nonce1, nonce2, networkId), sizeof(associationSKey));

                // Send message
                EV << "Sending GENERATE_ASSOCIATION_KEY message...\n";
                msgOut = createMessageUplink("generateAssociationKeyMsg", true,
                        address, IPv4_ADDRESS_SIZE, ++fCntUp, MSG_PORT_GENERATE_ASSOCIATION_KEY, payload, LORA_FRAME_SIZE_APP_PAYLOAD);
                sendBroadcast(this, msgOut->dup(), LORA_GATE_OUT);
            }
        }*/
        else if (stage == STAGE_PAIRING) {
            if (!messageReceived) {
                // The PAIRING_REQUEST response has not been received,
                // repeat the sending
                EV << "Resending PAIRING_REQUEST message (according to NbTrans parameter)...\n";
                resendMessage(commonSKey);

                // Send signal for statistic collection
                emit(signalRetransmitted, 1u);
                emit(signalRetransmittedCount, ++messagesRetransmitted);
            }
            else {
                // Pairing has been accepted,
                // send CONNECTION message to the network server

                stage = STAGE_CONNECTION;
                delete msgOut;

                // Build payload
               uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {
                   requestId,
                   selectedGatewayAddress[0],
                   selectedGatewayAddress[1],
                   selectedGatewayAddress[2],
                   selectedGatewayAddress[3],
               };

               // Send message
               EV << "Sending CONNECTION message...\n";
               msgOut = createMessageUplink(
                       "connectionMsg", true,
                       address, IPv4_ADDRESS_SIZE,
                       ++fCntUp, MSG_PORT_CONNECTION, payload, LORA_FRAME_SIZE_APP_PAYLOAD,
                       nwkSKey,
                       spreadingFactor, transmissionPower,
                       bandwidth, channelFrequency);
               sendMessage(true);
            }
        }
        else if (stage == STAGE_CONNECTION) {
            if (!messageReceived) {
                // The CONNECTION response has not been received,
                // repeat the sending
                EV << "Resending CONNECTION message (according to NbTrans parameter)...\n";
                resendMessage(nwkSKey);

                // Send signal for statistic collection
                emit(signalRetransmitted, 1u);
                emit(signalRetransmittedCount, ++messagesRetransmitted);
            }
            else {
                // Association completed and verified.
                // Negotiate symmetric key

                stage = STAGE_GENERATE_ASSOCIATION_KEY;
                delete msgOut;

                // Send joinEUI, devEUI, nonce (in OTAA)

                //=======================================================
                // Generate two nonces for deriving a symmetric key
                // to share with the selected gateway
                /*unsigned nonce1 = generateNonce(0xFFFFFF);
                unsigned nonce2 = generateNonce(0xFFFF);

                // Convert nonces to array of bytes
                uint8_t nonce1_[4];
                uintToBytes(nonce1, nonce1_);
                uint8_t nonce2_[4];
                uintToBytes(nonce2, nonce2_);

                // Build payload
                uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {
                    nonce1_[0],
                    nonce1_[1],
                    nonce1_[2],
                    nonce1_[3],
                    nonce2_[0],
                    nonce2_[1],
                    nonce2_[2],
                    nonce2_[3],
                    networkId[0],
                    networkId[1],
                    networkId[2]
                };

                // Derive session key exclusively shared with the associated gateway
                strncpy(associationSKey, generateKey(associationKey, nonce1, nonce2, networkId), sizeof(associationSKey));*/
                //=======================================================

                //=======================================================
                // As messages are encrypted with the session key shared between the end device
                // and the gateways in the radio range and forwarded messages to level-2 or greater gateways
                // are still encrypted with gateways keys,
                // here we can reuse the OTAA mechanism

                // Generate a nonce for deriving a symmetric key
                nonceDev = generateNonce(0xFFFF);

                EV << "nonceDev: " << nonceDev << "\n";

                // Convert nonce to array of bytes
                uint8_t nonceDev_[4];
                uintToBytes(nonceDev, nonceDev_);

                // Build payload
                uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {
                    requestId,
                    selectedGatewayAddress[0],
                    selectedGatewayAddress[1],
                    selectedGatewayAddress[2],
                    selectedGatewayAddress[3],
                    nonceDev_[0],
                    nonceDev_[1],
                    nonceDev_[2],
                    nonceDev_[3]
                };
                //=======================================================

                // Send message
                EV << "Sending GENERATE_ASSOCIATION_KEY message...\n";
                msgOut = createMessageUplink(
                        "generateAssociationKeyMsg", true,
                        address, IPv4_ADDRESS_SIZE,
                        ++fCntUp, MSG_PORT_GENERATE_ASSOCIATION_KEY, payload, LORA_FRAME_SIZE_APP_PAYLOAD,
                        commonSKey,
                        spreadingFactor, transmissionPower,
                        bandwidth, channelFrequency);
                sendMessage(true);
            }
        }
        else if (stage == STAGE_GENERATE_ASSOCIATION_KEY) {
            // The GENERATE_KEY response has not been received,
            // repeat the sending
            EV << "Resending GENERATE_ASSOCIATION_KEY message (according to NbTrans parameter)...\n";
            resendMessage(commonSKey);

            // Send signal for statistic collection
            emit(signalRetransmitted, 1u);
            emit(signalRetransmittedCount, ++messagesRetransmitted);
        }
        else if (stage == STAGE_DATA_PROFILE) {
            if (!messageReceived) {
                // The DATA_PROFILE response has not been received,
                // repeat the sending
                //EV << "Resending DATA_PROFILE message (according to NbTrans parameter)...\n";
                //resendMessage(associationSKey);

                // Send signal for statistic collection
                emit(signalRetransmitted, 1u);
                emit(signalRetransmittedCount, ++messagesRetransmitted);
            }

            // In our deployment, the data profile is represented only by the location string
            EV << "Sending data profile...\n";

            // Calculate remaining bytes to send
            uint8_t bytes2send = LOCATION_STRING_LENGTH - locationStringIndex;

            EV << "locationStringIndex: " << (int) locationStringIndex <<"\n";
            EV << "Remaining bytes to send: " << (int) bytes2send <<"\n";

            // Specify the starting index from which the location string is copied
            // so that retransmissions due to lost responses does not cause bad receptions
            uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = { locationStringIndex };

            // Compare the length of the string to the payload size
            if (bytes2send < LORA_FRAME_SIZE_APP_PAYLOAD-1)
                // The location string fits in the payload.
                // Copy the location string
                memcpy(&payload[1], &locationString[locationStringIndex], bytes2send);
            else
                // Copy a portion of the location string (without adding termination char to reduce number of messages)
                memcpy(&payload[1], &locationString[locationStringIndex], LORA_FRAME_SIZE_APP_PAYLOAD-1);

            EV << "locationString in payload: " << &payload[1] <<"\n";

            // From here small overhead because the gateways just forward frames as they are and does not need
            // to decrypt and reencrypt to recalculate the MIC (FORWARD)

            // Send data profile
            msgOut = createMessageUplink(
                    "dataProfileMsg", false,
                    address, IPv4_ADDRESS_SIZE,
                    ++fCntUp, MSG_PORT_DATA_PROFILE, payload, LORA_FRAME_SIZE_APP_PAYLOAD,
                    associationSKey,
                    spreadingFactor, transmissionPower,
                    bandwidth, channelFrequency);
            sendMessage(false);
        }
        // Disabled for evaluating the number of messages of the Gateway-Device coordination protocol.
        // Enable for evaluating the correctness of the entire protocol
        //else {
        else if (realDeployment && simTime() < timeLimit) {
            // Pairing algorithm is ended
            EV << "Sending data...\n";

            // Get timestamp
            //simtime_t time = simTime();
            std::time_t time = std::time(nullptr);

            // Convert timestamp to array of bytes
            uint8_t timestampBytes[4] = {};
            uintToBytes(time, timestampBytes);

            // Get data
            uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD] = {
                readSensor(),
                timestampBytes[0],
                timestampBytes[1],
                timestampBytes[2],
                timestampBytes[3]
            };

            // Send data periodically
            msgOut = createMessageUplink(
                "dataMsg", false,
                address, IPv4_ADDRESS_SIZE,
                ++fCntUp, MSG_PORT_DATA, payload, LORA_FRAME_SIZE_APP_PAYLOAD,
                associationSKey,
                spreadingFactor, transmissionPower, bandwidth, channelFrequency);
            sendMessage(false);

            // Schedule every 30 minutes (from the dataset)

            // Send signal for statistic collection
            //emit(signalSent, 1u);
            //emit(signalSentCount, ++messagesOut);

            // Schedule receive windows where start times are defined using the end of the transmission as a reference
            EV << "Starting timeouts for opening the receive windows...\n";
            scheduleAt(msgOutArrivalTime + 1800-2, eventTimeoutRX1);
            scheduleAt(msgOutArrivalTime + 1800-1, eventTimeoutRX2);

            // Schedule next transmission window
            EV << "Starting timeout for opening the next transmission window...\n";
            scheduleAt(msgOutArrivalTime + 1800, eventTimeoutTX);

            return;
        }
        else{
            // Terminate and notify gateways to stop the simulation when all end devices have finished the protocol run
            cModule* parent = getParentModule();
            const int nGateways = parent->par("nGateways").intValue();

            for (int i=0; i<nGateways; i++) {
                // Get the gateway in the vector at index i
                LoRaGateway* gateway = dynamic_cast<LoRaGateway*>(parent->getSubmodule("gateways", i));
                gateway->deviceFinish();
            }

            // Cancel the duty cycle timeout
            cancelAndDelete(eventTimeoutDutyCycle);
            eventTimeoutDutyCycle = nullptr;

            return;
        }

        // Send signal for statistic collection
        emit(signalSent, 1u);
        emit(signalSentCount, ++messagesOut);

        // Schedule receive windows where start times are defined using the end of the transmission as a reference
        EV << "Starting timeouts for opening the receive windows...\n";
        scheduleAt(msgOutArrivalTime + RX_DELAY_1, eventTimeoutRX1);
        scheduleAt(msgOutArrivalTime + RX_DELAY_2, eventTimeoutRX2);

        // Schedule next transmission window
        EV << "Starting timeout for opening the next transmission window...\n";
        //scheduleAt(msgOutArrivalTime + TX_DELAY, eventTimeoutTX);

        // Change transmit periodicity to prevent synchronization of end device transmissions
        // when downlinks are not expected to be retransmitted
        if (stage == STAGE_HELLO || stage == STAGE_FORWARD)
            scheduleAt(msgOutArrivalTime + TX_DELAY, eventTimeoutTX);
        else {
            std::random_device rd {};
            std::mt19937 gen { rd() };
            std::uniform_int_distribution<> distribution(0, 60);

            int waitTime = distribution(gen);
            //EV << "Wake up after " << (msgOutArrivalTime+TX_DELAY+waitTime-simTime()) << " seconds\n";
            scheduleAt(msgOutArrivalTime + TX_DELAY + waitTime, eventTimeoutTX);
        }
    }
    else if (msgIn == eventTimeoutRX1) {
        // Previous window is ended, update the transmission window state of the end device
        transmissionWindowState = WINDOW_RX_1;
        //messageReceived = MSG_NULL;
        messageReceived = false;
        EV << "Timeout expired. RX1 window begins\n";
    }
    else if (msgIn == eventTimeoutRX2) {
        // Previous window is ended, update the transmission window state of the end device
        transmissionWindowState = WINDOW_RX_2;
        EV << "Timeout expired. RX2 window begins\n";
    }
    else if (msgIn == eventTimeoutDutyCycle) {
        // Duty cycle interval is ended
        dutyCycleUsed          = 0;
        dutyCycleStartInterval = 0;
        EV << "Timeout expired. Duty cycle interval reset\n";
    }
    else {
        // The message is not a timeout and comes from another LoRa device.
        // Check if the end device is listening
        if (transmissionWindowState == WINDOW_TX || messageReceived) {
            messageReceived ? EV << "Already listened a frame in this receive window\n" :
                              EV << "The device is not in a receive window\n";

            // Send signal for statistic collection
            emit(signalInvalid, 1u);
            emit(signalInvalidCount, ++messagesInvalid);

            delete msgIn;
            return;
        }

        //======================
        // Replaced with background noise and interferences

        // Lose the message with a certain probability
        //if (uniform(0, 1) < 2) {
        //if (uniform(0, 1) < MSG_LOSS_PROBABILITY) {
        /*if (((rand() + (int) uniform(0,100)) % 101) < MSG_LOSS_PROBABILITY) {
            EV << "Message lost!\n";

            // Make animation more informative
            bubble("Message lost!");

            // Delete the message as in OMNeT++ once sent out,
            // a message no longer belongs to the sender module and
            // it is taken over by the simulation kernel, and will eventually be delivered to the destination module.
            // Once the message arrives in the destination module, that module will have full authority over it
            delete msgIn;

            // Send signal for statistic collection
            emit(signalLost, 1u);
            emit(signalLostCount, ++messagesLost);

            return;
        }*/
        //======================

        if (!surviveMessageToLoRaInterference(msgIn))
            return;

        // The end device is in a receive window
        uint8_t port;
        uint8_t payloadIn[LORA_FRAME_SIZE_APP_PAYLOAD] = {};
        uint8_t frameType;

        // Check if the frame is valid and get the port
        // (ignore for the moment possible BAD_REQUEST_ID because the message can be a MAC command)
        uint8_t error = isValidLoRaFrame(msgIn, &frameType, &port, payloadIn);
        EV << "Error: " << (unsigned) error << "\n";
        EV << "Frame Type: " << (unsigned) frameType << "\n";

        if (error && error != BAD_REQUEST_ID) {
            printError(error);

            // Send signal for statistic collection
            emit(signalInvalid, 1u);
            emit(signalInvalidCount, ++messagesInvalid);

            delete msgIn;
            return;
        }

        // Send signal for statistic collection
        emit(signalReceived, 1u);
        emit(signalReceivedCount, ++messagesIn);

        // Check if the message is received in the first receive window
        if (transmissionWindowState == WINDOW_RX_1) {
            EV << "Canceling second receive window...\n";
            cancelEvent(eventTimeoutRX2);
        }

        // Check if the message is a JOIN_ACCEPT
        if (stage == STAGE_ACTIVATION && frameType == JOIN_ACCEPT) {
            // To obtain the correct Join Accept use MIC and difference root keys per end device

            // Only a message can be received in a receive window
            //messageReceived = MSG_JOIN_ACCEPT;
            messageReceived = true;

            EV << "Received JOIN_ACCEPT message\n";

            // Retrieve information from the message
            LoRaPhysicalFrame* gatewayPhysicalMsg = dynamic_cast<LoRaPhysicalFrame*>(msgIn);
            LoRaDatalinkFrame* gatewayDatalinkMsg = dynamic_cast<LoRaDatalinkFrame*>(gatewayPhysicalMsg->getEncapsulatedPacket());
            LoRaJoinAcceptFrame* joinAcceptMsg    = dynamic_cast<LoRaJoinAcceptFrame*>(gatewayDatalinkMsg->getEncapsulatedPacket());

            uint32_t joinNonce = joinAcceptMsg->getJoinNonce();
            getArrayInMessageJoinAccept(joinAcceptMsg, &LoRaJoinAcceptFrame::getNetID, networkId, NETWORK_ID_SIZE);
            getArrayInMessageJoinAccept(joinAcceptMsg, &LoRaJoinAcceptFrame::getDevAddr, address, IPv4_ADDRESS_SIZE);
            //uint8_t DLSettings = joinAcceptMsg->getDLSettings();
            //uint8_t RXDelay    = joinAcceptMsg->getRXDelay();

            // Elaborate information
            EV << "joinNonce: " << std::hex << joinNonce << std::dec << "\n";

            printKey(networkId, sizeof(networkId), "networkId", EV);

            EV << "End device IP address: "
                << (int) address[0] << "."
                << (int) address[1] << "."
                << (int) address[2] << "."
                << (int) address[3]
                << "\n";

            // Derive session key exclusively shared with the network server
            memcpy(nwkSKey, generateKey(nwkKey, joinNonce, nonceDev, networkId), sizeof(nwkSKey));

            // Derive session key exclusively shared with the application server
            memcpy(appSKey, generateKey(appKey, joinNonce, nonceDev, networkId), sizeof(appSKey));

            printKey(nwkSKey, sizeof(nwkSKey), "nwkSKey", EV);
            printKey(appSKey, sizeof(appSKey), "appSKey", EV);

            EV << "Canceling next transmit window...\n";
            cancelEvent(eventTimeoutTX);

            // Go to next stage
            stage = STAGE_GENERATE_COMMON_KEY;

            // Wake up when the gateway activation should be finished ([180, 360] seconds)
            //int waitTime = 180 + (rand() + (int) uniform(0, 180)) % 181;
            //int waitTime = 10 + (rand() + (int) uniform(0, 180)) % 3; // XXX: temp for heavy testing
            std::random_device rd {};
            std::mt19937 gen { rd() };
            std::uniform_int_distribution<> distribution(180, 360);
            int waitTime = distribution(gen);
            EV << "Wake up after " << waitTime << " seconds\n";
            scheduleAt(simTime() + waitTime, eventTimeoutTX);

            delete msgIn;
            return;
        }

        EV << "Received message for the end device with port: " << (int) port << "\n";

        // Check if the message is a MAC command
        if (port == MSG_PORT_MAC_CMD) {
            // Only a message can be received in a receive window
            //messageReceived = MSG_MAC_CMD;
            messageReceived = true;

            // Execute command from LoRa network server
            EV << "Executing MAC command from LoRa network server...\n";
            delete msgIn;
            return;
        }
        else if (stage == STAGE_GENERATE_COMMON_KEY) {
            // Only a message can be received in a receive window
            messageReceived = true;

            // Check if the message has been received by at least a gateway
            if (payloadIn[0] == ACK) {
                EV << "Received ACK for GENERATE_COMMON_KEY message\n";

                // Use the stats messages map to store that an ACK has been received
                statsMessages.insert(std::array<uint8_t, IPv4_ADDRESS_SIZE> {0,0,0,0});

                // Only a message can be received in a receive window
                //messageReceived = MSG_ACK;
            }
            else if (payloadIn[0] == NACK) {
                EV << "Received NACK for GENERATE_COMMON_KEY message\n";

                // Only a message can be received in a receive window
                //messageReceived = MSG_NACK;

                EV << "Canceling next transmit window...\n";
                cancelEvent(eventTimeoutTX);

                // Wake up when the gateway activation should be finished ([120, 240] seconds)
                int waitTime = 120 + (rand() + (int) uniform(0, 120)) % 121;
                EV << "Wake up after " << waitTime << " seconds\n";
                scheduleAt(simTime() + waitTime, eventTimeoutTX);
            }

            delete msgIn;
            return;
        }

        // Check if the request ID included in the message is valid as the message is not a MAC command
        if (error) {
            printError(error);
            delete msgIn;
            return;
        }

        // Only a message can be received in a receive window
        messageReceived = true;

        // Check if the message is received during STAGE_HELLO or STAGE_FORWARD
        if (stage <= STAGE_FORWARD) {
            // Only a message can be received in a receive window
            //messageReceived = MSG_STATS;

            EV << "Received STATS message\n";

            // Convert timestamp to array of bytes
            //std::array<uint8_t, 4> timestampBytes;
            //uintToBytes(timestamp, timestampBytes.data());

            // Collect gateway replies
            std::array<uint8_t, IPv4_ADDRESS_SIZE> gatewayAddress;

            // Get sender gateway position for calculating the RSSI
            LoRaGateway* gateway = dynamic_cast<LoRaGateway*>(msgIn->getSenderModule());
            unsigned gatewayPosX = gateway->getPosX();
            unsigned gatewayPosY = gateway->getPosY();
            int rssi = calculateRSSI(this, gatewayPosX, gatewayPosY, posX, posY, EV);
            EV << "RSSI: " << rssi << "\n";

            // Compute the score associated with the STATS message and
            // get the IP address of the gateway
            int score;
            analyzeStatsMessage(payloadIn, rssi, gatewayAddress.data(), &score);

            EV << "Gateway IP address: "
                << (int) gatewayAddress[0] << "."
                << (int) gatewayAddress[1] << "."
                << (int) gatewayAddress[2] << "."
                << (int) gatewayAddress[3]
                << "\n";
            EV << "Score: " << score << "\n";

            // Check if the gateway IP address is already present in the map
            if (statsMessages.find(gatewayAddress) == statsMessages.end())
                // The gateway IP address is not in the map, add it
                statsMessages.insert(gatewayAddress);

            // Push the gateway IP address in the list of gateways to be ACKed
            gateways2ACK.push_back(gatewayAddress);

            // Insert the gateway IP address in the map indexed by the score
            // using the expected arrival time of last HELLO/FORWARD message
            //insertGatewayInDecisionMap(score, gatewayAddress, timestampBytes);
            //insertGatewayInDecisionMap(score, gatewayAddress, timestamp);
            insertGatewayInDecisionMap(score, gatewayAddress, msgOutArrivalTime);
        }

        /*else if (stage == STAGE_PAIRING) {
            // Check if the incoming message is a response from the selected gateway
            if (port == MSG_PORT_PAIRING_ACCEPT) {
                // Only a message can be received in a receive window
                messageReceived = MSG_PAIRING_ACCEPT;

                EV << "Received PAIRING_ACCEPT message";

                // Check if the pairing request has been accepted by the selected gateway
                isPairingAccepted = isPairingRequestAccepted(msgIn);
                if (!isPairingAccepted) {
                    // Two solutions because the network server verification should arrive after the gateway ACK:
                    // - Await the verification of the network server before start sending data
                    //   to ignore subsequent messages received during sending (this can be also avoided
                    //   checking the isPairingVerified variable that should). The real advantage is to avoid managing
                    //   the LoRaWAN limitation where the ACK/NACK is sent and the data must be queued and sent
                    //   in the next transmission but the end device can be blocked in non starting sending data
                    //   if the message from the network server never arrives
                    // - Start sending but manage the request of the server verification during the sending
                    //   by queuing data and then sending a pair in the next transmission. Manage also ignore
                    //   possible other requests.

                    EV << " with NACK";

                    // Search another gateway
                    stage = STAGE_FORWARD;
                    searchNewGateway = true;
                    isPairingVerified = false;
                }

                EV << "\n";
            }
            else {
                // Only a message can be received in a receive window
                messageReceived = MSG_PAIRING_VERIFY;

                EV << "Received PAIRING_VERIFY message\n";

                // The message comes from the network server (only if the pairing message has been accepted by the gateway)
                // Check incoming message from network server
                isPairingVerified = verifyPairing(msgIn);
            }
        }*/
        else if (stage == STAGE_PAIRING) {
            // Only a message can be received in a receive window
            //messageReceived = MSG_PAIRING_ACCEPT;

            // Check if the pairing request has been accepted by the selected gateway
            if (payloadIn[1] == ACK)
                EV << "Received ACK for PAIRING_REQUEST message\n";
            else if (payloadIn[1] == NACK) {
                EV << "Received NACK for PAIRING_REQUEST message\n";

                // Search another gateway
                stage = STAGE_FORWARD;
                searchNewGateway = true;
                //isPairingVerified = false;
            }
            else {
                EV << "Received STATS_UPDATE for PAIRING_REQUEST message\n";
                // The decision was based on an old state of the gateway.
                // Get reception timestamp (almost equal to sending timestamp as radio waves go very fast)
                //simtime_t timestamp = simTime();
                //std::time_t timestamp = std::time(nullptr);
                //EV << "Timestamp: " << timestamp << "\n";

                // Convert timestamp to array of bytes
                //std::array<uint8_t, 4> timestampBytes;
                //uintToBytes(timestamp, timestampBytes.data());

                // Get sender gateway position for calculating the RSSI
                LoRaGateway* gateway = dynamic_cast<LoRaGateway*>(msgIn->getSenderModule());
                unsigned gatewayPosX = gateway->getPosX();
                unsigned gatewayPosY = gateway->getPosY();
                int rssi = calculateRSSI(this, gatewayPosX, gatewayPosY, posX, posY, EV);
                EV << "RSSI: " << rssi << "\n";

                // Recalculate score about the selected gateway
                int score = calculateScoreStatsMessage(&payloadIn[2], rssi);
                EV << "Score: " << score << "\n";

                // Insert the gateway IP address in the map indexed by the score
                //insertGatewayInDecisionMap(score, selectedGatewayAddress, timestampBytes);
                //insertGatewayInDecisionMap(score, selectedGatewayAddress, timestamp);
                insertGatewayInDecisionMap(score, selectedGatewayAddress, msgOutArrivalTime);

                // Search another gateway
                stage = STAGE_FORWARD;
                searchNewGateway = true;
                //isPairingVerified = false;

                delete msgOut;
            }
        }
        else if (stage == STAGE_CONNECTION) {
            // Only a message can be received in a receive window
            //messageReceived = MSG_CONNECTION_ACCEPT;

            statsMessages.clear();
        }
        else if (stage == STAGE_GENERATE_ASSOCIATION_KEY) {
            // The previous message has been received, go to next stage
            stage = STAGE_DATA_PROFILE;

            // Get the gateway nonce
            uint8_t nonceGw_[4] = {
                payloadIn[1],
                payloadIn[2],
                payloadIn[3],
                payloadIn[4]
            };

            unsigned nonceGw = bytesToUint(nonceGw_);

            EV << "nonceGw: " << nonceGw << "\n";
            EV << "nonceDev: " << nonceDev << "\n";

            // Derive session key exclusively shared with the associated gateway
            //strncpy(associationSKey, generateKey(associationKey, nonceGw, nonceDev, networkId), sizeof(associationSKey));
            memcpy(associationSKey, generateKey(associationKey, nonceGw, nonceDev, networkId), sizeof(associationSKey));

            printKey(associationSKey, sizeof(associationSKey), "associationSKey", EV);

            delete msgOut;
        }
        else if (stage == STAGE_DATA_PROFILE) {
            // The previous message has been received, update the string index
            // because must be updated only when the previous message has been received.
            // Be careful a lost response causes a retransmission and a bad final reception
            // (e.g. two times Sapienza/Bu results in Sapienza/BuSapienza/Bu at the receiver)
            locationStringIndex += LORA_FRAME_SIZE_APP_PAYLOAD-1;

            // Check if the data profile has ended
            if (locationStringIndex >= LOCATION_STRING_LENGTH || !locationString[locationStringIndex])
                // The data profile is ended and the pairing algorithm too
                stage = STAGE_DATA;
        }

        delete msgIn;
    }
}

void LoRaEndDevice::refreshDisplay() const {
    char buf[50] = {};
    snprintf(buf, sizeof(buf), "in: %lu out: %lu", messagesIn, messagesOut);
    getDisplayString().setTagArg("t", 0, buf);
}

// Check if the port is valid w.r.t. the end device stage
bool LoRaEndDevice::isValidPort(uint8_t port) {
    return (stage == STAGE_GENERATE_COMMON_KEY && port == MSG_PORT_GENERATE_COMMON_KEY)                     ||
           (stage == STAGE_HELLO && port == MSG_PORT_STATS)                                                 ||
           (stage == STAGE_FORWARD && port == MSG_PORT_STATS)                                               ||
           //(stage == STAGE_PAIRING && (port == MSG_PORT_PAIRING_ACCEPT || port == MSG_PORT_PAIRING_VERIFY)) ||
           (stage == STAGE_PAIRING && port == MSG_PORT_PAIRING_ACCEPT)                                      ||
           (stage == STAGE_CONNECTION && port == MSG_PORT_CONNECTION)                                       ||
           (stage == STAGE_GENERATE_ASSOCIATION_KEY && port == MSG_PORT_GENERATE_ASSOCIATION_KEY)           ||
           (stage == STAGE_DATA_PROFILE && port == MSG_PORT_DATA_PROFILE)                                   ||
           (stage == STAGE_DATA && port == MSG_PORT_DATA)                                                   ||
           port == MSG_PORT_MAC_CMD;
}

// Check if the frame is valid and get the message port.
// Return the specific error in case of invalid frame, 0 if valid
uint8_t LoRaEndDevice::isValidLoRaFrame(cMessage* msg, uint8_t* frameType, uint8_t* port, uint8_t* payload) {
    if (!msg || !frameType || !port || !payload)
        return BAD_PARAMS;

    LoRaPhysicalFrame* gatewayPhysicalMsg = dynamic_cast<LoRaPhysicalFrame*>(msg);
    if (!gatewayPhysicalMsg)
        return BAD_FRAME;

    LoRaDatalinkFrame* gatewayDatalinkMsg = dynamic_cast<LoRaDatalinkFrame*>(gatewayPhysicalMsg->getEncapsulatedPacket());
    if (!gatewayDatalinkMsg)
        return BAD_FRAME;

    // Get the frame type
    *frameType = gatewayDatalinkMsg->getFType();
    if (*frameType == JOIN_ACCEPT) {
        LoRaJoinAcceptFrame* joinAcceptMsg = dynamic_cast<LoRaJoinAcceptFrame*>(gatewayDatalinkMsg->getEncapsulatedPacket());
        if (!joinAcceptMsg)
            return BAD_FRAME;

        int numKeys = 1;
        uint8_t* keys[numKeys] = { nwkKey };

        // Check the MIC of the message for data integrity & authentication
        if (!isValidMIC(gatewayDatalinkMsg, joinAcceptMsg, keys, numKeys))
            return BAD_MIC;

        return 0;
    }
    else if (stage == STAGE_ACTIVATION) {
        return BAD_FRAME;
    }

    LoRaAppDownlinkFrame* gatewayAppMsg = dynamic_cast<LoRaAppDownlinkFrame*>(gatewayDatalinkMsg->getEncapsulatedPacket());
    if (!gatewayAppMsg)
        return BAD_FRAME;

    // Get the device address
    uint8_t deviceAddress[IPv4_ADDRESS_SIZE];
    getArrayInMessageDownlink(gatewayAppMsg, &LoRaAppDownlinkFrame::getDeviceAddress, deviceAddress, IPv4_ADDRESS_SIZE);

    /*EV << "End device IP address: "
        << (int) address[0] << "."
        << (int) address[1] << "."
        << (int) address[2] << "."
        << (int) address[3]
        << "\n";
    EV << "Received end device IP address: "
        << (int) deviceAddress[0] << "."
        << (int) deviceAddress[1] << "."
        << (int) deviceAddress[2] << "."
        << (int) deviceAddress[3]
        << "\n";*/

    // Check if the frame is intended for the current end device address
    if (memcmp(address, deviceAddress, IPv4_ADDRESS_SIZE))
        return BAD_ADDRESS;

    // The MIC can be verified with the nwkSKey or with the commonSKey or
    // with the associationSKey or with the commonKey (only during session keys generation).
    // The end device does not know who sent the message, in the original protocol only the network server can send
    int numKeys = 2;
    uint8_t* keys[numKeys] = { nwkSKey };

    if (stage == STAGE_GENERATE_COMMON_KEY)
        keys[1] = commonKey;
    else if (stage == STAGE_DATA_PROFILE)
        keys[1] = associationSKey;
    else
        keys[1] = commonSKey;

    // Check the MIC of the message for data integrity & authentication
    if (!isValidMIC(gatewayDatalinkMsg, gatewayAppMsg, keys, numKeys))
        return BAD_MIC;

    // Check if the frame is a retransmission
    uint16_t counter = gatewayAppMsg->getCounter();
    if (counter <= fCntDown)
        return BAD_COUNTER;
    // Update frame counter for downlinks
    // ATTENZIONE PERÒ PERCHÈ SE FRAME NON VALIDA POI NON RICEVO PIÙ IL MESSAGGIO VERO?
    // DIPENDE SE INCREMENTA CHI MANDA O MENO.
    // DEVO INCREMENTARE SOLO QUANDO LA FRAME È VALIDA?
    fCntDown = counter;

    // Check if the message port is valid
    *port = gatewayAppMsg->getPort();
    if (!isValidPort(*port))
        return BAD_PORT;

    getArrayInMessageDownlink(gatewayAppMsg, &LoRaAppDownlinkFrame::getPayload, payload, LORA_FRAME_SIZE_APP_PAYLOAD);

    // Check if the requestId matches
    return requestId == payload[0] ? 0 : BAD_REQUEST_ID;
}

void LoRaEndDevice::printError(uint8_t error) {
    switch (error) {
        case BAD_ADDRESS:
            EV << "Received frame for a different end device address\n";
            break;
        case BAD_MIC:
            EV << "Received invalid MIC in the frame\n";
            // Only a message can be received in a receive window
            //messageReceived = MSG_MAC_CMD;
            break;
        case BAD_COUNTER:
            EV << "Received invalid counter in the frame\n";
            // Only a message can be received in a receive window
            //messageReceived = MSG_MAC_CMD;
            break;
        case BAD_PORT:
            EV << "Received invalid port in the frame\n";
            // Only a message can be received in a receive window
            //messageReceived = MSG_MAC_CMD;
            break;
        case BAD_REQUEST_ID:
            EV << "Received invalid request ID in the frame\n";
            // Only a message can be received in a receive window
            //messageReceived = MSG_MAC_CMD;
            break;
        default:
            EV << "Received invalid frame, error: " << (int) error << "\n";
    }
}

// Calculate the score of the STATS message where payload is already shifted to the first useful parameter
int LoRaEndDevice::calculateScoreStatsMessage(uint8_t* payload, int rssi) {
    // Get CPU-load, GPU-load, storage IO, network IO, IP address (sorted by priority)
    // STATS message structure
    // request ID -> 1 byte
    // CPU-load -> 1 byte (e.g. 20 means 20%, float truncated)
    // GPU-load -> 1 byte (e.g. 13 means 13%, float truncated)
    // RAM load -> 1 byte (e.g. 30 means 30%, float truncated)
    // storage occupancy -> 1 byte (e.g. 55 means 55%, float truncated)
    // storage input -> 1 byte (e.g. 20 means 20%, float truncated) ?
    // storage output -> 1 byte (e.g. 20 means 20%, float truncated) ?
    // network input -> 1 byte (e.g. 20 means 20%, float truncated)
    // network output -> 1 byte (e.g. 20 means 20%, float truncated)
    // IP address -> 4 bytes (e.g. 192, 168, 1, 1 means 192.168.1.1)
    // Total 11(+2?) bytes :)

    uint8_t cpuLoad = payload[0];
    if (cpuLoad > CPU_LOAD_THRESHOLD)
        return -1;

    uint8_t gpuLoad = payload[1];
    if (gpuLoad > GPU_LOAD_THRESHOLD)
        return -1;

    uint8_t ramLoad = payload[2];
    if (ramLoad > RAM_LOAD_THRESHOLD)
        return -1;

    uint8_t storageLoad = payload[3];
    if (storageLoad > STORAGE_LOAD_THRESHOLD)
        return -1;

    uint8_t networkIn = payload[4];
    if (networkIn > NETWORK_IN_THRESHOLD)
        return -1;

    uint8_t networkOut = payload[5];
    if (networkOut > NETWORK_OUT_THRESHOLD)
        return -1;

    EV << "cpu load: " << (int) cpuLoad << "\n";
    EV << "gpu load: " << (int) gpuLoad << "\n";
    EV << "ram load: " << (int) ramLoad << "\n";
    EV << "storage load: " << (int) storageLoad << "\n";
    EV << "network IN: " << (int) networkIn << "\n";
    EV << "network OUT: " << (int) networkOut << "\n";

    // XXX: Unweighted sum vs weighted sum
    // Sum -RSSI to obtain lower score, better gateway
    return cpuLoad + gpuLoad + ramLoad + storageLoad + networkIn + networkOut - rssi;
}

void LoRaEndDevice::analyzeStatsMessage(uint8_t* payload, int rssi, uint8_t* gatewayIpAddress, int* score) {
    // Copy the IP address of the gateway
    memcpy(gatewayIpAddress, &payload[7], IPv4_ADDRESS_SIZE);

    // Get CPU-load, GPU-load, storage IO, network IO, IP address (sorted by priority)
    // STATS message structure
    // request ID -> 1 byte
    // CPU-load -> 1 byte (e.g. 20 means 20%, float truncated)
    // GPU-load -> 1 byte (e.g. 13 means 13%, float truncated)
    // RAM load -> 1 byte (e.g. 30 means 30%, float truncated)
    // storage occupancy -> 1 byte (e.g. 55 means 55%, float truncated)
    // storage input -> 1 byte (e.g. 20 means 20%, float truncated) ?
    // storage output -> 1 byte (e.g. 20 means 20%, float truncated) ?
    // network input -> 1 byte (e.g. 20 means 20%, float truncated)
    // network output -> 1 byte (e.g. 20 means 20%, float truncated)
    // IP address -> 4 bytes (e.g. 192, 168, 1, 1 means 192.168.1.1)
    // Total 11(+2?) bytes :)

    *score = calculateScoreStatsMessage(&payload[1], rssi);
}

// Insert the gateway & timestamp at which the STATS message was received in the map indexed by the associated score
void LoRaEndDevice::insertGatewayInDecisionMap(
        int score,
        std::array<uint8_t, IPv4_ADDRESS_SIZE>& gatewayAddress,
        //std::array<uint8_t, 4>& timestampBytes) {
        //std::time_t timestamp) {
        simtime_t timestamp) {
    // If score is invalid (i.e. gateway does not satisfy requirements),
    // then do not insert the gateway in the map of sorted gateways
    // from which then select the best gateway(s)
    if (score < 0)
        return;

    // Create the tuple to insert in the list
    //std::tuple<std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::array<uint8_t, 4>> tuple {gatewayAddress, timestampBytes};
    //std::tuple<std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::time_t> tuple {gatewayAddress, timestamp};
    std::tuple<std::array<uint8_t, IPv4_ADDRESS_SIZE>, simtime_t> tuple {gatewayAddress, timestamp};

    // Check if the score is already present in the map
    if (sortedGateways.find(score) != sortedGateways.end()) {
        // The score is in the map, add the gateway to the corresponding list
        // to front to select the last STATS message received for a given score
        // as it is closer in time
        //sortedGateways[score].push_back(gatewayAddress);
        //sortedGateways[score].push_front(gatewayAddress);

        sortedGateways[score].push_front(tuple);
    }
    else {
        // The score is not in the map
        // Create a list and push the gateway
        //std::list<std::tuple<std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::array<uint8_t, 4>>> listGateways {tuple};
        //std::list<std::tuple<std::array<uint8_t, IPv4_ADDRESS_SIZE>, std::time_t>> listGateways {tuple};
        std::list<std::tuple<std::array<uint8_t, IPv4_ADDRESS_SIZE>, simtime_t>> listGateways {tuple};

        // Add the score and the corresponding list in the map of sorted gateways
        sortedGateways[score] = listGateways;
    }
}

/*bool LoRaEndDevice::verifyPairing(cMessage *msg) {
    // Analyze network server checking RP IP address
    LoRaDatalinkFrame* gatewayDatalinkMsg = check_and_cast<LoRaDatalinkFrame*>(msg);
    LoRaAppDownlinkFrame* gatewayAppMsg = check_and_cast<LoRaAppDownlinkFrame*>(gatewayDatalinkMsg->getEncapsulatedPacket());

    // Get the payload
    uint8_t payload[LORA_FRAME_SIZE_APP_PAYLOAD];
    getArrayInMessageDownlink(gatewayAppMsg, &LoRaAppDownlinkFrame::getPayload, payload, LORA_FRAME_SIZE_APP_PAYLOAD);

    // Check if the gateway addresses match
    return memcmp(selectedGatewayAddress, &payload[1], IPv4_ADDRESS_SIZE) == 0 ? true : false;
}*/


// Return the sensor's measurement (0-100 -> 0-1 L)
uint8_t LoRaEndDevice::readSensor() {
    return rand() % 101;
}

// Send end device message considering interferences and duty cycle
void LoRaEndDevice::sendMessage(bool sendDuplicate) {
    // Check if the duty cycle interval is started through the used duty cycle
    if (dutyCycleUsed == 0) {
        // Start duty cycle interval
        simtime_t simtime = simTime();
        dutyCycleStartInterval = simtime;

        // Schedule reset of duty cycle at the end of the interval
        scheduleAt(simtime + DUTY_CYCLE_INTERVAL, eventTimeoutDutyCycle);
    }

    EV << "Calculate Time On Air\n";

    // Calculate preamble and frame airtime
    auto tuple = calculateTimeOnAir(msgOut);

    // Convert frame time on air from ms to s
    double airtimePreamble = std::get<0>(tuple) / 1000;
    double airtimeFrame    = std::get<1>(tuple) / 1000;

    // Check if computation succeeded
    if (airtimePreamble < 0 || airtimeFrame < 0)
        return;

    // Calculate arrival times
    simtime_t simtime = simTime();
    simtime_t arrivalPreamble = simtime + airtimePreamble;
    simtime_t arrivalFrame    = simtime + airtimeFrame;

    EV << "Simulation time: " << simtime << "\n";
    EV << "Expected arrival preamble time: " << arrivalPreamble << "\n";
    EV << "Expected arrival frame time: " << arrivalFrame << "\n";

    // Save expected arrival time
    msgOutArrivalTime = arrivalFrame;

    // Update total time on air of the end device to be compliant with duty cycle
    dutyCycleUsed += airtimeFrame;
    EV << "Consumed time on air in duty cycle interval: " << dutyCycleUsed/DUTY_CYCLE_INTERVAL << "\n";

    // Calculate transmission data rate
    double datarate = calculateDatarate(msgOut); // bits/s

    // Update channels data rate and delay
    for (int i=0; i<gateSize(LORA_GATE_OUT); i++) {
        // OUT->IN connection
        cGate* deviceGate  = gate(LORA_GATE_OUT, i);
        cDatarateChannel* channel = dynamic_cast<cDatarateChannel*>(deviceGate->getChannel());
        channel->setDatarate(datarate);
        channel->setDelay(airtimeFrame);
        channel->callInitialize();

        //EV << "deviceGate OUT:" << deviceGate << "\n";

        // IN<-OUT connection
        cGate* gatewayGate = deviceGate->getNextGate();
        gatewayGate = gatewayGate->getOtherHalf();
        channel = dynamic_cast<cDatarateChannel*>(gatewayGate->getChannel());
        channel->setDatarate(datarate);
        channel->setDelay(airtimeFrame);
        channel->callInitialize();

        //EV << "gatewayGate OUT:" << gatewayGate << "\n";
    }


    EV << "Send message\n";
    cPacket* msg = sendDuplicate ? msgOut->dup() : msgOut;
    sendBroadcast(this, msg, LORA_GATE_OUT);

    EV << "Notify neighbors\n";
    EV << "neighborDevicesInterferences.size(): " << neighborDevicesInterferences.size() << "\n";

    // Notify neighbors for handling interferences
    notifyNeighborDevices(msg, arrivalPreamble, arrivalFrame);

    EV << "Verify Transmission Interference\n";

    // Verify if the message interferes with previously received messages
    // To apply multiple interference to a message use the ID that is shared among duplicates available to neighbors
    // instead of the pointer that is related to the particular duplicate
    verifyTransmissionInterference(msg);

    //****************
    EV << "Msg name: " << msgOut->getName() << "\n";
    EV << "Msg creation time: " << msgOut->getCreationTime() << "\n";
    EV << "Msg arrival time: " << msgOut->getArrivalTime() << "\n";
    EV << "Msg sending time: " << msgOut->getSendingTime() << "\n";
    EV << "Msg duration time: " << msgOut->getDuration() << "\n";

    EV << "Msg->dup() name: " << msg->getName() << "\n";
    EV << "Msg->dup() creation time: " << msg->getCreationTime() << "\n";
    EV << "Msg->dup() arrival time: " << msg->getArrivalTime() << "\n";
    EV << "Msg->dup() sending time: " << msg->getSendingTime() << "\n";
    EV << "Msg->dup() duration time: " << msg->getDuration() << "\n";
    //****************
}

// Resend end device message considering interferences and duty cycle
void LoRaEndDevice::resendMessage(uint8_t* keyMIC) {
    // By default NbTrans parameter is 1.
    // If the gateway receives the message but the response is lost,
    // if the frame counter for uplinks does not increment,
    // then the gateway does not accept the frame because detects a duplicate

    // Check if the message is a LoRa frame (should be)
    LoRaPhysicalFrame* phyMsg = dynamic_cast<LoRaPhysicalFrame*>(msgOut);
    if (!phyMsg)
        return;

    LoRaDatalinkFrame* dlMsg = dynamic_cast<LoRaDatalinkFrame*>(phyMsg->getEncapsulatedPacket());
    if (!dlMsg)
        return;

    // Decapsulate LoRa App layer
    LoRaAppUplinkFrame* appMsg = dynamic_cast<LoRaAppUplinkFrame*>(dlMsg->decapsulate());
    if (!appMsg)
        return;

    // Increment the frame counter
    appMsg->setCounter(++fCntUp);

    // Encapsulate LoRa App layer
    dlMsg->encapsulate(appMsg);

    // Recalculate MIC
    //calculateMIC(dlMsg, appMsg, keyMIC);
    calculateMIC(dlMsg, keyMIC);

    // Do not need to update stored message as the physical pointer is still valid
    //msgOut = phyMsg;

    // Send the updated message
    sendMessage(true);
}

// Notify neighbors for handling interferences
void LoRaEndDevice::notifyNeighborDevices(cPacket* msg, simtime_t arrivalPreamble, simtime_t arrivalFrame) {
    // Notify neighbor end devices about the transmitted message
    //================================
    //for (std::tuple<cModule*, simsignal_t> tuple : neighborDevicesSignalInterferences)
    //    std::get<0>(tuple)->emit(std::get<1>(tuple), msg);
    //================================

    //================================
    for (auto neighborDevice : neighborDevicesInterferences)
        neighborDevice->receiveNotification(msg, arrivalPreamble, arrivalFrame);
    //================================
}

// Notify neighbors for handling interferences
void LoRaEndDevice::notifyNeighborGateways(
        //cPacket* msgInterference, std::list<std::tuple<cPacket*, int, bool>>& interferedMessages) {
        cPacket* msgInterference, std::list<std::tuple<cPacket*, cModule*, bool>>& interferedMessages) {
    //=============================
    // Create a wrapper message including the possible interfering message and the interfered ones
    /*cMessage* msg = new cMessage("Interference");

    // Cannot insert the message that has been sent on the channel
    //msg->addObject(msgInterference);
    // I have to send a duplicate
    msg->addObject(msgInterference->dup());

    // Iterate interfered messages
    for (std::tuple<cPacket*, bool> tuple : msgsInterfered) {
        cPacket* msgInterfered = std::get<0>(tuple);
        std::get<1>(tuple) ? msgInterfered->setKind(1) : msgInterfered->setKind(2);
        //msg->addObject(msgInterfered);
        msg->addObject(msgInterfered->dup());
    }

    // Notify neighbor gateways about the interference
    //for (std::tuple<cModule*, simsignal_t> tuple : neighborGatewaysSignalInterferences)
    //    std::get<0>(tuple)->emit(std::get<1>(tuple), msg);*/
    //=============================

    //=============================
    // Without signals there is no need for the wrapper message

    for (auto neighborGateway : neighborGatewaysInterferences)
        dynamic_cast<LoRaGateway*>(neighborGateway)->handleInterferenceUplink(msgInterference, interferedMessages);
    //=============================
}

// Verify if the end device transmission interferes with at least a message collected
// via notification by a neighbor and notify the gateways in the radio range
void LoRaEndDevice::verifyTransmissionInterference(cPacket* msg) {
    //=============================================
    // Iterate list of neighbour messages
    /*for (std::tuple<cMessage*, simtime_t, simtime_t> tuple : neighbourMessages) {
        // Verify if a possible interference occurred during the preamble transmission
        if(msg->getSendingTime() <= std::get<1>(tuple))
            // Notify neighbour gateways of the possible interference
            notifyNeighbours(msg, std::get<0>(tuple));

        // Verify if a possible interference occurred during the payload transmission
        else if (msg->getSendingTime() <= std::get<2>(tuple))
            // Notify neighbour gateways of the possible interference
            notifyNeighbours(msg, std::get<0>(tuple));

        else
            // The tuple is expired, remove the head of the list since tuples are entered at the end of the list
            neighbourMessages.pop_front();
    }*/
    //=============================================


    //=============================================
    // List of possible interfered messages composed of tuples
    // (interfered message, is interfering during preamble?)
    // to send just a signal instead of one per possible interference
    //std::list<std::tuple<cPacket*, int, bool>> interferedMessages;
    std::list<std::tuple<cPacket*, cModule*, bool>> interferedMessages;

    EV << "verifyTransmissionInterference - Msg sending time: " << msg->getSendingTime() << "\n";

    int removals = 0;
    for (auto tuple : neighborMessages) {
        EV << "verifyTransmissionInterference - Neighbor msg preamble expiration: " << std::get<1>(tuple) << "\n";
        EV << "verifyTransmissionInterference - Neighbor msg frame expiration: " << std::get<2>(tuple) << "\n";

        cPacket* neighborMsg = std::get<0>(tuple);
        //int neighborId       = neighborMsg->getSenderModuleId();
        cModule* neighborId  = neighborMsg->getSenderModule();
        EV << "verifyTransmissionInterference - NeighborId: " << neighborId << "\n";

        // Verify if a possible interference occurred during the preamble transmission
        if(msg->getSendingTime() <= std::get<1>(tuple))
            //interferedMessages.push_back(std::tuple<cPacket*, int, bool> {neighborMsg, neighborId, true});
            interferedMessages.push_back(std::tuple<cPacket*, cModule*, bool> {neighborMsg, neighborId, true});

        // Verify if a possible interference occurred during the header+payload transmission
        else if (msg->getSendingTime() <= std::get<2>(tuple))
            //interferedMessages.push_back(std::tuple<cPacket*, int, bool> {neighborMsg, neighborId, false});
            interferedMessages.push_back(std::tuple<cPacket*, cModule*, bool> {neighborMsg, neighborId, false});

        else
            // The tuple is expired, remove the head of the list since tuples are appended at the end of the list
            //neighborMessages.pop_front(); // QUI CRASHA
            removals++;
    }

    EV << "Items to remove from neighborMessages: " << removals << "\n";

    // TODO: Integrate the removal into above iteration
    for (int i=0; i < removals; i++)
        neighborMessages.pop_front();

    // Check if a possible interference occurred
    if (!interferedMessages.empty())
        // Notify neighbor gateways of the possible interferences
        notifyNeighborGateways(msg, interferedMessages);
    //=============================================
}

// Return true if the message survives to interference, false otherwise
bool LoRaEndDevice::surviveMessageToLoRaInterference(cMessage* msg) {
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
        LoRaGateway* gateway = dynamic_cast<LoRaGateway*>(msg->getSenderModule());
        if (!gateway)
            return false;

        int rssi = calculateRSSI(this, gateway->getPosX(), gateway->getPosY(), posX, posY, EV);

        EV << "Message RSSI: " << rssi << "\n";

        // Apply external noise and insert the message in interferences
        tuple = applyExternalNoise(msg, rssi, interferences, EV);
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
        emit(signalLostCount, ++messagesLost);

        return false;
    }

    return true;
}



// ************** PUBLIC METHODS **************
unsigned LoRaEndDevice::getPosX() {
    return posX;
}

unsigned LoRaEndDevice::getPosY() {
    return posY;
}


void LoRaEndDevice::addNeighborDevice(LoRaEndDevice* neighbor) {
    if (!neighbor)
        return;

    EV << "============= addNeighborDevice =============\n";

    EV << "neighbor->getId(): " << neighbor->getId() << "\n";
    neighborDevicesInterferences.insert(neighbor);

    EV << "=====================================\n";
}

void LoRaEndDevice::addNeighborGateway(LoRaGateway* neighbor) {
    if (!neighbor)
        return;

    EV << "============= addNeighborGateway =============\n";

    EV << "neighbor->getId(): " << neighbor->getId() << "\n";
    neighborGatewaysInterferences.insert(neighbor);

    EV << "=====================================\n";
}

void LoRaEndDevice::receiveNotification(cPacket* msg, simtime_t arrivalPreamble, simtime_t arrivalFrame) {
    if (!msg)
        return;

    //EV << "============= receiveNotification =============\n";
    // Append the message to the list of messages sent by neighbors
    neighborMessages.push_back(
            std::tuple<cPacket*, simtime_t, simtime_t> {msg, arrivalPreamble, arrivalFrame});

    // Handle interferences from the transmitter of the new message and non from the interfered
    // as A interfering with B implies B interfered by A, so, it is sufficient to handle it once
    //EV << "=====================================\n";
}


void LoRaEndDevice::handleInterferenceDownlink(
        cPacket* msgInterference, cModule* interferer, std::list<std::tuple<cPacket*, cModule*, bool>>& interferedMessages) {
    if (!msgInterference || !interferer || interferedMessages.size() == 0)
        return;

    // Check if the gateway is in the radio range (should be)
    //if (neighborGatewaysInterferences.find(msgInterference->getSenderModule()) == neighborGatewaysInterferences.end())
    if (neighborGatewaysInterferences.find(interferer) == neighborGatewaysInterferences.end())
        return;

    EV << "============= handleInterference =============\n";
    EV << "Device that manages the interference: " << this << "\n";

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
    //LoRaGateway* gatewayInterference = dynamic_cast<LoRaGateway*>(msgInterference_->getSenderModule());
    LoRaGateway* gatewayInterference = dynamic_cast<LoRaGateway*>(interferer);
    if (!gatewayInterference)
        return;

    int rssiInterference = calculateRSSI(this, gatewayInterference->getPosX(), gatewayInterference->getPosY(), posX, posY, EV);
    EV << "Possible interference RSSI: " << rssiInterference << "\n";

    auto tupleInterference = applyExternalNoise(msgInterference_, rssiInterference, sfInterference, bwInterference, interferences, EV);
    if (std::get<1>(tupleInterference) < 0)
        return;

    // Get possible interference shared ID between original message and duplicates
    long interferenceId = msgInterference->getTreeId();

    // Compare possible interference message with possible interfered messages
    // (collected via notifications by the end device source before it sent its message and
    // other time on airs are not ended)
    for (auto tuple : interferedMessages) {
        cModule* interfered = std::get<1>(tuple);

        // Check if the gateway is in the radio range
        if (neighborGatewaysInterferences.find(interfered) == neighborGatewaysInterferences.end())
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


        // Get possible interfered message RSSI
        //LoRaGateway* gatewaySignal = dynamic_cast<LoRaGateway*>(msgSignal->getSenderModule());
        LoRaGateway* gatewaySignal = dynamic_cast<LoRaGateway*>(interfered);
        if (!gatewaySignal)
            return;

        int rssiSignal = calculateRSSI(this, gatewaySignal->getPosX(), gatewaySignal->getPosY(), posX, posY, EV);
        EV << "Signal RSSI: " << rssiSignal << "\n";

        std::tuple<double, float> tupleSignal;

        // Check if noise has already been applied to this signal
        //auto it = interferences.find(msgSignal);
        long signalId = msgSignal->getTreeId();
        auto it = interferences.find(signalId);

        if (it == interferences.end()) {
            tupleSignal = applyExternalNoise(msgSignal, rssiSignal, sfSignal, bwSignal, interferences, EV);
            if (std::get<1>(tupleSignal) < 0)
                continue;
        }
        else {
            tupleSignal = it->second;
            EV << "NoiseAndInterference power: " << std::get<0>(tupleSignal) << " mW\n";
        }


        // Check if the two messages are transmitted using the same bandwidth
        if (bwInterference == bwSignal) {
            EV << "The signal and the possible interference are transmitted in the same bandwidth!\n";

            // Check if the two messages are transmitted using the same channel
            if (cfInterference == cfSignal) {
                EV << "The signal and the possible interference are transmitted in the same ch frequency!\n";

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
                            std::get<1>(tupleSignal)       += 0.1;
                            std::get<1>(tupleInterference) = 1;
                        }
                        else {
                            // Signal and Interference are dropped
                            std::get<1>(tupleSignal)       = 1;
                            std::get<1>(tupleInterference) = 1;
                        }
                    }
                    else {
                        if (sir >= sirThreshold) {
                            // Signal is dropped with a probability of 0-5%,
                            // Interference is dropped
                            std::get<1>(tupleSignal)      += 0.03;
                            std::get<1>(tupleInterference) = 1;
                        }
                        else {
                            // Signal and Interference are dropped
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
                        //std::get<1>(tupleSignal)       += 0;
                        std::get<1>(tupleInterference) = 1;
                    }
                    else {
                        // Signal is dropped, Interference?
                        // Based on same SF study, interferer is always dropped
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
                                std::get<1>(tupleSignal)       += 0.85;
                                std::get<1>(tupleInterference) += 0.85;
                            }
                            else {
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
                                        std::get<1>(tupleSignal)       += 0.3;
                                        std::get<1>(tupleInterference) += 0.3;
                                        break;
                                    case 11:
                                        std::get<1>(tupleSignal)       += 0.2;
                                        std::get<1>(tupleInterference) += 0.2;
                                        break;
                                    case 10:
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


//}
