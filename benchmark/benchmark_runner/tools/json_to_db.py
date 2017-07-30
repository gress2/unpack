import json
import sys

from lib/db_writer import DBWriter

try:
    output_json = open(sys.argv[1])
except IndexError:
    print "Must provide location of json file. python json_to_db.py <PATH_TO_JSON>"
    exit()

with open('config/run_config.json') as rc:
    run_config = json.load(rc)

writer = DBWriter(run_config)

with open(sys.argv[1]) as output_json:
    for line in output_json:
        writer.write(json.loads(line))

