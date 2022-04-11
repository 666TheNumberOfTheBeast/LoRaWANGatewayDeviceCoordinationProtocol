import pandas as pd
import numpy as np
#import sys
import argparse

from pathlib import Path
from datetime import datetime

'''
if len(sys.argv) < 2:
    sys.exit("No file to open!\nUsage: python3 " + sys.argv[0] + " <filename.csv> [<out_directory_parent>]")

# URL to .csv file
data_url = sys.argv[1]

# Check if the directory where to put results is provided
if len(sys.argv) == 3:
    # Check if the final "/" is included in the string
    out_directory_parent = sys.argv[2]
    if out_directory_parent[-1] != "/":
        out_directory_parent += "/"
'''

parser = argparse.ArgumentParser()
parser.add_argument("filename", type=str, help="The name of the file with exported data")
parser.add_argument("-o", type=str, help="The output directory parent")
group = parser.add_mutually_exclusive_group()
group.add_argument("-m", action="store_true", help="Compute mean of vectors referring to the same stats")
group.add_argument("-M", action="store_true", help="Compute median of vectors referring to the same stats")
args = parser.parse_args()

# URL to .csv file
data_url = args.filename

# Get out directory parent
out_directory_parent = "./"
if args.o:
    out_directory_parent = args.o

    # Check if the final "/" is included in the string
    if out_directory_parent[-1] != "/":
        out_directory_parent += "/"

# Get compute mean option
compute_mean = args.m

# Get compute mean option
compute_median = args.M


#pd.set_option('display.max_rows', None)

# Read input file
df = pd.read_csv(data_url)

# Get columns of interest
df = df[["module", "name", "vectime", "vecvalue"]]
#print(df)

# Drop rows having a null value
df = df.dropna()
#print(df)

#print("\n")
#print(df.dtypes)
#print(df.shape)


# Create directory path
if compute_mean:
    out_directory = out_directory_parent + "mean_output_csv/"
elif compute_median:
    out_directory = out_directory_parent + "median_output_csv/"
else:
    # Get current time
    now = datetime.now()

    # Convert to yyyy mm dd HH MM SS
    strtime = now.strftime("%Y%m%d_%H%M%S")

    out_directory = out_directory_parent + "output_csv_" + strtime + "/"

# Create directory
Path(out_directory).mkdir()
#print("Files will be saved into " + out_directory)


# Create a CSV from the row
def toCSV(row):
    if compute_mean or compute_median:
        times  = row["vectime"]
        values = row["vecvalue"]
    else:
        times  = row["vectime"].split()
        values = row["vecvalue"].split()


    out = pd.DataFrame(data={"vectime": times, "vecvalue": values})

    module = row["module"].split(".")
    stat   = row["name"].split(":")
    filename = out_directory + module[-1] + "_" + stat[0] + ".csv"

    out.to_csv(filename, index=False)

    #print(filename + " saved into " + out_directory)

#df.apply(toCSV, axis=1)

'''
# Convert the time and value strings to lists
def toArray(row):
    row["vectime"]  = row["vectime"].split()
    row["vecvalue"] = row["vecvalue"].split()

    return row

df = df.apply(toArray, axis=1)
print(df)'''



if compute_mean or compute_median:
    #max_len = 0

    # Convert the time and value strings to lists and get the max len
    def toArray(row):
        #global max_len
        
        row["vectime"]  = [float(x) for x in row["vectime"].split()]
        row["vecvalue"] = [float(x) for x in row["vecvalue"].split()]

        #array_len = len(row["vectime"])
        #if array_len > max_len:
        #    max_len = array_len

        return row

    df = df.apply(toArray, axis=1)
    #print(df)

    #print("\nmax len:", max_len)


    # Get the length of every list
    df["len"] = df["vectime"].apply(len)
    #print(df)

    # Get the max length per group
    max_lens = df.groupby(["module", "name"])["len"].max()

    # Join the max length with the general data frame
    df = df.merge(max_lens.to_frame("max_len"), left_on=["module", "name"], right_on=["module", "name"])
    #print(df)


    # Pad lists of time and value
    def padArray(row):
        #pad_len = max_len - len(row["vectime"])
        pad_len = row["max_len"] - len(row["vectime"])

        # Pad with zeros (bad idea for calculating the mean)
        #padding = [0 for i in range(pad_len)]
        #row["vectime"]  = np.array( row["vectime"] + padding )
        #row["vecvalue"] = np.array( row["vecvalue"] + padding )

        # Pad with last recorded value to keep the line constant
        pad_time = row["vectime"][-1]
        padding_time = [pad_time for _ in range(pad_len)]
        pad_value = row["vecvalue"][-1]
        padding_value = [pad_value for _ in range(pad_len)]

        row["vectime"]  = np.array( row["vectime"] + padding_time )
        row["vecvalue"] = np.array( row["vecvalue"] + padding_value )
        
        return row

    df = df.apply(padArray, axis=1)
    #print(df)


    # Get group sizes
    df_count = df.groupby(["module", "name"]).size()

    # Join the group sizes with the general data frame
    df = df.merge(df_count.to_frame("count"), left_on=["module", "name"], right_on=["module", "name"])
    #print(df)

    # Get the max size of the groups
    max_group_size = df["count"].max()
    #print("max_group_size:", max_group_size)

    # Fill a group with missing rows
    missing_rows = []
    #cols = [col for col in df.columns]
    def fillGroup(row):
        global missing_rows

        max_len = row["max_len"]
        for i in range(max_group_size - row["count"]):
            missing_rows.append([row["module"], row["name"], [0 for _ in range(max_len)], [0 for _ in range(max_len)], max_len, max_len, max_group_size])

        row["count"] = max_group_size
        return row

    # Filter groups with less than max_group_size items and fill it with missing items
    df.groupby(["module", "name"]).filter(lambda x: len(x) < max_group_size).apply(fillGroup, axis=1)

    # Append missing rows to the general data frame
    df = pd.concat([df, pd.DataFrame(missing_rows, columns=df.columns)])





    #prova = df[(df["module"] == "LoRaNetworkServerEdgeRealScenarioTest.gateways[0]") & (df["name"] == "cpu:vector")]
    #prova = df[(df["module"] == "LoRaNetworkServerEdgeRealScenarioTest.gateways[0]") & (df["name"] == "connected:vector")]
    #prova = df[(df["module"] == "LoRaNetworkServerEdgeRealScenarioTest.gateways[0]") & (df["name"] == "messageLostCount:vector")]
    #prova = df[(df["module"] == "LoRaNetworkServerEdgeRealScenarioTest.endDevices[6]") & (df["name"] == "messageReceivedCount:vector")]
    #prova = df[(df["module"] == "LoRaNetworkServerEdgeRealScenarioTest.endDevices[0]") & (df["name"] == "messageReceivedCount:vector")]
    #prova = df[(df["module"] == "LoRaNetworkServerEdgeRealScenarioTest.endDevices[0]") & (df["name"] == "messageSentCount:vector")]
    #prova = df[(df["module"] == "LoRaNetworkServerEdgeRealScenarioTest.endDevices[0]") & (df["name"] == "messageRetransmittedCount:vector")]
    #print(prova)

    #pd.set_option('display.max_rows', None)

    # Calculate mean or median of times and values
    op = np.mean if compute_mean else np.median
    times_mean  = df.groupby(["module", "name"])["vectime"].apply(op).reset_index()
    values_mean = df.groupby(["module", "name"])["vecvalue"].apply(op).reset_index()

    # Join the results
    df = times_mean.merge(values_mean, on=["module", "name"])
    #print(df)

    '''
    print("\n")
    print(prova)
    print("\n")
    #p = df[(df["module"] == "LoRaNetworkServerEdgeRealScenarioTest.gateways[0]") & (df["name"] == "messageLostCount:vector")]
    #p = df[(df["module"] == "LoRaNetworkServerEdgeRealScenarioTest.endDevices[6]") & (df["name"] == "messageReceivedCount:vector")]
    #p = df[(df["module"] == "LoRaNetworkServerEdgeRealScenarioTest.endDevices[0]") & (df["name"] == "messageReceivedCount:vector")]
    #p = df[(df["module"] == "LoRaNetworkServerEdgeRealScenarioTest.endDevices[0]") & (df["name"] == "messageSentCount:vector")]
    p = df[(df["module"] == "LoRaNetworkServerEdgeRealScenarioTest.endDevices[0]") & (df["name"] == "messageRetransmittedCount:vector")]
    print(p)

    print("\nvectimes group:")
    prova["vectime"].apply(print)
    print("\nvecvalues group:")
    prova["vecvalue"].apply(print)
    
    print("\nvectimes mean:")
    p["vectime"].apply(print)
    print("\nvecvalues mean:")
    p["vecvalue"].apply(print)
    '''



    '''
    # Filter rows
    prova = df[(df["module"] == "LoRaNetworkServerEdgeRealScenarioTest.gateways[0]") & (df["name"] == "cpu:vector")]
    print(prova)

    #prova["vectime"]  = prova["vectime"].apply(len)
    #prova["vecvalue"] = prova["vecvalue"].apply(len)
    #print(prova)

    print("\nmax len:", max_len)

    # Pad lists of time and value with zeros
    def padArray(row):
        #print("vectime len:", len(row["vectime"]))
        #print("vecvalue len:", len(row["vecvalue"]))

        pad_len = max_len - len(row["vectime"])
        padding = [0 for i in range(pad_len)] 

        row["vectime"]  = np.array( row["vectime"] + padding )
        row["vecvalue"] = np.array( row["vecvalue"] + padding )

        #print("vectime padded len:", len(row["vectime"]))
        #print("vecvalue padded len:", len(row["vecvalue"]))
        #print(row["vectime"])
        #print(row["vecvalue"])

        #row["vectime"]  = np.array([0,1,2])
        #row["vecvalue"] = np.array([1,2,3])

        #a = [np.random.randint(0, 10, 10)]
        #print(a)
        
        return row

    prova = prova.apply(padArray, axis=1)
    print(prova)

    #prova = prova.groupby(["module", "name"])[["vectime", "vecvalue"]].apply(np.mean)
    #times_mean  = prova.groupby(["module", "name"])["vectime"].apply(np.mean).to_frame()
    #values_mean = prova.groupby(["module", "name"])["vecvalue"].apply(np.mean).to_frame()
    times_mean  = prova.groupby(["module", "name"])["vectime"].apply(np.mean).reset_index()
    values_mean = prova.groupby(["module", "name"])["vecvalue"].apply(np.mean).reset_index()
    #print(times_mean)
    #print(values_mean)
    times_mean = times_mean.merge(values_mean, on=["module", "name"])
    print(times_mean)
    '''


df.apply(toCSV, axis=1)