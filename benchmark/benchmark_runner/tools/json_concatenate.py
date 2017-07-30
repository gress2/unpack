import glob
import sys 

try:
    json_dir = sys.argv[1]
except IndexError:
    print "Must provide directory of json files. python json_concatenate.py <PATH_TO_JSON>"
    exit()

files = glob.glob(json_dir + "/*.json")
with open(json_dir + "/benchmarks.concat.json", "w") as output:
    for fname in files:
        with open(fname) as file:
            for line in file:
                output.write(line)
