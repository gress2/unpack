import json
import hashlib
from datetime import datetime

class FileWriter:
    file_target = None
    def __init__(self, run_config):
        self.file_target = run_config["file_target"]
    def write(self, entry, filename = None):
        if not filename:
            filename = datetime.now().strftime('%Y-%m-%d-%H-%M-%S-%f') 
            filename += json.dumps(entry)
            filename = hashlib.md5(filename).hexdigest()
        with open(self.file_target + "/" + filename + ".out", "a") as f:
            json.dump(entry, f) 
            f.write('\n')
