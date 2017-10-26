import itertools
import json
import random
import subprocess
import sys
from datetime import datetime
import hashlib
import os

from lib.file_writer import FileWriter
from lib.json_builder import JSONBuilder
from lib.output_parser import OutputParser

rundir = "benchmark_runner"
cgfdir = os.path.realpath(os.getcwd())
cfgdir = str(cgfdir).rpartition(rundir)[0] + rundir + "/config/"
with open(cfgdir + 'run_config.json') as run_config_f:
    run_config = json.load(run_config_f)
with open(cfgdir + 'typemap.json') as typemap_f:
    typemap = json.load(typemap_f)["types"]

cpp_out = sys.stdin.readlines()

opt_args = []
run_opt_strs = cpp_out[0].split(";")
for opt_str in run_opt_strs:
    split = opt_str.split("=")
    if len(split) == 2:
        opt_args.append(split[1])

timings = [float(i) for i in cpp_out[1:]]
executable = opt_args[0]
parser = OutputParser(executable)
json_builder = JSONBuilder(run_config)

if run_config["write_mode"] == "db":
    from db_writer import DBWriter
    writer = DBWriter(run_config)
if run_config["write_mode"] == "file":
    writer = FileWriter(run_config)

args = opt_args[0:9]
args[0] = run_config["executable"]
type = typemap[int(args[4])]

num_timings = len(timings)
print(cpp_out[0][:-1] + "=>" + str(num_timings))

filename = None
for timing in timings:
    entry = json_builder.build(args, timing, type["type"])
    if not filename:
        filename = datetime.now().strftime('%Y-%m-%d-%H-%M-%S-%f')
        filename += json.dumps(entry)
        filename = hashlib.md5(filename).hexdigest()
    writer.write(entry, filename)

