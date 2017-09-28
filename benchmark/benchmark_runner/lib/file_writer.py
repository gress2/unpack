import json
import hashlib

class FileWriter:
    file_target = None
    def __init__(self, run_config):
        self.file_target = run_config["file_target"]
    def write(self, entry):
        with open(self.file_target + "/" + str(hashlib.md5(json.dumps(entry)).hexdigest()) + ".out", "a") as f:
            json.dump(entry, f) 
            f.write('\n')
