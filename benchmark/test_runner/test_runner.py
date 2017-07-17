import json
import sys
from subprocess import call

from output_parser import OutputParser
from output_writer import OutputWriter

with open('run_config.json') as run_config_f:
    run_config = json.load(run_config_f)

parser = OutputParser(run_config["executable"])
writer = OutputWriter(run_config["database"])

data_layout = ["aos", "soa"]
container = ["vector", "list", "deque"]
container_size = range(10000, 1000000, 10000)
type_index = range(18)
operation_complexity = ["simple", "complex"]
access_pattern = ["single", "independent", "combined"]
iterations = range(100, 10000, 100)

writer.write({})
parser.parse("test")
