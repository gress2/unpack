import json
import copy
import matplotlib.pyplot as plt
import numpy as np
import pprint

from db_reader import DBReader

filename = "benchmark.json"
db = dict()

with open("run_config.json") as run_config_f:
    run_config = json.load(run_config_f)

def print_tuple(container):
    text = ""
    char = 0
    for element in container:
        current = str(element[0]) + " = " + str(element[1])
        if (char + len(current) > 128):
            text += ",\n" + current
            char = len(current)
        elif (len(text) > 0):
            text += ", " + current
            char += 2 + len(current)
        else:
            text += current
            char += len(current)
    return text

def plot_entry(data, title = "", x = "iterations", y = "timing"):
    dictionary = dict()
    plt.figure(figsize = (19.2, 10.8), dpi = 100)
    plt.grid(linestyle='dashed')
    for entry in data:
        serie = copy.deepcopy(entry)
        serie["system"] = tuple(sorted(serie["system"].items()))
        del serie[x]
        del serie[y]
        serie = tuple(sorted(serie.items()))
        if serie in dictionary:
            dictionary[serie].append(entry)
        else:
            dictionary[serie] = [entry]
    for serie in dictionary:
        xdata = []
        ydata = []
        for element in dictionary[serie]:
            xdata.append(element[x])
            ydata.append(element[y])
        container_size = int(dictionary[serie][0]["container_size"])
        plt.loglog(
            xdata,
            ydata,
            linewidth=2,
            basex=2,
            label="container_size = " + str(container_size)
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

for key in db:
    plot_entry(db[key], print_tuple(key))
    print("done")
