#!/bin/bash

if [ ! $# -eq 2 ]
then
    printf "No directory to open or number of gateways missing!\nUsage: ${0} <directory_path> <number_gateways>\n"
    exit 1
fi

# Get directory path
data_path=$1

# Append "/" to the directory path if not already present
if [ ! ${data_path: -1} = "/" ]
then
    data_path+="/"
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

# Do not enable ignore option when a single input file does not exist (lost may not exist) because is useless to generate an empty graph
python3 plotData.py Lost\ messages\ \(frequency\) s -o $out_directory/messageLost.pdf -i ${data_path}endDevices[0]_messageLost.csv &
python3 plotData.py Lost\ messages\ \(sum\) s -o $out_directory/messageLostCountScattered.pdf -i ${data_path}endDevices[0]_messageLostCount.csv &
python3 plotData.py Lost\ messages\ \(sum\) l -o $out_directory/messageLostCountLine.pdf -i ${data_path}endDevices[0]_messageLostCount.csv &

python3 plotData.py Invalid\ messages\ \(frequency\) s -o $out_directory/messageInvalid.pdf -i ${data_path}endDevices[0]_messageInvalid.csv &
python3 plotData.py Invalid\ messages\ \(sum\) s -o $out_directory/messageInvalidCountScattered.pdf -i ${data_path}endDevices[0]_messageInvalidCount.csv &
python3 plotData.py Invalid\ messages\ \(sum\) l -o $out_directory/messageInvalidCountLine.pdf -i ${data_path}endDevices[0]_messageInvalidCount.csv &

# Do not enable ignore option when a single input file does not exist (retransmissions may not exist) because is useless to generate an empty graph
python3 plotData.py Retransmitted\ messages\ \(frequency\) s -o $out_directory/messageRetransmitted.pdf -i ${data_path}endDevices[0]_messageRetransmitted.csv &
python3 plotData.py Retransmitted\ messages\ \(sum\) s -o $out_directory/messageRetransmittedCountScattered.pdf -i ${data_path}endDevices[0]_messageRetransmittedCount.csv &
python3 plotData.py Retransmitted\ messages\ \(sum\) l -o $out_directory/messageRetransmittedCountLine.pdf -i ${data_path}endDevices[0]_messageRetransmittedCount.csv &

# Do not enable ignore option when a single input file does not exist (interferences may not exist) because is useless to generate an empty graph
python3 plotData.py Interferences\ \(frequency\) s -o $out_directory/interferences.pdf -i ${data_path}endDevices[0]_interference.csv &
python3 plotData.py Interferences\ \(sum\) s -o $out_directory/interferencesCountScattered.pdf -i ${data_path}endDevices[0]_interferenceCount.csv &
python3 plotData.py Interferences\ \(sum\) l -o $out_directory/interferencesCountLine.pdf -i ${data_path}endDevices[0]_interferenceCount.csv &


python3 plotData.py Sent\ and\ Received\ messages\ \(frequency\) s -I -o $out_directory/messagesSentAndReceived.pdf -l sent received -i ${data_path}endDevices[0]_messageSent.csv ${data_path}endDevices[0]_messageReceived.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) s -I -o $out_directory/messagesSentAndReceivedCountScattered.pdf -l sent received -i ${data_path}endDevices[0]_messageSentCount.csv ${data_path}endDevices[0]_messageReceivedCount.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) l -I -o $out_directory/messagesSentAndReceivedCountLine.pdf -l sent received -i ${data_path}endDevices[0]_messageSentCount.csv ${data_path}endDevices[0]_messageReceivedCount.csv &

python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedBar.pdf -l sent received -g device0 -i ${data_path}endDevices[0]_messageSentCount.csv ${data_path}endDevices[0]_messageReceivedCount.csv &

# Enable ignore option when multiple input files are used and an input can miss (here retransmissions)
python3 plotData.py Sent\ and\ Retransmitted\ messages\ \(frequency\) s -I -o $out_directory/messagesSentAndRetransmitted.pdf -l sent retransmitted -i ${data_path}endDevices[0]_messageSent.csv ${data_path}endDevices[0]_messageRetransmitted.csv &
python3 plotData.py Sent\ and\ Retransmitted\ messages\ \(sum\) s -I -o $out_directory/messagesSentAndRetransmittedCountScattered.pdf -l sent retransmitted -i ${data_path}endDevices[0]_messageSentCount.csv ${data_path}endDevices[0]_messageRetransmittedCount.csv &
python3 plotData.py Sent\ and\ Retransmitted\ messages\ \(sum\) l -I -o $out_directory/messagesSentAndRetransmittedCountLine.pdf -l sent retransmitted -i ${data_path}endDevices[0]_messageSentCount.csv ${data_path}endDevices[0]_messageRetransmittedCount.csv &

python3 plotData.py Sent\ and\ Retransmitted\ messages b -I -o $out_directory/messagesSentAndRetransmittedBar.pdf -l sent retransmitted -g device0 -i ${data_path}endDevices[0]_messageSentCount.csv ${data_path}endDevices[0]_messageRetransmittedCount.csv &


python3 plotData.py Received\ and\ Lost\ messages\ \(frequency\) s -I -o $out_directory/messagesReceivedAndLost.pdf -l received lost -i ${data_path}endDevices[0]_messageReceived.csv ${data_path}endDevices[0]_messageLost.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedAndLostCountScattered.pdf -l received lost -i ${data_path}endDevices[0]_messageReceivedCount.csv ${data_path}endDevices[0]_messageLostCount.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedAndLostCountLine.pdf -l received lost -i ${data_path}endDevices[0]_messageReceivedCount.csv ${data_path}endDevices[0]_messageLostCount.csv &

python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostBar.pdf -l received lost -g device0 -i ${data_path}endDevices[0]_messageReceivedCount.csv ${data_path}endDevices[0]_messageLostCount.csv &


python3 plotData.py In\ and\ Received\ messages\ \(sum\) s -I -o $out_directory/messagesInAndReceivedCountScattered.pdf -l \in received -i ${data_path}endDevices[0]_messageInCount.csv ${data_path}endDevices[0]_messageReceivedCount.csv &
python3 plotData.py In\ and\ Received\ messages\ \(sum\) l -I -o $out_directory/messagesInAndReceivedCountLine.pdf -l \in received -i ${data_path}endDevices[0]_messageInCount.csv ${data_path}endDevices[0]_messageReceivedCount.csv &
python3 plotData.py In\ and\ Received\ messages b -I -o $out_directory/messagesInAndReceivedBar.pdf -l \in received -g device0 -i ${data_path}endDevices[0]_messageInCount.csv ${data_path}endDevices[0]_messageReceivedCount.csv &

python3 plotData.py In\ and\ Lost\ messages\ \(sum\) s -I -o $out_directory/messagesInAndLostCountScattered.pdf -l \in lost -i ${data_path}endDevices[0]_messageInCount.csv ${data_path}endDevices[0]_messageLostCount.csv &
python3 plotData.py In\ and\ Lost\ messages\ \(sum\) l -I -o $out_directory/messagesInAndLostCountLine.pdf -l \in lost -i ${data_path}endDevices[0]_messageInCount.csv ${data_path}endDevices[0]_messageLostCount.csv &
python3 plotData.py In\ and\ Lost\ messages b -I -o $out_directory/messagesInAndLostBar.pdf -l \in lost -g device0 -i ${data_path}endDevices[0]_messageInCount.csv ${data_path}endDevices[0]_messageLostCount.csv &

python3 plotData.py In\ and\ Invalid\ messages\ \(sum\) s -I -o $out_directory/messagesInAndInvalidCountScattered.pdf -l \in invalid -i ${data_path}endDevices[0]_messageInCount.csv ${data_path}endDevices[0]_messageInvalidCount.csv &
python3 plotData.py In\ and\ Invalid\ messages\ \(sum\) l -I -o $out_directory/messagesInAndInvalidCountLine.pdf -l \in invalid -i ${data_path}endDevices[0]_messageInCount.csv ${data_path}endDevices[0]_messageInvalidCount.csv &
python3 plotData.py In\ and\ Invalid\ messages b -I -o $out_directory/messagesInAndInvalidBar.pdf -l \in invalid -g device0 -i ${data_path}endDevices[0]_messageInCount.csv ${data_path}endDevices[0]_messageInvalidCount.csv &


# Enable ignore option when multiple input files are used and an input can miss.
# Downlink interferences and possible interferences are counted by all covered end devices, so, they are the same for all of them
python3 plotData.py Possible\ interferences\ and\ interferences\ \(frequency\) s -I -o $out_directory/possibleInterferencesAndInterferences.pdf -l possible\ interferences interferences -i ${data_path}endDevices[0]_interferencePossible.csv ${data_path}endDevices[0]_interference.csv &
python3 plotData.py Possible\ interferences\ and\ interferences\ \(sum\) s -I -o $out_directory/possibleInterferencesAndInterferencesCountScattered.pdf -l possible\ interferences interferences -i ${data_path}endDevices[0]_interferencePossibleCount.csv ${data_path}endDevices[0]_interferenceCount.csv &
python3 plotData.py Possible\ interferences\ and\ interferences\ \(sum\) l -I -o $out_directory/possibleInterferencesAndInterferencesCountLine.pdf -l possible\ interferences interferences -i ${data_path}endDevices[0]_interferencePossibleCount.csv ${data_path}endDevices[0]_interferenceCount.csv &

python3 plotData.py In\ messages\ and\ interferences b -I -o $out_directory/messagesInAndInterferencesBar.pdf -l \in interferences -g endDevice0 -i ${data_path}endDevices[0]_messageInCount.csv ${data_path}endDevices[0]_interferenceCount.csv &
python3 plotData.py Possible\ interferences\ and\ interferences b -I -o $out_directory/possibleInterferencesAndInterferencesBar.pdf -l possible\ interferences interferences -g endDevice0 -i ${data_path}endDevices[0]_interferencePossibleCount.csv ${data_path}endDevices[0]_interferenceCount.csv &
#======================================



#==== STATS ABOUT MULTIPLE DEVICES ====
out_directory="${data_path}multiple_devices/"
echo "Plotting data into $out_directory"

python3 plotData.py Sent\ messages\ \(frequency\) s -I -o $out_directory/messagesSent.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageSent.csv ${data_path}endDevices[1]_messageSent.csv ${data_path}endDevices[2]_messageSent.csv ${data_path}endDevices[3]_messageSent.csv ${data_path}endDevices[4]_messageSent.csv &
python3 plotData.py Sent\ messages\ \(sum\) s -I -o $out_directory/messagesSentCountScattered.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageSentCount.csv ${data_path}endDevices[1]_messageSentCount.csv ${data_path}endDevices[2]_messageSentCount.csv ${data_path}endDevices[3]_messageSentCount.csv ${data_path}endDevices[4]_messageSentCount.csv &
python3 plotData.py Sent\ messages\ \(sum\) l -I -o $out_directory/messagesSentCountLine.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageSentCount.csv ${data_path}endDevices[1]_messageSentCount.csv ${data_path}endDevices[2]_messageSentCount.csv ${data_path}endDevices[3]_messageSentCount.csv ${data_path}endDevices[4]_messageSentCount.csv &

python3 plotData.py Received\ messages\ \(frequency\) s -I -o $out_directory/messageReceived.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageReceived.csv ${data_path}endDevices[1]_messageReceived.csv ${data_path}endDevices[2]_messageReceived.csv ${data_path}endDevices[3]_messageReceived.csv ${data_path}endDevices[4]_messageReceived.csv &
python3 plotData.py Received\ messages\ \(sum\) s -I -o $out_directory/messageReceivedCountScattered.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageReceivedCount.csv ${data_path}endDevices[1]_messageReceivedCount.csv ${data_path}endDevices[2]_messageReceivedCount.csv ${data_path}endDevices[3]_messageReceivedCount.csv ${data_path}endDevices[4]_messageReceivedCount.csv &
python3 plotData.py Received\ messages\ \(sum\) l -I -o $out_directory/messageReceivedCountLine.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageReceivedCount.csv ${data_path}endDevices[1]_messageReceivedCount.csv ${data_path}endDevices[2]_messageReceivedCount.csv ${data_path}endDevices[3]_messageReceivedCount.csv ${data_path}endDevices[4]_messageReceivedCount.csv &

python3 plotData.py Lost\ messages\ \(frequency\) s -I -o $out_directory/messageLost.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageLost.csv ${data_path}endDevices[1]_messageLost.csv ${data_path}endDevices[2]_messageLost.csv ${data_path}endDevices[3]_messageLost.csv ${data_path}endDevices[4]_messageLost.csv &
python3 plotData.py Lost\ messages\ \(sum\) s -I -o $out_directory/messageLostCountScattered.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageLostCount.csv ${data_path}endDevices[1]_messageLostCount.csv ${data_path}endDevices[2]_messageLostCount.csv ${data_path}endDevices[3]_messageLostCount.csv ${data_path}endDevices[4]_messageLostCount.csv &
python3 plotData.py Lost\ messages\ \(sum\) l -I -o $out_directory/messageLostCountLine.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageLostCount.csv ${data_path}endDevices[1]_messageLostCount.csv ${data_path}endDevices[2]_messageLostCount.csv ${data_path}endDevices[3]_messageLostCount.csv ${data_path}endDevices[4]_messageLostCount.csv &

python3 plotData.py Invalid\ messages\ \(frequency\) s -I -o $out_directory/messageInvalid.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageInvalid.csv ${data_path}endDevices[1]_messageInvalid.csv ${data_path}endDevices[2]_messageInvalid.csv ${data_path}endDevices[3]_messageInvalid.csv ${data_path}endDevices[4]_messageInvalid.csv &
python3 plotData.py Invalid\ messages\ \(sum\) s -I -o $out_directory/messageInvalidCountScattered.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageInvalidCount.csv ${data_path}endDevices[1]_messageInvalidCount.csv ${data_path}endDevices[2]_messageInvalidCount.csv ${data_path}endDevices[3]_messageInvalidCount.csv ${data_path}endDevices[4]_messageInvalidCount.csv &
python3 plotData.py Invalid\ messages\ \(sum\) l -I -o $out_directory/messageInvalidCountLine.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageInvalidCount.csv ${data_path}endDevices[1]_messageInvalidCount.csv ${data_path}endDevices[2]_messageInvalidCount.csv ${data_path}endDevices[3]_messageInvalidCount.csv ${data_path}endDevices[4]_messageInvalidCount.csv &

python3 plotData.py Retransmitted\ messages\ \(frequency\) s -I -o $out_directory/messageRetransmitted.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageRetransmitted.csv ${data_path}endDevices[1]_messageRetransmitted.csv ${data_path}endDevices[2]_messageRetransmitted.csv ${data_path}endDevices[3]_messageRetransmitted.csv ${data_path}endDevices[4]_messageRetransmitted.csv &
python3 plotData.py Retransmitted\ messages\ \(sum\) s -I -o $out_directory/messageRetransmittedCountScattered.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageRetransmittedCount.csv ${data_path}endDevices[1]_messageRetransmittedCount.csv ${data_path}endDevices[2]_messageRetransmittedCount.csv ${data_path}endDevices[3]_messageRetransmittedCount.csv ${data_path}endDevices[4]_messageRetransmittedCount.csv &
python3 plotData.py Retransmitted\ messages\ \(sum\) l -I -o $out_directory/messageRetransmittedCountLine.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageRetransmittedCount.csv ${data_path}endDevices[1]_messageRetransmittedCount.csv ${data_path}endDevices[2]_messageRetransmittedCount.csv ${data_path}endDevices[3]_messageRetransmittedCount.csv ${data_path}endDevices[4]_messageRetransmittedCount.csv &

python3 plotData.py Interferences\ messages\ \(frequency\) s -I -o $out_directory/interferences.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_interference.csv ${data_path}endDevices[1]_interference.csv ${data_path}endDevices[2]_interference.csv ${data_path}endDevices[3]_interference.csv ${data_path}endDevices[4]_interference.csv &
python3 plotData.py Interferences\ messages\ \(sum\) s -I -o $out_directory/interferencesCountScattered.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_interferenceCount.csv ${data_path}endDevices[1]_interferenceCount.csv ${data_path}endDevices[2]_interferenceCount.csv ${data_path}endDevices[3]_interferenceCount.csv ${data_path}endDevices[4]_interferenceCount.csv &
python3 plotData.py Interferences\ messages\ \(sum\) l -I -o $out_directory/interferencesCountLine.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_interferenceCount.csv ${data_path}endDevices[1]_interferenceCount.csv ${data_path}endDevices[2]_interferenceCount.csv ${data_path}endDevices[3]_interferenceCount.csv ${data_path}endDevices[4]_interferenceCount.csv &

# These automatically plot charts with min, max and mean
python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedBar.pdf -l sent received -g device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageSentCount.csv ${data_path}endDevices[0]_messageReceivedCount.csv ${data_path}endDevices[1]_messageSentCount.csv ${data_path}endDevices[1]_messageReceivedCount.csv ${data_path}endDevices[2]_messageSentCount.csv ${data_path}endDevices[2]_messageReceivedCount.csv ${data_path}endDevices[3]_messageSentCount.csv ${data_path}endDevices[3]_messageReceivedCount.csv ${data_path}endDevices[4]_messageSentCount.csv ${data_path}endDevices[4]_messageReceivedCount.csv &
python3 plotData.py Sent\ and\ Retransmitted\ messages b -I -o $out_directory/messagesSentAndRetransmittedBar.pdf -l sent retransmitted -g device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageSentCount.csv ${data_path}endDevices[0]_messageRetransmittedCount.csv ${data_path}endDevices[1]_messageSentCount.csv ${data_path}endDevices[1]_messageRetransmittedCount.csv ${data_path}endDevices[2]_messageSentCount.csv ${data_path}endDevices[2]_messageRetransmittedCount.csv ${data_path}endDevices[3]_messageSentCount.csv ${data_path}endDevices[3]_messageRetransmittedCount.csv ${data_path}endDevices[4]_messageSentCount.csv ${data_path}endDevices[4]_messageRetransmittedCount.csv &
python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostBar.pdf -l received lost -g device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageReceivedCount.csv ${data_path}endDevices[0]_messageLostCount.csv ${data_path}endDevices[1]_messageReceivedCount.csv ${data_path}endDevices[1]_messageLostCount.csv ${data_path}endDevices[2]_messageReceivedCount.csv ${data_path}endDevices[2]_messageLostCount.csv ${data_path}endDevices[3]_messageReceivedCount.csv ${data_path}endDevices[3]_messageLostCount.csv ${data_path}endDevices[4]_messageReceivedCount.csv ${data_path}endDevices[4]_messageLostCount.csv &

python3 plotData.py In\ and\ Received\ messages b -I -o $out_directory/messagesInAndReceivedBar.pdf -l \in received -g device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageInCount.csv ${data_path}endDevices[0]_messageReceivedCount.csv ${data_path}endDevices[1]_messageInCount.csv ${data_path}endDevices[1]_messageReceivedCount.csv ${data_path}endDevices[2]_messageInCount.csv ${data_path}endDevices[2]_messageReceivedCount.csv ${data_path}endDevices[3]_messageInCount.csv ${data_path}endDevices[3]_messageReceivedCount.csv ${data_path}endDevices[4]_messageInCount.csv ${data_path}endDevices[4]_messageReceivedCount.csv &
python3 plotData.py In\ and\ Lost\ messages b -I -o $out_directory/messagesInAndLostBar.pdf -l \in lost -g device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageInCount.csv ${data_path}endDevices[0]_messageLostCount.csv ${data_path}endDevices[1]_messageInCount.csv ${data_path}endDevices[1]_messageLostCount.csv ${data_path}endDevices[2]_messageInCount.csv ${data_path}endDevices[2]_messageLostCount.csv ${data_path}endDevices[3]_messageInCount.csv ${data_path}endDevices[3]_messageLostCount.csv ${data_path}endDevices[4]_messageInCount.csv ${data_path}endDevices[4]_messageLostCount.csv &
python3 plotData.py In\ and\ Invalid\ messages b -I -o $out_directory/messagesInAndInvalidBar.pdf -l \in invalid -g device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageInCount.csv ${data_path}endDevices[0]_messageInvalidCount.csv ${data_path}endDevices[1]_messageInCount.csv ${data_path}endDevices[1]_messageInvalidCount.csv ${data_path}endDevices[2]_messageInCount.csv ${data_path}endDevices[2]_messageInvalidCount.csv ${data_path}endDevices[3]_messageInCount.csv ${data_path}endDevices[3]_messageInvalidCount.csv ${data_path}endDevices[4]_messageInCount.csv ${data_path}endDevices[4]_messageInvalidCount.csv &

python3 plotData.py In\ messages\ and\ interferences b -I -o $out_directory/messagesInAndInterferencesBar.pdf -l \in interferences -g device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageInCount.csv ${data_path}endDevices[0]_interferenceCount.csv ${data_path}endDevices[1]_messageInCount.csv ${data_path}endDevices[1]_interferenceCount.csv ${data_path}endDevices[2]_messageInCount.csv ${data_path}endDevices[2]_interferenceCount.csv ${data_path}endDevices[3]_messageInCount.csv ${data_path}endDevices[3]_interferenceCount.csv ${data_path}endDevices[4]_messageInCount.csv ${data_path}endDevices[4]_interferenceCount.csv &
python3 plotData.py Possible\ interferences\ and\ interferences b -I -o $out_directory/possibleInterferencesAndInterferencesBar.pdf -l possible\ interferences interferences -g device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_interferencePossibleCount.csv ${data_path}endDevices[0]_interferenceCount.csv ${data_path}endDevices[1]_interferencePossibleCount.csv ${data_path}endDevices[1]_interferenceCount.csv ${data_path}endDevices[2]_interferencePossibleCount.csv ${data_path}endDevices[2]_interferenceCount.csv ${data_path}endDevices[3]_interferencePossibleCount.csv ${data_path}endDevices[3]_interferenceCount.csv ${data_path}endDevices[4]_interferencePossibleCount.csv ${data_path}endDevices[4]_interferenceCount.csv &
#======================================

wait


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

python3 plotData.py Sent\ messages\ \(frequency\) s -o $out_directory/messagesSentLoRa.pdf -l LoRa\ gateways LoRa\ devices -i ${data_path}gateways[0]_messageSentLoRaGateways.csv ${data_path}gateways[0]_messageSentLoRaDevices.csv &
python3 plotData.py Sent\ messages\ \(sum\) s -o $out_directory/messagesSentLoRaCountScattered.pdf -l LoRa\ gateways LoRa\ devices -i ${data_path}gateways[0]_messageSentLoRaGatewaysCount.csv ${data_path}gateways[0]_messageSentLoRaDevicesCount.csv &
python3 plotData.py Sent\ messages\ \(sum\) l -o $out_directory/messagesSentLoRaCountLine.pdf -l LoRa\ gateways LoRa\ devices -i ${data_path}gateways[0]_messageSentLoRaGatewaysCount.csv ${data_path}gateways[0]_messageSentLoRaDevicesCount.csv &

python3 plotData.py Sent\ messages\ \(frequency\) s -o $out_directory/messagesSentIp.pdf -l IP\ gateways IP\ server -i ${data_path}gateways[0]_messageSentIpGateways.csv ${data_path}gateways[0]_messageSentIpServer.csv &
python3 plotData.py Sent\ messages\ \(sum\) s -o $out_directory/messagesSentIpCountScattered.pdf -l IP\ gateways IP\ server -i ${data_path}gateways[0]_messageSentIpGatewaysCount.csv ${data_path}gateways[0]_messageSentIpServerCount.csv &
python3 plotData.py Sent\ messages\ \(sum\) l -o $out_directory/messagesSentIpCountLine.pdf -l IP\ gateways IP\ server -i ${data_path}gateways[0]_messageSentIpGatewaysCount.csv ${data_path}gateways[0]_messageSentIpServerCount.csv &


python3 plotData.py Received\ messages\ \(frequency\) s -o $out_directory/messagesReceived.pdf -i ${data_path}gateways[0]_messageReceived.csv &
python3 plotData.py Received\ messages\ \(sum\) s -o $out_directory/messagesReceivedCountScattered.pdf -i ${data_path}gateways[0]_messageReceivedCount.csv &
python3 plotData.py Received\ messages\ \(sum\) l -o $out_directory/messagesReceivedCountLine.pdf -i ${data_path}gateways[0]_messageReceivedCount.csv &

python3 plotData.py Received\ messages\ \(frequency\) s -o $out_directory/messagesReceivedLoRaIp.pdf -l LoRa IP -i ${data_path}gateways[0]_messageReceivedLoRa.csv ${data_path}gateways[0]_messageReceivedIp.csv &
python3 plotData.py Received\ messages\ \(sum\) s -o $out_directory/messagesReceivedLoRaIpCountScattered.pdf -l LoRa IP -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv &
python3 plotData.py Received\ messages\ \(sum\) l -o $out_directory/messagesReceivedLoRaIpCountLine.pdf -l LoRa IP -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv &

python3 plotData.py Received\ messages\ \(frequency\) s -o $out_directory/messagesReceivedLoRa.pdf -l LoRa\ gateways LoRa\ devices -i ${data_path}gateways[0]_messageReceivedLoRaGateways.csv ${data_path}gateways[0]_messageReceivedLoRaDevices.csv &
python3 plotData.py Received\ messages\ \(sum\) s -o $out_directory/messagesReceivedLoRaCountScattered.pdf -l LoRa\ gateways LoRa\ devices -i ${data_path}gateways[0]_messageReceivedLoRaGatewaysCount.csv ${data_path}gateways[0]_messageReceivedLoRaDevicesCount.csv &
python3 plotData.py Received\ messages\ \(sum\) l -o $out_directory/messagesReceivedLoRaCountLine.pdf -l LoRa\ gateways LoRa\ devices -i ${data_path}gateways[0]_messageReceivedLoRaGatewaysCount.csv ${data_path}gateways[0]_messageReceivedLoRaDevicesCount.csv &

python3 plotData.py Received\ messages\ \(frequency\) s -o $out_directory/messagesReceivedIp.pdf -l IP\ gateways IP\ server -i ${data_path}gateways[0]_messageReceivedIpGateways.csv ${data_path}gateways[0]_messageReceivedIpServer.csv &
python3 plotData.py Received\ messages\ \(sum\) s -o $out_directory/messagesReceivedIpCountScattered.pdf -l IP\ gateways IP\ server -i ${data_path}gateways[0]_messageReceivedIpGatewaysCount.csv ${data_path}gateways[0]_messageReceivedIpServerCount.csv &
python3 plotData.py Received\ messages\ \(sum\) l -o $out_directory/messagesReceivedIpCountLine.pdf -l IP\ gateways IP\ server -i ${data_path}gateways[0]_messageReceivedIpGatewaysCount.csv ${data_path}gateways[0]_messageReceivedIpServerCount.csv &


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

python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedBar.pdf -l sent received -g gateway0 -i ${data_path}gateways[0]_messageSentCount.csv ${data_path}gateways[0]_messageReceivedCount.csv &
python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedLoRaIpBar.pdf -l sent\ LoRa received\ LoRa sent\ IP received\ IP -g gateway0 -i ${data_path}gateways[0]_messageSentLoRaCount.csv ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageSentIpCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv &
python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedLoRaBar.pdf -l sent\ LoRa\ gateways received\ LoRa\ gateways sent\ LoRa\ devices received\ LoRa\ devices -g gateway0 -i ${data_path}gateways[0]_messageSentLoRaGatewaysCount.csv ${data_path}gateways[0]_messageReceivedLoRaGatewaysCount.csv ${data_path}gateways[0]_messageSentLoRaDevicesCount.csv ${data_path}gateways[0]_messageReceivedLoRaDevicesCount.csv &
python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedIpBar.pdf -l sent\ IP\ gateways received\ IP\ gateways sent\ IP\ server received\ IP\ server -g gateway0 -i ${data_path}gateways[0]_messageSentIpGatewaysCount.csv ${data_path}gateways[0]_messageReceivedIpGatewaysCount.csv ${data_path}gateways[0]_messageSentIpServerCount.csv ${data_path}gateways[0]_messageReceivedIpServerCount.csv &


# Do not enable ignore option when a single input file does not exist (lost messages may not exist) because is useless to generate an empty graph
python3 plotData.py Lost\ messages\ \(frequency\) s -o $out_directory/messagesLost.pdf -i ${data_path}gateways[0]_messageLost.csv &
python3 plotData.py Lost\ messages\ \(sum\) s -o $out_directory/messagesLostCountScattered.pdf -i ${data_path}gateways[0]_messageLostCount.csv &
python3 plotData.py Lost\ messages\ \(sum\) l -o $out_directory/messagesLostCountLine.pdf -i ${data_path}gateways[0]_messageLostCount.csv &

# Enable ignore option when multiple input files are used and an input can miss (here lost messages)
python3 plotData.py Lost\ messages\ \(frequency\) s -I -o $out_directory/messagesLostLoRaIp.pdf -l LoRa IP -i ${data_path}gateways[0]_messageLostLoRa.csv ${data_path}gateways[0]_messageLostIp.csv &
python3 plotData.py Lost\ messages\ \(sum\) s -I -o $out_directory/messagesLostLoRaIpCountScattered.pdf -l LoRa IP -i ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[0]_messageLostIpCount.csv &
python3 plotData.py Lost\ messages\ \(sum\) l -I -o $out_directory/messagesLostLoRaIpCountLine.pdf -l LoRa IP -i ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[0]_messageLostIpCount.csv &

python3 plotData.py Lost\ messages\ \(frequency\) s -o $out_directory/messagesLostLoRa.pdf -l LoRa\ gateways LoRa\ devices -i ${data_path}gateways[0]_messageLostLoRaGateways.csv ${data_path}gateways[0]_messageLostLoRaDevices.csv &
python3 plotData.py Lost\ messages\ \(sum\) s -o $out_directory/messagesLostLoRaCountScattered.pdf -l LoRa\ gateways LoRa\ devices -i ${data_path}gateways[0]_messageLostLoRaGatewaysCount.csv ${data_path}gateways[0]_messageLostLoRaDevicesCount.csv &
python3 plotData.py Lost\ messages\ \(sum\) l -o $out_directory/messagesLostLoRaCountLine.pdf -l LoRa\ gateways LoRa\ devices -i ${data_path}gateways[0]_messageLostLoRaGatewaysCount.csv ${data_path}gateways[0]_messageLostLoRaDevicesCount.csv &

python3 plotData.py Lost\ messages\ \(frequency\) s -o $out_directory/messagesLostIp.pdf -l IP\ gateways IP\ server -i ${data_path}gateways[0]_messageLostIpGateways.csv ${data_path}gateways[0]_messageLostIpServer.csv &
python3 plotData.py Lost\ messages\ \(sum\) s -o $out_directory/messagesLostIpCountScattered.pdf -l IP\ gateways IP\ server -i ${data_path}gateways[0]_messageLostIpGatewaysCount.csv ${data_path}gateways[0]_messageLostIpServerCount.csv &
python3 plotData.py Lost\ messages\ \(sum\) l -o $out_directory/messagesLostIpCountLine.pdf -l IP\ gateways IP\ server -i ${data_path}gateways[0]_messageLostIpGatewaysCount.csv ${data_path}gateways[0]_messageLostIpServerCount.csv &


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

python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostBar.pdf -l received lost -g gateway0 -i ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[0]_messageLostCount.csv &
python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostLoRaIpBar.pdf -l received\ LoRa lost\ LoRa received\ IP lost\ IP -g gateway0 -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[0]_messageLostIpCount.csv &
python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostLoRaBar.pdf -l received\ LoRa\ gateways lost\ LoRa\ gateways received\ LoRa\ devices lost\ LoRa\ devices -g gateway0 -i ${data_path}gateways[0]_messageReceivedLoRaGatewaysCount.csv ${data_path}gateways[0]_messageLostLoRaGatewaysCount.csv ${data_path}gateways[0]_messageReceivedLoRaDevicesCount.csv ${data_path}gateways[0]_messageLostLoRaDevicesCount.csv &
python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostIpBar.pdf -l received\ IP\ gateways lost\ IP\ gateways received\ IP\ server lost\ IP\ server -g gateway0 -i ${data_path}gateways[0]_messageReceivedIpGatewaysCount.csv ${data_path}gateways[0]_messageLostIpGatewaysCount.csv ${data_path}gateways[0]_messageReceivedIpServerCount.csv ${data_path}gateways[0]_messageLostIpServerCount.csv &


python3 plotData.py In\ messages\ \(sum\) s -o $out_directory/messagesInCountScattered.pdf -i ${data_path}gateways[0]_messageInCount.csv &
python3 plotData.py In\ messages\ \(sum\) l -o $out_directory/messagesInCountLine.pdf -i ${data_path}gateways[0]_messageInCount.csv &

python3 plotData.py In\ messages\ \(sum\) s -o $out_directory/messagesInLoRaIpCountScattered.pdf -l LoRa IP -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageInIpCount.csv &
python3 plotData.py In\ messages\ \(sum\) l -o $out_directory/messagesInLoRaIpCountLine.pdf -l LoRa IP -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageInIpCount.csv &

python3 plotData.py In\ and\ Received\ messages b -I -o $out_directory/messagesInAndReceivedBar.pdf -l \in received -g gateway0 -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[0]_messageReceivedCount.csv &
python3 plotData.py In\ and\ Received\ messages b -I -o $out_directory/messagesInAndReceivedLoRaIpBar.pdf -l \in\ LoRa received\ LoRa \in\ IP received\ IP -g gateway0 -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageInIpCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv &

python3 plotData.py In\ and\ Lost\ messages b -I -o $out_directory/messagesInAndLostBar.pdf -l \in lost -g gateway0 -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[0]_messageLostCount.csv &
python3 plotData.py In\ and\ Lost\ messages b -I -o $out_directory/messagesInAndLostLoRaIpBar.pdf -l \in\ LoRa lost\ LoRa \in\ IP lost\ IP -g gateway0 -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[0]_messageInIpCount.csv ${data_path}gateways[0]_messageLostIpCount.csv &

python3 plotData.py In\ and\ Invalid\ messages b -I -o $out_directory/messagesInAndInvalidBar.pdf -l \in invalid -g gateway0 -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[0]_messageInvalidCount.csv &
python3 plotData.py In\ and\ Invalid\ messages b -I -o $out_directory/messagesInAndInvalidLoRaIpBar.pdf -l \in\ LoRa invalid\ LoRa \in\ IP invalid\ IP -g gateway0 -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageInvalidLoRaCount.csv ${data_path}gateways[0]_messageInvalidIpCount.csv ${data_path}gateways[0]_messageInvalidIpCount.csv &


# Do not enable ignore option when a single input file does not exist (interferences may not exist) because is useless to generate an empty graph
python3 plotData.py Interferences\ \(frequency\) s -o $out_directory/interferences.pdf -i ${data_path}gateways[0]_interference.csv &
python3 plotData.py Interferences\ \(sum\) s -o $out_directory/interferencesCountScattered.pdf -i ${data_path}gateways[0]_interferenceCount.csv &
python3 plotData.py Interferences\ \(sum\) l -o $out_directory/interferencesCountLine.pdf -i ${data_path}gateways[0]_interferenceCount.csv &

# Enable ignore option when multiple input files are used and an input can miss
python3 plotData.py Possible\ interferences\ and\ interferences\ \(frequency\) s -I -o $out_directory/possibleInterferencesAndInterferences.pdf -l possible\ interferences interferences -i ${data_path}gateways[0]_interferencePossible.csv ${data_path}gateways[0]_interference.csv &
python3 plotData.py Possible\ interferences\ and\ interferences\ \(sum\) s -I -o $out_directory/possibleInterferencesAndInterferencesCountScattered.pdf -l possible\ interferences interferences -i ${data_path}gateways[0]_interferencePossibleCount.csv ${data_path}gateways[0]_interferenceCount.csv &
python3 plotData.py Possible\ interferences\ and\ interferences\ \(sum\) l -I -o $out_directory/possibleInterferencesAndInterferencesCountLine.pdf -l possible\ interferences interferences -i ${data_path}gateways[0]_interferencePossibleCount.csv ${data_path}gateways[0]_interferenceCount.csv &

python3 plotData.py In\ messages\ and\ interferences b -I -o $out_directory/messagesInAndInterferencesBar.pdf -l \in interferences -g gateway0 -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[0]_interferenceCount.csv &
python3 plotData.py Possible\ interferences\ and\ interferences b -I -o $out_directory/possibleInterferencesAndInterferencesBar.pdf -l possible\ interferences interferences -g gateway0 -i ${data_path}gateways[0]_interferencePossibleCount.csv ${data_path}gateways[0]_interferenceCount.csv &


python3 plotData.py RSSIs h -o $out_directory/RSSIs.pdf -i ${data_path}gateways[0]_rssi.csv &
#==============================

wait


#=== STATS ABOUT MULTIPLE GATEWAYS ===
out_directory="${data_path}multiple_gateways/"
echo "Plotting data into $out_directory"

# Get all CSV files about end devices (true) or gateways (false) ending with the provided string
function getAllFiles () {
    local exported_data_files=()

    if [ $3 = true ]
    then
        dev="endDevices"
    else
        dev="gateways"
    fi

    # Get all device files
    for ((i=0;i<$2;i++))
    do
        exported_data_files+=(${data_path}${dev}[$i]$1)
    done

    echo ${exported_data_files[@]}
}

function getAllLabels () {
    local labels=()

    if [ $2 = true ]
    then
        if [ $1 -gt 7 ]
        then
            dev="dev"
        else
            dev="device"
        fi
    else
        if [ $1 -gt 7 ]
        then
            dev="gw"
        else
            dev="gateway"
        fi
    fi

    # Get all device labels
    for ((i=0;i<$1;i++))
    do
        labels+=(${dev}$i)
    done

    echo ${labels[@]}
}

connected=$(getAllFiles "_connected.csv" $2 false)
gateway_labels=$(getAllLabels $2 false)

python3 plotData.py Connected\ devices\ \(sum\) s -I -o $out_directory/connectedDevicesScattered.pdf -l $gateway_labels -i $connected
python3 plotData.py Connected\ devices\ \(sum\) l -I -o $out_directory/connectedDevicesLine.pdf -l $gateway_labels -i $connected
python3 plotData.py Connected\ devices b -I -o $out_directory/connectedDevicesBar.pdf -g $gateway_labels -i $connected

#python3 plotData.py Connected\ devices\ \(sum\) s -I -o $out_directory/connectedDevicesScattered.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_connected.csv ${data_path}gateways[1]_connected.csv
#python3 plotData.py Connected\ devices\ \(sum\) l -I -o $out_directory/connectedDevicesLine.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_connected.csv ${data_path}gateways[1]_connected.csv
#python3 plotData.py Connected\ devices b -I -o $out_directory/connectedDevicesBar.pdf -g gateway0 gateway1 -i ${data_path}gateways[0]_connected.csv ${data_path}gateways[1]_connected.csv

python3 plotData.py Resources l -I -o $out_directory/resources.pdf -l gateway0\ CPU gateway0\ GPU gateway0\ RAM gateway1\ CPU gateway1\ GPU gateway1\ RAM -i ${data_path}gateways[0]_cpu.csv ${data_path}gateways[0]_gpu.csv ${data_path}gateways[0]_ram.csv ${data_path}gateways[1]_cpu.csv ${data_path}gateways[1]_gpu.csv ${data_path}gateways[1]_ram.csv &
python3 plotData.py Storage l -I -o $out_directory/storage.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_storage.csv ${data_path}gateways[1]_storage.csv &
python3 plotData.py NetworkIO l -I -o $out_directory/networkIO.pdf -l gateway0\ In\(byte/s\) gateway0\ Out\(byte/s\) gateway1\ In\(byte/s\) gateway1\ Out\(byte/s\) -i ${data_path}gateways[0]_networkIn.csv ${data_path}gateways[0]_networkOut.csv ${data_path}gateways[1]_networkIn.csv ${data_path}gateways[1]_networkOut.csv &


python3 plotData.py Sent\ messages\ \(frequency\) s -I -o $out_directory/messagesSent.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageSent.csv ${data_path}gateways[1]_messageSent.csv &
python3 plotData.py Sent\ messages\ \(sum\) s -I -o $out_directory/messagesSentCountScattered.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageSentCount.csv ${data_path}gateways[1]_messageSentCount.csv &
python3 plotData.py Sent\ messages\ \(sum\) l -I -o $out_directory/messagesSentCountLine.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageSentCount.csv ${data_path}gateways[1]_messageSentCount.csv &

python3 plotData.py Sent\ messages\ \(frequency\) s -I -o $out_directory/messagesSentLoRaIp.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageSentLoRa.csv ${data_path}gateways[0]_messageSentIp.csv ${data_path}gateways[1]_messageSentLoRa.csv ${data_path}gateways[1]_messageSentIp.csv &
python3 plotData.py Sent\ messages\ \(sum\) s -I -o $out_directory/messagesSentLoRaIpCountScattered.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageSentLoRaCount.csv ${data_path}gateways[0]_messageSentIpCount.csv ${data_path}gateways[1]_messageSentLoRaCount.csv ${data_path}gateways[1]_messageSentIpCount.csv &
python3 plotData.py Sent\ messages\ \(sum\) l -I -o $out_directory/messagesSentLoRaIpCountLine.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageSentLoRaCount.csv ${data_path}gateways[0]_messageSentIpCount.csv ${data_path}gateways[1]_messageSentLoRaCount.csv ${data_path}gateways[1]_messageSentIpCount.csv &

python3 plotData.py Sent\ messages\ \(frequency\) s -I -o $out_directory/messagesSentLoRa.pdf -l gateway0\ LoRa\ gateways gateway0\ LoRa\ devices gateway1\ LoRa\ gateways gateway1\ LoRa\ devices -i ${data_path}gateways[0]_messageSentLoRaGateways.csv ${data_path}gateways[0]_messageSentLoRaDevices.csv ${data_path}gateways[1]_messageSentLoRaGateways.csv ${data_path}gateways[1]_messageSentLoRaDevices.csv &
python3 plotData.py Sent\ messages\ \(sum\) s -I -o $out_directory/messagesSentLoRaCountScattered.pdf -l gateway0\ LoRa\ gateways gateway0\ LoRa\ devices gateway1\ LoRa\ gateways gateway1\ LoRa\ devices -i ${data_path}gateways[0]_messageSentLoRaGatewaysCount.csv ${data_path}gateways[0]_messageSentLoRaDevicesCount.csv ${data_path}gateways[1]_messageSentLoRaGatewaysCount.csv ${data_path}gateways[1]_messageSentLoRaDevicesCount.csv &
python3 plotData.py Sent\ messages\ \(sum\) l -I -o $out_directory/messagesSentLoRaCountLine.pdf -l gateway0\ LoRa\ gateways gateway0\ LoRa\ devices gateway1\ LoRa\ gateways gateway1\ LoRa\ devices -i ${data_path}gateways[0]_messageSentLoRaGatewaysCount.csv ${data_path}gateways[0]_messageSentLoRaDevicesCount.csv ${data_path}gateways[1]_messageSentLoRaGatewaysCount.csv ${data_path}gateways[1]_messageSentLoRaDevicesCount.csv &

python3 plotData.py Sent\ messages\ \(frequency\) s -I -o $out_directory/messagesSentIp.pdf -l gateway0\ IP\ gateways gateway0\ IP\ server gateway1\ IP\ gateways gateway1\ IP\ server -i ${data_path}gateways[0]_messageSentIpGateways.csv ${data_path}gateways[0]_messageSentIpServer.csv ${data_path}gateways[1]_messageSentIpGateways.csv ${data_path}gateways[1]_messageSentIpServer.csv &
python3 plotData.py Sent\ messages\ \(sum\) s -I -o $out_directory/messagesSentIpCountScattered.pdf -l gateway0\ IP\ gateways gateway0\ IP\ server gateway1\ IP\ gateways gateway1\ IP\ server -i ${data_path}gateways[0]_messageSentIpGatewaysCount.csv ${data_path}gateways[0]_messageSentIpServerCount.csv ${data_path}gateways[1]_messageSentIpGatewaysCount.csv ${data_path}gateways[1]_messageSentIpServerCount.csv &
python3 plotData.py Sent\ messages\ \(sum\) l -I -o $out_directory/messagesSentIpCountLine.pdf -l gateway0\ IP\ gateways gateway0\ IP\ server gateway1\ IP\ gateways gateway1\ IP\ server -i ${data_path}gateways[0]_messageSentIpGatewaysCount.csv ${data_path}gateways[0]_messageSentIpServerCount.csv ${data_path}gateways[1]_messageSentIpGatewaysCount.csv ${data_path}gateways[1]_messageSentIpServerCount.csv &


python3 plotData.py Received\ messages\ \(frequency\) s -I -o $out_directory/messagesReceived.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageReceived.csv ${data_path}gateways[1]_messageReceived.csv &
python3 plotData.py Received\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedCountScattered.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[1]_messageReceivedCount.csv &
python3 plotData.py Received\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedCountLine.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[1]_messageReceivedCount.csv &

python3 plotData.py Received\ messages\ \(frequency\) s -I -o $out_directory/messagesReceivedLoRaIp.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageReceivedLoRa.csv ${data_path}gateways[0]_messageReceivedIp.csv ${data_path}gateways[1]_messageReceivedLoRa.csv ${data_path}gateways[1]_messageReceivedIp.csv &
python3 plotData.py Received\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedLoRaIpCountScattered.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[1]_messageReceivedLoRaCount.csv ${data_path}gateways[1]_messageReceivedIpCount.csv &
python3 plotData.py Received\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedLoRaIpCountLine.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[1]_messageReceivedLoRaCount.csv ${data_path}gateways[1]_messageReceivedIpCount.csv &

python3 plotData.py Received\ messages\ \(frequency\) s -I -o $out_directory/messagesReceivedLoRa.pdf -l gateway0\ LoRa\ gateways gateway0\ LoRa\ devices gateway1\ LoRa\ gateways gateway1\ LoRa\ devices -i ${data_path}gateways[0]_messageReceivedLoRaGateways.csv ${data_path}gateways[0]_messageReceivedLoRaDevices.csv ${data_path}gateways[1]_messageReceivedLoRaGateways.csv ${data_path}gateways[1]_messageReceivedLoRaDevices.csv &
python3 plotData.py Received\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedLoRaCountScattered.pdf -l gateway0\ LoRa\ gateways gateway0\ LoRa\ devices gateway1\ LoRa\ gateways gateway1\ LoRa\ devices -i ${data_path}gateways[0]_messageReceivedLoRaGatewaysCount.csv ${data_path}gateways[0]_messageReceivedLoRaDevicesCount.csv ${data_path}gateways[1]_messageReceivedLoRaGatewaysCount.csv ${data_path}gateways[1]_messageReceivedLoRaDevicesCount.csv &
python3 plotData.py Received\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedLoRaCountLine.pdf -l gateway0\ LoRa\ gateways gateway0\ LoRa\ devices gateway1\ LoRa\ gateways gateway1\ LoRa\ devices -i ${data_path}gateways[0]_messageReceivedLoRaGatewaysCount.csv ${data_path}gateways[0]_messageReceivedLoRaDevicesCount.csv ${data_path}gateways[1]_messageReceivedLoRaGatewaysCount.csv ${data_path}gateways[1]_messageReceivedLoRaDevicesCount.csv &

python3 plotData.py Received\ messages\ \(frequency\) s -I -o $out_directory/messagesReceivedIp.pdf -l gateway0\ IP\ gateways gateway0\ IP\ server gateway1\ IP\ gateways gateway1\ IP\ server -i ${data_path}gateways[0]_messageReceivedIpGateways.csv ${data_path}gateways[0]_messageReceivedIpServer.csv ${data_path}gateways[1]_messageReceivedIpGateways.csv ${data_path}gateways[1]_messageReceivedIpServer.csv &
python3 plotData.py Received\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedIpCountScattered.pdf -l gateway0\ IP\ gateways gateway0\ IP\ server gateway1\ IP\ gateways gateway1\ IP\ server -i ${data_path}gateways[0]_messageReceivedIpGatewaysCount.csv ${data_path}gateways[0]_messageReceivedIpServerCount.csv ${data_path}gateways[1]_messageReceivedIpGatewaysCount.csv ${data_path}gateways[1]_messageReceivedIpServerCount.csv &
python3 plotData.py Received\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedIpCountLine.pdf -l gateway0\ IP\ gateways gateway0\ IP\ server gateway1\ IP\ gateways gateway1\ IP\ server -i ${data_path}gateways[0]_messageReceivedIpGatewaysCount.csv ${data_path}gateways[0]_messageReceivedIpServerCount.csv ${data_path}gateways[1]_messageReceivedIpGatewaysCount.csv ${data_path}gateways[1]_messageReceivedIpServerCount.csv &


python3 plotData.py Sent\ and\ Received\ messages\ \(frequency\) s -I -o $out_directory/messagesSentAndReceived.pdf -l gateway0\ sent gateway0\ received gateway1\ sent gateway1\ received -i ${data_path}gateways[0]_messageSent.csv ${data_path}gateways[0]_messageReceived.csv ${data_path}gateways[1]_messageSent.csv ${data_path}gateways[1]_messageReceived.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) s -I -o $out_directory/messagesSentAndReceivedCountScattered.pdf -l gateway0\ sent gateway0\ received gateway1\ sent gateway1\ received -i ${data_path}gateways[0]_messageSentCount.csv ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[1]_messageSentCount.csv ${data_path}gateways[1]_messageReceivedCount.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) l -I -o $out_directory/messagesSentAndReceivedCountLine.pdf -l gateway0\ sent gateway0\ received gateway1\ sent gateway1\ received -i ${data_path}gateways[0]_messageSentCount.csv ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[1]_messageSentCount.csv ${data_path}gateways[1]_messageReceivedCount.csv &

python3 plotData.py Sent\ and\ Received\ messages\ \(frequency\) s -I -o $out_directory/messagesSentAndReceivedLoRa.pdf -l gateway0\ sent\ LoRa gateway0\ received\ LoRa gateway1\ sent\ LoRa gateway1\ received\ LoRa -i ${data_path}gateways[0]_messageSentLoRa.csv ${data_path}gateways[0]_messageReceivedLoRa.csv ${data_path}gateways[1]_messageSentLoRa.csv ${data_path}gateways[1]_messageReceivedLoRa.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) s -I -o $out_directory/messagesSentAndReceivedLoRaCountScattered.pdf -l gateway0\ sent\ LoRa gateway0\ received\ LoRa gateway1\ sent\ LoRa gateway1\ received\ LoRa -i ${data_path}gateways[0]_messageSentLoRaCount.csv ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[1]_messageSentLoRaCount.csv ${data_path}gateways[1]_messageReceivedLoRaCount.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) l -I -o $out_directory/messagesSentAndReceivedLoRaCountLine.pdf -l gateway0\ sent\ LoRa gateway0\ received\ LoRa gateway1\ sent\ LoRa gateway1\ received\ LoRa -i ${data_path}gateways[0]_messageSentLoRaCount.csv ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[1]_messageSentLoRaCount.csv ${data_path}gateways[1]_messageReceivedLoRaCount.csv &

python3 plotData.py Sent\ and\ Received\ messages\ \(frequency\) s -I -o $out_directory/messagesSentAndReceivedIp.pdf -l gateway0\ sent\ IP gateway0\ received\ IP gateway1\ sent\ IP gateway1\ received\ IP -i ${data_path}gateways[0]_messageSentIp.csv ${data_path}gateways[0]_messageReceivedIp.csv ${data_path}gateways[1]_messageSentIp.csv ${data_path}gateways[1]_messageReceivedIp.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) s -I -o $out_directory/messagesSentAndReceivedIpCountScattered.pdf -l gateway0\ sent\ IP gateway0\ received\ IP gateway1\ sent\ IP gateway1\ received\ IP -i ${data_path}gateways[0]_messageSentIpCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[1]_messageSentIpCount.csv ${data_path}gateways[1]_messageReceivedIpCount.csv &
python3 plotData.py Sent\ and\ Received\ messages\ \(sum\) l -I -o $out_directory/messagesSentAndReceivedIpCountLine.pdf -l gateway0\ sent\ IP gateway0\ received\ IP gateway1\ sent\ IP gateway1\ received\ IP -i ${data_path}gateways[0]_messageSentIpCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[1]_messageSentIpCount.csv ${data_path}gateways[1]_messageReceivedIpCount.csv &

python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedBar.pdf -l sent received -g gateway0 gateway1 -i ${data_path}gateways[0]_messageSentCount.csv ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[1]_messageSentCount.csv ${data_path}gateways[1]_messageReceivedCount.csv &
#python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedLoRaBar.pdf -l sent\ LoRa received\ LoRa -g gateway0 gateway1 -i ${data_path}gateways[0]_messageSentLoRaCount.csv ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[1]_messageSentLoRaCount.csv ${data_path}gateways[1]_messageReceivedLoRaCount.csv &
#python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedIpBar.pdf -l sent\ IP received\ IP -g gateway0 gateway1 -i ${data_path}gateways[0]_messageSentIpCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[1]_messageSentIpCount.csv ${data_path}gateways[1]_messageReceivedIpCount.csv &
python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedLoRaBar.pdf -l sent\ LoRa\ gateways received\ LoRa\ gateways sent\ LoRa\ devices received\ LoRa\ devices -g gateway0 gateway1 -i ${data_path}gateways[0]_messageSentLoRaGatewaysCount.csv ${data_path}gateways[0]_messageReceivedLoRaGatewaysCount.csv ${data_path}gateways[0]_messageSentLoRaDevicesCount.csv ${data_path}gateways[0]_messageReceivedLoRaDevicesCount.csv ${data_path}gateways[1]_messageSentLoRaGatewaysCount.csv ${data_path}gateways[1]_messageReceivedLoRaGatewaysCount.csv ${data_path}gateways[1]_messageSentLoRaDevicesCount.csv ${data_path}gateways[1]_messageReceivedLoRaDevicesCount.csv &
python3 plotData.py Sent\ and\ Received\ messages b -I -o $out_directory/messagesSentAndReceivedIpBar.pdf -l sent\ IP\ gateways received\ IP\ gateways sent\ IP\ server received\ IP\ server -g gateway0 gateway1 -i ${data_path}gateways[0]_messageSentIpGatewaysCount.csv ${data_path}gateways[0]_messageReceivedIpGatewaysCount.csv ${data_path}gateways[0]_messageSentIpServerCount.csv ${data_path}gateways[0]_messageReceivedIpServerCount.csv ${data_path}gateways[1]_messageSentIpGatewaysCount.csv ${data_path}gateways[1]_messageReceivedIpGatewaysCount.csv ${data_path}gateways[1]_messageSentIpServerCount.csv ${data_path}gateways[1]_messageReceivedIpServerCount.csv &


# Enable ignore option when multiple input files are used and an input can miss (here lost messages)
python3 plotData.py Lost\ messages\ \(frequency\) s -I -o $out_directory/messagesLost.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageLost.csv ${data_path}gateways[1]_messageLost.csv &
python3 plotData.py Lost\ messages\ \(sum\) s -I -o $out_directory/messagesLostCountScattered.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageLostCount.csv ${data_path}gateways[1]_messageLostCount.csv &
python3 plotData.py Lost\ messages\ \(sum\) l -I -o $out_directory/messagesLostCountLine.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageLostCount.csv ${data_path}gateways[1]_messageLostCount.csv &

python3 plotData.py Lost\ messages\ \(frequency\) s -I -o $out_directory/messagesLostLoRaIp.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageLostLoRa.csv ${data_path}gateways[0]_messageLostIp.csv ${data_path}gateways[1]_messageLostLoRa.csv ${data_path}gateways[1]_messageLostIp.csv &
python3 plotData.py Lost\ messages\ \(sum\) s -I -o $out_directory/messagesLostLoRaIpCountScattered.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[0]_messageLostIpCount.csv ${data_path}gateways[1]_messageLostLoRaCount.csv ${data_path}gateways[1]_messageLostIpCount.csv &
python3 plotData.py Lost\ messages\ \(sum\) l -I -o $out_directory/messagesLostLoRaIpCountLine.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[0]_messageLostIpCount.csv ${data_path}gateways[1]_messageLostLoRaCount.csv ${data_path}gateways[1]_messageLostIpCount.csv &

python3 plotData.py Lost\ messages\ \(frequency\) s -o $out_directory/messagesLostLoRa.pdf -l gateway0\ LoRa\ gateways gateway0\ LoRa\ devices gateway1\ LoRa\ gateways gateway1\ LoRa\ devices -i ${data_path}gateways[0]_messageLostLoRaGateways.csv ${data_path}gateways[0]_messageLostLoRaDevices.csv ${data_path}gateways[1]_messageLostLoRaGateways.csv ${data_path}gateways[1]_messageLostLoRaDevices.csv &
python3 plotData.py Lost\ messages\ \(sum\) s -o $out_directory/messagesLostLoRaCountScattered.pdf -l gateway0\ LoRa\ gateways gateway0\ LoRa\ devices gateway1\ LoRa\ gateways gateway1\ LoRa\ devices -i ${data_path}gateways[0]_messageLostLoRaGatewaysCount.csv ${data_path}gateways[0]_messageLostLoRaDevicesCount.csv ${data_path}gateways[1]_messageLostLoRaGatewaysCount.csv ${data_path}gateways[1]_messageLostLoRaDevicesCount.csv &
python3 plotData.py Lost\ messages\ \(sum\) l -o $out_directory/messagesLostLoRaCountLine.pdf -l gateway0\ LoRa\ gateways gateway0\ LoRa\ devices gateway1\ LoRa\ gateways gateway1\ LoRa\ devices -i ${data_path}gateways[0]_messageLostLoRaGatewaysCount.csv ${data_path}gateways[0]_messageLostLoRaDevicesCount.csv ${data_path}gateways[1]_messageLostLoRaGatewaysCount.csv ${data_path}gateways[1]_messageLostLoRaDevicesCount.csv &

python3 plotData.py Lost\ messages\ \(frequency\) s -o $out_directory/messagesLostIp.pdf -l gateway0\ IP\ gateways gateway0\ IP\ server gateway1\ IP\ gateways gateway1\ IP\ server -i ${data_path}gateways[0]_messageLostIpGateways.csv ${data_path}gateways[0]_messageLostIpServer.csv ${data_path}gateways[1]_messageLostIpGateways.csv ${data_path}gateways[1]_messageLostIpServer.csv &
python3 plotData.py Lost\ messages\ \(sum\) s -o $out_directory/messagesLostIpCountScattered.pdf -l gateway0\ IP\ gateways gateway0\ IP\ server gateway1\ IP\ gateways gateway1\ IP\ server -i ${data_path}gateways[0]_messageLostIpGatewaysCount.csv ${data_path}gateways[0]_messageLostIpServerCount.csv ${data_path}gateways[1]_messageLostIpGatewaysCount.csv ${data_path}gateways[1]_messageLostIpServerCount.csv &
python3 plotData.py Lost\ messages\ \(sum\) l -o $out_directory/messagesLostIpCountLine.pdf -l gateway0\ IP\ gateways gateway0\ IP\ server gateway1\ IP\ gateways gateway1\ IP\ server -i ${data_path}gateways[0]_messageLostIpGatewaysCount.csv ${data_path}gateways[0]_messageLostIpServerCount.csv ${data_path}gateways[1]_messageLostIpGatewaysCount.csv ${data_path}gateways[1]_messageLostIpServerCount.csv &


python3 plotData.py Received\ and\ Lost\ messages\ \(frequency\) s -I -o $out_directory/messagesReceivedAndLost.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceived.csv ${data_path}gateways[0]_messageLost.csv ${data_path}gateways[1]_messageReceived.csv ${data_path}gateways[1]_messageLost.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedAndLostCountScattered.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[0]_messageLostCount.csv ${data_path}gateways[1]_messageReceivedCount.csv ${data_path}gateways[1]_messageLostCount.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedAndLostCountLine.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[0]_messageLostCount.csv ${data_path}gateways[1]_messageReceivedCount.csv ${data_path}gateways[1]_messageLostCount.csv &

python3 plotData.py Received\ and\ Lost\ messages\ \(frequency\) s -I -o $out_directory/messagesReceivedAndLostLoRa.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceivedLoRa.csv ${data_path}gateways[0]_messageLostLoRa.csv ${data_path}gateways[1]_messageReceivedLoRa.csv ${data_path}gateways[1]_messageLostLoRa.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedAndLostLoRaCountScattered.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[1]_messageReceivedLoRaCount.csv ${data_path}gateways[1]_messageLostLoRaCount.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedAndLostLoRaCountLine.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[1]_messageReceivedLoRaCount.csv ${data_path}gateways[1]_messageLostLoRaCount.csv &

python3 plotData.py Received\ and\ Lost\ messages\ \(frequency\) s -I -o $out_directory/messagesReceivedAndLostIp.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceivedIp.csv ${data_path}gateways[0]_messageLostIp.csv ${data_path}gateways[1]_messageReceivedIp.csv ${data_path}gateways[1]_messageLostIp.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedAndLostIpCountScattered.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[0]_messageLostIpCount.csv ${data_path}gateways[1]_messageReceivedIpCount.csv ${data_path}gateways[1]_messageLostIpCount.csv &
python3 plotData.py Received\ and\ Lost\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedAndLostIpCountLine.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[0]_messageLostIpCount.csv ${data_path}gateways[1]_messageReceivedIpCount.csv ${data_path}gateways[1]_messageLostIpCount.csv &

python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostBar.pdf -l received lost -g gateway0 gateway1 -i ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[0]_messageLostCount.csv ${data_path}gateways[1]_messageReceivedCount.csv ${data_path}gateways[1]_messageLostCount.csv &
#python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostLoRaBar.pdf -l received\ LoRa lost\ LoRa -g gateway0 gateway1 -i ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[1]_messageReceivedLoRaCount.csv ${data_path}gateways[1]_messageLostLoRaCount.csv &
#python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostIpBar.pdf -l received\ IP lost\ IP -g gateway0 gateway1 -i ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[0]_messageLostIpCount.csv ${data_path}gateways[1]_messageReceivedIpCount.csv ${data_path}gateways[1]_messageLostIpCount.csv &
python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostLoRaBar.pdf -l received\ LoRa\ gateways lost\ LoRa\ gateways received\ LoRa\ devices lost\ LoRa\ devices -g gateway0 gateway1 -i ${data_path}gateways[0]_messageReceivedLoRaGatewaysCount.csv ${data_path}gateways[0]_messageLostLoRaGatewaysCount.csv ${data_path}gateways[0]_messageReceivedLoRaDevicesCount.csv ${data_path}gateways[0]_messageLostLoRaDevicesCount.csv ${data_path}gateways[1]_messageReceivedLoRaGatewaysCount.csv ${data_path}gateways[1]_messageLostLoRaGatewaysCount.csv ${data_path}gateways[1]_messageReceivedLoRaDevicesCount.csv ${data_path}gateways[1]_messageLostLoRaDevicesCount.csv &
python3 plotData.py Received\ and\ Lost\ messages b -I -o $out_directory/messagesReceivedAndLostIpBar.pdf -l received\ IP\ gateways lost\ IP\ gateways received\ IP\ server lost\ IP\ server -g gateway0 gateway1 -i ${data_path}gateways[0]_messageReceivedIpGatewaysCount.csv ${data_path}gateways[0]_messageLostIpGatewaysCount.csv ${data_path}gateways[0]_messageReceivedIpServerCount.csv ${data_path}gateways[0]_messageLostIpServerCount.csv ${data_path}gateways[1]_messageReceivedIpGatewaysCount.csv ${data_path}gateways[1]_messageLostIpGatewaysCount.csv ${data_path}gateways[1]_messageReceivedIpServerCount.csv ${data_path}gateways[1]_messageLostIpServerCount.csv &


python3 plotData.py In\ messages\ \(sum\) s -o $out_directory/messagesInCountScattered.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[1]_messageInCount.csv &
python3 plotData.py In\ messages\ \(sum\) l -o $out_directory/messagesInCountLine.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[1]_messageInCount.csv &

python3 plotData.py In\ messages\ \(sum\) s -o $out_directory/messagesInLoRaIpCountScattered.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageInIpCount.csv ${data_path}gateways[1]_messageInLoRaCount.csv ${data_path}gateways[1]_messageInIpCount.csv &
python3 plotData.py In\ messages\ \(sum\) l -o $out_directory/messagesInLoRaIpCountLine.pdf -l gateway0\ LoRa gateway0\ IP gateway1\ LoRa gateway1\ IP -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageInIpCount.csv ${data_path}gateways[1]_messageInLoRaCount.csv ${data_path}gateways[1]_messageInIpCount.csv &

python3 plotData.py In\ and\ Received\ messages b -I -o $out_directory/messagesInAndReceivedBar.pdf -l \in received -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[0]_messageReceivedCount.csv ${data_path}gateways[1]_messageInCount.csv ${data_path}gateways[1]_messageReceivedCount.csv &
python3 plotData.py In\ and\ Received\ messages b -I -o $out_directory/messagesInAndReceivedLoRaBar.pdf -l \in\ LoRa received\ LoRa -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageReceivedLoRaCount.csv ${data_path}gateways[1]_messageInLoRaCount.csv ${data_path}gateways[1]_messageReceivedLoRaCount.csv &
python3 plotData.py In\ and\ Received\ messages b -I -o $out_directory/messagesInAndReceivedIpBar.pdf -l \in\ IP received\ IP -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInIpCount.csv ${data_path}gateways[0]_messageReceivedIpCount.csv ${data_path}gateways[1]_messageInIpCount.csv ${data_path}gateways[1]_messageReceivedIpCount.csv &

python3 plotData.py In\ and\ Lost\ messages b -I -o $out_directory/messagesInAndLostBar.pdf -l \in lost -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[0]_messageLostCount.csv ${data_path}gateways[1]_messageInCount.csv ${data_path}gateways[1]_messageLostCount.csv &
python3 plotData.py In\ and\ Lost\ messages b -I -o $out_directory/messagesInAndLostLoRaBar.pdf -l \in\ LoRa lost\ LoRa -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageLostLoRaCount.csv ${data_path}gateways[1]_messageInLoRaCount.csv ${data_path}gateways[1]_messageLostLoRaCount.csv &
python3 plotData.py In\ and\ Lost\ messages b -I -o $out_directory/messagesInAndLostIpBar.pdf -l \in\ IP lost\ IP -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInIpCount.csv ${data_path}gateways[0]_messageLostIpCount.csv ${data_path}gateways[1]_messageInIpCount.csv ${data_path}gateways[1]_messageLostIpCount.csv &

python3 plotData.py In\ and\ Invalid\ messages b -I -o $out_directory/messagesInAndInvalidBar.pdf -l \in invalid -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[0]_messageInvalidCount.csv ${data_path}gateways[1]_messageInCount.csv ${data_path}gateways[1]_messageInvalidCount.csv &
python3 plotData.py In\ and\ Invalid\ messages b -I -o $out_directory/messagesInAndInvalidLoRaBar.pdf -l \in\ LoRa invalid\ LoRa -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInLoRaCount.csv ${data_path}gateways[0]_messageInvalidLoRaCount.csv ${data_path}gateways[1]_messageInLoRaCount.csv ${data_path}gateways[1]_messageInvalidLoRaCount.csv &
python3 plotData.py In\ and\ Invalid\ messages b -I -o $out_directory/messagesInAndInvalidIpBar.pdf -l \in\ IP invalid\ IP -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInIpCount.csv ${data_path}gateways[0]_messageInvalidIpCount.csv ${data_path}gateways[1]_messageInIpCount.csv ${data_path}gateways[1]_messageInvalidIpCount.csv &


# Do not enable ignore option when a single input file does not exist (interferences may not exist) because is useless to generate an empty graph
python3 plotData.py Interferences\ \(frequency\) s -o $out_directory/interferences.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_interference.csv ${data_path}gateways[1]_interference.csv &
python3 plotData.py Interferences\ \(sum\) s -o $out_directory/interferencesCountScattered.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_interferenceCount.csv ${data_path}gateways[1]_interferenceCount.csv &
python3 plotData.py Interferences\ \(sum\) l -o $out_directory/interferencesCountLine.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_interferenceCount.csv ${data_path}gateways[1]_interferenceCount.csv &

python3 plotData.py Possible\ interferences\ and\ interferences\ \(frequency\) s -I -o $out_directory/possibleInterferencesAndInterferences.pdf -l gateway0\ possible\ interferences gateway0\ interferences gateway1\ possible\ interferences gateway1\ interferences -i ${data_path}gateways[0]_interferencePossible.csv ${data_path}gateways[0]_interference.csv ${data_path}gateways[1]_interferencePossible.csv ${data_path}gateways[1]_interference.csv &
python3 plotData.py Possible\ interferences\ and\ interferences\ \(sum\) s -I -o $out_directory/possibleInterferencesAndInterferencesCountScattered.pdf -l gateway0\ possible\ interferences gateway0\ interferences gateway1\ possible\ interferences gateway1\ interferences -i ${data_path}gateways[0]_interferencePossibleCount.csv ${data_path}gateways[0]_interferenceCount.csv ${data_path}gateways[1]_interferencePossibleCount.csv ${data_path}gateways[1]_interferenceCount.csv &
python3 plotData.py Possible\ interferences\ and\ interferences\ \(sum\) l -I -o $out_directory/possibleInterferencesAndInterferencesCountLine.pdf  -l gateway0\ possible\ interferences gateway0\ interferences gateway1\ possible\ interferences gateway1\ interferences -i ${data_path}gateways[0]_interferencePossibleCount.csv ${data_path}gateways[0]_interferenceCount.csv ${data_path}gateways[1]_interferencePossibleCount.csv ${data_path}gateways[1]_interferenceCount.csv &

python3 plotData.py In\ messages\ and\ interferences b -I -o $out_directory/messagesInAndInterferencesBar.pdf -l \in interferences -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInCount.csv ${data_path}gateways[0]_interferenceCount.csv ${data_path}gateways[1]_messageInCount.csv ${data_path}gateways[1]_interferenceCount.csv &
python3 plotData.py Possible\ interferences\ and\ interferences b -I -o $out_directory/possibleInterferencesAndInterferencesBar.pdf -l possible\ interferences interferences -g gateway0 gateway1 -i ${data_path}gateways[0]_interferencePossibleCount.csv ${data_path}gateways[0]_interferenceCount.csv ${data_path}gateways[1]_interferencePossibleCount.csv ${data_path}gateways[1]_interferenceCount.csv &


python3 plotData.py RSSIs h -o $out_directory/RSSIs.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_rssi.csv ${data_path}gateways[1]_rssi.csv &
#==============================

# Wait parallel executions end
wait

#printf "\nAll charts are plotted. Bye :)\n"



#++++++++++++++++++++++++++++++ DATA ++++++++++++++++++++++++++++++
#==== STATS ABOUT A SINGLE DEVICE ====
out_directory="${data_path}single_device/"
echo "Plotting data into $out_directory"

python3 plotData.py Sent\ data\ messages\ \(frequency\) s -o $out_directory/messageSentData.pdf -i ${data_path}endDevices[0]_messageSentData.csv &
python3 plotData.py Sent\ data\ messages\ \(sum\) s -o $out_directory/messageSentDataCountScattered.pdf -i ${data_path}endDevices[0]_messageSentDataCount.csv &
python3 plotData.py Sent\ data\ messages\ \(sum\) l -o $out_directory/messageSentDataCountLine.pdf -i ${data_path}endDevices[0]_messageSentDataCount.csv &

# These automatically plot charts with min, max and mean
python3 plotData.py Sent\ data\ messages b -I -o $out_directory/messagesSentDataBar.pdf -l sent -g device -i ${data_path}endDevices[0]_messageSentDataCount.csv &
#======================================



#==== STATS ABOUT MULTIPLE DEVICES ====
out_directory="${data_path}multiple_devices/"
echo "Plotting data into $out_directory"

python3 plotData.py Sent\ messages\ \(frequency\) s -I -o $out_directory/messagesSentData.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageSentData.csv ${data_path}endDevices[1]_messageSentData.csv ${data_path}endDevices[2]_messageSentData.csv ${data_path}endDevices[3]_messageSentData.csv ${data_path}endDevices[4]_messageSentData.csv &
python3 plotData.py Sent\ messages\ \(sum\) s -I -o $out_directory/messagesSentDataCountScattered.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageSentDataCount.csv ${data_path}endDevices[1]_messageSentDataCount.csv ${data_path}endDevices[2]_messageSentDataCount.csv ${data_path}endDevices[3]_messageSentDataCount.csv ${data_path}endDevices[4]_messageSentDataCount.csv &
python3 plotData.py Sent\ messages\ \(sum\) l -I -o $out_directory/messagesSentDataCountLine.pdf -l device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageSentDataCount.csv ${data_path}endDevices[1]_messageSentDataCount.csv ${data_path}endDevices[2]_messageSentDataCount.csv ${data_path}endDevices[3]_messageSentDataCount.csv ${data_path}endDevices[4]_messageSentDataCount.csv &

# These automatically plot charts with min, max and mean
python3 plotData.py Sent\ data\ messages b -I -o $out_directory/messagesSentDataBar.pdf -l sent -g device0 device1 device2 device3 device4 -i ${data_path}endDevices[0]_messageSentDataCount.csv ${data_path}endDevices[1]_messageSentDataCount.csv ${data_path}endDevices[2]_messageSentDataCount.csv ${data_path}endDevices[3]_messageSentDataCount.csv ${data_path}endDevices[4]_messageSentDataCount.csv &
#======================================

wait

#==== STATS ABOUT A SINGLE GATEWAY ====
out_directory="${data_path}single_gateway/"
echo "Plotting data into $out_directory"

python3 plotData.py Sent\ data\ messages\ \(frequency\) s -o $out_directory/messagesSentData.pdf -i ${data_path}gateways[0]_messageSentData.csv &
python3 plotData.py Sent\ data\ messages\ \(sum\) s -o $out_directory/messagesSentDataCountScattered.pdf -i ${data_path}gateways[0]_messageSentDataCount.csv &
python3 plotData.py Sent\ data\ messages\ \(sum\) l -o $out_directory/messagesSentDataCountLine.pdf -i ${data_path}gateways[0]_messageSentDataCount.csv &

python3 plotData.py Sent\ data\ messages\ \(frequency\) s -o $out_directory/messagesSentDataIp.pdf -l IP\ gateways IP\ server -i ${data_path}gateways[0]_messageSentGatewaysData.csv ${data_path}gateways[0]_messageSentServerData.csv &
python3 plotData.py Sent\ data\ messages\ \(sum\) s -o $out_directory/messagesSentDataIpCountScattered.pdf -l IP\ gateways IP\ server -i ${data_path}gateways[0]_messageSentGatewaysDataCount.csv ${data_path}gateways[0]_messageSentServerDataCount.csv &
python3 plotData.py Sent\ data\ messages\ \(sum\) l -o $out_directory/messagesSentDataIpCountLine.pdf -l IP\ gateways IP\ server -i ${data_path}gateways[0]_messageSentGatewaysDataCount.csv ${data_path}gateways[0]_messageSentServerDataCount.csv &


python3 plotData.py Received\ data\ messages\ \(frequency\) s -o $out_directory/messagesReceivedData.pdf -i ${data_path}gateways[0]_messageReceivedData.csv &
python3 plotData.py Received\ data\ messages\ \(sum\) s -o $out_directory/messagesReceivedDataCountScattered.pdf -i ${data_path}gateways[0]_messageReceivedDataCount.csv &
python3 plotData.py Received\ data\ messages\ \(sum\) l -o $out_directory/messagesReceivedDataCountLine.pdf -i ${data_path}gateways[0]_messageReceivedDataCount.csv &

python3 plotData.py Received\ data\ messages\ \(frequency\) s -o $out_directory/messagesReceivedDataLoRaIp.pdf -l LoRa\ devices IP\ gateways IP\ server -i ${data_path}gateways[0]_messageReceivedDevicesData.csv ${data_path}gateways[0]_messageReceivedGatewaysData.csv ${data_path}gateways[0]_messageReceivedServerData.csv &
python3 plotData.py Received\ data\ messages\ \(sum\) s -o $out_directory/messagesReceivedDataLoRaIpCountScattered.pdf -l LoRa\ devices IP\ gateways IP\ server -i ${data_path}gateways[0]_messageReceivedDevicesDataCount.csv ${data_path}gateways[0]_messageReceivedGatewaysDataCount.csv ${data_path}gateways[0]_messageReceivedServerDataCount.csv &
python3 plotData.py Received\ data\ messages\ \(sum\) l -o $out_directory/messagesReceivedDataLoRaIpCountLine.pdf -l LoRa\ devices IP\ gateways IP\ server -i ${data_path}gateways[0]_messageReceivedDevicesDataCount.csv ${data_path}gateways[0]_messageReceivedGatewaysDataCount.csv ${data_path}gateways[0]_messageReceivedServerDataCount.csv &


python3 plotData.py Sent\ and\ Received\ data\ messages\ \(frequency\) s -I -o $out_directory/messagesSentAndReceivedData.pdf -l sent received -i ${data_path}gateways[0]_messageSentData.csv ${data_path}gateways[0]_messageReceivedData.csv &
python3 plotData.py Sent\ and\ Received\ data\ messages\ \(sum\) s -I -o $out_directory/messagesSentAndReceivedDataCountScattered.pdf -l sent received -i ${data_path}gateways[0]_messageSentDataCount.csv ${data_path}gateways[0]_messageReceivedDataCount.csv &
python3 plotData.py Sent\ and\ Received\ data\ messages\ \(sum\) l -I -o $out_directory/messagesSentAndReceivedDataCountLine.pdf -l sent received -i ${data_path}gateways[0]_messageSentDataCount.csv ${data_path}gateways[0]_messageReceivedDataCount.csv &

python3 plotData.py Sent\ and\ Received\ data\ messages b -I -o $out_directory/messagesSentAndReceivedDataBar.pdf -l sent received -g gateway0 -i ${data_path}gateways[0]_messageSentDataCount.csv ${data_path}gateways[0]_messageReceivedDataCount.csv &


# Do not enable ignore option when a single input file does not exist (lost messages may not exist) because is useless to generate an empty graph
python3 plotData.py Lost\ data\ messages\ \(frequency\) s -o $out_directory/messagesLostData.pdf -i ${data_path}gateways[0]_messageLostData.csv &
python3 plotData.py Lost\ data\ messages\ \(sum\) s -o $out_directory/messagesLostDataCountScattered.pdf -i ${data_path}gateways[0]_messageLostDataCount.csv &
python3 plotData.py Lost\ data\ messages\ \(sum\) l -o $out_directory/messagesLostDataCountLine.pdf -i ${data_path}gateways[0]_messageLostDataCount.csv &

# Enable ignore option when multiple input files are used and an input can miss (here lost messages)
python3 plotData.py Received\ and\ Lost\ data\ messages\ \(frequency\) s -I -o $out_directory/messagesReceivedAndLostData.pdf -l received lost -i ${data_path}gateways[0]_messageReceivedData.csv ${data_path}gateways[0]_messageLostData.csv &
python3 plotData.py Received\ and\ Lost\ data\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedAndLostDataCountScattered.pdf -l received lost -i ${data_path}gateways[0]_messageReceivedDataCount.csv ${data_path}gateways[0]_messageLostDataCount.csv &
python3 plotData.py Received\ and\ Lost\ data\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedAndLostDataCountLine.pdf -l received lost -i ${data_path}gateways[0]_messageReceivedDataCount.csv ${data_path}gateways[0]_messageLostDataCount.csv &

python3 plotData.py Received\ and\ Lost\ data\ messages b -I -o $out_directory/messagesReceivedAndLostDataBar.pdf -l received lost -g gateway0 -i ${data_path}gateways[0]_messageReceivedDataCount.csv ${data_path}gateways[0]_messageLostDataCount.csv &


python3 plotData.py In\ data\ messages\ \(sum\) s -o $out_directory/messagesInDataCountScattered.pdf -i ${data_path}gateways[0]_messageInDataCount.csv &
python3 plotData.py In\ data\ messages\ \(sum\) l -o $out_directory/messagesInDataCountLine.pdf -i ${data_path}gateways[0]_messageInDataCount.csv &

python3 plotData.py In\ and\ Received\ data\ messages b -I -o $out_directory/messagesInAndReceivedDataBar.pdf -l \in received -g gateway0 -i ${data_path}gateways[0]_messageInDataCount.csv ${data_path}gateways[0]_messageReceivedDataCount.csv &
python3 plotData.py In\ and\ Received\ data\ messages b -I -o $out_directory/messagesInAndReceivedLoRaIpBar.pdf -l \in received\ LoRa\ devices received\ IP\ gateways received\ IP\ server -g gateway0 -i ${data_path}gateways[0]_messageInDataCount.csv ${data_path}gateways[0]_messageReceivedDevicesDataCount.csv ${data_path}gateways[0]_messageReceivedGatewaysDataCount.csv ${data_path}gateways[0]_messageReceivedServerDataCount.csv &

python3 plotData.py In\ and\ Lost\ data\ messages b -I -o $out_directory/messagesInAndLostDataBar.pdf -l \in lost -g gateway0 -i ${data_path}gateways[0]_messageInDataCount.csv ${data_path}gateways[0]_messageLostDataCount.csv &
python3 plotData.py In\ and\ Invalid\ data\ messages b -I -o $out_directory/messagesInAndInvalidDataBar.pdf -l \in invalid -g gateway0 -i ${data_path}gateways[0]_messageInDataCount.csv ${data_path}gateways[0]_messageInvalidDataCount.csv &


# Do not enable ignore option when a single input file does not exist (interferences may not exist) because is useless to generate an empty graph
python3 plotData.py Interferences\ data\ \(frequency\) s -o $out_directory/interferencesData.pdf -i ${data_path}gateways[0]_interferenceData.csv &
python3 plotData.py Interferences\ data\ \(sum\) s -o $out_directory/interferencesDataCountScattered.pdf -i ${data_path}gateways[0]_interferenceDataCount.csv &
python3 plotData.py Interferences\ data\ \(sum\) l -o $out_directory/interferencesDataCountLine.pdf -i ${data_path}gateways[0]_interferenceDataCount.csv &

# Enable ignore option when multiple input files are used and an input can miss
python3 plotData.py Possible\ interferences\ and\ interferences\ data\ \(frequency\) s -I -o $out_directory/possibleInterferencesAndInterferencesData.pdf -l possible\ interferences interferences -i ${data_path}gateways[0]_interferencePossibleData.csv ${data_path}gateways[0]_interferenceData.csv &
python3 plotData.py Possible\ interferences\ and\ interferences\ data\ \(sum\) s -I -o $out_directory/possibleInterferencesAndInterferencesDataCountScattered.pdf -l possible\ interferences interferences -i ${data_path}gateways[0]_interferencePossibleDataCount.csv ${data_path}gateways[0]_interferenceDataCount.csv &
python3 plotData.py Possible\ interferences\ and\ interferences\ data\ \(sum\) l -I -o $out_directory/possibleInterferencesAndInterferencesDataCountLine.pdf -l possible\ interferences interferences -i ${data_path}gateways[0]_interferencePossibleDataCount.csv ${data_path}gateways[0]_interferenceDataCount.csv &

python3 plotData.py In\ messages\ and\ interferences\ data b -I -o $out_directory/messagesInAndInterferencesDataBar.pdf -l \in interferences -g gateway0 -i ${data_path}gateways[0]_messageInDataCount.csv ${data_path}gateways[0]_interferenceDataCount.csv &
python3 plotData.py Possible\ interferences\ and\ interferences\ data b -I -o $out_directory/possibleInterferencesAndInterferencesDataBar.pdf -l possible\ interferences interferences -g gateway0 -i ${data_path}gateways[0]_interferencePossibleDataCount.csv ${data_path}gateways[0]_interferenceDataCount.csv &
#==============================

wait

#=== STATS ABOUT MULTIPLE GATEWAYS ===
out_directory="${data_path}multiple_gateways/"
echo "Plotting data into $out_directory"

python3 plotData.py Sent\ data\ messages\ \(frequency\) s -o $out_directory/messagesSentData.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageSentData.csv ${data_path}gateways[1]_messageSentData.csv &
python3 plotData.py Sent\ data\ messages\ \(sum\) s -o $out_directory/messagesSentDataCountScattered.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageSentDataCount.csv ${data_path}gateways[1]_messageSentDataCount.csv &
python3 plotData.py Sent\ data\ messages\ \(sum\) l -o $out_directory/messagesSentDataCountLine.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageSentDataCount.csv ${data_path}gateways[1]_messageSentDataCount.csv &

python3 plotData.py Sent\ data\ messages\ \(frequency\) s -o $out_directory/messagesSentDataIp.pdf -l gateway0\ IP\ gateways gateway0\ IP\ server gateway1\ IP\ gateways gateway1\ IP\ server -i ${data_path}gateways[0]_messageSentGatewaysData.csv ${data_path}gateways[0]_messageSentServerData.csv ${data_path}gateways[1]_messageSentGatewaysData.csv ${data_path}gateways[1]_messageSentServerData.csv &
python3 plotData.py Sent\ data\ messages\ \(sum\) s -o $out_directory/messagesSentDataIpCountScattered.pdf -l gateway0\ IP\ gateways gateway0\ IP\ server gateway1\ IP\ gateways gateway1\ IP\ server -i ${data_path}gateways[0]_messageSentGatewaysDataCount.csv ${data_path}gateways[0]_messageSentServerDataCount.csv ${data_path}gateways[1]_messageSentGatewaysDataCount.csv ${data_path}gateways[1]_messageSentServerDataCount.csv &
python3 plotData.py Sent\ data\ messages\ \(sum\) l -o $out_directory/messagesSentDataIpCountLine.pdf -l gateway0\ IP\ gateways gateway0\ IP\ server gateway1\ IP\ gateways gateway1\ IP\ server -i ${data_path}gateways[0]_messageSentGatewaysDataCount.csv ${data_path}gateways[0]_messageSentServerDataCount.csv ${data_path}gateways[1]_messageSentGatewaysDataCount.csv ${data_path}gateways[1]_messageSentServerDataCount.csv &


python3 plotData.py Received\ data\ messages\ \(frequency\) s -o $out_directory/messagesReceivedData.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageReceivedData.csv ${data_path}gateways[1]_messageReceivedData.csv &
python3 plotData.py Received\ data\ messages\ \(sum\) s -o $out_directory/messagesReceivedDataCountScattered.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageReceivedDataCount.csv ${data_path}gateways[1]_messageReceivedDataCount.csv &
python3 plotData.py Received\ data\ messages\ \(sum\) l -o $out_directory/messagesReceivedDataCountLine.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageReceivedDataCount.csv ${data_path}gateways[1]_messageReceivedDataCount.csv &

python3 plotData.py Received\ data\ messages\ \(frequency\) s -o $out_directory/messagesReceivedDataLoRaIp.pdf -l gateway0\ LoRa\ devices gateway0\ IP\ gateways gateway0\ IP\ server gateway1\ LoRa\ devices gateway1\ IP\ gateways gateway1\ IP\ server -i ${data_path}gateways[0]_messageReceivedDevicesData.csv ${data_path}gateways[0]_messageReceivedGatewaysData.csv ${data_path}gateways[0]_messageReceivedServerData.csv ${data_path}gateways[1]_messageReceivedDevicesData.csv ${data_path}gateways[1]_messageReceivedGatewaysData.csv ${data_path}gateways[1]_messageReceivedServerData.csv &
python3 plotData.py Received\ data\ messages\ \(sum\) s -o $out_directory/messagesReceivedDataLoRaIpCountScattered.pdf -l gateway0\ LoRa\ devices gateway0\ IP\ gateways gateway0\ IP\ server gateway1\ LoRa\ devices gateway1\ IP\ gateways gateway1\ IP\ server -i ${data_path}gateways[0]_messageReceivedDevicesDataCount.csv ${data_path}gateways[0]_messageReceivedGatewaysDataCount.csv ${data_path}gateways[0]_messageReceivedServerDataCount.csv ${data_path}gateways[1]_messageReceivedDevicesDataCount.csv ${data_path}gateways[1]_messageReceivedGatewaysDataCount.csv ${data_path}gateways[1]_messageReceivedServerDataCount.csv &
python3 plotData.py Received\ data\ messages\ \(sum\) l -o $out_directory/messagesReceivedDataLoRaIpCountLine.pdf -l gateway0\ LoRa\ devices gateway0\ IP\ gateways gateway0\ IP\ server gateway1\ LoRa\ devices gateway1\ IP\ gateways gateway1\ IP\ server -i ${data_path}gateways[0]_messageReceivedDevicesDataCount.csv ${data_path}gateways[0]_messageReceivedGatewaysDataCount.csv ${data_path}gateways[0]_messageReceivedServerDataCount.csv ${data_path}gateways[1]_messageReceivedDevicesDataCount.csv ${data_path}gateways[1]_messageReceivedGatewaysDataCount.csv ${data_path}gateways[1]_messageReceivedServerDataCount.csv &


python3 plotData.py Sent\ and\ Received\ data\ messages\ \(frequency\) s -I -o $out_directory/messagesSentAndReceivedData.pdf -l gateway0\ sent gateway0\ received gateway1\ sent gateway1\ received -i ${data_path}gateways[0]_messageSentData.csv ${data_path}gateways[0]_messageReceivedData.csv ${data_path}gateways[1]_messageSentData.csv ${data_path}gateways[1]_messageReceivedData.csv &
python3 plotData.py Sent\ and\ Received\ data\ messages\ \(sum\) s -I -o $out_directory/messagesSentAndReceivedDataCountScattered.pdf -l gateway0\ sent gateway0\ received gateway1\ sent gateway1\ received -i ${data_path}gateways[0]_messageSentDataCount.csv ${data_path}gateways[0]_messageReceivedDataCount.csv ${data_path}gateways[1]_messageSentDataCount.csv ${data_path}gateways[1]_messageReceivedDataCount.csv &
python3 plotData.py Sent\ and\ Received\ data\ messages\ \(sum\) l -I -o $out_directory/messagesSentAndReceivedDataCountLine.pdf -l gateway0\ sent gateway0\ received gateway1\ sent gateway1\ received -i ${data_path}gateways[0]_messageSentDataCount.csv ${data_path}gateways[0]_messageReceivedDataCount.csv ${data_path}gateways[1]_messageSentDataCount.csv ${data_path}gateways[1]_messageReceivedDataCount.csv &

python3 plotData.py Sent\ and\ Received\ data\ messages b -I -o $out_directory/messagesSentAndReceivedDataBar.pdf -l sent received -g gateway0 gateway1 -i ${data_path}gateways[0]_messageSentDataCount.csv ${data_path}gateways[0]_messageReceivedDataCount.csv ${data_path}gateways[1]_messageSentDataCount.csv ${data_path}gateways[1]_messageReceivedDataCount.csv &


# Do not enable ignore option when a single input file does not exist (lost messages may not exist) because is useless to generate an empty graph
python3 plotData.py Lost\ data\ messages\ \(frequency\) s -o $out_directory/messagesLostData.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageLostData.csv ${data_path}gateways[1]_messageLostData.csv &
python3 plotData.py Lost\ data\ messages\ \(sum\) s -o $out_directory/messagesLostDataCountScattered.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageLostDataCount.csv ${data_path}gateways[1]_messageLostDataCount.csv &
python3 plotData.py Lost\ data\ messages\ \(sum\) l -o $out_directory/messagesLostDataCountLine.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageLostDataCount.csv ${data_path}gateways[1]_messageLostDataCount.csv &

# Enable ignore option when multiple input files are used and an input can miss (here lost messages)
python3 plotData.py Received\ and\ Lost\ data\ messages\ \(frequency\) s -I -o $out_directory/messagesReceivedAndLostData.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceivedData.csv ${data_path}gateways[0]_messageLostData.csv ${data_path}gateways[1]_messageReceivedData.csv ${data_path}gateways[1]_messageLostData.csv &
python3 plotData.py Received\ and\ Lost\ data\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedAndLostDataCountScattered.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceivedDataCount.csv ${data_path}gateways[0]_messageLostDataCount.csv ${data_path}gateways[1]_messageReceivedDataCount.csv ${data_path}gateways[1]_messageLostDataCount.csv &
python3 plotData.py Received\ and\ Lost\ data\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedAndLostDataCountLine.pdf -l gateway0\ received gateway0\ lost gateway1\ received gateway1\ lost -i ${data_path}gateways[0]_messageReceivedDataCount.csv ${data_path}gateways[0]_messageLostDataCount.csv ${data_path}gateways[1]_messageReceivedDataCount.csv ${data_path}gateways[1]_messageLostDataCount.csv &

python3 plotData.py Received\ and\ Lost\ data\ messages b -I -o $out_directory/messagesReceivedAndLostDataBar.pdf -l received lost -g gateway0 gateway1 -i ${data_path}gateways[0]_messageReceivedDataCount.csv ${data_path}gateways[0]_messageLostDataCount.csv ${data_path}gateways[1]_messageReceivedDataCount.csv ${data_path}gateways[1]_messageLostDataCount.csv &


python3 plotData.py In\ data\ messages\ \(sum\) s -o $out_directory/messagesInDataCountScattered.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageInDataCount.csv ${data_path}gateways[1]_messageInDataCount.csv &
python3 plotData.py In\ data\ messages\ \(sum\) l -o $out_directory/messagesInDataCountLine.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_messageInDataCount.csv ${data_path}gateways[1]_messageInDataCount.csv &

python3 plotData.py In\ and\ Received\ data\ messages b -I -o $out_directory/messagesInAndReceivedDataBar.pdf -l \in received -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInDataCount.csv ${data_path}gateways[0]_messageReceivedDataCount.csv ${data_path}gateways[1]_messageInDataCount.csv ${data_path}gateways[1]_messageReceivedDataCount.csv &
python3 plotData.py In\ and\ Received\ data\ messages b -I -o $out_directory/messagesInAndReceivedLoRaIpBar.pdf -l \in received\ LoRa\ devices received\ IP\ gateways received\ IP\ server -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInDataCount.csv ${data_path}gateways[0]_messageReceivedDevicesDataCount.csv ${data_path}gateways[0]_messageReceivedGatewaysDataCount.csv ${data_path}gateways[0]_messageReceivedServerDataCount.csv ${data_path}gateways[1]_messageInDataCount.csv ${data_path}gateways[1]_messageReceivedDevicesDataCount.csv ${data_path}gateways[1]_messageReceivedGatewaysDataCount.csv ${data_path}gateways[1]_messageReceivedServerDataCount.csv &

python3 plotData.py In\ and\ Lost\ data\ messages b -I -o $out_directory/messagesInAndLostDataBar.pdf -l \in lost -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInDataCount.csv ${data_path}gateways[0]_messageLostDataCount.csv ${data_path}gateways[1]_messageInDataCount.csv ${data_path}gateways[1]_messageLostDataCount.csv &
python3 plotData.py In\ and\ Invalid\ data\ messages b -I -o $out_directory/messagesInAndInvalidDataBar.pdf -l \in invalid -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInDataCount.csv ${data_path}gateways[0]_messageInvalidDataCount.csv ${data_path}gateways[1]_messageInDataCount.csv ${data_path}gateways[1]_messageInvalidDataCount.csv &


# Do not enable ignore option when a single input file does not exist (interferences may not exist) because is useless to generate an empty graph
python3 plotData.py Interferences\ data\ \(frequency\) s -o $out_directory/interferencesData.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_interferenceData.csv ${data_path}gateways[1]_interferenceData.csv &
python3 plotData.py Interferences\ data\ \(sum\) s -o $out_directory/interferencesDataCountScattered.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_interferenceDataCount.csv ${data_path}gateways[1]_interferenceDataCount.csv &
python3 plotData.py Interferences\ data\ \(sum\) l -o $out_directory/interferencesDataCountLine.pdf -l gateway0 gateway1 -i ${data_path}gateways[0]_interferenceDataCount.csv ${data_path}gateways[1]_interferenceDataCount.csv &


# Enable ignore option when multiple input files are used and an input can miss
python3 plotData.py Possible\ interferences\ and\ interferences\ data\ \(frequency\) s -I -o $out_directory/possibleInterferencesAndInterferencesData.pdf -l gateway0\ possible\ interferences gateway0\ interferences gateway1\ possible\ interferences gateway1\ interferences -i ${data_path}gateways[0]_interferencePossibleData.csv ${data_path}gateways[0]_interferenceData.csv ${data_path}gateways[1]_interferencePossibleData.csv ${data_path}gateways[1]_interferenceData.csv &
python3 plotData.py Possible\ interferences\ and\ interferences\ data\ \(sum\) s -I -o $out_directory/possibleInterferencesAndInterferencesDataCountScattered.pdf -l gateway0\ possible\ interferences gateway0\ interferences gateway1\ possible\ interferences gateway1\ interferences -i ${data_path}gateways[0]_interferencePossibleDataCount.csv ${data_path}gateways[0]_interferenceDataCount.csv ${data_path}gateways[1]_interferencePossibleDataCount.csv ${data_path}gateways[1]_interferenceDataCount.csv &
python3 plotData.py Possible\ interferences\ and\ interferences\ data\ \(sum\) l -I -o $out_directory/possibleInterferencesAndInterferencesDataCountLine.pdf -l gateway0\ possible\ interferences gateway0\ interferences gateway1\ possible\ interferences gateway1\ interferences -i ${data_path}gateways[0]_interferencePossibleDataCount.csv ${data_path}gateways[0]_interferenceDataCount.csv ${data_path}gateways[1]_interferencePossibleDataCount.csv ${data_path}gateways[1]_interferenceDataCount.csv &

python3 plotData.py In\ messages\ and\ interferences\ data b -I -o $out_directory/messagesInAndInterferencesDataBar.pdf -l \in interferences -g gateway0 gateway1 -i ${data_path}gateways[0]_messageInDataCount.csv ${data_path}gateways[0]_interferenceDataCount.csv ${data_path}gateways[1]_messageInDataCount.csv ${data_path}gateways[1]_interferenceDataCount.csv &
python3 plotData.py Possible\ interferences\ and\ interferences\ data b -I -o $out_directory/possibleInterferencesAndInterferencesDataBar.pdf -l gateway0\ possible\ interferences gateway0\ interferences gateway1\ possible\ interferences gateway1\ interferences -g gateway0 gateway1 -i ${data_path}gateways[0]_interferencePossibleDataCount.csv ${data_path}gateways[0]_interferenceDataCount.csv ${data_path}gateways[1]_interferencePossibleDataCount.csv ${data_path}gateways[1]_interferenceDataCount.csv &
#==============================

# Wait parallel executions end
wait

printf "\nAll charts are plotted. Bye :)\n"
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++