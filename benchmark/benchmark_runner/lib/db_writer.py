import pymongo

class DBWriter: 
    collection = None
    run_config = None
    def __init__(self, run_config):
        self.run_config = run_config
        client = pymongo.MongoClient()
        db = client[run_config["db"]]
        self.collection = db[run_config["db_collection"]]
    def write(self, entry):
        self.collection.insert_one(entry)

