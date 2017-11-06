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
        title += str(dictionary[key]) 
    return title

def no_unicode_keys(data):
    return dict((str(key), value) for (key, value) in data.items())

def plot(data, title = "", x = "iterations", y = "timing", line_base = "container_size"):
    plt.figure(figsize = (19.2, 10.8), dpi = 100)
    plt.grid(linestyle = 'dashed')
    line_base_dict = dict()
    for entry in data:
        entry = no_unicode_keys(entry)
        entry['timing'] = np.mean(entry['timing'])
        l_base_val = entry[line_base] # creating a separate line based on this value
        if l_base_val in line_base_dict:
            line_base_dict[l_base_val].append(entry) # add a set of timings into this line base dict
        else:
            line_base_dict[l_base_val] = [entry] 
    for base in line_base_dict: # actually plot the line now
        data_ = { 'x': [], 'y': [] }
        for element in line_base_dict[base]:
            data_['x'].append(float(element[x]))
            data_['y'].append(float(element[y]))
        data_frame = df.from_dict(data_)
        data_frame.sort('x', inplace=True)
        ax = plt.gca()
        ax.plot(data_frame['x'], data_frame['y'], label = line_base + " = " + str(base))
        ax.set_xscale('log')
        ax.set_xlabel(x)
        ax.set_ylabel(y)
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

dim_set = ["container_size", "iterations", "timing", "access_pattern", "compiler", "complexity", \
        "container", "optimization", "orientation", "type", "column"]
line_basis = "orientation"
x_axis = "iterations"
y_axis = "timing"
variable = [x_axis, y_axis, line_basis]
constant = list(set(dim_set) - set(variable))

query_set, vdict = reader.createQueriesFromCartesian(variable, constant)
qset_len = len(query_set)
progress = 0
for query in query_set:
    progress += 1
    title = json_str_to_title(json.dumps(query))
    print('(' + str(progress) + '/' + str(qset_len) + ') ' + title)
    data = reader.query(query, vdict)
    plot(data, title, x_axis, y_axis, line_basis)
