#!/bin/bash

# Exit on error
#set -e


if [ $# -eq 1 ]
then
    data_path=$1

    if [ ! ${data_path: -1} = "/" ]
    then
        data_path+="/"
    fi

    #echo $data_path
    #echo "**************"
fi

#==== STATS ABOUT A SINGLE DEVICE ====
out_directory="${data_path}single_device/"
echo "Plotting data into $out_directory"

python3 plotData.py Sent\ messages\ \(frequency\) s -o $out_directory/messageSent.pdf -i ${data_path}endDevices[0]_messageSent.csv &
python3 plotData.py Sent\ messages\ \(sum\) s -o $out_directory/messageSentCountScattered.pdf -i ${data_path}endDevices[0]_messageSentCount.csv &
python3 plotData.py Sent\ messages\ \(sum\) l -o $out_directory/messageSentCountLine.pdf -i ${data_path}endDevices[0]_messageSentCount.csv &

python3 plotData.py Received\ messages\ \(frequency\) s -o $out_directory/messageReceived.pdf -i ${data_path}endDevices[0]_messageReceived.csv &
python3 plotData.py Received\ messages\ \(sum\) s -o $out_directory/messageReceivedCountScattered.pdf -i ${data_path}endDevices[0]_messageReceivedCount.csv &
python3 plotData.py Received\ messages\ \(sum\) l -o $out_directory/messageReceivedCountLine.pdf -i ${data_path}endDevices[0]_messageReceivedCount.csv &

# Do not enable ignore option when a single input file does not exist (retransmissions may not exist) because is useless to generate an empty graph
python3 plotData.py Retransmitted\ messages\ \(frequency\) s -o $out_directory/messageRetransmitted.pdf -i ${data_path}endDevices[0]_messageRetransmitted.csv &
python3 plotData.py Retransmitted\ messages\ \(sum\) s -o $out_directory/messageRetransmittedCountScattered.pdf -i ${data_path}endDevices[0]_messageRetransmittedCount.csv &
python3 plotData.py Retransmitted\ messages\ \(sum\) l -o $out_directory/messageRetransmittedCountLine.pdf -i ${data_path}endDevices[0]_messageRetransmittedCount.csv &

# Do not enable ignore option when a single input file does not exist (interferences may not exist) because is useless to generate an empty graph
python3 plotData.py Lost\ messages\ \(frequency\) s -o $out_directory/messageLost.pdf -i ${data_path}endDevices[0]_messageLost.csv &
python3 plotData.py Lost\ messages\ \(sum\) s -o $out_directory/messageLostCountScattered.pdf -i ${data_path}endDevices[0]_messageLostCount.csv &
python3 plotData.py Lost\ messages\ \(sum\) l -o $out_directory/messageLostCountLine.pdf -i ${data_path}endDevices[0]_messageLostCount.csv &

# Do not enable ignore option when a single input file does not exist (interferences may not exist) because is useless to generate an empty graph
python3 plotData.py Interferences\ \(frequency\) s -o $out_directory/interferences.pdf -i ${data_path}endDevices[0]_interference.csv &
python3 plotData.py Interferences\ \(sum\) s -o $out_directory/interferencesCountScattered.pdf -i ${data_path}endDevices[0]_interferenceCount.csv &
python3 plotData.py Interferences\ \(sum\) l -o $out_directory/interferencesCountLine.pdf -i ${data_path}endDevices[0]_interferenceCount.csv &


python3 plotData.py Sent\ and\ Received\ messages\ \(frequency\) s -I -o $out_directory/messagesSentAndReceived.pdf -l sent received -i ${data_path}endDevices[0]_messageSent.csv ${data_path}endDevices[0]_messageReceived.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) s -I -o $out_directory/messagesSentAndReceivedCountScattered.pdf -l sent received -i ${data_path}endDevices[0]_messageSentCount.csv ${data_path}endDevices[0]_messageReceivedCount.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) l -I -o $out_directory/messagesSentAndReceivedCountLine.pdf -l sent received -i ${data_path}endDevices[0]_messageSentCount.csv ${data_path}endDevices[0]_messageReceivedCount.csv &

#python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedBar.pdf -l sent received -i ${data_path}endDevices[0]_messageSentCount.csv ${data_path}endDevices[0]_messageReceivedCount.csv &
python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedBar.pdf -l sent received -g device0 -i ${data_path}endDevices[0]_messageSentCount.csv ${data_path}endDevices[0]_messageReceivedCount.csv &

# Enable ignore option when multiple input files are used and an input can miss (here retransmissions)
python3 plotData.py Sent\ and\ Retransmitted\ messages\ \(frequency\) s -I -o $out_directory/messagesSentAndRetransmitted.pdf -l sent retransmitted -i ${data_path}endDevices[0]_messageSent.csv ${data_path}endDevices[0]_messageRetransmitted.csv &
python3 plotData.py Sent\ and\ Retransmitted\ messages\ \(sum\) s -I -o $out_directory/messagesSentAndRetransmittedCountScattered.pdf -l sent retransmitted -i ${data_path}endDevices[0]_messageSentCount.csv ${data_path}endDevices[0]_messageRetransmittedCount.csv &
python3 plotData.py Sent\ and\ Retransmitted\ messages\ \(sum\) l -I -o $out_directory/messagesSentAndRetransmittedCountLine.pdf -l sent retransmitted -i ${data_path}endDevices[0]_messageSentCount.csv ${data_path}endDevices[0]_messageRetransmittedCount.csv &

python3 plotData.py Sent\ and\ Retransmitted\ messages b -I -o $out_directory/messagesSentAndRetransmittedBar.pdf -l sent retransmitted -g device0 -i ${data_path}endDevices[0]_messageSentCount.csv ${data_path}endDevices[0]_messageRetransmittedCount.csv &


python3 plotData.py Received\ and\ Lost\ messages\ \(frequency\) s -I -o $out_directory/messagesReceivedAndLost.pdf -l received lost -i ${data_path}endDevices[0]_messageReceived.csv ${data_path}endDevices[0]_messageLost.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedAndLostCountScattered.pdf -l received lost -i ${data_path}endDevices[0]_messageReceivedCount.csv ${data_path}endDevices[0]_messageLostCount.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedAndLostCountLine.pdf -l received lost -i ${data_path}endDevices[0]_messageReceivedCount.csv ${data_path}endDevices[0]_messageLostCount.csv &


# Enable ignore option when multiple input files are used and an input can miss.
# Downlink interferences and possible interferences are counted for all covered end devices so they are the same for all
python3 plotData.py Possible\ interferences\ and\ interferences\ \(frequency\) s -I -o $out_directory/possibleInterferencesAndInterferences.pdf -l possible\ interferences interferences -i ${data_path}endDevices[0]_interferencePossible.csv ${data_path}endDevices[0]_interference.csv &
python3 plotData.py Possible\ interferences\ and\ interferences\ \(sum\) s -I -o $out_directory/possibleInterferencesAndInterferencesCountScattered.pdf -l possible\ interferences interferences -i ${data_path}endDevices[0]_interferencePossibleCount.csv ${data_path}endDevices[0]_interferenceCount.csv &
python3 plotData.py Possible\ interferences\ and\ interferences\ \(sum\) l -I -o $out_directory/possibleInterferencesAndInterferencesCountLine.pdf -l possible\ interferences interferences -i ${data_path}endDevices[0]_interferencePossibleCount.csv ${data_path}endDevices[0]_interferenceCount.csv &

python3 plotData.py In\ messages\ and\ interferences b -I -o $out_directory/messagesInAndInterferencesBar.pdf -l \in interferences -g endDevice0 -i ${data_path}endDevices[0]_messageInCount.csv ${data_path}endDevices[0]_interferenceCount.csv &
python3 plotData.py Possible\ interferences\ and\ interferences b -I -o $out_directory/possibleInterferencesAndInterferencesBar.pdf -l possible\ interferences interferences -g endDevice0 -i ${data_path}endDevices[0]_interferencePossibleCount.csv ${data_path}endDevices[0]_interferenceCount.csv &
#======================================



#prova
#python3 plotData.py Sent\ and\ Retransmitted\ messages b -l sent retransmitted -i endDevices[0]_messageSentCount.csv endDevices[0]_messageRetransmittedCount.csv
#python3 plotData.py Sent\ and\ Retransmitted\ messages\ \(sum\) s -l sent retransmitted -i endDevices[0]_messageSentCount.csv endDevices[0]_messageRetransmittedCount.csv -I
#python3 plotData.py Retransmitted\ messages\ \(frequency\) s -i endDevices[0]_messageRetransmitted.csv

#python3 plotData.py Sent\ and\ Received\ messages b -I -l sent received -g dev0 -i endDevices[0]_messageSentCount.csv endDevices[0]_messageReceivedCount.csv
#python3 plotData.py Sent\ and\ Received\ messages b -I -l sent received -g dev0 dev1 -i endDevices[0]_messageSentCount.csv endDevices[0]_messageReceivedCount.csv endDevices[1]_messageSentCount.csv endDevices[1]_messageReceivedCount.csv
#python3 plotData.py Sent\ and\ Received\ messages b -I -l sent received -g dev0 dev1 dev2 -i endDevices[0]_messageSentCount.csv endDevices[0]_messageReceivedCount.csv endDevices[1]_messageSentCount.csv endDevices[1]_messageReceivedCount.csv endDevices[2]_messageSentCount.csv endDevices[2]_messageReceivedCount.csv
#python3 plotData.py Sent\ and\ Received\ messages b -I -l sent received -g dev0 dev1 dev2 dev3 -i endDevices[0]_messageSentCount.csv endDevices[0]_messageReceivedCount.csv endDevices[1]_messageSentCount.csv endDevices[1]_messageReceivedCount.csv endDevices[2]_messageSentCount.csv endDevices[2]_messageReceivedCount.csv endDevices[3]_messageSentCount.csv endDevices[3]_messageReceivedCount.csv

#python3 plotData.py Sent\ messages b -I -g dev0 -i endDevices[0]_messageSentCount.csv
#python3 plotData.py Sent\ messages b -I -g dev0 dev1 dev2 -i endDevices[0]_messageSentCount.csv endDevices[1]_messageSentCount.csv endDevices[2]_messageSentCount.csv
#python3 plotData.py Sent\ messages b -I -g dev0 dev1 dev2 dev3 -i endDevices[0]_messageSentCount.csv endDevices[1]_messageSentCount.csv endDevices[2]_messageSentCount.csv endDevices[3]_messageSentCount.csv





#==== STATS ABOUT MULTIPLE DEVICES ====
out_directory="${data_path}multiple_devices/"
echo "Plotting data into $out_directory"

python3 plotData.py Sent\ messages\ \(frequency\) s -I -o $out_directory/messagesSent.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageSent.csv ${data_path}endDevices[1]_messageSent.csv ${data_path}endDevices[2]_messageSent.csv ${data_path}endDevices[3]_messageSent.csv ${data_path}endDevices[4]_messageSent.csv &
python3 plotData.py Sent\ messages\ \(sum\) s -I -o $out_directory/messagesSentCountScattered.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageSentCount.csv ${data_path}endDevices[1]_messageSentCount.csv ${data_path}endDevices[2]_messageSentCount.csv ${data_path}endDevices[3]_messageSentCount.csv ${data_path}endDevices[4]_messageSentCount.csv &
python3 plotData.py Sent\ messages\ \(sum\) l -I -o $out_directory/messagesSentCountLine.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageSentCount.csv ${data_path}endDevices[1]_messageSentCount.csv ${data_path}endDevices[2]_messageSentCount.csv ${data_path}endDevices[3]_messageSentCount.csv ${data_path}endDevices[4]_messageSentCount.csv &

python3 plotData.py Received\ messages\ \(frequency\) s -I -o $out_directory/messageReceived.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageReceived.csv ${data_path}endDevices[1]_messageReceived.csv ${data_path}endDevices[2]_messageReceived.csv ${data_path}endDevices[3]_messageReceived.csv ${data_path}endDevices[4]_messageReceived.csv &
python3 plotData.py Received\ messages\ \(sum\) s -I -o $out_directory/messageReceivedCountScattered.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageReceivedCount.csv ${data_path}endDevices[1]_messageReceivedCount.csv ${data_path}endDevices[2]_messageReceivedCount.csv ${data_path}endDevices[3]_messageReceivedCount.csv ${data_path}endDevices[4]_messageReceivedCount.csv &
python3 plotData.py Received\ messages\ \(sum\) l -I -o $out_directory/messageReceivedCountLine.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageReceivedCount.csv ${data_path}endDevices[1]_messageReceivedCount.csv ${data_path}endDevices[2]_messageReceivedCount.csv ${data_path}endDevices[3]_messageReceivedCount.csv ${data_path}endDevices[4]_messageReceivedCount.csv &

python3 plotData.py Retransmitted\ messages\ \(frequency\) s -I -o $out_directory/messageRetransmitted.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageRetransmitted.csv ${data_path}endDevices[1]_messageRetransmitted.csv ${data_path}endDevices[2]_messageRetransmitted.csv ${data_path}endDevices[3]_messageRetransmitted.csv ${data_path}endDevices[4]_messageRetransmitted.csv &
python3 plotData.py Retransmitted\ messages\ \(sum\) s -I -o $out_directory/messageRetransmittedCountScattered.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageRetransmittedCount.csv ${data_path}endDevices[1]_messageRetransmittedCount.csv ${data_path}endDevices[2]_messageRetransmittedCount.csv ${data_path}endDevices[3]_messageRetransmittedCount.csv ${data_path}endDevices[4]_messageRetransmittedCount.csv &
python3 plotData.py Retransmitted\ messages\ \(sum\) l -I -o $out_directory/messageRetransmittedCountLine.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageRetransmittedCount.csv ${data_path}endDevices[1]_messageRetransmittedCount.csv ${data_path}endDevices[2]_messageRetransmittedCount.csv ${data_path}endDevices[3]_messageRetransmittedCount.csv ${data_path}endDevices[4]_messageRetransmittedCount.csv &

python3 plotData.py Lost\ messages\ \(frequency\) s -I -o $out_directory/messageLost.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageLost.csv ${data_path}endDevices[1]_messageLost.csv ${data_path}endDevices[2]_messageLost.csv ${data_path}endDevices[3]_messageLost.csv ${data_path}endDevices[4]_messageLost.csv &
python3 plotData.py Lost\ messages\ \(sum\) s -I -o $out_directory/messageLostCountScattered.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageLostCount.csv ${data_path}endDevices[1]_messageLostCount.csv ${data_path}endDevices[2]_messageLostCount.csv ${data_path}endDevices[3]_messageLostCount.csv ${data_path}endDevices[4]_messageLostCount.csv &
python3 plotData.py Lost\ messages\ \(sum\) l -I -o $out_directory/messageLostCountLine.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageLostCount.csv ${data_path}endDevices[1]_messageLostCount.csv ${data_path}endDevices[2]_messageLostCount.csv ${data_path}endDevices[3]_messageLostCount.csv ${data_path}endDevices[4]_messageLostCount.csv &

python3 plotData.py Interferences\ messages\ \(frequency\) s -I -o $out_directory/interferences.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_interference.csv ${data_path}endDevices[1]_interference.csv ${data_path}endDevices[2]_interference.csv ${data_path}endDevices[3]_interference.csv ${data_path}endDevices[4]_interference.csv &
python3 plotData.py Interferences\ messages\ \(sum\) s -I -o $out_directory/interferencesCountScattered.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_interferenceCount.csv ${data_path}endDevices[1]_interferenceCount.csv ${data_path}endDevices[2]_interferenceCount.csv ${data_path}endDevices[3]_interferenceCount.csv ${data_path}endDevices[4]_interferenceCount.csv &
python3 plotData.py Interferences\ messages\ \(sum\) l -I -o $out_directory/interferencesCountLine.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_interferenceCount.csv ${data_path}endDevices[1]_interferenceCount.csv ${data_path}endDevices[2]_interferenceCount.csv ${data_path}endDevices[3]_interferenceCount.csv ${data_path}endDevices[4]_interferenceCount.csv &

# These automatically plot charts with min, max and mean
python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedBar.pdf -l sent received -g device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageSentCount.csv ${data_path}endDevices[0]_messageReceivedCount.csv ${data_path}endDevices[1]_messageSentCount.csv ${data_path}endDevices[1]_messageReceivedCount.csv ${data_path}endDevices[2]_messageSentCount.csv ${data_path}endDevices[2]_messageReceivedCount.csv ${data_path}endDevices[3]_messageSentCount.csv ${data_path}endDevices[3]_messageReceivedCount.csv ${data_path}endDevices[4]_messageSentCount.csv ${data_path}endDevices[4]_messageReceivedCount.csv &
python3 plotData.py Sent\ and\ Retransmitted\ messages b -I -o $out_directory/messagesSentAndRetransmittedBar.pdf -l sent retransmitted -g device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageSentCount.csv ${data_path}endDevices[0]_messageRetransmittedCount.csv ${data_path}endDevices[1]_messageSentCount.csv ${data_path}endDevices[1]_messageRetransmittedCount.csv ${data_path}endDevices[2]_messageSentCount.csv ${data_path}endDevices[2]_messageRetransmittedCount.csv ${data_path}endDevices[3]_messageSentCount.csv ${data_path}endDevices[3]_messageRetransmittedCount.csv ${data_path}endDevices[4]_messageSentCount.csv ${data_path}endDevices[4]_messageRetransmittedCount.csv &
python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostBar.pdf -l received lost -g device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageReceivedCount.csv ${data_path}endDevices[0]_messageLostCount.csv ${data_path}endDevices[1]_messageReceivedCount.csv ${data_path}endDevices[1]_messageLostCount.csv ${data_path}endDevices[2]_messageReceivedCount.csv ${data_path}endDevices[2]_messageLostCount.csv ${data_path}endDevices[3]_messageReceivedCount.csv ${data_path}endDevices[3]_messageLostCount.csv ${data_path}endDevices[4]_messageReceivedCount.csv ${data_path}endDevices[4]_messageLostCount.csv &
python3 plotData.py In\ messages\ and\ interferences b -I -o $out_directory/messagesInAndInterferencesBar.pdf -l \in interferences -g device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageInCount.csv ${data_path}endDevices[0]_interferenceCount.csv ${data_path}endDevices[1]_messageInCount.csv ${data_path}endDevices[1]_interferenceCount.csv ${data_path}endDevices[2]_messageInCount.csv ${data_path}endDevices[2]_interferenceCount.csv ${data_path}endDevices[3]_messageInCount.csv ${data_path}endDevices[3]_interferenceCount.csv ${data_path}endDevices[4]_messageInCount.csv ${data_path}endDevices[4]_interferenceCount.csv &
python3 plotData.py Possible\ interferences\ and\ interferences b -I -o $out_directory/possibleInterferencesAndInterferencesBar.pdf -l possible\ interferences interferences -g device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_interferencePossibleCount.csv ${data_path}endDevices[0]_interferenceCount.csv ${data_path}endDevices[1]_interferencePossibleCount.csv ${data_path}endDevices[1]_interferenceCount.csv ${data_path}endDevices[2]_interferencePossibleCount.csv ${data_path}endDevices[2]_interferenceCount.csv ${data_path}endDevices[3]_interferencePossibleCount.csv ${data_path}endDevices[3]_interferenceCount.csv ${data_path}endDevices[4]_interferencePossibleCount.csv ${data_path}endDevices[4]_interferenceCount.csv &
#======================================


#==== STATS ABOUT A SINGLE GATEWAY ====
out_directory="${data_path}single_gateway/"
echo "Plotting data into $out_directory"

python3 plotData.py Connected\ devices\ \(sum\) s -o $out_directory/connectedDevicesScattered.pdf -i ${data_path}gateways[0]_connected.csv &
python3 plotData.py Connected\ devices\ \(sum\) l -o $out_directory/connectedDevicesLine.pdf -i ${data_path}gateways[0]_connected.csv &

python3 plotData.py Resources l -o $out_directory/resources.pdf -l CPU GPU RAM -i ${data_path}gateways[0]_cpu.csv ${data_path}gateways[0]_gpu.csv ${data_path}gateways[0]_ram.csv &
python3 plotData.py Storage l -o $out_directory/storage.pdf -i ${data_path}gateways[0]_storage.csv &
python3 plotData.py NetworkIO l -o $out_directory/networkIO.pdf -l In\(byte/s\) Out\(byte/s\) -i ${data_path}gateways[0]_networkIn.csv ${data_path}gateways[0]_networkOut.csv &


python3 plotData.py Sent\ messages\ \(frequency\) s -o $out_directory/messagesSent.pdf -i ${data_path}gateways[0]_messageSent.csv &
python3 plotData.py Sent\ messages\ \(sum\) s -o $out_directory/messagesSentCountScattered.pdf -i ${data_path}gateways[0]_messageSentCount.csv &
python3 plotData.py Sent\ messages\ \(sum\) l -o $out_directory/messagesSentCountLine.pdf -i ${data_path}gateways[0]_messageSentCount.csv &

python3 plotData.py Sent\ messages\ \(frequency\) s -o $out_directory/messagesSentLoRaIp.pdf -l LoRa IP -i ${data_path}gateways[0]_messageSentLoRa.csv ${data_path}gateways[0]_messageSentIp.csv &
python3 plotData.py Sent\ messages\ \(sum\) s -o $out_directory/messagesSentLoRaIpCountScattered.pdf -l LoRa IP -i ${data_path}gateways[0]_messageSentLoRaCount.csv ${data_path}gateways[0]_messageSentIpCount.csv &
python3 plotData.py Sent\ messages\ \(sum\) l -o $out_directory/messagesSentLoRaIpCountLine.pdf -l LoRa IP -i ${data_path}gateways[0]_messageSentLoRaCount.csv ${data_path}gateways[0]_messageSentIpCount.csv &

python3 plotData.py Received\ messages\ \(frequency\) s -o $out_directory/messagesReceived.pdf -i ${data_path}gateways[0]_messageReceived.csv &
python3 plotData.py Received\ messages\ \(sum\) s -o $out_directory/messagesReceivedCountScattered.pdf -i ${data_path}gateways[0]_messageReceivedCount.csv &
python3 plotData.py Received\ messages\ \(sum\) l -o $out_directory/messagesReceivedCountLine.pdf -i ${data_path}gateways[0]_messageReceivedCount.csv &

python3 plotData.py Received\ messages\ \(frequency\) s -o $out_directory/messagesReceivedLoRaIp.pdf -l LoRa IP -i ${data_path}gateways[0]_messageReceivedLoRa.csv ${data_path}gateways[0]_messageReceivedIp.csv &
python3 plotData.py Received\ messages\ \(sum\) s -o $out_directory/messagesReceivedLoRaIpCountScattered.pdf -l LoRa IP -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv &
python3 plotData.py Received\ messages\ \(sum\) l -o $out_directory/messagesReceivedLoRaIpCountLine.pdf -l LoRa IP -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv &

python3 plotData.py Sent\ and\ Received\ messages\ \(frequency\) s -I -o $out_directory/messagesSentAndReceived.pdf -l sent received -i ${data_path}gateways[0]_messageSent.csv ${data_path}gateways[0]_messageReceived.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) s -I -o $out_directory/messagesSentAndReceivedCountScattered.pdf -l sent received -i ${data_path}gateways[0]_messageSentCount.csv ${data_path}gateways[0]_messageReceivedCount.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) l -I -o $out_directory/messagesSentAndReceivedCountLine.pdf -l sent received -i ${data_path}gateways[0]_messageSentCount.csv ${data_path}gateways[0]_messageReceivedCount.csv &

python3 plotData.py Sent\ and\ Received\ messages\ \(frequency\) s -I -o $out_directory/messagesSentAndReceivedLoRa.pdf -l sent\ LoRa received\ LoRa -i ${data_path}gateways[0]_messageSentLoRa.csv ${data_path}gateways[0]_messageReceivedLoRa.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) s -I -o $out_directory/messagesSentAndReceivedLoRaCountScattered.pdf -l sent\ LoRa received\ LoRa -i ${data_path}gateways[0]_messageSentLoRaCount.csv ${data_path}gateways[0]_messageReceivedLoRaCount.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) l -I -o $out_directory/messagesSentAndReceivedLoRaCountLine.pdf -l sent\ LoRa received\ LoRa -i ${data_path}gateways[0]_messageSentLoRaCount.csv ${data_path}gateways[0]_messageReceivedLoRaCount.csv &

python3 plotData.py Sent\ and\ Received\ messages\ \(frequency\) s -I -o $out_directory/messagesSentAndReceivedIp.pdf -l sent\ IP received\ IP -i ${data_path}gateways[0]_messageSentIp.csv ${data_path}gateways[0]_messageReceivedIp.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) s -I -o $out_directory/messagesSentAndReceivedIpCountScattered.pdf -l sent\ IP received\ IP -i ${data_path}gateways[0]_messageSentIpCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) l -I -o $out_directory/messagesSentAndReceivedIpCountLine.pdf -l sent\ IP received\ IP -i ${data_path}gateways[0]_messageSentIpCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv &

python3 plotData.py Sent\ and\ Received\ messages\ \(frequency\) s -I -o $out_directory/messagesSentAndReceivedLoRaIp.pdf -l sent\ LoRa received\ LoRa sent\ IP received\ IP -i ${data_path}gateways[0]_messageSentLoRa.csv ${data_path}gateways[0]_messageReceivedLoRa.csv ${data_path}gateways[0]_messageSentIp.csv ${data_path}gateways[0]_messageReceivedIp.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) s -I -o $out_directory/messagesSentAndReceivedLoRaIpCountScattered.pdf -l sent\ LoRa received\ LoRa sent\ IP received\ IP -i ${data_path}gateways[0]_messageSentLoRaCount.csv ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageSentIpCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) l -I -o $out_directory/messagesSentAndReceivedLoRaIpCountLine.pdf -l sent\ LoRa received\ LoRa sent\ IP received\ IP -i ${data_path}gateways[0]_messageSentLoRaCount.csv ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageSentIpCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv &


# Do not enable ignore option when a single input file does not exist (lost messages may not exist) because is useless to generate an empty graph
python3 plotData.py Lost\ messages\ \(frequency\) s -o $out_directory/messagesLost.pdf -i ${data_path}gateways[0]_messageLost.csv &
python3 plotData.py Lost\ messages\ \(sum\) s -o $out_directory/messagesLostCountScattered.pdf -i ${data_path}gateways[0]_messageLostCount.csv &
python3 plotData.py Lost\ messages\ \(sum\) l -o $out_directory/messagesLostCountLine.pdf -i ${data_path}gateways[0]_messageLostCount.csv &

# Enable ignore option when multiple input files are used and an input can miss (here lost messages)
python3 plotData.py Lost\ messages\ \(frequency\) s -I -o $out_directory/messagesLostLoRaIp.pdf -l LoRa IP -i ${data_path}gateways[0]_messageLostLoRa.csv ${data_path}gateways[0]_messageLostIp.csv &
python3 plotData.py Lost\ messages\ \(sum\) s -I -o $out_directory/messagesLostLoRaIpCountScattered.pdf -l LoRa IP -i ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[0]_messageLostIpCount.csv &
python3 plotData.py Lost\ messages\ \(sum\) l -I -o $out_directory/messagesLostLoRaIpCountLine.pdf -l LoRa IP -i ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[0]_messageLostIpCount.csv &

python3 plotData.py Received\ and\ Lost\ messages\ \(frequency\) s -I -o $out_directory/messagesReceivedAndLost.pdf -l received lost -i ${data_path}gateways[0]_messageReceived.csv ${data_path}gateways[0]_messageLost.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedAndLostCountScattered.pdf -l received lost -i ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[0]_messageLostCount.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedAndLostCountLine.pdf -l received lost -i ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[0]_messageLostCount.csv &

python3 plotData.py Received\ and\ Lost\ messages\ \(frequency\) s -I -o $out_directory/messagesReceivedAndLostLoRa.pdf -l received lost -i ${data_path}gateways[0]_messageReceivedLoRa.csv ${data_path}gateways[0]_messageLostLoRa.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedAndLostLoRaCountScattered.pdf -l received lost -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageLostLoRaCount.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedAndLostLoRaCountLine.pdf -l received lost -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageLostLoRaCount.csv &

python3 plotData.py Received\ and\ Lost\ messages\ \(frequency\) s -I -o $out_directory/messagesReceivedAndLostIp.pdf -l received lost -i ${data_path}gateways[0]_messageReceivedIp.csv ${data_path}gateways[0]_messageLostIp.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedAndLostIpCountScattered.pdf -l received lost -i ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[0]_messageLostIpCount.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedAndLostIpCountLine.pdf -l received lost -i ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[0]_messageLostIpCount.csv &

python3 plotData.py Received\ and\ Lost\ messages\ \(frequency\) s -I -o $out_directory/messagesReceivedAndLostLoRaIp.pdf -l received\ LoRa lost\ LoRa received\ IP lost\ IP -i ${data_path}gateways[0]_messageReceivedLoRa.csv ${data_path}gateways[0]_messageLostLoRa.csv ${data_path}gateways[0]_messageReceivedIp.csv ${data_path}gateways[0]_messageLostIp.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedAndLostLoRaIpCountScattered.pdf -l received\ LoRa lost\ LoRa received\ IP lost\ IP -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[0]_messageLostIpCount.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedAndLostLoRaIpCountLine.pdf -l received\ LoRa lost\ LoRa received\ IP lost\ IP -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[0]_messageLostIpCount.csv &

#python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedBar.pdf -l sent received -i ${data_path}gateways[0]_messageSentCount.csv ${data_path}gateways[0]_messageReceivedCount.csv &
#python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedLoRaIpBar.pdf -l sent\ LoRa received\ LoRa sent\ IP received\ IP -i ${data_path}gateways[0]_messageSentLoRaCount.csv ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageSentIpCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv &
python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedBar.pdf -l sent received -g gateway0 -i ${data_path}gateways[0]_messageSentCount.csv ${data_path}gateways[0]_messageReceivedCount.csv &
python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedLoRaIpBar.pdf -l sent\ LoRa received\ LoRa sent\ IP received\ IP -g gateway0 -i ${data_path}gateways[0]_messageSentLoRaCount.csv ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageSentIpCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv &

#python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostBar.pdf -l received lost -i ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[0]_messageLostCount.csv &
#python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostLoRaIpBar.pdf -l received\ LoRa lost\ LoRa received\ IP lost\ IP -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[0]_messageLostIpCount.csv &
python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostBar.pdf -l received lost -g gateway0 -i ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[0]_messageLostCount.csv &
python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostLoRaIpBar.pdf -l received\ LoRa lost\ LoRa received\ IP lost\ IP -g gateway0 -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[0]_messageLostIpCount.csv &


python3 plotData.py In\ messages\ \(sum\) s -o $out_directory/messagesInCountScattered.pdf -i ${data_path}gateways[0]_messageInCount.csv &
python3 plotData.py In\ messages\ \(sum\) l -o $out_directory/messagesInCountLine.pdf -i ${data_path}gateways[0]_messageInCount.csv &

python3 plotData.py In\ messages\ \(sum\) s -o $out_directory/messagesInLoRaIpCountScattered.pdf -l LoRa IP -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageInIpCount.csv &
python3 plotData.py In\ messages\ \(sum\) l -o $out_directory/messagesInLoRaIpCountLine.pdf -l LoRa IP -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageInIpCount.csv &

#python3 plotData.py In\ and\ Received\ messages b -I -o $out_directory/messagesInAndReceivedBar.pdf -l \in received -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[0]_messageReceivedCount.csv &
#python3 plotData.py In\ and\ Received\ messages b -I -o $out_directory/messagesInAndReceivedLoRaIpBar.pdf -l \in\ LoRa received\ LoRa \in\ IP received\ IP -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageInIpCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv &
python3 plotData.py In\ and\ Received\ messages b -I -o $out_directory/messagesInAndReceivedBar.pdf -l \in received -g gateway0 -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[0]_messageReceivedCount.csv &
python3 plotData.py In\ and\ Received\ messages b -I -o $out_directory/messagesInAndReceivedLoRaIpBar.pdf -l \in\ LoRa received\ LoRa \in\ IP received\ IP -g gateway0 -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageInIpCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv &

#python3 plotData.py In\ and\ Lost\ messages b -I -o $out_directory/messagesInAndLostBar.pdf -l \in lost -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[0]_messageLostCount.csv &
#python3 plotData.py In\ and\ Lost\ messages b -I -o $out_directory/messagesInAndLostLoRaIpBar.pdf -l \in\ LoRa lost\ LoRa \in\ IP lost\ IP -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[0]_messageInIpCount.csv ${data_path}gateways[0]_messageLostIpCount.csv &
python3 plotData.py In\ and\ Lost\ messages b -I -o $out_directory/messagesInAndLostBar.pdf -l \in lost -g gateway0 -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[0]_messageLostCount.csv &
python3 plotData.py In\ and\ Lost\ messages b -I -o $out_directory/messagesInAndLostLoRaIpBar.pdf -l \in\ LoRa lost\ LoRa \in\ IP lost\ IP -g gateway0 -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[0]_messageInIpCount.csv ${data_path}gateways[0]_messageLostIpCount.csv &


# Do not enable ignore option when a single input file does not exist (interferences may not exist) because is useless to generate an empty graph
python3 plotData.py Interferences\ \(frequency\) s -o $out_directory/interferences.pdf -i ${data_path}gateways[0]_interference.csv &
python3 plotData.py Interferences\ \(sum\) s -o $out_directory/interferencesCountScattered.pdf -i ${data_path}gateways[0]_interferenceCount.csv &
python3 plotData.py Interferences\ \(sum\) l -o $out_directory/interferencesCountLine.pdf -i ${data_path}gateways[0]_interferenceCount.csv &

# Enable ignore option when multiple input files are used and an input can miss
python3 plotData.py Possible\ interferences\ and\ interferences\ \(frequency\) s -I -o $out_directory/possibleInterferencesAndInterferences.pdf -l possible\ interferences interferences -i ${data_path}gateways[0]_interferencePossible.csv ${data_path}gateways[0]_interference.csv &
python3 plotData.py Possible\ interferences\ and\ interferences\ \(sum\) s -I -o $out_directory/possibleInterferencesAndInterferencesCountScattered.pdf -l possible\ interferences interferences -i ${data_path}gateways[0]_interferencePossibleCount.csv ${data_path}gateways[0]_interferenceCount.csv &
python3 plotData.py Possible\ interferences\ and\ interferences\ \(sum\) l -I -o $out_directory/possibleInterferencesAndInterferencesCountLine.pdf -l possible\ interferences interferences -i ${data_path}gateways[0]_interferencePossibleCount.csv ${data_path}gateways[0]_interferenceCount.csv &

#python3 plotData.py In\ messages\ and\ interferences b -I -o $out_directory/messagesInAndInterferencesBar.pdf -l \in interferences -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[0]_interferenceCount.csv &
#python3 plotData.py Possible\ interferences\ and\ interferences b -I -o $out_directory/possibleInterferencesAndInterferencesBar.pdf -l possible\ interferences interferences -i ${data_path}gateways[0]_interferencePossibleCount.csv ${data_path}gateways[0]_interferenceCount.csv &
python3 plotData.py In\ messages\ and\ interferences b -I -o $out_directory/messagesInAndInterferencesBar.pdf -l \in interferences -g gateway0 -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[0]_interferenceCount.csv &
python3 plotData.py Possible\ interferences\ and\ interferences b -I -o $out_directory/possibleInterferencesAndInterferencesBar.pdf -l possible\ interferences interferences -g gateway0 -i ${data_path}gateways[0]_interferencePossibleCount.csv ${data_path}gateways[0]_interferenceCount.csv &


python3 plotData.py RSSIs h -o $out_directory/RSSIs.pdf -i ${data_path}gateways[0]_rssi.csv &
#==============================


# prova
#python3 mergeCSV.py -o aaa.csv -i gateways[0]_messageLostCount.csv gateways[0]_messageReceivedCount.csv
#python3 mergeCSV.py -I -o aaa.csv -i gateways[0]_messageLostCount.csv gateways[0]_messageReceivedCount99.csv

#python3 plotData.py Storage l -I -o storage.pdf -l gateway0 gateway1 -i /home/ivan/Scaricati/master_thesis_implementation/Results/simulations/LoED_20190301/mean_exported_data_20220314_040427/mean_output_csv/gateways[0]_storage.csv /home/ivan/Scaricati/master_thesis_implementation/Results/simulations/LoED_20190301/mean_exported_data_20220314_040427/mean_output_csv/gateways[1]_storage.csv



#=== STATS ABOUT MULTIPLE GATEWAYS ===
out_directory="${data_path}multiple_gateways/"
echo "Plotting data into $out_directory"

python3 plotData.py Connected\ devices\ \(sum\) s -I -o $out_directory/connectedDevicesScattered.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_connected.csv ${data_path}gateways[1]_connected.csv &
python3 plotData.py Connected\ devices\ \(sum\) l -I -o $out_directory/connectedDevicesLine.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_connected.csv ${data_path}gateways[1]_connected.csv &
python3 plotData.py Connected\ devices b -I -o $out_directory/connectedDevicesBar.pdf -g gateway0 gateway1 -i ${data_path}gateways[0]_connected.csv ${data_path}gateways[1]_connected.csv 

python3 plotData.py Resources l -I -o $out_directory/resources.pdf -l gateway0\ CPU gateway0\ GPU gateway0\ RAM gateway1\ CPU gateway1\ GPU gateway1\ RAM -i ${data_path}gateways[0]_cpu.csv ${data_path}gateways[0]_gpu.csv ${data_path}gateways[0]_ram.csv ${data_path}gateways[1]_cpu.csv ${data_path}gateways[1]_gpu.csv ${data_path}gateways[1]_ram.csv &
python3 plotData.py Storage l -I -o $out_directory/storage.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_storage.csv ${data_path}gateways[1]_storage.csv &
python3 plotData.py NetworkIO l -I -o $out_directory/networkIO.pdf -l gateway0\ In\(byte/s\) gateway0\ Out\(byte/s\) gateway1\ In\(byte/s\) gateway1\ Out\(byte/s\) -i ${data_path}gateways[0]_networkIn.csv ${data_path}gateways[0]_networkOut.csv ${data_path}gateways[1]_networkIn.csv ${data_path}gateways[1]_networkOut.csv &


python3 plotData.py Sent\ messages\ \(frequency\) s -I -o $out_directory/messagesSent.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageSent.csv ${data_path}gateways[1]_messageSent.csv &
python3 plotData.py Sent\ messages\ \(sum\) s -I -o $out_directory/messagesSentCountScattered.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageSentCount.csv ${data_path}gateways[1]_messageSentCount.csv &
python3 plotData.py Sent\ messages\ \(sum\) l -I -o $out_directory/messagesSentCountLine.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageSentCount.csv ${data_path}gateways[1]_messageSentCount.csv &

python3 plotData.py Sent\ messages\ \(frequency\) s -I -o $out_directory/messagesSentLoRaIp.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageSentLoRa.csv ${data_path}gateways[0]_messageSentIp.csv ${data_path}gateways[1]_messageSentLoRa.csv ${data_path}gateways[1]_messageSentIp.csv &
python3 plotData.py Sent\ messages\ \(sum\) s -I -o $out_directory/messagesSentLoRaIpCountScattered.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageSentLoRaCount.csv ${data_path}gateways[0]_messageSentIpCount.csv ${data_path}gateways[1]_messageSentLoRaCount.csv ${data_path}gateways[1]_messageSentIpCount.csv &
python3 plotData.py Sent\ messages\ \(sum\) l -I -o $out_directory/messagesSentLoRaIpCountLine.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageSentLoRaCount.csv ${data_path}gateways[0]_messageSentIpCount.csv ${data_path}gateways[1]_messageSentLoRaCount.csv ${data_path}gateways[1]_messageSentIpCount.csv &

python3 plotData.py Received\ messages\ \(frequency\) s -I -o $out_directory/messagesReceived.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageReceived.csv ${data_path}gateways[1]_messageReceived.csv &
python3 plotData.py Received\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedCountScattered.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[1]_messageReceivedCount.csv &
python3 plotData.py Received\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedCountLine.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[1]_messageReceivedCount.csv &

python3 plotData.py Received\ messages\ \(frequency\) s -I -o $out_directory/messagesReceivedLoRaIp.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageReceivedLoRa.csv ${data_path}gateways[0]_messageReceivedIp.csv ${data_path}gateways[1]_messageReceivedLoRa.csv ${data_path}gateways[1]_messageReceivedIp.csv &
python3 plotData.py Received\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedLoRaIpCountScattered.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[1]_messageReceivedLoRaCount.csv ${data_path}gateways[1]_messageReceivedIpCount.csv &
python3 plotData.py Received\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedLoRaIpCountLine.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[1]_messageReceivedLoRaCount.csv ${data_path}gateways[1]_messageReceivedIpCount.csv &

python3 plotData.py Sent\ and\ Received\ messages\ \(frequency\) s -I -o $out_directory/messagesSentAndReceived.pdf -l gateway0\ sent gateway0\ received gateway1\ sent gateway1\ received -i ${data_path}gateways[0]_messageSent.csv ${data_path}gateways[0]_messageReceived.csv ${data_path}gateways[1]_messageSent.csv ${data_path}gateways[1]_messageReceived.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) s -I -o $out_directory/messagesSentAndReceivedCountScattered.pdf -l gateway0\ sent gateway0\ received gateway1\ sent gateway1\ received -i ${data_path}gateways[0]_messageSentCount.csv ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[1]_messageSentCount.csv ${data_path}gateways[1]_messageReceivedCount.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) l -I -o $out_directory/messagesSentAndReceivedCountLine.pdf -l gateway0\ sent gateway0\ received gateway1\ sent gateway1\ received -i ${data_path}gateways[0]_messageSentCount.csv ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[1]_messageSentCount.csv ${data_path}gateways[1]_messageReceivedCount.csv &

python3 plotData.py Sent\ and\ Received\ messages\ \(frequency\) s -I -o $out_directory/messagesSentAndReceivedLoRa.pdf -l gateway0\ sent\ LoRa gateway0\ received\ LoRa gateway1\ sent\ LoRa gateway1\ received\ LoRa -i ${data_path}gateways[0]_messageSentLoRa.csv ${data_path}gateways[0]_messageReceivedLoRa.csv ${data_path}gateways[1]_messageSentLoRa.csv ${data_path}gateways[1]_messageReceivedLoRa.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) s -I -o $out_directory/messagesSentAndReceivedLoRaCountScattered.pdf -l gateway0\ sent\ LoRa gateway0\ received\ LoRa gateway1\ sent\ LoRa gateway1\ received\ LoRa -i ${data_path}gateways[0]_messageSentLoRaCount.csv ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[1]_messageSentLoRaCount.csv ${data_path}gateways[1]_messageReceivedLoRaCount.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) l -I -o $out_directory/messagesSentAndReceivedLoRaCountLine.pdf -l gateway0\ sent\ LoRa gateway0\ received\ LoRa gateway1\ sent\ LoRa gateway1\ received\ LoRa -i ${data_path}gateways[0]_messageSentLoRaCount.csv ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[1]_messageSentLoRaCount.csv ${data_path}gateways[1]_messageReceivedLoRaCount.csv &

python3 plotData.py Sent\ and\ Received\ messages\ \(frequency\) s -I -o $out_directory/messagesSentAndReceivedIp.pdf -l gateway0\ sent\ IP gateway0\ received\ IP gateway1\ sent\ IP gateway1\ received\ IP -i ${data_path}gateways[0]_messageSentIp.csv ${data_path}gateways[0]_messageReceivedIp.csv ${data_path}gateways[1]_messageSentIp.csv ${data_path}gateways[1]_messageReceivedIp.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) s -I -o $out_directory/messagesSentAndReceivedIpCountScattered.pdf -l gateway0\ sent\ IP gateway0\ received\ IP gateway1\ sent\ IP gateway1\ received\ IP -i ${data_path}gateways[0]_messageSentIpCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[1]_messageSentIpCount.csv ${data_path}gateways[1]_messageReceivedIpCount.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) l -I -o $out_directory/messagesSentAndReceivedIpCountLine.pdf -l gateway0\ sent\ IP gateway0\ received\ IP gateway1\ sent\ IP gateway1\ received\ IP -i ${data_path}gateways[0]_messageSentIpCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[1]_messageSentIpCount.csv ${data_path}gateways[1]_messageReceivedIpCount.csv &


# Enable ignore option when multiple input files are used and an input can miss (here lost messages)
python3 plotData.py Lost\ messages\ \(frequency\) s -I -o $out_directory/messagesLost.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageLost.csv ${data_path}gateways[1]_messageLost.csv &
python3 plotData.py Lost\ messages\ \(sum\) s -I -o $out_directory/messagesLostCountScattered.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageLostCount.csv ${data_path}gateways[1]_messageLostCount.csv &
python3 plotData.py Lost\ messages\ \(sum\) l -I -o $out_directory/messagesLostCountLine.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageLostCount.csv ${data_path}gateways[1]_messageLostCount.csv &

python3 plotData.py Lost\ messages\ \(frequency\) s -I -o $out_directory/messagesLostLoRaIp.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageLostLoRa.csv ${data_path}gateways[0]_messageLostIp.csv ${data_path}gateways[1]_messageLostLoRa.csv ${data_path}gateways[1]_messageLostIp.csv &
python3 plotData.py Lost\ messages\ \(sum\) s -I -o $out_directory/messagesLostLoRaIpCountScattered.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[0]_messageLostIpCount.csv ${data_path}gateways[1]_messageLostLoRaCount.csv ${data_path}gateways[1]_messageLostIpCount.csv &
python3 plotData.py Lost\ messages\ \(sum\) l -I -o $out_directory/messagesLostLoRaIpCountLine.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[0]_messageLostIpCount.csv ${data_path}gateways[1]_messageLostLoRaCount.csv ${data_path}gateways[1]_messageLostIpCount.csv &

python3 plotData.py Received\ and\ Lost\ messages\ \(frequency\) s -I -o $out_directory/messagesReceivedAndLost.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceived.csv ${data_path}gateways[0]_messageLost.csv ${data_path}gateways[1]_messageReceived.csv ${data_path}gateways[1]_messageLost.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedAndLostCountScattered.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[0]_messageLostCount.csv ${data_path}gateways[1]_messageReceivedCount.csv ${data_path}gateways[1]_messageLostCount.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedAndLostCountLine.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[0]_messageLostCount.csv ${data_path}gateways[1]_messageReceivedCount.csv ${data_path}gateways[1]_messageLostCount.csv &

python3 plotData.py Received\ and\ Lost\ messages\ \(frequency\) s -I -o $out_directory/messagesReceivedAndLostLoRa.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceivedLoRa.csv ${data_path}gateways[0]_messageLostLoRa.csv ${data_path}gateways[1]_messageReceivedLoRa.csv ${data_path}gateways[1]_messageLostLoRa.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedAndLostLoRaCountScattered.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[1]_messageReceivedLoRaCount.csv ${data_path}gateways[1]_messageLostLoRaCount.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedAndLostLoRaCountLine.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[1]_messageReceivedLoRaCount.csv ${data_path}gateways[1]_messageLostLoRaCount.csv &

python3 plotData.py Received\ and\ Lost\ messages\ \(frequency\) s -I -o $out_directory/messagesReceivedAndLostIp.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceivedIp.csv ${data_path}gateways[0]_messageLostIp.csv ${data_path}gateways[1]_messageReceivedIp.csv ${data_path}gateways[1]_messageLostIp.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedAndLostIpCountScattered.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[0]_messageLostIpCount.csv ${data_path}gateways[1]_messageReceivedIpCount.csv ${data_path}gateways[1]_messageLostIpCount.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedAndLostIpCountLine.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[0]_messageLostIpCount.csv ${data_path}gateways[1]_messageReceivedIpCount.csv ${data_path}gateways[1]_messageLostIpCount.csv &

#python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedBar.pdf -l gateway0\ sent gateway0\ received gateway1\ sent gateway1\ received -i ${data_path}gateways[0]_messageSentCount.csv ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[1]_messageSentCount.csv ${data_path}gateways[1]_messageReceivedCount.csv &
#python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedLoRaBar.pdf -l gateway0\ sent\ LoRa gateway0\ received\ LoRa gateway1\ sent\ LoRa gateway1\ received\ LoRa -i ${data_path}gateways[0]_messageSentLoRaCount.csv ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[1]_messageSentLoRaCount.csv ${data_path}gateways[1]_messageReceivedLoRaCount.csv &
#python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedIpBar.pdf -l gateway0\ sent\ IP gateway0\ received\ IP gateway1\ sent\ IP gateway1\ received\ IP -i ${data_path}gateways[0]_messageSentIpCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[1]_messageSentIpCount.csv ${data_path}gateways[1]_messageReceivedIpCount.csv &
python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedBar.pdf -l sent received -g gateway0 gateway1 -i ${data_path}gateways[0]_messageSentCount.csv ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[1]_messageSentCount.csv ${data_path}gateways[1]_messageReceivedCount.csv &
python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedLoRaBar.pdf -l sent\ LoRa received\ LoRa -g gateway0 gateway1 -i ${data_path}gateways[0]_messageSentLoRaCount.csv ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[1]_messageSentLoRaCount.csv ${data_path}gateways[1]_messageReceivedLoRaCount.csv &
python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedIpBar.pdf -l sent\ IP received\ IP -g gateway0 gateway1 -i ${data_path}gateways[0]_messageSentIpCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[1]_messageSentIpCount.csv ${data_path}gateways[1]_messageReceivedIpCount.csv &

#python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostBar.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[0]_messageLostCount.csv ${data_path}gateways[1]_messageReceivedCount.csv ${data_path}gateways[1]_messageLostCount.csv &
#python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostLoRaBar.pdf -l gateway0\ received\ LoRa gateway0\ lost\ LoRa gateway1\ received\ LoRa gateway1\ lost\ LoRa -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[1]_messageReceivedLoRaCount.csv ${data_path}gateways[1]_messageLostLoRaCount.csv &
#python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostIpBar.pdf -l gateway0\ received\ IP gateway0\ lost\ IP gateway1\ received\ IP gateway1\ lost\ IP -i ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[0]_messageLostIpCount.csv ${data_path}gateways[1]_messageReceivedIpCount.csv ${data_path}gateways[1]_messageLostIpCount.csv &
python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostBar.pdf -l received lost -g gateway0 gateway1 -i ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[0]_messageLostCount.csv ${data_path}gateways[1]_messageReceivedCount.csv ${data_path}gateways[1]_messageLostCount.csv &
python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostLoRaBar.pdf -l received\ LoRa lost\ LoRa -g gateway0 gateway1 -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[1]_messageReceivedLoRaCount.csv ${data_path}gateways[1]_messageLostLoRaCount.csv &
python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostIpBar.pdf -l received\ IP lost\ IP -g gateway0 gateway1 -i ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[0]_messageLostIpCount.csv ${data_path}gateways[1]_messageReceivedIpCount.csv ${data_path}gateways[1]_messageLostIpCount.csv &


python3 plotData.py In\ messages\ \(sum\) s -o $out_directory/messagesInCountScattered.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[1]_messageInCount.csv &
python3 plotData.py In\ messages\ \(sum\) l -o $out_directory/messagesInCountLine.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[1]_messageInCount.csv &

python3 plotData.py In\ messages\ \(sum\) s -o $out_directory/messagesInLoRaIpCountScattered.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageInIpCount.csv ${data_path}gateways[1]_messageInLoRaCount.csv ${data_path}gateways[1]_messageInIpCount.csv &
python3 plotData.py In\ messages\ \(sum\) l -o $out_directory/messagesInLoRaIpCountLine.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageInIpCount.csv ${data_path}gateways[1]_messageInLoRaCount.csv ${data_path}gateways[1]_messageInIpCount.csv &

#python3 plotData.py In\ and\ Received\ messages b -I -o $out_directory/messagesInAndReceivedBar.pdf -l gateway0\ \in gateway0\ received gateway1\ \in gateway1\ received -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[1]_messageInCount.csv ${data_path}gateways[1]_messageReceivedCount.csv &
#python3 plotData.py In\ and\ Received\ messages b -I -o $out_directory/messagesInAndReceivedLoRaBar.pdf -l gateway0\ \in\ LoRa gateway0\ received\ LoRa gateway1\ \in\ LoRa gateway1\ received\ LoRa -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[1]_messageInLoRaCount.csv ${data_path}gateways[1]_messageReceivedLoRaCount.csv &
#python3 plotData.py In\ and\ Received\ messages b -I -o $out_directory/messagesInAndReceivedIpBar.pdf -l gateway0\ \in\ IP gateway0\ received\ IP gateway1\ \in\ IP gateway1\ received\ IP -i ${data_path}gateways[0]_messageInIpCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[1]_messageInIpCount.csv ${data_path}gateways[1]_messageReceivedIpCount.csv &
python3 plotData.py In\ and\ Received\ messages b -I -o $out_directory/messagesInAndReceivedBar.pdf -l \in received -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[1]_messageInCount.csv ${data_path}gateways[1]_messageReceivedCount.csv &
python3 plotData.py In\ and\ Received\ messages b -I -o $out_directory/messagesInAndReceivedLoRaBar.pdf -l \in\ LoRa received\ LoRa -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[1]_messageInLoRaCount.csv ${data_path}gateways[1]_messageReceivedLoRaCount.csv &
python3 plotData.py In\ and\ Received\ messages b -I -o $out_directory/messagesInAndReceivedIpBar.pdf -l \in\ IP received\ IP -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInIpCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[1]_messageInIpCount.csv ${data_path}gateways[1]_messageReceivedIpCount.csv &

#python3 plotData.py In\ and\ Lost\ messages b -I -o $out_directory/messagesInAndLostBar.pdf -l gateway0\ \in gateway0\ lost gateway1\ \in gateway1\ lost -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[0]_messageLostCount.csv ${data_path}gateways[1]_messageInCount.csv ${data_path}gateways[1]_messageLostCount.csv &
#python3 plotData.py In\ and\ Lost\ messages b -I -o $out_directory/messagesInAndLostLoRaBar.pdf -l gateway0\ \in\ LoRa gateway0\ lost\ LoRa gateway1\ \in\ LoRa gateway1\ lost\ LoRa -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[1]_messageInLoRaCount.csv ${data_path}gateways[1]_messageLostLoRaCount.csv &
#python3 plotData.py In\ and\ Lost\ messages b -I -o $out_directory/messagesInAndLostIpBar.pdf -l gateway0\ \in\ IP gateway0\ lost\ IP gateway1\ \in\ IP gateway1\ lost\ IP -i ${data_path}gateways[0]_messageInIpCount.csv ${data_path}gateways[0]_messageLostIpCount.csv ${data_path}gateways[1]_messageInIpCount.csv ${data_path}gateways[1]_messageLostIpCount.csv &
python3 plotData.py In\ and\ Lost\ messages b -I -o $out_directory/messagesInAndLostBar.pdf -l \in lost -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[0]_messageLostCount.csv ${data_path}gateways[1]_messageInCount.csv ${data_path}gateways[1]_messageLostCount.csv &
python3 plotData.py In\ and\ Lost\ messages b -I -o $out_directory/messagesInAndLostLoRaBar.pdf -l \in\ LoRa lost\ LoRa -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[1]_messageInLoRaCount.csv ${data_path}gateways[1]_messageLostLoRaCount.csv &
python3 plotData.py In\ and\ Lost\ messages b -I -o $out_directory/messagesInAndLostIpBar.pdf -l \in\ IP lost\ IP -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInIpCount.csv ${data_path}gateways[0]_messageLostIpCount.csv ${data_path}gateways[1]_messageInIpCount.csv ${data_path}gateways[1]_messageLostIpCount.csv &


# Do not enable ignore option when a single input file does not exist (interferences may not exist) because is useless to generate an empty graph
python3 plotData.py Interferences\ \(frequency\) s -o $out_directory/interferences.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_interference.csv ${data_path}gateways[1]_interference.csv &
python3 plotData.py Interferences\ \(sum\) s -o $out_directory/interferencesCountScattered.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_interferenceCount.csv ${data_path}gateways[1]_interferenceCount.csv &
python3 plotData.py Interferences\ \(sum\) l -o $out_directory/interferencesCountLine.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_interferenceCount.csv ${data_path}gateways[1]_interferenceCount.csv &

python3 plotData.py Possible\ interferences\ and\ interferences\ \(frequency\) s -I -o $out_directory/possibleInterferencesAndInterferences.pdf -l gateway0\ possible\ interferences gateway0\ interferences gateway1\ possible\ interferences gateway1\ interferences -i ${data_path}gateways[0]_interferencePossible.csv ${data_path}gateways[0]_interference.csv ${data_path}gateways[1]_interferencePossible.csv ${data_path}gateways[1]_interference.csv &
python3 plotData.py Possible\ interferences\ and\ interferences\ \(sum\) s -I -o $out_directory/possibleInterferencesAndInterferencesCountScattered.pdf -l gateway0\ possible\ interferences gateway0\ interferences gateway1\ possible\ interferences gateway1\ interferences -i ${data_path}gateways[0]_interferencePossibleCount.csv ${data_path}gateways[0]_interferenceCount.csv ${data_path}gateways[1]_interferencePossibleCount.csv ${data_path}gateways[1]_interferenceCount.csv &
python3 plotData.py Possible\ interferences\ and\ interferences\ \(sum\) l -I -o $out_directory/possibleInterferencesAndInterferencesCountLine.pdf  -l gateway0\ possible\ interferences gateway0\ interferences gateway1\ possible\ interferences gateway1\ interferences -i ${data_path}gateways[0]_interferencePossibleCount.csv ${data_path}gateways[0]_interferenceCount.csv ${data_path}gateways[1]_interferencePossibleCount.csv ${data_path}gateways[1]_interferenceCount.csv &

#python3 plotData.py In\ messages\ and\ interferences b -I -o $out_directory/messagesInAndInterferencesBar.pdf -l gateway0\ \in gateway0\ interferences gateway1\ \in gateway1\ interferences -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[0]_interferenceCount.csv ${data_path}gateways[1]_messageInCount.csv ${data_path}gateways[1]_interferenceCount.csv &
#python3 plotData.py Possible\ interferences\ and\ interferences b -I -o $out_directory/possibleInterferencesAndInterferencesBar.pdf -l gateway0\ possible\ interferences gateway0\ interferences gateway1\ possible\ interferences gateway1\ interferences -i ${data_path}gateways[0]_interferencePossibleCount.csv ${data_path}gateways[0]_interferenceCount.csv ${data_path}gateways[1]_interferencePossibleCount.csv ${data_path}gateways[1]_interferenceCount.csv &
python3 plotData.py In\ messages\ and\ interferences b -I -o $out_directory/messagesInAndInterferencesBar.pdf -l \in interferences -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[0]_interferenceCount.csv ${data_path}gateways[1]_messageInCount.csv ${data_path}gateways[1]_interferenceCount.csv &
python3 plotData.py Possible\ interferences\ and\ interferences b -I -o $out_directory/possibleInterferencesAndInterferencesBar.pdf -l possible\ interferences interferences -g gateway0 gateway1 -i ${data_path}gateways[0]_interferencePossibleCount.csv ${data_path}gateways[0]_interferenceCount.csv ${data_path}gateways[1]_interferencePossibleCount.csv ${data_path}gateways[1]_interferenceCount.csv &


python3 plotData.py RSSIs h -o $out_directory/RSSIs.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_rssi.csv ${data_path}gateways[1]_rssi.csv &
#==============================

# Wait parallel executions end
wait
printf "\nAll charts are plotted. Bye :)\n"