import itertools
import json
import pprint
import pymongo

class DBReader:
    collection = None
    def __init__(self, run_config):
        client = pymongo.MongoClient()
        db = client[run_config["db"]]
        self.collection = db[run_config["db_collection"]]
    def buildDict(self, keys, values):
        queryDict = dict()
        for i in range(0, len(keys)):
            queryDict[keys[i]] = values[i]
        return queryDict
    def createQueriesFromCartesian(self, variables, constants):
        distinct_values = dict()
        variables_dict = dict()
        for var in variables:
            variables_dict[var] = 1
        for constant in constants:
            print("Finding set of distinct values for: " + constant)
            distinct_values[constant] = self.collection.distinct(constant)
        sets = [ value for key, value in distinct_values.iteritems() ]
        queries = []
        for combination in itertools.product(*sets):
            queries.append(self.buildDict(distinct_values.keys(), list(combination)))
        return [queries, variables_dict]
    def query(self, query, vdict):
        cursor = self.collection.find(query, vdict)
        results = []
        for entry in cursor:
            results.append(entry)
        return results

#    def getValuesFromCartesian(self, values, cartesian_keys, run_id = None):
#        distinct_values = dict()
#        values_dict = dict()
#        results = dict()
#        for value in values:
#            values_dict[value] = 1
#        values_dict["_id"] = 0
#        for key in cartesian_keys: 
#            print(key)
#            distinct_values[key] = self.collection.distinct(key)
#        sets = [ value for key,value in distinct_values.iteritems() ]
#        for combination in itertools.product(*sets):
#            query = self.buildDict(distinct_values.keys(), list(combination)) 
#            print(query)
#            if run_id is not None:
#                query["run_id"] = run_id; 
#            query_string = json.dumps(query)
#            results[query_string] = []
#            cursor = self.collection.find(query, values_dict)
#            for entry in cursor: # should be one...
#                results[query_string].append(entry)
#        return results
