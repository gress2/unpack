import itertools
import json
import subprocess
import sys

from output_parser import OutputParser
from db_writer import DBWriter

with open('run_config.json') as run_config_f:
    run_config = json.load(run_config_f)

executable = run_config["executable"]
parser = OutputParser(executable)
writer = DBWriter(run_config)

data_layout = ["aos", "soa"]
container = ["vector"]
container_size = [str(x) for x in range(100000, 1000000, 100000)]
type_index = [str(x) for x in range(18)]
operation_complexity = ["simple", "complex"]
access_pattern = ["single", "independent", "combined"]
iterations = [str(x) for x in range(1000, 10000, 1000)]

parameter_space = [[executable], data_layout, container, container_size, type_index,
        operation_complexity, access_pattern, iterations]

for combination in itertools.product(*parameter_space): # ~17k
    args = list(combination)
    if "unpack_benchmark" in executable:
        args.insert(0, "time")
    timing = parser.parse(subprocess.check_output(args, stderr=subprocess.STDOUT))
    writer.write(combination, timing)

