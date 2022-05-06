#!/bin/bash

# Example
#./plotDataCompareTests.sh /home/ivan/Scaricati/master_thesis_implementation/Results/simulations/


# Exit on error
set -e

in_directory=$1

# Append "/" to the directory path if not already present
if [ ! ${1: -1} = "/" ]
then
    in_directory+="/"
fi

: '
./plotDataTotalMessages.sh ${in_directory}stochastic_10_3/ 10 3
./plotDataTotalMessages.sh ${in_directory}stochastic_20_3/ 20 3
./plotDataTotalMessages.sh ${in_directory}stochastic_40_3/ 40 3
./plotDataTotalMessages.sh ${in_directory}stochastic_80_3/ 80 3
./plotDataTotalMessages.sh ${in_directory}stochastic_160_3/ 160 3
./plotDataTotalMessages.sh ${in_directory}stochastic_320_3/ 320 3
./plotDataTotalMessages.sh ${in_directory}stochastic_640_3/ 640 3
./plotDataTotalMessages.sh ${in_directory}stochastic_1080_3/ 1080 3

./plotDataTotalMessages.sh ${in_directory}stochastic_100_2/ 100 2
./plotDataTotalMessages.sh ${in_directory}stochastic_100_4/ 100 4
./plotDataTotalMessages.sh ${in_directory}stochastic_100_8/ 100 8
./plotDataTotalMessages.sh ${in_directory}stochastic_100_16/ 100 16
'


# Append "/" to the directory path if not already present
function getFullDirectoryName() {
    dir=$1

    if [ ! ${1: -1} = "/" ]
    then
        dir+="/"
    fi

    echo $dir
}


# Get the nested directories containing the mean values
stochastic_10_3="${in_directory}stochastic_10_3/"
mean_directory=$(ls $stochastic_10_3 | grep "^mean_exported_data*")
stochastic_10_3=$(find $stochastic_10_3$mean_directory -mindepth 1 -maxdepth 1 -type d)
stochastic_10_3=$(getFullDirectoryName $stochastic_10_3)
#printf "\n$stochastic_10_3\n"

stochastic_20_3="${in_directory}stochastic_20_3/"
mean_directory=$(ls $stochastic_20_3 | grep "^mean_exported_data*")
stochastic_20_3=$(find $stochastic_20_3$mean_directory -mindepth 1 -maxdepth 1 -type d)
stochastic_20_3=$(getFullDirectoryName $stochastic_20_3)
#printf "\n$stochastic_20_3\n"

stochastic_40_3="${in_directory}stochastic_40_3/"
mean_directory=$(ls $stochastic_40_3 | grep "^mean_exported_data*")
stochastic_40_3=$(find $stochastic_40_3$mean_directory -mindepth 1 -maxdepth 1 -type d)
stochastic_40_3=$(getFullDirectoryName $stochastic_40_3)
#printf "\n$stochastic_40_3\n"

stochastic_80_3="${in_directory}stochastic_80_3/"
mean_directory=$(ls $stochastic_80_3 | grep "^mean_exported_data*")
stochastic_80_3=$(find $stochastic_80_3$mean_directory -mindepth 1 -maxdepth 1 -type d)
stochastic_80_3=$(getFullDirectoryName $stochastic_80_3)
#printf "\n$stochastic_80_3\n"

stochastic_160_3="${in_directory}stochastic_160_3/"
mean_directory=$(ls $stochastic_160_3 | grep "^mean_exported_data*")
stochastic_160_3=$(find $stochastic_160_3$mean_directory -mindepth 1 -maxdepth 1 -type d)
stochastic_160_3=$(getFullDirectoryName $stochastic_160_3)
#printf "\n$stochastic_160_3\n"

stochastic_320_3="${in_directory}stochastic_320_3/"
mean_directory=$(ls $stochastic_320_3 | grep "^mean_exported_data*")
stochastic_320_3=$(find $stochastic_320_3$mean_directory -mindepth 1 -maxdepth 1 -type d)
stochastic_320_3=$(getFullDirectoryName $stochastic_320_3)
#printf "\n$stochastic_320_3\n"


stochastic_100_2="${in_directory}stochastic_100_2/"
mean_directory=$(ls $stochastic_100_2 | grep "^mean_exported_data*")
stochastic_100_2=$(find $stochastic_100_2$mean_directory -mindepth 1 -maxdepth 1 -type d)
stochastic_100_2=$(getFullDirectoryName $stochastic_100_2)
#printf "\n$stochastic_100_2\n"

stochastic_100_4="${in_directory}stochastic_100_4/"
mean_directory=$(ls $stochastic_100_4 | grep "^mean_exported_data*")
stochastic_100_4=$(find $stochastic_100_4$mean_directory -mindepth 1 -maxdepth 1 -type d)
stochastic_100_4=$(getFullDirectoryName $stochastic_100_4)
#printf "\n$stochastic_100_4\n"

stochastic_100_8="${in_directory}stochastic_100_8/"
mean_directory=$(ls $stochastic_100_8 | grep "^mean_exported_data*")
stochastic_100_8=$(find $stochastic_100_8$mean_directory -mindepth 1 -maxdepth 1 -type d)
stochastic_100_8=$(getFullDirectoryName $stochastic_100_8)
#printf "\n$stochastic_100_8\n"

stochastic_100_16="${in_directory}stochastic_100_16/"
mean_directory=$(ls $stochastic_100_16 | grep "^mean_exported_data*")
stochastic_100_16=$(find $stochastic_100_16$mean_directory -mindepth 1 -maxdepth 1 -type d)
stochastic_100_16=$(getFullDirectoryName $stochastic_100_16)
#printf "\n$stochastic_100_16\n"


#==== STATS ABOUT TOTAL NUMBER OF MESSAGES - CONST GATEWAYS, INCREASING DEVICES ====
out_directory="${in_directory}total_messages_comparison_fixed_gateways/"
printf "\nPlotting data into $out_directory\n"

# Total number of sent, received and lost uplinks
python3 plotData.py Sent\ uplinks\ \(sum\) s -I -o $out_directory/messagesSentUplinksCountScattered.pdf -l sent\ 10-3 sent\ 20-3 sent\ 40-3 sent\ 80-3 sent\ 160-3 sent\ 320-3 -i ${stochastic_10_3}messageSentDevicesCount10_3.csv ${stochastic_20_3}messageSentDevicesCount20_3.csv ${stochastic_40_3}messageSentDevicesCount40_3.csv ${stochastic_80_3}messageSentDevicesCount80_3.csv ${stochastic_160_3}messageSentDevicesCount160_3.csv ${stochastic_320_3}messageSentDevicesCount320_3.csv &
python3 plotData.py Sent\ uplinks\ \(sum\) l -I -o $out_directory/messagesSentUplinksCountLine.pdf -l sent\ 10-3 sent\ 20-3 sent\ 40-3 sent\ 80-3 sent\ 160-3 sent\ 320-3 -i ${stochastic_10_3}messageSentDevicesCount10_3.csv ${stochastic_20_3}messageSentDevicesCount20_3.csv ${stochastic_40_3}messageSentDevicesCount40_3.csv ${stochastic_80_3}messageSentDevicesCount80_3.csv ${stochastic_160_3}messageSentDevicesCount160_3.csv ${stochastic_320_3}messageSentDevicesCount320_3.csv &

python3 plotData.py Received\ uplinks\ \(sum\) s -I -o $out_directory/messagesReceivedUplinksCountScattered.pdf -l received\ 10-3 received\ 20-3 received\ 40-3 received\ 80-3 received\ 160-3 received\ 320-3 -i ${stochastic_10_3}messageReceivedGatewaysLoRaDevicesCount10_3.csv ${stochastic_20_3}messageReceivedGatewaysLoRaDevicesCount20_3.csv ${stochastic_40_3}messageReceivedGatewaysLoRaDevicesCount40_3.csv ${stochastic_80_3}messageReceivedGatewaysLoRaDevicesCount80_3.csv ${stochastic_160_3}messageReceivedGatewaysLoRaDevicesCount160_3.csv ${stochastic_320_3}messageReceivedGatewaysLoRaDevicesCount320_3.csv &
python3 plotData.py Received\ uplinks\ \(sum\) l -I -o $out_directory/messagesReceivedUplinksCountLine.pdf -l received\ 10-3 received\ 20-3 received\ 40-3 received\ 80-3 received\ 160-3 received\ 320-3 -i ${stochastic_10_3}messageReceivedGatewaysLoRaDevicesCount10_3.csv ${stochastic_20_3}messageReceivedGatewaysLoRaDevicesCount20_3.csv ${stochastic_40_3}messageReceivedGatewaysLoRaDevicesCount40_3.csv ${stochastic_80_3}messageReceivedGatewaysLoRaDevicesCount80_3.csv ${stochastic_160_3}messageReceivedGatewaysLoRaDevicesCount160_3.csv ${stochastic_320_3}messageReceivedGatewaysLoRaDevicesCount320_3.csv &

python3 plotData.py Lost\ uplinks\ \(sum\) s -I -o $out_directory/messagesLostUplinksCountScattered.pdf -l lost\ 10-3 lost\ 20-3 lost\ 40-3 lost\ 80-3 lost\ 160-3 lost\ 320-3 -i ${stochastic_10_3}messageLostGatewaysLoRaDevicesCount10_3.csv ${stochastic_20_3}messageLostGatewaysLoRaDevicesCount20_3.csv ${stochastic_40_3}messageLostGatewaysLoRaDevicesCount40_3.csv ${stochastic_80_3}messageLostGatewaysLoRaDevicesCount80_3.csv ${stochastic_160_3}messageLostGatewaysLoRaDevicesCount160_3.csv ${stochastic_320_3}messageLostGatewaysLoRaDevicesCount320_3.csv &
python3 plotData.py Lost\ uplinks\ \(sum\) l -I -o $out_directory/messagesLostUplinksCountLine.pdf -l lost\ 10-3 lost\ 20-3 lost\ 40-3 lost\ 80-3 lost\ 160-3 lost\ 320-3 -i ${stochastic_10_3}messageLostGatewaysLoRaDevicesCount10_3.csv ${stochastic_20_3}messageLostGatewaysLoRaDevicesCount20_3.csv ${stochastic_40_3}messageLostGatewaysLoRaDevicesCount40_3.csv ${stochastic_80_3}messageLostGatewaysLoRaDevicesCount80_3.csv ${stochastic_160_3}messageLostGatewaysLoRaDevicesCount160_3.csv ${stochastic_320_3}messageLostGatewaysLoRaDevicesCount320_3.csv &

python3 plotData.py Sent,\ Received\ and\ Lost\ uplinks b -I -o $out_directory/messagesSentReceivedAndLostUplinksBar.pdf -l sent received lost -g 10-3 20-3 40-3 80-3 160-3 320-3 -i ${stochastic_10_3}messageSentDevicesCount10_3.csv ${stochastic_10_3}messageReceivedGatewaysLoRaDevicesCount10_3.csv ${stochastic_10_3}messageLostGatewaysLoRaDevicesCount10_3.csv ${stochastic_20_3}messageSentDevicesCount20_3.csv ${stochastic_20_3}messageReceivedGatewaysLoRaDevicesCount20_3.csv ${stochastic_20_3}messageLostGatewaysLoRaDevicesCount20_3.csv ${stochastic_40_3}messageSentDevicesCount40_3.csv ${stochastic_40_3}messageReceivedGatewaysLoRaDevicesCount40_3.csv ${stochastic_40_3}messageLostGatewaysLoRaDevicesCount40_3.csv ${stochastic_80_3}messageSentDevicesCount80_3.csv ${stochastic_80_3}messageReceivedGatewaysLoRaDevicesCount80_3.csv ${stochastic_80_3}messageLostGatewaysLoRaDevicesCount80_3.csv ${stochastic_160_3}messageSentDevicesCount160_3.csv ${stochastic_160_3}messageReceivedGatewaysLoRaDevicesCount160_3.csv ${stochastic_160_3}messageLostGatewaysLoRaDevicesCount160_3.csv ${stochastic_320_3}messageSentDevicesCount320_3.csv ${stochastic_320_3}messageReceivedGatewaysLoRaDevicesCount320_3.csv ${stochastic_320_3}messageLostGatewaysLoRaDevicesCount320_3.csv &


# Total number of sent, received and lost downlinks to devices
python3 plotData.py Sent\ downlinks\ \(sum\) s -I -o $out_directory/messagesSentDownlinksForDevicesCountScattered.pdf -l sent\ 10-3 sent\ 20-3 sent\ 40-3 sent\ 80-3 sent\ 160-3 sent\ 320-3 -i ${stochastic_10_3}messageSentGatewaysLoRaDevicesCount10_3.csv ${stochastic_20_3}messageSentGatewaysLoRaDevicesCount20_3.csv ${stochastic_40_3}messageSentGatewaysLoRaDevicesCount40_3.csv ${stochastic_80_3}messageSentGatewaysLoRaDevicesCount80_3.csv ${stochastic_160_3}messageSentGatewaysLoRaDevicesCount160_3.csv ${stochastic_320_3}messageSentGatewaysLoRaDevicesCount320_3.csv &
python3 plotData.py Sent\ downlinks\ \(sum\) l -I -o $out_directory/messagesSentDownlinksForDevicesCountLine.pdf -l sent\ 10-3 sent\ 20-3 sent\ 40-3 sent\ 80-3 sent\ 160-3 sent\ 320-3 -i ${stochastic_10_3}messageSentGatewaysLoRaDevicesCount10_3.csv ${stochastic_20_3}messageSentGatewaysLoRaDevicesCount20_3.csv ${stochastic_40_3}messageSentGatewaysLoRaDevicesCount40_3.csv ${stochastic_80_3}messageSentGatewaysLoRaDevicesCount80_3.csv ${stochastic_160_3}messageSentGatewaysLoRaDevicesCount160_3.csv ${stochastic_320_3}messageSentGatewaysLoRaDevicesCount320_3.csv &

python3 plotData.py Received\ downlinks\ \(sum\) s -I -o $out_directory/messagesReceivedDownlinksForDevicesCountScattered.pdf -l received\ 10-3 received\ 20-3 received\ 40-3 received\ 80-3 received\ 160-3 received\ 320-3 -i ${stochastic_10_3}messageReceivedDevicesCount10_3.csv ${stochastic_20_3}messageReceivedDevicesCount20_3.csv ${stochastic_40_3}messageReceivedDevicesCount40_3.csv ${stochastic_80_3}messageReceivedDevicesCount80_3.csv ${stochastic_160_3}messageReceivedDevicesCount160_3.csv ${stochastic_320_3}messageReceivedDevicesCount320_3.csv &
python3 plotData.py Received\ downlinks\ \(sum\) l -I -o $out_directory/messagesReceivedDownlinksForDevicesCountLine.pdf -l received\ 10-3 received\ 20-3 received\ 40-3 received\ 80-3 received\ 160-3 received\ 320-3 -i ${stochastic_10_3}messageReceivedDevicesCount10_3.csv ${stochastic_20_3}messageReceivedDevicesCount20_3.csv ${stochastic_40_3}messageReceivedDevicesCount40_3.csv ${stochastic_80_3}messageReceivedDevicesCount80_3.csv ${stochastic_160_3}messageReceivedDevicesCount160_3.csv ${stochastic_320_3}messageReceivedDevicesCount320_3.csv &

python3 plotData.py Lost\ downlinks\ \(sum\) s -I -o $out_directory/messagesLostDownlinksForDevicesCountScattered.pdf -l lost\ 10-3 lost\ 20-3 lost\ 40-3 lost\ 80-3 lost\ 160-3 lost\ 320-3 -i ${stochastic_10_3}messageLostDevicesCount10_3.csv ${stochastic_20_3}messageLostDevicesCount20_3.csv ${stochastic_40_3}messageLostDevicesCount40_3.csv ${stochastic_80_3}messageLostDevicesCount80_3.csv ${stochastic_160_3}messageLostDevicesCount160_3.csv ${stochastic_320_3}messageLostDevicesCount320_3.csv &
python3 plotData.py Lost\ downlinks\ \(sum\) l -I -o $out_directory/messagesLostDownlinksForDevicesCountLine.pdf -l lost\ 10-3 lost\ 20-3 lost\ 40-3 lost\ 80-3 lost\ 160-3 lost\ 320-3 -i ${stochastic_10_3}messageLostDevicesCount10_3.csv ${stochastic_20_3}messageLostDevicesCount20_3.csv ${stochastic_40_3}messageLostDevicesCount40_3.csv ${stochastic_80_3}messageLostDevicesCount80_3.csv ${stochastic_160_3}messageLostDevicesCount160_3.csv ${stochastic_320_3}messageLostDevicesCount320_3.csv &

python3 plotData.py Sent,\ Received\ and\ Lost\ uplinks b -I -o $out_directory/messagesSentReceivedAndLostDownlinksForDevicesBar.pdf -l sent received lost -g 10-3 20-3 40-3 80-3 160-3 320-3 -i ${stochastic_10_3}messageSentGatewaysLoRaDevicesCount10_3.csv ${stochastic_10_3}messageReceivedDevicesCount10_3.csv ${stochastic_10_3}messageLostDevicesCount10_3.csv ${stochastic_20_3}messageSentGatewaysLoRaDevicesCount20_3.csv ${stochastic_20_3}messageReceivedDevicesCount20_3.csv ${stochastic_20_3}messageLostDevicesCount20_3.csv ${stochastic_40_3}messageSentGatewaysLoRaDevicesCount40_3.csv ${stochastic_40_3}messageReceivedDevicesCount40_3.csv ${stochastic_40_3}messageLostDevicesCount40_3.csv ${stochastic_80_3}messageSentGatewaysLoRaDevicesCount80_3.csv ${stochastic_80_3}messageReceivedDevicesCount80_3.csv ${stochastic_80_3}messageLostDevicesCount80_3.csv ${stochastic_160_3}messageSentGatewaysLoRaDevicesCount160_3.csv ${stochastic_160_3}messageReceivedDevicesCount160_3.csv ${stochastic_160_3}messageLostDevicesCount160_3.csv ${stochastic_320_3}messageSentGatewaysLoRaDevicesCount320_3.csv ${stochastic_320_3}messageReceivedDevicesCount320_3.csv ${stochastic_320_3}messageLostDevicesCount320_3.csv &


# Total number of sent, received and lost downlinks to gateways
python3 plotData.py Sent\ downlinks\ \(sum\) s -I -o $out_directory/messagesSentDownlinksForGatewaysCountScattered.pdf -l sent\ 10-3 sent\ 20-3 sent\ 40-3 sent\ 80-3 sent\ 160-3 sent\ 320-3 -i ${stochastic_10_3}messageSentGatewaysLoRaGatewaysCount10_3.csv ${stochastic_20_3}messageSentGatewaysLoRaGatewaysCount20_3.csv ${stochastic_40_3}messageSentGatewaysLoRaGatewaysCount40_3.csv ${stochastic_80_3}messageSentGatewaysLoRaGatewaysCount80_3.csv ${stochastic_160_3}messageSentGatewaysLoRaGatewaysCount160_3.csv ${stochastic_320_3}messageSentGatewaysLoRaGatewaysCount320_3.csv &
python3 plotData.py Sent\ downlinks\ \(sum\) l -I -o $out_directory/messagesSentDownlinksForGatewaysCountLine.pdf -l sent\ 10-3 sent\ 20-3 sent\ 40-3 sent\ 80-3 sent\ 160-3 sent\ 320-3 -i ${stochastic_10_3}messageSentGatewaysLoRaGatewaysCount10_3.csv ${stochastic_20_3}messageSentGatewaysLoRaGatewaysCount20_3.csv ${stochastic_40_3}messageSentGatewaysLoRaGatewaysCount40_3.csv ${stochastic_80_3}messageSentGatewaysLoRaGatewaysCount80_3.csv ${stochastic_160_3}messageSentGatewaysLoRaGatewaysCount160_3.csv ${stochastic_320_3}messageSentGatewaysLoRaGatewaysCount320_3.csv &

python3 plotData.py Received\ downlinks\ \(sum\) s -I -o $out_directory/messagesReceivedDownlinksForGatewaysCountScattered.pdf -l received\ 10-3 received\ 20-3 received\ 40-3 received\ 80-3 received\ 160-3 received\ 320-3 -i ${stochastic_10_3}messageReceivedGatewaysLoRaGatewaysCount10_3.csv ${stochastic_20_3}messageReceivedGatewaysLoRaGatewaysCount20_3.csv ${stochastic_40_3}messageReceivedGatewaysLoRaGatewaysCount40_3.csv ${stochastic_80_3}messageReceivedGatewaysLoRaGatewaysCount80_3.csv ${stochastic_160_3}messageReceivedGatewaysLoRaGatewaysCount160_3.csv ${stochastic_320_3}messageReceivedGatewaysLoRaGatewaysCount320_3.csv &
python3 plotData.py Received\ downlinks\ \(sum\) l -I -o $out_directory/messagesReceivedDownlinksForGatewaysCountLine.pdf -l received\ 10-3 received\ 20-3 received\ 40-3 received\ 80-3 received\ 160-3 received\ 320-3 -i ${stochastic_10_3}messageReceivedGatewaysLoRaGatewaysCount10_3.csv ${stochastic_20_3}messageReceivedGatewaysLoRaGatewaysCount20_3.csv ${stochastic_40_3}messageReceivedGatewaysLoRaGatewaysCount40_3.csv ${stochastic_80_3}messageReceivedGatewaysLoRaGatewaysCount80_3.csv ${stochastic_160_3}messageReceivedGatewaysLoRaGatewaysCount160_3.csv ${stochastic_320_3}messageReceivedGatewaysLoRaGatewaysCount320_3.csv &

python3 plotData.py Lost\ downlinks\ \(sum\) s -I -o $out_directory/messagesLostDownlinksForGatewaysCountScattered.pdf -l lost\ 10-3 lost\ 20-3 lost\ 40-3 lost\ 80-3 lost\ 160-3 lost\ 320-3 -i ${stochastic_10_3}messageLostGatewaysLoRaGatewaysCount10_3.csv ${stochastic_20_3}messageLostGatewaysLoRaGatewaysCount20_3.csv ${stochastic_40_3}messageLostGatewaysLoRaGatewaysCount40_3.csv ${stochastic_80_3}messageLostGatewaysLoRaGatewaysCount80_3.csv ${stochastic_160_3}messageLostGatewaysLoRaGatewaysCount160_3.csv ${stochastic_320_3}messageLostGatewaysLoRaGatewaysCount320_3.csv &
python3 plotData.py Lost\ downlinks\ \(sum\) l -I -o $out_directory/messagesLostDownlinksForGatewaysCountLine.pdf -l lost\ 10-3 lost\ 20-3 lost\ 40-3 lost\ 80-3 lost\ 160-3 lost\ 320-3 -i ${stochastic_10_3}messageLostGatewaysLoRaGatewaysCount10_3.csv ${stochastic_20_3}messageLostGatewaysLoRaGatewaysCount20_3.csv ${stochastic_40_3}messageLostGatewaysLoRaGatewaysCount40_3.csv ${stochastic_80_3}messageLostGatewaysLoRaGatewaysCount80_3.csv ${stochastic_160_3}messageLostGatewaysLoRaGatewaysCount160_3.csv ${stochastic_320_3}messageLostGatewaysLoRaGatewaysCount320_3.csv &

python3 plotData.py Sent,\ Received\ and\ Lost\ downlinks b -I -o $out_directory/messagesSentReceivedAndLostDownlinksForGatewaysBar.pdf -l sent received lost -g 10-3 20-3 40-3 80-3 160-3 320-3 -i ${stochastic_10_3}messageSentGatewaysLoRaGatewaysCount10_3.csv ${stochastic_10_3}messageReceivedGatewaysLoRaGatewaysCount10_3.csv ${stochastic_10_3}messageLostGatewaysLoRaGatewaysCount10_3.csv ${stochastic_20_3}messageSentGatewaysLoRaGatewaysCount20_3.csv ${stochastic_20_3}messageReceivedGatewaysLoRaGatewaysCount20_3.csv ${stochastic_20_3}messageLostGatewaysLoRaGatewaysCount20_3.csv ${stochastic_40_3}messageSentGatewaysLoRaGatewaysCount40_3.csv ${stochastic_40_3}messageReceivedGatewaysLoRaGatewaysCount40_3.csv ${stochastic_40_3}messageLostGatewaysLoRaGatewaysCount40_3.csv ${stochastic_80_3}messageSentGatewaysLoRaGatewaysCount80_3.csv ${stochastic_80_3}messageReceivedGatewaysLoRaGatewaysCount80_3.csv ${stochastic_80_3}messageLostGatewaysLoRaGatewaysCount80_3.csv ${stochastic_160_3}messageSentGatewaysLoRaGatewaysCount160_3.csv ${stochastic_160_3}messageReceivedGatewaysLoRaGatewaysCount160_3.csv ${stochastic_160_3}messageLostGatewaysLoRaGatewaysCount160_3.csv ${stochastic_320_3}messageSentGatewaysLoRaGatewaysCount320_3.csv ${stochastic_320_3}messageReceivedGatewaysLoRaGatewaysCount320_3.csv ${stochastic_320_3}messageLostGatewaysLoRaGatewaysCount320_3.csv &


# Total number of sent, received and lost IP messages to gateways
python3 plotData.py Sent\ IP\ gateway\ messages\ \(sum\) s -I -o $out_directory/messagesSentIpForGatewaysCountScattered.pdf -l sent\ 10-3 sent\ 20-3 sent\ 40-3 sent\ 80-3 sent\ 160-3 sent\ 320-3 -i ${stochastic_10_3}messageSentGatewaysIpGatewaysCount10_3.csv ${stochastic_20_3}messageSentGatewaysIpGatewaysCount20_3.csv ${stochastic_40_3}messageSentGatewaysIpGatewaysCount40_3.csv ${stochastic_80_3}messageSentGatewaysIpGatewaysCount80_3.csv ${stochastic_160_3}messageSentGatewaysIpGatewaysCount160_3.csv ${stochastic_320_3}messageSentGatewaysIpGatewaysCount320_3.csv &
python3 plotData.py Sent\ IP\ gateway\ messages\ \(sum\) l -I -o $out_directory/messagesSentIpForGatewaysCountLine.pdf -l sent\ 10-3 sent\ 20-3 sent\ 40-3 sent\ 80-3 sent\ 160-3 sent\ 320-3 -i ${stochastic_10_3}messageSentGatewaysIpGatewaysCount10_3.csv ${stochastic_20_3}messageSentGatewaysIpGatewaysCount20_3.csv ${stochastic_40_3}messageSentGatewaysIpGatewaysCount40_3.csv ${stochastic_80_3}messageSentGatewaysIpGatewaysCount80_3.csv ${stochastic_160_3}messageSentGatewaysIpGatewaysCount160_3.csv ${stochastic_320_3}messageSentGatewaysIpGatewaysCount320_3.csv &

python3 plotData.py Received\ IP\ gateway\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedIpForGatewaysCountScattered.pdf -l received\ 10-3 received\ 20-3 received\ 40-3 received\ 80-3 received\ 160-3 received\ 320-3 -i ${stochastic_10_3}messageReceivedGatewaysIpGatewaysCount10_3.csv ${stochastic_20_3}messageReceivedGatewaysIpGatewaysCount20_3.csv ${stochastic_40_3}messageReceivedGatewaysIpGatewaysCount40_3.csv ${stochastic_80_3}messageReceivedGatewaysIpGatewaysCount80_3.csv ${stochastic_160_3}messageReceivedGatewaysIpGatewaysCount160_3.csv ${stochastic_320_3}messageReceivedGatewaysIpGatewaysCount320_3.csv &
python3 plotData.py Received\ IP\ gateway\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedIpForGatewaysCountLine.pdf -l received\ 10-3 received\ 20-3 received\ 40-3 received\ 80-3 received\ 160-3 received\ 320-3 -i ${stochastic_10_3}messageReceivedGatewaysIpGatewaysCount10_3.csv ${stochastic_20_3}messageReceivedGatewaysIpGatewaysCount20_3.csv ${stochastic_40_3}messageReceivedGatewaysIpGatewaysCount40_3.csv ${stochastic_80_3}messageReceivedGatewaysIpGatewaysCount80_3.csv ${stochastic_160_3}messageReceivedGatewaysIpGatewaysCount160_3.csv ${stochastic_320_3}messageReceivedGatewaysIpGatewaysCount320_3.csv &

python3 plotData.py Lost\ IP\ gateway\ messages\ \(sum\) s -I -o $out_directory/messagesLostIpForGatewaysCountScattered.pdf -l lost\ 10-3 lost\ 20-3 lost\ 40-3 lost\ 80-3 lost\ 160-3 lost\ 320-3 -i ${stochastic_10_3}messageLostGatewaysIpGatewaysCount10_3.csv ${stochastic_20_3}messageLostGatewaysIpGatewaysCount20_3.csv ${stochastic_40_3}messageLostGatewaysIpGatewaysCount40_3.csv ${stochastic_80_3}messageLostGatewaysIpGatewaysCount80_3.csv ${stochastic_160_3}messageLostGatewaysIpGatewaysCount160_3.csv ${stochastic_320_3}messageLostGatewaysIpGatewaysCount320_3.csv &
python3 plotData.py Lost\ IP\ gateway\ messages\ \(sum\) l -I -o $out_directory/messagesLostIpForGatewaysCountLine.pdf -l lost\ 10-3 lost\ 20-3 lost\ 40-3 lost\ 80-3 lost\ 160-3 lost\ 320-3 -i ${stochastic_10_3}messageLostGatewaysIpGatewaysCount10_3.csv ${stochastic_20_3}messageLostGatewaysIpGatewaysCount20_3.csv ${stochastic_40_3}messageLostGatewaysIpGatewaysCount40_3.csv ${stochastic_80_3}messageLostGatewaysIpGatewaysCount80_3.csv ${stochastic_160_3}messageLostGatewaysIpGatewaysCount160_3.csv ${stochastic_320_3}messageLostGatewaysIpGatewaysCount320_3.csv &

python3 plotData.py Sent,\ Received\ and\ Lost\ IP\ gateway\ messages b -I -o $out_directory/messagesSentReceivedAndLostIpForGatewaysBar.pdf -l sent received lost -g 10-3 20-3 40-3 80-3 160-3 320-3 -i ${stochastic_10_3}messageSentGatewaysIpGatewaysCount10_3.csv ${stochastic_10_3}messageReceivedGatewaysIpGatewaysCount10_3.csv ${stochastic_10_3}messageLostGatewaysIpGatewaysCount10_3.csv ${stochastic_20_3}messageSentGatewaysIpGatewaysCount20_3.csv ${stochastic_20_3}messageReceivedGatewaysIpGatewaysCount20_3.csv ${stochastic_20_3}messageLostGatewaysIpGatewaysCount20_3.csv ${stochastic_40_3}messageSentGatewaysIpGatewaysCount40_3.csv ${stochastic_40_3}messageReceivedGatewaysIpGatewaysCount40_3.csv ${stochastic_40_3}messageLostGatewaysIpGatewaysCount40_3.csv ${stochastic_80_3}messageSentGatewaysIpGatewaysCount80_3.csv ${stochastic_80_3}messageReceivedGatewaysIpGatewaysCount80_3.csv ${stochastic_80_3}messageLostGatewaysIpGatewaysCount80_3.csv ${stochastic_160_3}messageSentGatewaysIpGatewaysCount160_3.csv ${stochastic_160_3}messageReceivedGatewaysIpGatewaysCount160_3.csv ${stochastic_160_3}messageLostGatewaysIpGatewaysCount160_3.csv ${stochastic_320_3}messageSentGatewaysIpGatewaysCount320_3.csv ${stochastic_320_3}messageReceivedGatewaysIpGatewaysCount320_3.csv ${stochastic_320_3}messageLostGatewaysIpGatewaysCount320_3.csv &



#==== STATS ABOUT TOTAL NUMBER OF MESSAGES - CONST DEVICES, INCREASING GATEWAYS ====
out_directory="${in_directory}total_messages_comparison_fixed_devices/"
printf "\nPlotting data into $out_directory\n"

# Total number of sent, received and lost uplinks
python3 plotData.py Sent\ uplinks\ \(sum\) s -I -o $out_directory/messagesSentUplinksCountScattered.pdf -l sent\ 100-2 sent\ 100-4 sent\ 100-8 sent\ 100-16 -i ${stochastic_100_2}messageSentDevicesCount100_2.csv ${stochastic_100_4}messageSentDevicesCount100_4.csv ${stochastic_100_8}messageSentDevicesCount100_8.csv ${stochastic_100_16}messageSentDevicesCount100_16.csv &
python3 plotData.py Sent\ uplinks\ \(sum\) l -I -o $out_directory/messagesSentUplinksCountLine.pdf -l sent\ 100-2 sent\ 100-4 sent\ 100-8 sent\ 100-16 -i ${stochastic_100_2}messageSentDevicesCount100_2.csv ${stochastic_100_4}messageSentDevicesCount100_4.csv ${stochastic_100_8}messageSentDevicesCount100_8.csv ${stochastic_100_16}messageSentDevicesCount100_16.csv &

python3 plotData.py Received\ uplinks\ \(sum\) s -I -o $out_directory/messagesReceivedUplinksCountScattered.pdf -l received\ 100-2 received\ 100-4 received\ 100-8 received\ 100-16 -i ${stochastic_100_2}messageReceivedGatewaysLoRaDevicesCount100_2.csv ${stochastic_100_4}messageReceivedGatewaysLoRaDevicesCount100_4.csv ${stochastic_100_8}messageReceivedGatewaysLoRaDevicesCount100_8.csv ${stochastic_100_16}messageReceivedGatewaysLoRaDevicesCount100_16.csv &
python3 plotData.py Received\ uplinks\ \(sum\) l -I -o $out_directory/messagesReceivedUplinksCountLine.pdf -l received\ 100-2 received\ 100-4 received\ 100-8 received\ 100-16 -i ${stochastic_100_2}messageReceivedGatewaysLoRaDevicesCount100_2.csv ${stochastic_100_4}messageReceivedGatewaysLoRaDevicesCount100_4.csv ${stochastic_100_8}messageReceivedGatewaysLoRaDevicesCount100_8.csv ${stochastic_100_16}messageReceivedGatewaysLoRaDevicesCount100_16.csv &

python3 plotData.py Lost\ uplinks\ \(sum\) s -I -o $out_directory/messagesLostUplinksCountScattered.pdf -l lost\ 100-2 lost\ 100-4 lost\ 100-8 lost\ 100-16 -i ${stochastic_100_2}messageLostGatewaysLoRaDevicesCount100_2.csv ${stochastic_100_4}messageLostGatewaysLoRaDevicesCount100_4.csv ${stochastic_100_8}messageLostGatewaysLoRaDevicesCount100_8.csv ${stochastic_100_16}messageLostGatewaysLoRaDevicesCount100_16.csv &
python3 plotData.py Lost\ uplinks\ \(sum\) l -I -o $out_directory/messagesLostUplinksCountLine.pdf -l lost\ 100-2 lost\ 100-4 lost\ 100-8 lost\ 100-16 -i ${stochastic_100_2}messageLostGatewaysLoRaDevicesCount100_2.csv ${stochastic_100_4}messageLostGatewaysLoRaDevicesCount100_4.csv ${stochastic_100_8}messageLostGatewaysLoRaDevicesCount100_8.csv ${stochastic_100_16}messageLostGatewaysLoRaDevicesCount100_16.csv &

python3 plotData.py Sent,\ Received\ and\ Lost\ uplinks b -I -o $out_directory/messagesSentReceivedAndLostUplinksBar.pdf -l sent received lost -g 100-2 100-4 100-8 100-16 -i ${stochastic_100_2}messageSentDevicesCount100_2.csv ${stochastic_100_2}messageReceivedGatewaysLoRaDevicesCount100_2.csv ${stochastic_100_2}messageLostGatewaysLoRaDevicesCount100_2.csv ${stochastic_100_4}messageSentDevicesCount100_4.csv ${stochastic_100_4}messageReceivedGatewaysLoRaDevicesCount100_4.csv ${stochastic_100_4}messageLostGatewaysLoRaDevicesCount100_4.csv ${stochastic_100_8}messageSentDevicesCount100_8.csv ${stochastic_100_8}messageReceivedGatewaysLoRaDevicesCount100_8.csv ${stochastic_100_8}messageLostGatewaysLoRaDevicesCount100_8.csv ${stochastic_100_16}messageSentDevicesCount100_16.csv ${stochastic_100_16}messageReceivedGatewaysLoRaDevicesCount100_16.csv ${stochastic_100_16}messageLostGatewaysLoRaDevicesCount100_16.csv &


# Total number of sent, received and lost downlinks to devices
python3 plotData.py Sent\ downlinks\ \(sum\) s -I -o $out_directory/messagesSentDownlinksForDevicesCountScattered.pdf -l sent\ 100-2 sent\ 100-4 sent\ 100-8 sent\ 100-16 -i ${stochastic_100_2}messageSentGatewaysLoRaDevicesCount100_2.csv ${stochastic_100_4}messageSentGatewaysLoRaDevicesCount100_4.csv ${stochastic_100_8}messageSentGatewaysLoRaDevicesCount100_8.csv ${stochastic_100_16}messageSentGatewaysLoRaDevicesCount100_16.csv &
python3 plotData.py Sent\ downlinks\ \(sum\) l -I -o $out_directory/messagesSentDownlinksForDevicesCountLine.pdf -l sent\ 100-2 sent\ 100-4 sent\ 100-8 sent\ 100-16 -i ${stochastic_100_2}messageSentGatewaysLoRaDevicesCount100_2.csv ${stochastic_100_4}messageSentGatewaysLoRaDevicesCount100_4.csv ${stochastic_100_8}messageSentGatewaysLoRaDevicesCount100_8.csv ${stochastic_100_16}messageSentGatewaysLoRaDevicesCount100_16.csv &

python3 plotData.py Received\ downlinks\ \(sum\) s -I -o $out_directory/messagesReceivedDownlinksForDevicesCountScattered.pdf -l received\ 100-2 received\ 100-4 received\ 100-8 received\ 100-16 -i ${stochastic_100_2}messageReceivedDevicesCount100_2.csv ${stochastic_100_4}messageReceivedDevicesCount100_4.csv ${stochastic_100_8}messageReceivedDevicesCount100_8.csv ${stochastic_100_16}messageReceivedDevicesCount100_16.csv &
python3 plotData.py Received\ downlinks\ \(sum\) l -I -o $out_directory/messagesReceivedDownlinksForDevicesCountLine.pdf -l received\ 100-2 received\ 100-4 received\ 100-8 received\ 100-16 -i ${stochastic_100_2}messageReceivedDevicesCount100_2.csv ${stochastic_100_4}messageReceivedDevicesCount100_4.csv ${stochastic_100_8}messageReceivedDevicesCount100_8.csv ${stochastic_100_16}messageReceivedDevicesCount100_16.csv &

python3 plotData.py Lost\ downlinks\ \(sum\) s -I -o $out_directory/messagesLostDownlinksForDevicesCountScattered.pdf -l lost\ 100-2 lost\ 100-4 lost\ 100-8 lost\ 100-16 -i ${stochastic_100_2}messageLostDevicesCount100_2.csv ${stochastic_100_4}messageLostDevicesCount100_4.csv ${stochastic_100_8}messageLostDevicesCount100_8.csv ${stochastic_100_16}messageLostDevicesCount100_16.csv &
python3 plotData.py Lost\ downlinks\ \(sum\) l -I -o $out_directory/messagesLostDownlinksForDevicesCountLine.pdf -l lost\ 100-2 lost\ 100-4 lost\ 100-8 lost\ 100-16 -i ${stochastic_100_2}messageLostDevicesCount100_2.csv ${stochastic_100_4}messageLostDevicesCount100_4.csv ${stochastic_100_8}messageLostDevicesCount100_8.csv ${stochastic_100_16}messageLostDevicesCount100_16.csv &

python3 plotData.py Sent,\ Received\ and\ Lost\ uplinks b -I -o $out_directory/messagesSentReceivedAndLostDownlinksForDevicesBar.pdf -l sent received lost -g 100-2 100-4 100-8 100-16 -i ${stochastic_100_2}messageSentGatewaysLoRaDevicesCount100_2.csv ${stochastic_100_2}messageReceivedDevicesCount100_2.csv ${stochastic_100_2}messageLostDevicesCount100_2.csv ${stochastic_100_4}messageSentGatewaysLoRaDevicesCount100_4.csv ${stochastic_100_4}messageReceivedDevicesCount100_4.csv ${stochastic_100_4}messageLostDevicesCount100_4.csv ${stochastic_100_8}messageSentGatewaysLoRaDevicesCount100_8.csv ${stochastic_100_8}messageReceivedDevicesCount100_8.csv ${stochastic_100_8}messageLostDevicesCount100_8.csv ${stochastic_100_16}messageSentGatewaysLoRaDevicesCount100_16.csv ${stochastic_100_16}messageReceivedDevicesCount100_16.csv ${stochastic_100_16}messageLostDevicesCount100_16.csv &


# Total number of sent, received and lost downlinks to gateways
python3 plotData.py Sent\ downlinks\ \(sum\) s -I -o $out_directory/messagesSentDownlinksForGatewaysCountScattered.pdf -l sent\ 100-2 sent\ 100-4 sent\ 100-8 sent\ 100-16 -i ${stochastic_100_2}messageSentGatewaysLoRaGatewaysCount100_2.csv ${stochastic_100_4}messageSentGatewaysLoRaGatewaysCount100_4.csv ${stochastic_100_8}messageSentGatewaysLoRaGatewaysCount100_8.csv ${stochastic_100_16}messageSentGatewaysLoRaGatewaysCount100_16.csv &
python3 plotData.py Sent\ downlinks\ \(sum\) l -I -o $out_directory/messagesSentDownlinksForGatewaysCountLine.pdf -l sent\ 100-2 sent\ 100-4 sent\ 100-8 sent\ 100-16 -i ${stochastic_100_2}messageSentGatewaysLoRaGatewaysCount100_2.csv ${stochastic_100_4}messageSentGatewaysLoRaGatewaysCount100_4.csv ${stochastic_100_8}messageSentGatewaysLoRaGatewaysCount100_8.csv ${stochastic_100_16}messageSentGatewaysLoRaGatewaysCount100_16.csv &

python3 plotData.py Received\ downlinks\ \(sum\) s -I -o $out_directory/messagesReceivedDownlinksForGatewaysCountScattered.pdf -l received\ 100-2 received\ 100-4 received\ 100-8 received\ 100-16 -i ${stochastic_100_2}messageReceivedGatewaysLoRaGatewaysCount100_2.csv ${stochastic_100_4}messageReceivedGatewaysLoRaGatewaysCount100_4.csv ${stochastic_100_8}messageReceivedGatewaysLoRaGatewaysCount100_8.csv ${stochastic_100_16}messageReceivedGatewaysLoRaGatewaysCount100_16.csv &
python3 plotData.py Received\ downlinks\ \(sum\) l -I -o $out_directory/messagesReceivedDownlinksForGatewaysCountLine.pdf -l received\ 100-2 received\ 100-4 received\ 100-8 received\ 100-16 -i ${stochastic_100_2}messageReceivedGatewaysLoRaGatewaysCount100_2.csv ${stochastic_100_4}messageReceivedGatewaysLoRaGatewaysCount100_4.csv ${stochastic_100_8}messageReceivedGatewaysLoRaGatewaysCount100_8.csv ${stochastic_100_16}messageReceivedGatewaysLoRaGatewaysCount100_16.csv &

python3 plotData.py Lost\ downlinks\ \(sum\) s -I -o $out_directory/messagesLostDownlinksForGatewaysCountScattered.pdf -l lost\ 100-2 lost\ 100-4 lost\ 100-8 lost\ 100-16 -i ${stochastic_100_2}messageLostGatewaysLoRaGatewaysCount100_2.csv ${stochastic_100_4}messageLostGatewaysLoRaGatewaysCount100_4.csv ${stochastic_100_8}messageLostGatewaysLoRaGatewaysCount100_8.csv ${stochastic_100_16}messageLostGatewaysLoRaGatewaysCount100_16.csv &
python3 plotData.py Lost\ downlinks\ \(sum\) l -I -o $out_directory/messagesLostDownlinksForGatewaysCountLine.pdf -l lost\ 100-2 lost\ 100-4 lost\ 100-8 lost\ 100-16 -i ${stochastic_100_2}messageLostGatewaysLoRaGatewaysCount100_2.csv ${stochastic_100_4}messageLostGatewaysLoRaGatewaysCount100_4.csv ${stochastic_100_8}messageLostGatewaysLoRaGatewaysCount100_8.csv ${stochastic_100_16}messageLostGatewaysLoRaGatewaysCount100_16.csv &

python3 plotData.py Sent,\ Received\ and\ Lost\ downlinks b -I -o $out_directory/messagesSentReceivedAndLostDownlinksForGatewaysBar.pdf -l sent received lost -g 100-2 100-4 100-8 100-16 -i ${stochastic_100_2}messageSentGatewaysLoRaGatewaysCount100_2.csv ${stochastic_100_2}messageReceivedGatewaysLoRaGatewaysCount100_2.csv ${stochastic_100_2}messageLostGatewaysLoRaGatewaysCount100_2.csv ${stochastic_100_4}messageSentGatewaysLoRaGatewaysCount100_4.csv ${stochastic_100_4}messageReceivedGatewaysLoRaGatewaysCount100_4.csv ${stochastic_100_4}messageLostGatewaysLoRaGatewaysCount100_4.csv ${stochastic_100_8}messageSentGatewaysLoRaGatewaysCount100_8.csv ${stochastic_100_8}messageReceivedGatewaysLoRaGatewaysCount100_8.csv ${stochastic_100_8}messageLostGatewaysLoRaGatewaysCount100_8.csv ${stochastic_100_16}messageSentGatewaysLoRaGatewaysCount100_16.csv ${stochastic_100_16}messageReceivedGatewaysLoRaGatewaysCount100_16.csv ${stochastic_100_16}messageLostGatewaysLoRaGatewaysCount100_16.csv &


# Total number of sent, received and lost IP messages to gateways
python3 plotData.py Sent\ IP\ gateway\ messages\ \(sum\) s -I -o $out_directory/messagesSentIpForGatewaysCountScattered.pdf -l sent\ 100-2 sent\ 100-4 sent\ 100-8 sent\ 100-16 -i ${stochastic_100_2}messageSentGatewaysIpGatewaysCount100_2.csv ${stochastic_100_4}messageSentGatewaysIpGatewaysCount100_4.csv ${stochastic_100_8}messageSentGatewaysIpGatewaysCount100_8.csv ${stochastic_100_16}messageSentGatewaysIpGatewaysCount100_16.csv &
python3 plotData.py Sent\ IP\ gateway\ messages\ \(sum\) l -I -o $out_directory/messagesSentIpForGatewaysCountLine.pdf -l sent\ 100-2 sent\ 100-4 sent\ 100-8 sent\ 100-16 -i ${stochastic_100_2}messageSentGatewaysIpGatewaysCount100_2.csv ${stochastic_100_4}messageSentGatewaysIpGatewaysCount100_4.csv ${stochastic_100_8}messageSentGatewaysIpGatewaysCount100_8.csv ${stochastic_100_16}messageSentGatewaysIpGatewaysCount100_16.csv &

python3 plotData.py Received\ IP\ gateway\ messages\ \(sum\) s -I -o $out_directory/messagesReceivedIpForGatewaysCountScattered.pdf -l received\ 100-2 received\ 100-4 received\ 100-8 received\ 100-16 -i ${stochastic_100_2}messageReceivedGatewaysIpGatewaysCount100_2.csv ${stochastic_100_4}messageReceivedGatewaysIpGatewaysCount100_4.csv ${stochastic_100_8}messageReceivedGatewaysIpGatewaysCount100_8.csv ${stochastic_100_16}messageReceivedGatewaysIpGatewaysCount100_16.csv &
python3 plotData.py Received\ IP\ gateway\ messages\ \(sum\) l -I -o $out_directory/messagesReceivedIpForGatewaysCountLine.pdf -l received\ 100-2 received\ 100-4 received\ 100-8 received\ 100-16 -i ${stochastic_100_2}messageReceivedGatewaysIpGatewaysCount100_2.csv ${stochastic_100_4}messageReceivedGatewaysIpGatewaysCount100_4.csv ${stochastic_100_8}messageReceivedGatewaysIpGatewaysCount100_8.csv ${stochastic_100_16}messageReceivedGatewaysIpGatewaysCount100_16.csv &

python3 plotData.py Lost\ IP\ gateway\ messages\ \(sum\) s -I -o $out_directory/messagesLostIpForGatewaysCountScattered.pdf -l lost\ 100-2 lost\ 100-4 lost\ 100-8 lost\ 100-16 -i ${stochastic_100_2}messageLostGatewaysIpGatewaysCount100_2.csv ${stochastic_100_4}messageLostGatewaysIpGatewaysCount100_4.csv ${stochastic_100_8}messageLostGatewaysIpGatewaysCount100_8.csv ${stochastic_100_16}messageLostGatewaysIpGatewaysCount100_16.csv &
python3 plotData.py Lost\ IP\ gateway\ messages\ \(sum\) l -I -o $out_directory/messagesLostIpForGatewaysCountLine.pdf -l lost\ 100-2 lost\ 100-4 lost\ 100-8 lost\ 100-16 -i ${stochastic_100_2}messageLostGatewaysIpGatewaysCount100_2.csv ${stochastic_100_4}messageLostGatewaysIpGatewaysCount100_4.csv ${stochastic_100_8}messageLostGatewaysIpGatewaysCount100_8.csv ${stochastic_100_16}messageLostGatewaysIpGatewaysCount100_16.csv &

python3 plotData.py Sent,\ Received\ and\ Lost\ IP\ gateway\ messages b -I -o $out_directory/messagesSentReceivedAndLostIpForGatewaysBar.pdf -l sent received lost -g 100-2 100-4 100-8 100-16 -i ${stochastic_100_2}messageSentGatewaysIpGatewaysCount100_2.csv ${stochastic_100_2}messageReceivedGatewaysIpGatewaysCount100_2.csv ${stochastic_100_2}messageLostGatewaysIpGatewaysCount100_2.csv ${stochastic_100_4}messageSentGatewaysIpGatewaysCount100_4.csv ${stochastic_100_4}messageReceivedGatewaysIpGatewaysCount100_4.csv ${stochastic_100_4}messageLostGatewaysIpGatewaysCount100_4.csv ${stochastic_100_8}messageSentGatewaysIpGatewaysCount100_8.csv ${stochastic_100_8}messageReceivedGatewaysIpGatewaysCount100_8.csv ${stochastic_100_8}messageLostGatewaysIpGatewaysCount100_8.csv ${stochastic_100_16}messageSentGatewaysIpGatewaysCount100_16.csv ${stochastic_100_16}messageReceivedGatewaysIpGatewaysCount100_16.csv ${stochastic_100_16}messageLostGatewaysIpGatewaysCount100_16.csv &


# Wait parallel executions end
wait

printf "\nAll charts are plotted. Bye :)\n"