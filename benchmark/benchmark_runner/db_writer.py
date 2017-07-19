import pymongo

class DBWriter: 
    benchmarking_tool = None
    collection = None
    run_config = None
    def __init__(self, run_config):
        self.run_config = run_config
        client = pymongo.MongoClient(run_config["db_cluster"])
        db = client[run_config["db"]]
        self.collection = db[run_config["db_collection"]]
        exc = run_config["executable"]
        if "unpack_benchmark" in exc:
            self.benchmarking_tool = "benchmark"
        elif "unpack_chrono_benchmark" in exc:
            self.benchmarking_tool = "chrono_benchmark"
        elif "unpack_google_benchmark" in exc:
            self.benchmarking_tool = "google_benchmark"
    def write(self, run_params, timing):
        entry = {
            "access_pattern": run_params[6],
            "benchmarking_tool": self.benchmarking_tool,
            "compiler": self.run_config["compiler"],
            "complexity": run_params[5],
            "container": run_params[2],
            "container_size": run_params[3],
            "iterations": run_params[7],
            "optimization": self.run_config["optimization"],
            "orientation": run_params[1],
            "system": self.run_config["system"],
            "timing": timing,
            "type_index": run_params[4]
        }
        print entry
        self.collection.insert_one(entry)

