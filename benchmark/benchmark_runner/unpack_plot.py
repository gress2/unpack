import json
import copy
import matplotlib.pyplot as plt
import numpy as np
import pprint

from lib.db_reader import DBReader

filename = "benchmark.json"
db = dict()

with open("config/run_config.json") as run_config_f:
    run_config = json.load(run_config_f)

def json_str_to_title(json_s):
    dictionary = json.loads(json_s)
    title = ""
    for key in dictionary:
        if key == "system":
            continue
        if len(title) > 0:
            title += ", "
        if len(title) > 128:
            title += "\n"
        title += key + " = " + dictionary[key] 
    return title

def plot_entry(data, title = "", x = "iterations", y = "timing", line_base = "container_size"):
    plt.figure(figsize = (19.2, 10.8), dpi = 100)
    plt.grid(linestyle = 'dashed')
    line_base_dict = dict()
    for entry in data:
        l_base = entry[line_base]
        if l_base in line_base_dict:
            line_base_dict[l_base].append(entry)
        else:
            line_base_dict[l_base] = [entry]
    for base in line_base_dict:
        xdata = []
        ydata = []
        for element in line_base_dict[base]:
            xdata.append(element[x])
            ydata.append(element[y])
        plt.loglog(
            xdata,
            ydata,
            linewidth = 2,
            basex = 2,
            label = line_base + " = " + str(base)
        )
        plt.title(title)
        plt.legend()
    filename = title.replace("\n", "").replace(",", "_").replace(" = ", "-")
    filename = filename.replace(" ", "_").replace("__", "_").replace("__", "_")
    filename = filename.replace(".", "-").replace("std::", "").replace("<", "+")
    filename = filename.replace(">", "+")
    if (len(filename) > 0):
        plt.savefig("figures/" + filename + ".pdf", bbox_inches='tight')
    else:
        plt.show()
    plt.clf()
    plt.close()
    
reader = DBReader(run_config)
data = reader.getValuesFromCartesian(["container_size", "iterations", "timing"], ["access_pattern", \
        "compiler", "complexity", "container", "optimization", "orientation", "type"]) 

for entry in data:
    plot_entry(data[entry], json_str_to_title(entry), "iterations", "timing", "container_size")
