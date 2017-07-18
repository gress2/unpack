import pymongo

class OutputWriter: 
    client = None
    run_config = None
    def __init__(self, run_config):
        self.run_config = run_config
        self.client = pymongo.MongoClient(run_config["database"])
    def write(self, _dict):
        print self.client

