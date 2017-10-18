import json

from lib.db_writer import DBWriter

with open("config/run_config.json") as run_config_f:
    run_config = json.load(run_config_f)

db_writer = DBWriter(run_config)

with open("merged.out") as data:
    for line in data:
        json_line = json.loads(line)
        db_writer.write(json_line)
