import itertools
import json
import pprint
import pymongo

class DBReader:
    collection = None
    def __init__(self, run_config):
        client = pymongo.MongoClient(run_config["db_cluster"])
        db = client[run_config["db"]]
        self.collection = db[run_config["db_collection"]]
    def buildDict(self, keys, values):
        queryDict = dict()
        for i in range(0, len(keys)):
            queryDict[keys[i]] = values[i]
        return queryDict
    def getValuesFromCartesian(self, values, cartesian_keys):
        distinct_values = dict()
        values_dict = dict()
        results = dict()
        for value in values:
            values_dict[value] = 1
        values_dict["_id"] = 0
        for key in cartesian_keys: 
            distinct_values[key] = self.collection.distinct(key)
        sets = [ value for key,value in distinct_values.iteritems() ]
        for combination in itertools.product(*sets):
            query = self.buildDict(distinct_values.keys(), list(combination)) 
            query_string = json.dumps(query)
            results[query_string] = []
            cursor = self.collection.find(query, values_dict)
            for entry in cursor:
                results[query_string].append(entry)
        return results
