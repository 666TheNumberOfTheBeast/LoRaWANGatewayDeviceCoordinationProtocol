#!/bin/bash

if [ ! $# -eq 3 ]
then
    printf "Wrong arguments!\nUsage: ${0} <directory_path> <number_devices> <number_gateways>\n"
    exit 1
fi

if [ $2 -lt 0 ] || [ $3 -lt 0 ]
then
    printf "Negative number!\nUsage: ${0} <directory_path> <number_devices> <number_gateways>\n"
    exit 1
fi

# Exit on error
set -e

# Get directory path
directory_path=$1

# Append "/" to the directory path if not already present
if [ ! ${1: -1} = "/" ]
then
    directory_path+="/"
fi

# Get the nested directory containing the mean values
mean_directory=$(ls $directory_path | grep "^mean_exported_data*")
stats_directory=$(find $directory_path$mean_directory -mindepth 1 -maxdepth 1 -type d)

# Append "/" to the directory path if not already present
if [ ! ${stats_directory: -1} = "/" ]
then
    stats_directory+="/"
fi

#echo $mean_directory
#echo $stats_directory

printf "\nTrying to merge some CSV files to obtain total messages\n"

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
        exported_data_files+=(${stats_directory}${dev}[$i]$1)
    done

    echo ${exported_data_files[@]}
}

#res=$(getAllFiles "_messageSentCount.csv" $2 true)
#echo $res
#exit 1

: '
python3 mergeCSV.py -r -I -o ${stats_directory}messageSentDevicesCount${2}_${3}.csv -i $(getAllFiles "_messageSentCount.csv" $2 true) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageSentGatewaysCount${2}_${3}.csv -i $(getAllFiles "_messageSentCount.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageSentGatewaysLoRaCount${2}_${3}.csv -i $(getAllFiles "_messageSentLoRaCount.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageSentGatewaysLoRaGatewaysCount${2}_${3}.csv -i $(getAllFiles "_messageSentLoRaGatewaysCount.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageSentGatewaysLoRaDevicesCount${2}_${3}.csv -i $(getAllFiles "_messageSentLoRaDevicesCount.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageSentGatewaysIpCount${2}_${3}.csv -i $(getAllFiles "_messageSentIpCount.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageSentGatewaysIpGatewaysCount${2}_${3}.csv -i $(getAllFiles "_messageSentIpGatewaysCount.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageSentGatewaysIpServerCount${2}_${3}.csv -i $(getAllFiles "_messageSentIpServerCount.csv" $3 false) &

python3 mergeCSV.py -r -I -o ${stats_directory}messageReceivedDevicesCount${2}_${3}.csv -i $(getAllFiles "_messageReceivedCount.csv" $2 true) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageReceivedGatewaysCount${2}_${3}.csv -i $(getAllFiles "_messageReceivedCount.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageReceivedGatewaysLoRaCount${2}_${3}.csv -i $(getAllFiles "_messageReceivedLoRaCount.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageReceivedGatewaysLoRaGatewaysCount${2}_${3}.csv -i $(getAllFiles "_messageReceivedLoRaGatewaysCount.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageReceivedGatewaysLoRaDevicesCount${2}_${3}.csv -i $(getAllFiles "_messageReceivedLoRaDevicesCount.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageReceivedGatewaysIpCount${2}_${3}.csv -i $(getAllFiles "_messageReceivedIpCount.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageReceivedGatewaysIpGatewaysCount${2}_${3}.csv -i $(getAllFiles "_messageReceivedIpGatewaysCount.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageReceivedGatewaysIpServerCount${2}_${3}.csv -i $(getAllFiles "_messageReceivedIpServerCount.csv" $3 false) &

python3 mergeCSV.py -r -I -o ${stats_directory}messageLostDevicesCount${2}_${3}.csv -i $(getAllFiles "_messageLostCount.csv" $2 true) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageLostGatewaysCount${2}_${3}.csv -i $(getAllFiles "_messageLostCount.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageLostGatewaysLoRaCount${2}_${3}.csv -i $(getAllFiles "_messageLostLoRaCount.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageLostGatewaysLoRaGatewaysCount${2}_${3}.csv -i $(getAllFiles "_messageLostLoRaGatewaysCount.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageLostGatewaysLoRaDevicesCount${2}_${3}.csv -i $(getAllFiles "_messageLostLoRaDevicesCount.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageLostGatewaysIpCount${2}_${3}.csv -i $(getAllFiles "_messageLostIpCount.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageLostGatewaysIpGatewaysCount${2}_${3}.csv -i $(getAllFiles "_messageLostIpGatewaysCount.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageLostGatewaysIpServerCount${2}_${3}.csv -i $(getAllFiles "_messageLostIpServerCount.csv" $3 false) &
'

python3 mergeCSV.py -r -I -o ${stats_directory}messageSentDevicesCount${2}_${3}.csv -i $(getAllFiles "_messageSent.csv" $2 true) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageSentGatewaysCount${2}_${3}.csv -i $(getAllFiles "_messageSent.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageSentGatewaysLoRaCount${2}_${3}.csv -i $(getAllFiles "_messageSentLoRa.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageSentGatewaysLoRaGatewaysCount${2}_${3}.csv -i $(getAllFiles "_messageSentLoRaGateways.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageSentGatewaysLoRaDevicesCount${2}_${3}.csv -i $(getAllFiles "_messageSentLoRaDevices.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageSentGatewaysIpCount${2}_${3}.csv -i $(getAllFiles "_messageSentIp.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageSentGatewaysIpGatewaysCount${2}_${3}.csv -i $(getAllFiles "_messageSentIpGateways.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageSentGatewaysIpServerCount${2}_${3}.csv -i $(getAllFiles "_messageSentIpServer.csv" $3 false) &

python3 mergeCSV.py -r -I -o ${stats_directory}messageReceivedDevicesCount${2}_${3}.csv -i $(getAllFiles "_messageReceived.csv" $2 true) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageReceivedGatewaysCount${2}_${3}.csv -i $(getAllFiles "_messageReceived.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageReceivedGatewaysLoRaCount${2}_${3}.csv -i $(getAllFiles "_messageReceivedLoRa.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageReceivedGatewaysLoRaGatewaysCount${2}_${3}.csv -i $(getAllFiles "_messageReceivedLoRaGateways.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageReceivedGatewaysLoRaDevicesCount${2}_${3}.csv -i $(getAllFiles "_messageReceivedLoRaDevices.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageReceivedGatewaysIpCount${2}_${3}.csv -i $(getAllFiles "_messageReceivedIp.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageReceivedGatewaysIpGatewaysCount${2}_${3}.csv -i $(getAllFiles "_messageReceivedIpGateways.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageReceivedGatewaysIpServerCount${2}_${3}.csv -i $(getAllFiles "_messageReceivedIpServer.csv" $3 false) &

python3 mergeCSV.py -r -I -o ${stats_directory}messageLostDevicesCount${2}_${3}.csv -i $(getAllFiles "_messageLost.csv" $2 true) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageLostGatewaysCount${2}_${3}.csv -i $(getAllFiles "_messageLost.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageLostGatewaysLoRaCount${2}_${3}.csv -i $(getAllFiles "_messageLostLoRa.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageLostGatewaysLoRaGatewaysCount${2}_${3}.csv -i $(getAllFiles "_messageLostLoRaGateways.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageLostGatewaysLoRaDevicesCount${2}_${3}.csv -i $(getAllFiles "_messageLostLoRaDevices.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageLostGatewaysIpCount${2}_${3}.csv -i $(getAllFiles "_messageLostIp.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageLostGatewaysIpGatewaysCount${2}_${3}.csv -i $(getAllFiles "_messageLostIpGateways.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageLostGatewaysIpServerCount${2}_${3}.csv -i $(getAllFiles "_messageLostIpServer.csv" $3 false) &


# Wait parallel executions end
wait

#==== STATS ABOUT TOTAL NUMBER OF MESSAGES ====
out_directory="${stats_directory}total_messages/"
printf "\nPlotting data into $out_directory\n"

# Total number of sent, received and lost messages by devices
python3 plotData.py Sent,\ Received\ and\ Lost\ device\ messages\ \(sum\) s -I -o $out_directory/messagesSentReceivedAndLostDevicesCountScattered.pdf -l sent received lost -i ${stats_directory}messageSentDevicesCount${2}_${3}.csv ${stats_directory}messageReceivedDevicesCount${2}_${3}.csv ${stats_directory}messageLostDevicesCount${2}_${3}.csv &
python3 plotData.py Sent,\ Received\ and\ Lost\ device\ messages\ \(sum\) l -I -o $out_directory/messagesSentReceivedAndLostDevicesCountLine.pdf -l sent received lost -i ${stats_directory}messageSentDevicesCount${2}_${3}.csv ${stats_directory}messageReceivedDevicesCount${2}_${3}.csv ${stats_directory}messageLostDevicesCount${2}_${3}.csv &

python3 plotData.py Sent,\ Received\ and\ Lost\ device\ messages b -I -o $out_directory/messagesSentReceivedAndLostDevicesBar.pdf -l sent received lost -g devices -i ${stats_directory}messageSentDevicesCount${2}_${3}.csv ${stats_directory}messageReceivedDevicesCount${2}_${3}.csv ${stats_directory}messageLostDevicesCount${2}_${3}.csv &


# Total number of sent, received and lost messages by gateways
python3 plotData.py Sent,\ Received\ and\ Lost\ gateway\ messages\ \(sum\) s -I -o $out_directory/messagesSentReceivedAndLostGatewaysCountScattered.pdf -l sent received lost -i ${stats_directory}messageSentGatewaysCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysCount${2}_${3}.csv ${stats_directory}messageLostGatewaysCount${2}_${3}.csv &
python3 plotData.py Sent,\ Received\ and\ Lost\ gateway\ messages\ \(sum\) l -I -o $out_directory/messagesSentReceivedAndLostGatewaysCountLine.pdf -l sent received lost -i ${stats_directory}messageSentGatewaysCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysCount${2}_${3}.csv ${stats_directory}messageLostGatewaysCount${2}_${3}.csv &

python3 plotData.py Sent,\ Received\ and\ Lost\ gateway\ messages\ \(sum\) s -I -o $out_directory/messagesSentReceivedAndLostLoRaGatewaysCountScattered.pdf -l sent\ LoRa received\ LoRa lost\ LoRa -i ${stats_directory}messageSentGatewaysLoRaCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysLoRaCount${2}_${3}.csv ${stats_directory}messageLostGatewaysLoRaCount${2}_${3}.csv &
python3 plotData.py Sent,\ Received\ and\ Lost\ gateway\ messages\ \(sum\) l -I -o $out_directory/messagesSentReceivedAndLostLoRaGatewaysCountLine.pdf -l sent\ LoRa received\ LoRa lost\ LoRa -i ${stats_directory}messageSentGatewaysLoRaCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysLoRaCount${2}_${3}.csv ${stats_directory}messageLostGatewaysLoRaCount${2}_${3}.csv &

python3 plotData.py Sent,\ Received\ and\ Lost\ gateway\ messages\ \(sum\) s -I -o $out_directory/messagesSentReceivedAndLostIpGatewaysCountScattered.pdf -l sent\ IP received\ IP lost\ IP -i ${stats_directory}messageSentGatewaysIpCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysIpCount${2}_${3}.csv ${stats_directory}messageLostGatewaysIpCount${2}_${3}.csv &
python3 plotData.py Sent,\ Received\ and\ Lost\ gateway\ messages\ \(sum\) l -I -o $out_directory/messagesSentReceivedAndLostIpGatewaysCountLine.pdf -l sent\ IP received\ IP lost\ IP -i ${stats_directory}messageSentGatewaysIpCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysIpCount${2}_${3}.csv ${stats_directory}messageLostGatewaysIpCount${2}_${3}.csv &

python3 plotData.py Sent,\ Received\ and\ Lost\ gateway\ messages b -I -o $out_directory/messagesSentReceivedAndLostGatewaysBar.pdf -l sent received lost -g gateways -i ${stats_directory}messageSentGatewaysCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysCount${2}_${3}.csv ${stats_directory}messageLostGatewaysCount${2}_${3}.csv &
python3 plotData.py Sent,\ Received\ and\ Lost\ gateway\ messages b -I -o $out_directory/messagesSentReceivedAndLostLoRaGatewaysBar.pdf -l sent\ LoRa received\ LoRa lost\ LoRa -g gateways -i ${stats_directory}messageSentGatewaysLoRaCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysLoRaCount${2}_${3}.csv ${stats_directory}messageLostGatewaysLoRaCount${2}_${3}.csv &
python3 plotData.py Sent,\ Received\ and\ Lost\ gateway\ messages b -I -o $out_directory/messagesSentReceivedAndLostIpGatewaysBar.pdf -l sent\ IP received\ IP lost\ IP -g gateways -i ${stats_directory}messageSentGatewaysIpCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysIpCount${2}_${3}.csv ${stats_directory}messageLostGatewaysIpCount${2}_${3}.csv &


# Total number of sent, received and lost uplinks
python3 plotData.py Sent,\ Received\ and\ Lost\ uplinks\ \(sum\) s -I -o $out_directory/messagesSentReceivedAndLostUplinksCountScattered.pdf -l sent received lost -i ${stats_directory}messageSentDevicesCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysLoRaDevicesCount${2}_${3}.csv ${stats_directory}messageLostGatewaysLoRaDevicesCount${2}_${3}.csv &
python3 plotData.py Sent,\ Received\ and\ Lost\ uplinks\ \(sum\) l -I -o $out_directory/messagesSentReceivedAndLostUplinksCountLine.pdf -l sent received lost -i ${stats_directory}messageSentDevicesCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysLoRaDevicesCount${2}_${3}.csv ${stats_directory}messageLostGatewaysLoRaDevicesCount${2}_${3}.csv &

python3 plotData.py Sent,\ Received\ and\ Lost\ uplinks b -I -o $out_directory/messagesSentReceivedAndLostUplinksBar.pdf -l sent received lost -g uplinks -i ${stats_directory}messageSentDevicesCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysLoRaDevicesCount${2}_${3}.csv ${stats_directory}messageLostGatewaysLoRaDevicesCount${2}_${3}.csv &

# Total number of sent, received and lost downlinks to devices
python3 plotData.py Sent,\ Received\ and\ Lost\ downlinks\ \(sum\) s -I -o $out_directory/messagesSentReceivedAndLostDownlinksForDevicesCountScattered.pdf -l sent received lost -i ${stats_directory}messageSentGatewaysLoRaDevicesCount${2}_${3}.csv ${stats_directory}messageReceivedDevicesCount${2}_${3}.csv ${stats_directory}messageLostDevicesCount${2}_${3}.csv &
python3 plotData.py Sent,\ Received\ and\ Lost\ downlinks\ \(sum\) l -I -o $out_directory/messagesSentReceivedAndLostDownlinksForDevicesCountLine.pdf -l sent received lost -i ${stats_directory}messageSentGatewaysLoRaDevicesCount${2}_${3}.csv ${stats_directory}messageReceivedDevicesCount${2}_${3}.csv ${stats_directory}messageLostDevicesCount${2}_${3}.csv &

python3 plotData.py Sent,\ Received\ and\ Lost\ downlinks b -I -o $out_directory/messagesSentReceivedAndLostDownlinksForDevicesBar.pdf -l sent received lost -g downlinks\ for\ devices -i ${stats_directory}messageSentGatewaysLoRaDevicesCount${2}_${3}.csv ${stats_directory}messageReceivedDevicesCount${2}_${3}.csv ${stats_directory}messageLostDevicesCount${2}_${3}.csv &

# Total number of sent, received and lost downlinks to gateways
python3 plotData.py Sent,\ Received\ and\ Lost\ downlinks\ \(sum\) s -I -o $out_directory/messagesSentReceivedAndLostDownlinksForGatewaysCountScattered.pdf -l sent received lost -i ${stats_directory}messageSentGatewaysLoRaGatewaysCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysLoRaGatewaysCount${2}_${3}.csv ${stats_directory}messageLostGatewaysLoRaGatewaysCount${2}_${3}.csv &
python3 plotData.py Sent,\ Received\ and\ Lost\ downlinks\ \(sum\) l -I -o $out_directory/messagesSentReceivedAndLostDownlinksForGatewaysCountLine.pdf -l sent received lost -i ${stats_directory}messageSentGatewaysLoRaGatewaysCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysLoRaGatewaysCount${2}_${3}.csv ${stats_directory}messageLostGatewaysLoRaGatewaysCount${2}_${3}.csv &

python3 plotData.py Sent,\ Received\ and\ Lost\ downlinks b -I -o $out_directory/messagesSentReceivedAndLostDownlinksForGatewaysBar.pdf -l sent received lost -g downlinks\ for\ gateways -i ${stats_directory}messageSentGatewaysLoRaGatewaysCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysLoRaGatewaysCount${2}_${3}.csv ${stats_directory}messageLostGatewaysLoRaGatewaysCount${2}_${3}.csv &

# Total number of sent, received and lost IP messages to gateways
python3 plotData.py Sent,\ Received\ and\ Lost\ IP\ gateway\ messages\ \(sum\) s -I -o $out_directory/messagesSentReceivedAndLostIpGatewaysForGatewaysCountScattered.pdf -l sent received lost -i ${stats_directory}messageSentGatewaysIpGatewaysCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysIpGatewaysCount${2}_${3}.csv ${stats_directory}messageLostGatewaysIpGatewaysCount${2}_${3}.csv &
python3 plotData.py Sent,\ Received\ and\ Lost\ IP\ gateway\ messages\ \(sum\) l -I -o $out_directory/messagesSentReceivedAndLostIpGatewaysForGatewaysCountLine.pdf -l sent received lost -i ${stats_directory}messageSentGatewaysIpGatewaysCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysIpGatewaysCount${2}_${3}.csv ${stats_directory}messageLostGatewaysIpGatewaysCount${2}_${3}.csv &

python3 plotData.py Sent,\ Received\ and\ Lost\ IP\ gateway\ messages b -I -o $out_directory/messagesSentReceivedAndLostIpGatewaysForGatewaysBar.pdf -l sent received lost -g IP\ messages\ to\ gateways -i ${stats_directory}messageSentGatewaysIpGatewaysCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysIpGatewaysCount${2}_${3}.csv ${stats_directory}messageLostGatewaysIpGatewaysCount${2}_${3}.csv &

# Total number of sent, received and lost IP messages to server
python3 plotData.py Sent,\ Received\ and\ Lost\ IP\ gateway\ messages\ \(sum\) s -I -o $out_directory/messagesSentReceivedAndLostIpGatewaysForServerCountScattered.pdf -l sent received lost -i ${stats_directory}messageSentGatewaysIpServerCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysIpServerCount${2}_${3}.csv ${stats_directory}messageLostGatewaysIpServerCount${2}_${3}.csv &
python3 plotData.py Sent,\ Received\ and\ Lost\ IP\ gateway\ messages\ \(sum\) l -I -o $out_directory/messagesSentReceivedAndLostIpGatewaysForServerCountLine.pdf -l sent received lost -i ${stats_directory}messageSentGatewaysIpServerCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysIpServerCount${2}_${3}.csv ${stats_directory}messageLostGatewaysIpServerCount${2}_${3}.csv &

python3 plotData.py Sent,\ Received\ and\ Lost\ IP\ gateway\ messages b -I -o $out_directory/messagesSentReceivedAndLostIpGatewaysForServerBar.pdf -l sent received lost -g IP\ messages\ to\ server -i ${stats_directory}messageSentGatewaysIpServerCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysIpServerCount${2}_${3}.csv ${stats_directory}messageLostGatewaysIpServerCount${2}_${3}.csv &


# Wait parallel executions end
wait

#printf "\nAll charts are plotted. Bye :)\n"


#++++++++++++++++++++++++++++++ DATA ++++++++++++++++++++++++++++++
printf "\nTrying to merge some CSV files to obtain total data messages\n"

python3 mergeCSV.py -r -I -o ${stats_directory}messageSentDevicesDataCount${2}_${3}.csv -i $(getAllFiles "_messageSentData.csv" $2 true) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageSentGatewaysDataCount${2}_${3}.csv -i $(getAllFiles "_messageSentData.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageSentGatewaysIpGatewaysDataCount${2}_${3}.csv -i $(getAllFiles "_messageSentGatewaysData.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageSentGatewaysIpServerDataCount${2}_${3}.csv -i $(getAllFiles "_messageSentServerData.csv" $3 false) &

python3 mergeCSV.py -r -I -o ${stats_directory}messageReceivedGatewaysDataCount${2}_${3}.csv -i $(getAllFiles "_messageReceivedData.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageReceivedGatewaysLoRaDataCount${2}_${3}.csv -i $(getAllFiles "_messageReceivedDevicesData.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageReceivedGatewaysIpGatewaysDataCount${2}_${3}.csv -i $(getAllFiles "_messageReceivedGatewaysData.csv" $3 false) &
python3 mergeCSV.py -r -I -o ${stats_directory}messageReceivedGatewaysIpServerDataCount${2}_${3}.csv -i $(getAllFiles "_messageReceivedServerData.csv" $3 false) &

python3 mergeCSV.py -r -I -o ${stats_directory}messageLostGatewaysDataCount${2}_${3}.csv -i $(getAllFiles "_messageLostData.csv" $3 false) &

# Wait parallel executions end
wait

#==== STATS ABOUT TOTAL NUMBER OF MESSAGES ====
out_directory="${stats_directory}total_messages/"
printf "\nPlotting data into $out_directory\n"


# Total number of sent messages by devices
#python3 plotData.py Sent\ device\ data\ messages\ \(sum\) s -I -o $out_directory/messagesSentDevicesDataCountScattered.pdf -l sent -i ${stats_directory}messageSentDevicesDataCount${2}_${3}.csv &
#python3 plotData.py Sent\ device\ data\ messages\ \(sum\) l -I -o $out_directory/messagesSentDevicesDataCountLine.pdf -l sent -i ${stats_directory}messageSentDevicesDataCount${2}_${3}.csv &

#python3 plotData.py Sent\ device\ data\ messages b -I -o $out_directory/messagesSentDevicesDataBar.pdf -l sent -g devices -i ${stats_directory}messageSentDevicesDataCount${2}_${3}.csv &


# Total number of sent, received and lost messages by gateways
python3 plotData.py Sent,\ Received\ and\ Lost\ gateway\ data\ messages\ \(sum\) s -I -o $out_directory/messagesSentReceivedAndLostGatewaysDataCountScattered.pdf -l sent received lost -i ${stats_directory}messageSentGatewaysDataCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysDataCount${2}_${3}.csv ${stats_directory}messageLostGatewaysDataCount${2}_${3}.csv &
python3 plotData.py Sent,\ Received\ and\ Lost\ gateway\ data\ messages\ \(sum\) l -I -o $out_directory/messagesSentReceivedAndLostGatewaysDataCountLine.pdf -l sent received lost -i ${stats_directory}messageSentGatewaysDataCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysDataCount${2}_${3}.csv ${stats_directory}messageLostGatewaysDataCount${2}_${3}.csv &

python3 plotData.py Sent\ and\ Received\ gateway\ data\ messages\ \(sum\) s -I -o $out_directory/messagesSentAndReceivedIpGatewaysForGatewaysDataCountScattered.pdf -l sent\ IP received\ IP -i ${stats_directory}messageSentGatewaysIpGatewaysDataCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysIpGatewaysDataCount${2}_${3}.csv &
python3 plotData.py Sent\ and\ Received\ gateway\ data\ messages\ \(sum\) l -I -o $out_directory/messagesSentAndReceivedIpGatewaysForGatewaysDataCountLine.pdf -l sent\ IP received\ IP -i ${stats_directory}messageSentGatewaysIpGatewaysDataCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysIpGatewaysDataCount${2}_${3}.csv &

python3 plotData.py Sent\ and\ Received\ gateway\ data\ messages\ \(sum\) s -I -o $out_directory/messagesSentAndReceivedIpGatewaysForServerDataCountScattered.pdf -l sent\ IP received\ IP -i ${stats_directory}messageSentGatewaysIpServerDataCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysIpServerDataCount${2}_${3}.csv &
python3 plotData.py Sent\ and\ Received\ gateway\ data\ messages\ \(sum\) l -I -o $out_directory/messagesSentAndReceivedIpGatewaysForServerDataCountLine.pdf -l sent\ IP received\ IP -i ${stats_directory}messageSentGatewaysIpServerDataCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysIpServerDataCount${2}_${3}.csv &

python3 plotData.py Sent,\ Received\ and\ Lost\ gateway\ data\ messages b -I -o $out_directory/messagesSentReceivedAndLostGatewaysDataBar.pdf -l sent received lost -g gateways -i ${stats_directory}messageSentGatewaysDataCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysDataCount${2}_${3}.csv ${stats_directory}messageLostGatewaysDataCount${2}_${3}.csv &
python3 plotData.py Sent\ and\ Received\ gateway\ data\ messages b -I -o $out_directory/messagesSentAndReceivedIpGatewaysForGatewaysDataBar.pdf -l sent received -g IP\ messages\ to\ gateways -i ${stats_directory}messageSentGatewaysIpGatewaysDataCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysIpGatewaysDataCount${2}_${3}.csv &
python3 plotData.py Sent\ and\ Received\ gateway\ data\ messages b -I -o $out_directory/messagesSentAndReceivedIpGatewaysForServerDataBar.pdf -l sent received -g IP\ messages\ to\ server -i ${stats_directory}messageSentGatewaysIpServerDataCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysIpServerDataCount${2}_${3}.csv &


# Total number of sent, received and lost uplinks
python3 plotData.py Sent\ and\ Received\ uplinks\ \(sum\) s -I -o $out_directory/messagesSentAndReceivedUplinksDataCountScattered.pdf -l sent received -i ${stats_directory}messageSentDevicesDataCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysLoRaDataCount${2}_${3}.csv &
python3 plotData.py Sent\ and\ Received\ uplinks\ \(sum\) l -I -o $out_directory/messagesSentAndReceivedUplinksDataCountLine.pdf -l sent received -i ${stats_directory}messageSentDevicesDataCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysLoRaDataCount${2}_${3}.csv &

python3 plotData.py Sent\ and\ Received\ uplinks b -I -o $out_directory/messagesSentAndReceivedUplinksDataBar.pdf -l sent received -g uplinks -i ${stats_directory}messageSentDevicesDataCount${2}_${3}.csv ${stats_directory}messageReceivedGatewaysLoRaDataCount${2}_${3}.csv &
#==============================================
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

# Wait parallel executions end
wait

printf "\nAll charts are plotted. Bye :)\n"