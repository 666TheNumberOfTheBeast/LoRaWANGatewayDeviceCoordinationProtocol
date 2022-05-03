import pandas as pd
import numpy as np
import argparse

from pathlib import Path
from datetime import datetime


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
df = pd.read_csv(data_url, dtype={"vectime": object, "vecvalue": object})

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



if compute_mean or compute_median:
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
    #print("Filtered dataset cols converted to lists:\n", df)


    # Get the length of every list
    df["len"] = df["vectime"].apply(len)
    #print("\n\nAdded list lenghts:\n", df)

    # Get the max length per group
    max_lens = df.groupby(["module", "name"])["len"].max()

    # Join the max length with the general data frame
    df = df.merge(max_lens.to_frame("max_len"), left_on=["module", "name"], right_on=["module", "name"])
    #print("\n\nAdded max list lenghts per group:\n", df)


    # Pad lists of time and value
    def padArray(row):
        # Get the number of items to be appended as padding
        pad_len = row["max_len"] - len(row["vectime"])

        # Check if padding is not necessary
        if pad_len == 0:
            row["vectime"]  = np.array( row["vectime"] )
            row["vecvalue"] = np.array( row["vecvalue"] )
            return row

        # Calculate the mean of time intervals
        vectime_intervals_len = len(row["vectime"])-1

        # Check if the vector contains at least two values
        if vectime_intervals_len > 0:
            # Calculate the mean
            pad_time = 0
            for i in range(vectime_intervals_len):
                pad_time += row["vectime"][i+1] - row["vectime"][i]
                #print(pad_time)
            pad_time /= vectime_intervals_len
            #print("mean time interval:", pad_time)
        else:
            # Get the only available time
            pad_time = row["vectime"][-1]

        # Check if the row refers to a count
        if "Count" in row["name"]     or \
           "connected" in row["name"] or \
           "cpu" in row["name"]       or \
           "gpu" in row["name"]       or \
           "ram" in row["name"]       or \
           "storage" in row["name"]:
            # Pad with last recorded value to keep the line constant
            pad_value = row["vecvalue"][-1]
        else:
            # Pad with zero to denote no more sending
            pad_value = 0

        padding_time = [row["vectime"][-1]+(i+1)*pad_time for i in range(pad_len)]
        padding_value = [pad_value for _ in range(pad_len)]




        # Pad with zeros (bad idea for calculating the mean)
        #padding = [0 for _ in range(pad_len)]
        #row["vectime"]  = np.array( row["vectime"] + padding )
        #row["vecvalue"] = np.array( row["vecvalue"] + padding )

        # Pad with last recorded value to keep the line constant
        '''pad_time = row["vectime"][-1]
        padding_time = [pad_time for _ in range(pad_len)]
        pad_value = row["vecvalue"][-1]
        padding_value = [pad_value for _ in range(pad_len)]'''

        row["vectime"]  = np.array( row["vectime"] + padding_time )
        row["vecvalue"] = np.array( row["vecvalue"] + padding_value )
        
        return row

    df = df.apply(padArray, axis=1)
    #print("\n\nPadded lists:\n", df)


    # Get group sizes
    df_count = df.groupby(["module", "name"]).size()

    # Join the group sizes with the general data frame
    df = df.merge(df_count.to_frame("count"), left_on=["module", "name"], right_on=["module", "name"])
    #print("\n\nAdded group sizes:\n", df)

    # Get the max size of the groups
    max_group_size = df["count"].max()
    #print("max_group_size:", max_group_size)

    # Fill a group with missing rows
    missing_rows = []
    #cols = [col for col in df.columns]
    def fillGroup(row):
        global missing_rows

        max_len = row["max_len"]
        #print("max_len: ", max_len)
        #print("max_group_size - row['count']: ", max_group_size - row["count"])
        for i in range(max_group_size - row["count"]):
            missing_rows.append([row["module"], row["name"], [0 for _ in range(max_len)], [0 for _ in range(max_len)], max_len, max_len, max_group_size])

        row["count"] = max_group_size
        return row

    
    # Filter groups with less than max_group_size items and fill it with missing items
    #df.groupby(["module", "name"]).filter(lambda x: len(x) < max_group_size).apply(fillGroup, axis=1)
    df[df["count"] < max_group_size][["module", "name", "max_len", "count"]].drop_duplicates().apply(fillGroup, axis=1)

    # Append missing rows to the general data frame
    df = pd.concat([df, pd.DataFrame(missing_rows, columns=df.columns)])
    #print("\n\nAppended missing rows per group:\n", df)

    # Calculate mean or median of times and values
    op = np.mean if compute_mean else np.median
    times_mean  = df.groupby(["module", "name"])["vectime"].apply(op).reset_index()
    values_mean = df.groupby(["module", "name"])["vecvalue"].apply(op).reset_index()

    # Join the results
    df = times_mean.merge(values_mean, on=["module", "name"])
    #print("\n\nOperation applied (mean or median):\n", df)
    




    '''
    # EXAMPLE
    print("#"*6000)
    #pd.set_option("display.max_columns", None)
    #pd.set_option("display.max_colwidth", None)

    # Read input file
    df = pd.read_csv(data_url)

    # Get columns of interest
    df = df[["module", "name", "vectime", "vecvalue"]]
    #print(df)

    # Drop rows having a null value
    df = df.dropna()

    # Filter rows
    #prova = df[(df["module"] == "LoRaNetworkServerEdgeStochasticScenarioTest.gateways[0]") & (df["name"] == "cpu:vector")]
    #prova = df[(df["module"] == "LoRaNetworkServerEdgeStochasticScenarioTest.gateways[0]") & (df["name"] == "connected:vector")]
    #prova = df[(df["module"] == "LoRaNetworkServerEdgeStochasticScenarioTest.gateways[0]") & (df["name"] == "messageLostCount:vector")]
    #prova = df[(df["module"] == "LoRaNetworkServerEdgeStochasticScenarioTest.gateways[1]") & (df["name"] == "messageLostCount:vector")]
    #prova = df[(df["module"] == "LoRaNetworkServerEdgeStochasticScenarioTest.gateways[2]") & (df["name"] == "messageLostCount:vector")]
    #prova = df[(df["module"] == "LoRaNetworkServerEdgeStochasticScenarioTest.gateways[0]") & (df["name"] == "messageSentCount:vector")]
    #prova = df[(df["module"] == "LoRaNetworkServerEdgeStochasticScenarioTest.gateways[0]") & (df["name"] == "messageSentLoRaGatewaysCount:vector")]
    #prova = df[(df["module"] == "LoRaNetworkServerEdgeStochasticScenarioTest.gateways[1]") & (df["name"] == "messageSentLoRaGatewaysCount:vector")]
    #prova = df[(df["module"] == "LoRaNetworkServerEdgeStochasticScenarioTest.gateways[2]") & (df["name"] == "messageSentLoRaGatewaysCount:vector")]
    #prova = df[(df["module"] == "LoRaNetworkServerEdgeStochasticScenarioTest.gateways[0]") & (df["name"] == "messageSentLoRaGateways:vector")]
    #prova = df[(df["module"] == "LoRaNetworkServerEdgeStochasticScenarioTest.gateways[1]") & (df["name"] == "messageSentLoRaGateways:vector")]
    #prova = df[(df["module"] == "LoRaNetworkServerEdgeStochasticScenarioTest.gateways[2]") & (df["name"] == "messageSentLoRaGateways:vector")]
    #prova = df[df["name"] == "connected:vector"]
    prova = df[df["name"] == "cpu:vector"]
    #prova = df[df["name"] == "storage:vector"]


    #prova = df[(df["module"] == "LoRaNetworkServerEdgeStochasticScenarioTest.endDevices[6]") & (df["name"] == "messageReceivedCount:vector")]
    #prova = df[(df["module"] == "LoRaNetworkServerEdgeStochasticScenarioTest.endDevices[0]") & (df["name"] == "messageReceivedCount:vector")]
    #prova = df[(df["module"] == "LoRaNetworkServerEdgeStochasticScenarioTest.endDevices[0]") & (df["name"] == "messageSentCount:vector")]
    #prova = df[(df["module"] == "LoRaNetworkServerEdgeStochasticScenarioTest.endDevices[0]") & (df["name"] == "messageRetransmittedCount:vector")]
    print("\nFiltered dataset:\n", prova)

    # Convert the time and value strings to lists
    prova = prova.apply(toArray, axis=1)
    print("\n\nFiltered dataset cols converted to lists:\n", prova)


    # Get the length of every list
    prova["len"] = prova["vectime"].apply(len)
    print("\n\nAdded list lenghts:\n", prova)

    # Get the max length per group
    max_lens = prova.groupby(["module", "name"])["len"].max()

    # Join the max length with the general data frame
    prova = prova.merge(max_lens.to_frame("max_len"), left_on=["module", "name"], right_on=["module", "name"])
    print("\n\nAdded max list lenghts per group:\n", prova)

    # Pad lists of time and value
    prova = prova.apply(padArray, axis=1)
    print("\n\nPadded lists:\n", prova)


    # Get group sizes
    prova_count = prova.groupby(["module", "name"]).size()

    # Join the group sizes with the general data frame
    prova = prova.merge(prova_count.to_frame("count"), left_on=["module", "name"], right_on=["module", "name"])
    print("\n\nAdded group sizes:\n", prova)

    # Get the max size of the groups
    #max_group_size = prova["count"].max()
    #print("\nmax_group_size:", max_group_size)

    # Filter groups with less than max_group_size items and fill it with missing items
    missing_rows=[]
    #prova.groupby(["module", "name"]).filter(lambda x: len(x) < max_group_size).apply(fillGroup, axis=1) # Filter returns a df and not a groupbydf

    #print(prova.groupby(["module", "name"]).filter(lambda x: len(x) < max_group_size)[["module", "name", "max_len", "count"]].drop_duplicates())
    #prova.groupby(["module", "name"]).filter(lambda x: len(x) < max_group_size)[["module", "name", "max_len", "count"]].drop_duplicates().apply(fillGroup, axis=1)
    #print(prova[prova["count"] < max_group_size][["module", "name", "max_len", "count"]].drop_duplicates())
    prova[prova["count"] < max_group_size][["module", "name", "max_len", "count"]].drop_duplicates().apply(fillGroup, axis=1)
    
    # Append missing rows to the general data frame
    prova = pd.concat([prova, pd.DataFrame(missing_rows, columns=prova.columns)])
    print("\n\nAppended missing rows per group:\n", prova)


    # Calculate mean or median of times and values
    op = np.mean if compute_mean else np.median
    times_mean  = prova.groupby(["module", "name"])["vectime"].apply(op).reset_index()
    values_mean = prova.groupby(["module", "name"])["vecvalue"].apply(op).reset_index()

    # Join the results
    prova = times_mean.merge(values_mean, on=["module", "name"])
    print("\n\nOperation applied (mean or median):\n", prova)
    '''

df.apply(toCSV, axis=1)