#!/bin/bash

if [ ! $# -eq 2 ]
then
    printf "No directory where searching simulation executions or number of gateways missing!\nUsage: ${0} <directory_path> <number_gateways>\n"
    exit 1
fi

if [ $2 -lt 0 ]
then
    printf "Negative number!\nUsage: ${0} <directory_path> <number_gateways>\n"
    exit 1
fi

# Exit on error
set -e


directory_path=$1

# Append "/" to the directory path if not already present
if [ ! ${1: -1} = "/" ]
then
    directory_path+="/"
fi


# Get a directory per simulation execution 
directory_simulations=$(ls $directory_path)
exported_data_files=()

# Get an exported data file per directory referring to a simulation execution
for dir in $directory_simulations
do
    #echo $dir
    exported_data_files+=($directory_path$dir/export.csv)
done


#echo ${#exported_data_files[@]}
#echo ${exported_data_files[@]}

# Check if at least two exported data files are found
if [ ${#exported_data_files[@]} -lt 2 ]
then
    printf "No exported data file or less than two!\n"
    exit 1
fi

# Merge exported data files
printf "\nTrying to merge exported data file into ${directory_path}merged_executions.csv\n"
out_export_file="${directory_path}merged_executions.csv"
python3 mergeCSV.py -I -o $out_export_file -i ${exported_data_files[@]}

# Get current datetime as yyyy mm dd HH MM SS
strtime=$(date "+%Y%m%d_%H%M%S")

# Create directory that will contain the mean of exported data
out_directory="${directory_path}mean_exported_data_${strtime}/"
#out_directory="${directory_path}median_exported_data_${strtime}/"
mkdir $out_directory

# Export data from the merged CSV file to obtain a mean CSV file (-m) or median CSV file (-M) for each stats vector 
printf "\nSplitting merged exported data in many CSV files\n"
python3 splitExportedData.py $out_export_file -o $out_directory -m
#python3 splitExportedData.py $out_export_file -o $out_directory -M

# Get nested directory name where the multiple CSV files are stored
stats_directory=$(find $out_directory -mindepth 1 -maxdepth 1 -type d)
echo "Data splitted into $stats_directory"

# Append "/" to the directory path if not already present
if [ ! ${stats_directory: -1} = "/" ]
then
    stats_directory+="/"
fi

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

# Wait parallel executions end
wait

printf "\n"

# Plot data
./plotData.sh $stats_directory $2