import pandas as pd
import sys
import argparse

from pathlib import Path
from datetime import datetime


parser = argparse.ArgumentParser()
parser.add_argument("-i", type=str, nargs="+", required=True, help="The input filename(s) as .csv")
parser.add_argument("-o", type=str, help="The output filename as .csv")
parser.add_argument("-r", action="store_true", help="Replace and adjust the values of the output file (e.g. adjust values when merging two countings over time)")
parser.add_argument("-I", action="store_true", help="Ignore missing input file and treat it as zero value")
args = parser.parse_args()

# Get input filenames
data_urls = args.i

# Output filename
out_filename = ""
if args.o:
  out_filename = args.o
  if not out_filename.endswith((".csv")):
    sys.exit("Entered invalid output filename! Not a .csv file")

# Get replace option
replace = args.r

# Get ignore option
ignore = args.I



# Final dataframe
out_df = None

'''
# Iterate filenames
for data_url in data_urls:
  #print(data_url)
  
  # Check if the ignore option is enabled and the file exists
  if ignore and not Path(data_url).is_file():
    # Skip missing input files
    print("missing " + data_url)
    continue

  # Check if the final dataframe has been initialized
  if not isinstance(out_df, pd.DataFrame):
    out_df = pd.read_csv(data_url)
  else:
    in_df = pd.read_csv(data_url)

    print("Merged:\n", out_df)
    print("*"*25)
    print("To merge:\n", in_df)
    print("*"*25)

    #out_df = out_df.merge(in_df, how="outer", sort=True)
    out_df = out_df.merge(in_df, how="outer", sort=replace)


# Check if the final dataframe has been initialized
if not isinstance(out_df, pd.DataFrame):
  sys.exit("No filename was found!")

print("Merged:\n", out_df)

# Check if replace option is enabled
if replace:
  # Replace values with index
  out_df["vecvalue"] = out_df.apply(lambda x: x.name + 1, axis=1)
  print("Replaced:\n", out_df)


# Check if an out filename is provided
if not out_filename:
  # Get current time
  now = datetime.now()

  # Convert time to yyyy mm dd HH MM SS format
  strtime = now.strftime("%Y%m%d_%H%M%S")

  # Set out filename
  out_filename = strtime + ".csv"

# Save csv to file
#out_df.to_csv(out_filename, index=False)
'''



# CONCATENATION
# Iterate filenames
for data_url in data_urls:
  #print(data_url)
  
  # Check if the ignore option is enabled and the file exists
  if ignore and not Path(data_url).is_file():
    # Skip missing input files
    print("missing " + data_url)
    continue

  # Check if the final dataframe has been initialized
  if not isinstance(out_df, pd.DataFrame):
    out_df = pd.read_csv(data_url)
  else:
    in_df = pd.read_csv(data_url)

    #print("Merged:\n", out_df)
    #print("*"*35)
    #print("To merge:\n", in_df)
    #print("*"*35)

    out_df = pd.concat([out_df, in_df], ignore_index=True)


# Check if the final dataframe has been initialized
if not isinstance(out_df, pd.DataFrame):
  sys.exit("No filename was found!")

#print("Merged:\n", out_df)

out_df = out_df.sort_values(["vectime"])
#print("\n\nSorted:\n", out_df)

#out_count = out_df.groupby(["vectime"]).size()
#print("\n\nGroup count:\n", out_count)


# Check if replace option is enabled
if replace:
  # Replace values with index
  #out_df["vecvalue"] = out_df.apply(lambda x: x.name + 1, axis=1)
  # Replace values with cumulative sum (OK with frequencies but not with count stats)
  out_df["vecvalue"] = out_df["vecvalue"].cumsum()
  #print("\n\nReplaced:\n", out_df)


# Check if an out filename is provided
if not out_filename:
  # Get current time
  now = datetime.now()

  # Convert time to yyyy mm dd HH MM SS format
  strtime = now.strftime("%Y%m%d_%H%M%S")

  # Set out filename
  out_filename = strtime + ".csv"

# Save csv to file
out_df.to_csv(out_filename, index=False)





'''
# EXAMPLES
df1 = pd.DataFrame(
  {
    "vectime": [1, 3, 5],
    "vecvalue": [1, 2, 3]
  }
)

#df2 = pd.DataFrame(
#  {
#    "vectime": [2, 4, 6],
#    "vecvalue": [1, 3, 4]
#  }
#)

#df2 = pd.DataFrame(
#  {
#    "vectime": [1, 3, 5],
#    "vecvalue": [1, 2, 3]
#  }
#)

#df2 = pd.DataFrame(
#  {
#    "vectime": [1, 3, 5],
#    "vecvalue": [2, 3, 4]
#  }
#)

df2 = pd.DataFrame(
  {
    "vectime": [2, 3, 4, 5],
    "vecvalue": [1, 2, 3, 5]
  }
)

print("df1:\n", df1)
print("\n\ndf2:\n", df2)

out_df = pd.concat([df1, df2], ignore_index=True)
print("\n\nconcatenation:\n", out_df)

out_df = out_df.sort_values(["vectime"])
print("\n\nsorted:\n", out_df)

out_df["vecvalue"] = out_df["vecvalue"].cumsum()
print("\n\nSum values:\n", out_df)
'''