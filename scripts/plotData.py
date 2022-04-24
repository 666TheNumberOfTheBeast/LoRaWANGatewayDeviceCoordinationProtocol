from cProfile import label
import matplotlib.pyplot as plt
import sys
import csv
import argparse

from pathlib import Path
from datetime import datetime


'''
labels = ["Sent", "Received"]
groups = ["device0", "device1", "device2", "device3"]
bar_width = 0.25

# x and y shapes eq len(labels) x len(groups)
x_bars = []
y_bars = [[12, 14, 15, 13], [7, 5, 8, 6]]
for i in range(len(labels)):
  x_bars.append( [x + i*bar_width for x in range(len(groups))] )

color_bars = ["blue", "red", "orange", "yellow", "green"]

for i in range(len(labels)):
  plt.bar(x_bars[i], y_bars[i], label=labels[i], color=color_bars[i], width=bar_width)

plt.xticks([x + bar_width/2.0 for x in x_bars[0]], labels=groups)

plt.legend()

plt.show()
exit(1)
'''




parser = argparse.ArgumentParser()
parser.add_argument("chartname", type=str, help="The name of the chart")
parser.add_argument("charttype", type=str, help="The type of the chart as b (bar), h (hist), l (line) or s (scatter)")
parser.add_argument("-l", type=str, nargs="+", help="The input label(s)")
parser.add_argument("-g", type=str, nargs="+", help="The input group(s) for bar chart. Each group has a bar per label")
parser.add_argument("-i", type=str, nargs="+", required=True, help="The input filename(s) as .csv")
parser.add_argument("-o", type=str, help="The output filename as .pdf, .svg, .png, .jpg, .jpeg")
parser.add_argument("-I", action="store_true", help="Ignore missing input file and treat it as zero value")
args = parser.parse_args()

#print("chartname:", args.chartname)
#print("charttype:", args.charttype)
#print("l:", args.l)
#print("i:", args.i)
#print("o:", args.o)

# Title of the chart
chart_title = args.chartname

# Type of the chart
chart_type = args.charttype
if chart_type not in {"b", "h", "l", "s"}:
  sys.exit("Entered wrong charttype! The following type are supported: b (bar), h (hist), l (line) or s (scatter)")

# Output filename
out_filename = ""
if args.o:
  out_filename = args.o
  if not out_filename.endswith((".pdf", ".svg", ".png", ".jpg", ".jpeg")):
    sys.exit("Entered invalid output filename! The following extensions are supported: .pdf, .svg, .png, .jpg, .jpeg")

# CSV files
data_urls = args.i
for data_url in data_urls:
  if not data_url.endswith((".csv")):
    sys.exit("Entered invalid input filename! The following extensions are supported: .csv")

# Get ignore option
ignore = args.I

# Labels
labels = []
if args.l:
  labels = args.l

if len(labels) > 0 and chart_type != "b" and len(labels) != len(data_urls):
    sys.exit("Different length of labels and filenames!")

# Groups
groups = []
if args.g:
  groups = args.g

if chart_type == "b" and not groups:
    sys.exit("Entered bar chart without groups!")

if chart_type == "b" and len(groups) != len(data_urls) and not labels:
    sys.exit("Different length of groups and filenames but no labels (default one per group)!")


# Variables for bar chart
bar_width = 0.25
color_bars = ["blue", "red", "orange", "yellow", "green"]

# Get the num of labels (if no label is provided, assume 1)
labels_num = len(labels) if labels else 1
x_bars = []
y_bars = []

if chart_type == "b":
  # Calculate bars per label positions
  for i in range(labels_num):
    x_bars.append( [x + i*bar_width for x in range(len(groups))] )
    y_bars.append([])

# Index to iterate labels
l = 0

# Bool to detect if at least a filename is found
found = False

# Iterate files
#for data_url in data_urls:
for i in range(len(data_urls)):
  data_url = data_urls[i]

  x = []
  y = []
 
  # Check if the ignore option is enabled and the file exists
  if ignore and not Path(data_url).is_file():
    if chart_type == "b":
      # Treat it as zero in bar charts
      y_bars[i%labels_num].append(0)
    else:
      # Skip in the other chart types
      l += 1

    continue

  # At least a filename is found
  found = True

  # Open the CSV file
  with open(data_url, mode ="r") as file:
    # Read the CSV file
    csvFile = csv.reader(file)

    iline = 0
    
    for line in csvFile:
      if iline != 0:
          #print(line)
          x.append(float(line[0]))
          y.append(float(line[1]))

      iline += 1

  #quit(0)
  #print("Read " + i + " lines")
  
  # Store the max value for bar chart
  if chart_type == "b":
    # Store it in the appropriate label vector assuming input data are provided as
    # group1_label1 group1_label2 ... group1_labeln group2_label1 ... group2_labeln ... groupm_labeln
    y_bars[i%labels_num].append(y[-1])

  # Else plot the points
  elif chart_type == "h":
    if len(labels) > 1:
      plt.hist(y, label=labels[l])
      l += 1
    else:
      plt.hist(y)

  elif chart_type == "l":
    if len(labels) > 1:
      plt.plot(x, y, label=labels[l])
      l += 1
    else:
      plt.plot(x, y)

  elif chart_type == "s":
    if len(labels) > 1:
      plt.scatter(x, y, label=labels[l])
      l += 1
    else:
      plt.scatter(x, y, s=10)

# Check if at least a filename is found
if not found:
  # Exit because it is useless to plot an empty chart
  sys.exit("No filename was found!")


# Plot the points of bar chart
if chart_type == "b":
  #print("labels", labels)
  #print("groups", groups)
  #print("x_bars", x_bars)
  #print("y_bars", y_bars)
  #print("*"*25)

  # Plot n bars per label where n=len(groups)
  for i in range(labels_num):
    plt.bar(x_bars[i], y_bars[i], label=labels[i] if labels else None, color=color_bars[i] if labels else None, width=bar_width)

  # Set a name per group in the middle of the group
  #plt.xticks([x + bar_width/2.0 for x in x_bars[0]], labels=groups)
  #plt.xticks([x + (bar_width * (len(labels)-1)) / 2.0 for x in x_bars[0]], labels=groups)
  #plt.xticks([x + (bar_width * (labels_num-1)) / 2.0 for x in x_bars[0]], labels=groups)
  if len(groups) <= 6:
    plt.xticks([x + (bar_width * (labels_num-1)) / 2.0 for x in x_bars[0]], labels=groups)
  else:
    plt.xticks([x + (bar_width * (labels_num-1)) / 2.0 for x in x_bars[0]], labels=groups, fontsize=8, rotation=30)

  # Change x axis lower and upper bounds if few bars are displayed to reduce the "fatness" due to auto-scale
  if len(groups) == 1:
    if not labels or len(labels) == 1:
      plt.gca().set_xbound(-2.5*bar_width, bar_width*2.7)
    elif len(labels) == 2:
      plt.gca().set_xbound(-2*bar_width, bar_width*3)
    elif len(labels) == 3:
      plt.gca().set_xbound(-1.5*bar_width, bar_width*3.5)
    elif len(labels) == 4:
      plt.gca().set_xbound(-1*bar_width, bar_width*4)

elif chart_type in {"l", "s"}:
  # Name the axes
  plt.xlabel("Time (s)")
  plt.ylabel("Value")

# Show a legend on the plot if multiple lines
if len(labels) > 1:
  plt.legend()

# Name the chart
plt.title(chart_title)
 
# Show the plot
#plt.show()
#exit(0)

# Check if an out filename is provided
if out_filename:
  # Check if it includes directories
  out_split = out_filename.split("/")
  if (len(out_split) > 1):
    # Get the directory path
    out_directory = ""
    for i in range(0, len(out_split)-1):
      out_directory += out_split[i] + "/"

    # Create directory if does not exist
    try:
      Path(out_directory).mkdir(parents=True)
    except FileExistsError:
      pass
else:
  # Get current time
  now = datetime.now()

  # Convert time to yyyy mm dd HH MM SS format
  strtime = now.strftime("%Y%m%d_%H%M%S")

  # Set out filename
  out_filename = strtime + ".pdf"

# XXX TEMP
#out_filename = out_filename.replace(".pdf", ".svg")

# Save plot to file
plt.savefig(out_filename)


# Plot the min, max and mean of data per label in case of bar chart 
if chart_type == "b" and len(groups) > 1:
  # Clear previous plot
  plt.clf()

  # Init variables
  labels_new = ["min", "max", "mean"]
  color_bars = ["blue", "red", "purple"]
  x = []
  y = []
  for i in range(len(labels_new)):
    x.append( [x_ + i*bar_width for x_ in range(labels_num)] )
    y.append([])

  # Iterate data per label
  for y_bar in y_bars:
    y_min = -1
    y_max = -1
    sum = 0

    # Find min, max and mean
    for value in y_bar:
      if value > y_max:
        y_max = value
      if value < y_min or y_min < 0:
        y_min = value
      
      sum += value

    # Store min, max and mean
    y[0].append(y_min)
    y[1].append(y_max)
    y[2].append(sum/len(y_bar))


  #print("x", x)
  #print("y", y)

  # Plot n bars per label where n=len(groups)
  for i in range(len(labels_new)):
    plt.bar(x[i], y[i], label=labels_new[i], color=color_bars[i], width=bar_width)

  # Set a name per group in the middle of the group if at least a label is provided
  if labels:
    plt.xticks([x_ + (bar_width * (len(labels_new)-1)) / 2.0 for x_ in x[0]], labels=labels)
  else:
    plt.xticks([])

  # Change x axis lower and upper bounds if few bars are displayed to reduce the "fatness" due to auto-scale
  if not labels:
    plt.gca().set_xbound(-1.5*bar_width, bar_width*3.5)

  # Name the chart
  plt.title(chart_title)
  
  # Show the legend on the plot
  plt.legend()

  # Show the plot
  #plt.show()

  # Extend the out filename with MinMax
  out_filename_split = out_filename.split(".")
  out_filename = ""
  for i in range(0, len(out_filename_split)-1):
    out_filename += out_filename_split[i]

  out_filename += "MinMax." + out_filename_split[-1]

  # Save plot to file
  plt.savefig(out_filename)