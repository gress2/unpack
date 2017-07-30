import json
import copy
import matplotlib.pyplot as plt
import numpy as np
import pprint

from db_reader import DBReader

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

def plot_entry(data, title = "", x = "iterations", y = "timing"):
    plt.figure(figsize = (19.2, 10.8), dpi = 100)
    plt.grid(linestyle='dashed')
    cont_size_dict = dict()
    for entry in data:
        c_sz = entry["container_size"]
        if c_sz in cont_size_dict:
            cont_size_dict[c_sz].append(entry)
        else:
            cont_size_dict[c_sz] = [entry]
    for sz in cont_size_dict:
        xdata = []
        ydata = []
        for element in cont_size_dict[sz]:
            xdata.append(element[x])
            ydata.append(element[y])
        plt.loglog(
            xdata,
            ydata,
            linewidth=2,
            basex=2,
            label="container_size = " + str(sz)
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
    plot_entry(data[entry], json_str_to_title(entry))
