import itertools
import json
import subprocess
import sys
from tempfile import NamedTemporaryFile

from output_parser import OutputParser
from db_writer import DBWriter

with open('run_config.json') as run_config_f:
    run_config = json.load(run_config_f)

with open('typemap.json') as typemap_f:
    typemap = json.load(typemap_f)["types"]

executable = run_config["executable"]
parser = OutputParser(executable)
writer = DBWriter(run_config)

data_layout = ["aos", "soa"]
container = ["vector"]
container_size = [str(x) for x in [2**0, 2**10, 2**20]]
type_index = [str(x) for x in range(5)]
operation_complexity = ["simple", "complex"]
access_pattern = ["single", "independent", "combined"]
iterations = [str(2**x) for x in range(20)]

parameter_space = [[executable], data_layout, container, container_size, type_index,
        operation_complexity, access_pattern, iterations]

for combination in itertools.product(*parameter_space): # ~17k
    type = typemap[int(combination[4])]
    if (type["length"] * int(combination[3]) * int(combination[7]) > 1e8):
        continue
    args = list(combination)
    if "unpack_benchmark" in executable:
        args.insert(0, "time")
    timing = parser.parse(subprocess.check_output(args, stderr=subprocess.STDOUT))
    writer.write(combination, timing)

