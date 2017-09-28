import itertools
import json

with open('config/typemap.json') as typemap_f:
    typemap = json.load(typemap_f)["types"]

def appendArgName(argName, ary):
    return list(map(lambda x: "--" + argName + "=" + str(x), ary))

target_executable = ["./unpack_meta_benchmark"]
tool = appendArgName("tool", ["chrono"])
orientation = appendArgName("orientation", ["aos", "soa"])
container = appendArgName("container", ["vector"])
type = appendArgName("type", [typemap[x]["type"] for x in [0,1,2,3,4]])
size = appendArgName("size", [str(x) for x in [2**0, 2**10, 2**20]])
complexity = appendArgName("complexity", ["simple", "complex"])
access = appendArgName("access", ["single", "independent", "combined", "column"])
count = appendArgName("count", [str(2**x) for x in range(20)])

parameter_space = [target_executable, tool, orientation, container, type,
        complexity, access, count, size] 

ct = 0
f_num = 1

for combination in itertools.product(*parameter_space):
    ct += 1
    with open("app_conf/" + str(f_num) + "m.conf", "a") as f:
        f.write(str(ct) + " " + " ".join(list(combination)))
        f.write('\n')
    if (ct % 1000) == 0:
        ct = 0
        f_num += 1
