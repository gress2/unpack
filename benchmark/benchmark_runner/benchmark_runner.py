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

executable = sys.argv[1]
parser = OutputParser(executable)
json_builder = JSONBuilder(run_config)

if run_config["write_mode"] == "db":
    from db_writer import DBWriter 
    writer = DBWriter(run_config)
if run_config["write_mode"] == "file":
    writer = FileWriter(run_config)

print sys.argv

args = sys.argv[1:9]
type = typemap[int(args[4])]

if (type["length"] * int(args[3]) * int(args[7]) > 1e9):
    exit()
if "unpack_benchmark" in executable:
    args.insert(0, "time")
timing = parser.parse(subprocess.check_output(args, stderr=subprocess.STDOUT))
 
entry = json_builder.build(combination, timing, type["type"])
writer.write(entry)

