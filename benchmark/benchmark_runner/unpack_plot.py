import json
import copy
import matplotlib.pyplot as plt
import numpy as np
import pprint
from pandas import DataFrame as df

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
        title += key + " = " + str(dictionary[key]) 
    return title

def no_unicode_keys(data):
    return dict((str(key), value) for (key, value) in data.items())

def plot_entry(data, title = "", x = "iterations", y = "timing", line_base = "container_size"):
    plt.figure(figsize = (19.2, 10.8), dpi = 100)
    plt.grid(linestyle = 'dashed')
    line_base_dict = dict()
    for entry in data:
        entry = no_unicode_keys(entry)
        l_base = entry[line_base]
        if l_base in line_base_dict:
            line_base_dict[l_base].append(entry)
        else:
            line_base_dict[l_base] = [entry]
    for base in line_base_dict:
        data_ = { 'x': [], 'y': [] }
        for element in line_base_dict[base]:
            data_['x'].append(float(element[x]))
            data_['y'].append(float(element[y]))
        data_frame = df.from_dict(data_)
        data_frame.sort('x', inplace=True)
        ax = plt.gca()
        print(data_frame)
        ax.plot(data_frame['x'], data_frame['y'], label = line_base + " = " + str(base))
        ax.set_xscale('log')
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
        "compiler", "complexity", "container", "optimization", "orientation", "type", "column"]) 
titles = {}

for entry in data:
    title = json_str_to_title(entry)
    plot_entry(data[entry], json_str_to_title(entry), \
            "iterations", "timing", "container_size")
