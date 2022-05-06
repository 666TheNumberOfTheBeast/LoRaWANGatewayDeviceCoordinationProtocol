#!/bin/bash

printf "###########################################\n########## OMNeT++ data exporter ##########\n###########################################\n"

if [ ! $# -eq 2 ]
then
    printf "No file to open or number of gateways missing!\nUsage: ${0} <filename.vec> <number_gateways>\n"
    exit 1
fi

if [ $2 -lt 0 ]
then
    printf "Negative number!\nUsage: ${0} <filename.vec> <number_gateways>\n"
    exit 1
fi

# Exit on error
set -e

# Get current datetime as yyyy mm dd HH MM SS
strtime=$(date "+%Y%m%d_%H%M%S")

# Create directory that will contain exported data
out_directory="exported_data_${strtime}/"
mkdir $out_directory

# Export data into a single CSV file in the new directory
out_export_file="${out_directory}export.csv"
printf "\nExporting data into $out_export_file\n"
opp_scavetool x $1 -o $out_export_file

# Export data from the new CSV file to obtain a CSV file for each stats vector 
printf "\nSplitting exported data in many CSV files\n"
python3 splitExportedData.py $out_export_file -o $out_directory

# Get nested directory name where the multiple CSV files are stored
stats_directory=$(find $out_directory -mindepth 1 -maxdepth 1 -type d)
echo "Data splitted into $stats_directory"

printf "\nTrying to merge some CSV files to obtain explicit files for implicit data\n"

# Append "/" to the directory path if not already present
if [ ! ${stats_directory: -1} = "/" ]
then
    stats_directory+="/"
fi

: '
# Create csv file merging received, lost and invalid messages to obtain IN messages ignoring missing files (here lost messages)
python3 mergeCSV.py -r -I -o ${stats_directory}gateways[0]_messageInCount.csv -i ${stats_directory}gateways[0]_messageReceivedCount.csv ${stats_directory}gateways[0]_messageLostCount.csv ${stats_directory}gateways[0]_messageInvalidCount.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}gateways[0]_messageInLoRaCount.csv -i ${stats_directory}gateways[0]_messageReceivedLoRaCount.csv ${stats_directory}gateways[0]_messageLostLoRaCount.csv ${stats_directory}gateways[0]_messageInvalidLoRaCount.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}gateways[0]_messageInIpCount.csv -i ${stats_directory}gateways[0]_messageReceivedIpCount.csv ${stats_directory}gateways[0]_messageLostIpCount.csv ${stats_directory}gateways[0]_messageInvalidIpCount.csv &

# Create csv file merging received, lost and invalid  messages to obtain IN messages ignoring missing files (here lost messages)
python3 mergeCSV.py -r -I -o ${stats_directory}gateways[1]_messageInCount.csv -i ${stats_directory}gateways[1]_messageReceivedCount.csv ${stats_directory}gateways[1]_messageLostCount.csv ${stats_directory}gateways[1]_messageInvalidCount.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}gateways[1]_messageInLoRaCount.csv -i ${stats_directory}gateways[1]_messageReceivedLoRaCount.csv ${stats_directory}gateways[1]_messageLostLoRaCount.csv ${stats_directory}gateways[1]_messageInvalidLoRaCount.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}gateways[1]_messageInIpCount.csv -i ${stats_directory}gateways[1]_messageReceivedIpCount.csv ${stats_directory}gateways[1]_messageLostIpCount.csv ${stats_directory}gateways[0]_messageInvalidIpCount.csv &

# Create csv file merging received, lost and invalid  messages to obtain IN messages ignoring missing files (here lost messages)
python3 mergeCSV.py -r -I -o ${stats_directory}gateways[2]_messageInCount.csv -i ${stats_directory}gateways[2]_messageReceivedCount.csv ${stats_directory}gateways[2]_messageLostCount.csv ${stats_directory}gateways[2]_messageInvalidCount.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}gateways[2]_messageInLoRaCount.csv -i ${stats_directory}gateways[2]_messageReceivedLoRaCount.csv ${stats_directory}gateways[2]_messageLostLoRaCount.csv ${stats_directory}gateways[2]_messageInvalidLoRaCount.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}gateways[2]_messageInIpCount.csv -i ${stats_directory}gateways[2]_messageReceivedIpCount.csv ${stats_directory}gateways[2]_messageLostIpCount.csv ${stats_directory}gateways[0]_messageInvalidIpCount.csv &

# Create csv file merging received, lost and invalid  messages to obtain IN messages ignoring missing files (here lost messages)
python3 mergeCSV.py -r -I -o ${stats_directory}endDevices[0]_messageInCount.csv -i ${stats_directory}endDevices[0]_messageReceivedCount.csv ${stats_directory}endDevices[0]_messageLostCount.csv ${stats_directory}endDevices[0]_messageInvalidCount.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}endDevices[1]_messageInCount.csv -i ${stats_directory}endDevices[1]_messageReceivedCount.csv ${stats_directory}endDevices[1]_messageLostCount.csv ${stats_directory}endDevices[1]_messageInvalidCount.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}endDevices[2]_messageInCount.csv -i ${stats_directory}endDevices[2]_messageReceivedCount.csv ${stats_directory}endDevices[2]_messageLostCount.csv ${stats_directory}endDevices[2]_messageInvalidCount.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}endDevices[3]_messageInCount.csv -i ${stats_directory}endDevices[3]_messageReceivedCount.csv ${stats_directory}endDevices[3]_messageLostCount.csv ${stats_directory}endDevices[3]_messageInvalidCount.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}endDevices[4]_messageInCount.csv -i ${stats_directory}endDevices[4]_messageReceivedCount.csv ${stats_directory}endDevices[4]_messageLostCount.csv ${stats_directory}endDevices[4]_messageInvalidCount.csv &
'

# Create csv file merging received, lost and invalid messages to obtain IN messages ignoring missing files (here lost and invalid messages)
python3 mergeCSV.py -r -I -o ${stats_directory}gateways[0]_messageInCount.csv -i ${stats_directory}gateways[0]_messageReceived.csv ${stats_directory}gateways[0]_messageLost.csv ${stats_directory}gateways[0]_messageInvalid.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}gateways[0]_messageInLoRaCount.csv -i ${stats_directory}gateways[0]_messageReceivedLoRa.csv ${stats_directory}gateways[0]_messageLostLoRa.csv ${stats_directory}gateways[0]_messageInvalidLoRa.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}gateways[0]_messageInIpCount.csv -i ${stats_directory}gateways[0]_messageReceivedIp.csv ${stats_directory}gateways[0]_messageLostIp.csv ${stats_directory}gateways[0]_messageInvalidIp.csv &

# Create csv file merging received, lost and invalid messages to obtain IN messages ignoring missing files (here lost and invalid messages)
python3 mergeCSV.py -r -I -o ${stats_directory}gateways[1]_messageInCount.csv -i ${stats_directory}gateways[1]_messageReceived.csv ${stats_directory}gateways[1]_messageLost.csv ${stats_directory}gateways[1]_messageInvalid.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}gateways[1]_messageInLoRaCount.csv -i ${stats_directory}gateways[1]_messageReceivedLoRa.csv ${stats_directory}gateways[1]_messageLostLoRa.csv ${stats_directory}gateways[1]_messageInvalidLoRa.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}gateways[1]_messageInIpCount.csv -i ${stats_directory}gateways[1]_messageReceivedIp.csv ${stats_directory}gateways[1]_messageLostIp.csv ${stats_directory}gateways[0]_messageInvalidIp.csv &

# Create csv file merging received, lost and invalid messages to obtain IN messages ignoring missing files (here lost and invalid messages)
python3 mergeCSV.py -r -I -o ${stats_directory}gateways[2]_messageInCount.csv -i ${stats_directory}gateways[2]_messageReceived.csv ${stats_directory}gateways[2]_messageLost.csv ${stats_directory}gateways[2]_messageInvalid.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}gateways[2]_messageInLoRaCount.csv -i ${stats_directory}gateways[2]_messageReceivedLoRa.csv ${stats_directory}gateways[2]_messageLostLoRa.csv ${stats_directory}gateways[2]_messageInvalidLoRa.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}gateways[2]_messageInIpCount.csv -i ${stats_directory}gateways[2]_messageReceivedIp.csv ${stats_directory}gateways[2]_messageLostIp.csv ${stats_directory}gateways[0]_messageInvalidIp.csv &

# Create csv file merging received, lost and invalid messages to obtain IN messages ignoring missing files (here lost and invalid messages)
python3 mergeCSV.py -r -I -o ${stats_directory}endDevices[0]_messageInCount.csv -i ${stats_directory}endDevices[0]_messageReceived.csv ${stats_directory}endDevices[0]_messageLost.csv ${stats_directory}endDevices[0]_messageInvalid.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}endDevices[1]_messageInCount.csv -i ${stats_directory}endDevices[1]_messageReceived.csv ${stats_directory}endDevices[1]_messageLost.csv ${stats_directory}endDevices[1]_messageInvalid.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}endDevices[2]_messageInCount.csv -i ${stats_directory}endDevices[2]_messageReceived.csv ${stats_directory}endDevices[2]_messageLost.csv ${stats_directory}endDevices[2]_messageInvalid.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}endDevices[3]_messageInCount.csv -i ${stats_directory}endDevices[3]_messageReceived.csv ${stats_directory}endDevices[3]_messageLost.csv ${stats_directory}endDevices[3]_messageInvalid.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}endDevices[4]_messageInCount.csv -i ${stats_directory}endDevices[4]_messageReceived.csv ${stats_directory}endDevices[4]_messageLost.csv ${stats_directory}endDevices[4]_messageInvalid.csv &


#++++++++++++++++++++++++++++++ DATA ++++++++++++++++++++++++++++++
# Create csv file merging received, lost and invalid data messages to obtain IN data messages ignoring missing files (here lost and invalid messages)
python3 mergeCSV.py -r -I -o ${stats_directory}gateways[0]_messageInDataCount.csv -i ${stats_directory}gateways[0]_messageReceivedData.csv ${stats_directory}gateways[0]_messageLostData.csv ${stats_directory}gateways[0]_messageInvalidData.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}gateways[1]_messageInDataCount.csv -i ${stats_directory}gateways[1]_messageReceivedData.csv ${stats_directory}gateways[1]_messageLostData.csv ${stats_directory}gateways[1]_messageInvalidData.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}endDevices[0]_messageInDataCount.csv -i ${stats_directory}endDevices[0]_messageReceivedData.csv ${stats_directory}endDevices[0]_messageLostData.csv ${stats_directory}endDevices[0]_messageInvalidData.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}endDevices[1]_messageInDataCount.csv -i ${stats_directory}endDevices[1]_messageReceivedData.csv ${stats_directory}endDevices[1]_messageLostData.csv ${stats_directory}endDevices[1]_messageInvalidData.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}endDevices[2]_messageInDataCount.csv -i ${stats_directory}endDevices[2]_messageReceivedData.csv ${stats_directory}endDevices[2]_messageLostData.csv ${stats_directory}endDevices[2]_messageInvalidData.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}endDevices[3]_messageInDataCount.csv -i ${stats_directory}endDevices[3]_messageReceivedData.csv ${stats_directory}endDevices[3]_messageLostData.csv ${stats_directory}endDevices[3]_messageInvalidData.csv &
python3 mergeCSV.py -r -I -o ${stats_directory}endDevices[4]_messageInDataCount.csv -i ${stats_directory}endDevices[4]_messageReceivedData.csv ${stats_directory}endDevices[4]_messageLostData.csv ${stats_directory}endDevices[4]_messageInvalidData.csv &
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


# Wait parallel executions end
wait

printf "\n"

# Plot data
./plotData.sh $stats_directory $2