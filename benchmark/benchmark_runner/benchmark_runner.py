import itertools
import json
import subprocess
import sys

from lib.file_writer import FileWriter
from lib.json_builder import JSONBuilder
from lib.output_parser import OutputParser

with open('config/run_config.json') as run_config_f:
    run_config = json.load(run_config_f)
with open('config/typemap.json') as typemap_f:
    typemap = json.load(typemap_f)["types"]

cpp_out = sys.stdin.readlines()

opt_args = []
run_opt_strs = cpp_out[0].split(";")
for opt_str in run_opt_strs:
    split = opt_str.split("=")
    if len(split) == 2:
        opt_args.append(split[1])

timings = cpp_out[1:]
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


for timing in timings:
    entry = json_builder.build(args, timing, type["type"])
    writer.write(entry)
