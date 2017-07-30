class JSONBuilder:
    benchmarking_tool = None
    run_config = None
    def __init__(self, run_config):
        self.run_config = run_config
        exc = run_config["executable"]
        if "unpack_benchmark" in exc:
            self.benchmarking_tool = "benchmark"
        elif "unpack_chrono_benchmark" in exc:
            self.benchmarking_tool = "chrono_benchmark"
        elif "unpack_google_benchmark" in exc:
            self.benchmarking_tool = "google_benchmark"
    def build(self, run_params, timing, type):
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
            "run_id": self.run_config{"run_id"],
            "system": self.run_config["system"],
            "timing": timing,
            "type": type
        }
	print "type: " + entry["type"] + " orientation: " + entry["orientation"] + \
                " complexity: " + entry["complexity"] + " access_pattern: " + entry["access_pattern"] + \
                " container_sz: " + entry["container_size"] + " iterations: " + str(entry["iterations"]) + \
                " timing: " + str(entry["timing"])
        return entry
