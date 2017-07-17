import pymongo

class OutputWriter: 
    client = None
    def __init__(self, db):
        self.client = pymongo.MongoClient(db)
    def write(self, _dict):
        print self.client

