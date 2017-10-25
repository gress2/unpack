import itertools
import datetime
import os

target_executable = ["../../../../build/Release/bin/unpack_chrono_benchmark"]
data_layout = ["aos", "soa"]
container = ["vector"]
container_size = [str(2**x) for x in range(21)]
type_index = [str(x) for x in [0,1,2,3,4,5,6,7,8,9,10]]
operation_complexity = ["simple", "complex"]
access_pattern = ["single", "independent", "combined"]
iterations = [str(x) for x in [2**0, 2**5, 2**10, 2**15, 2**20]]
columns = ["column", "nocolumn"]
time_limit = "30"
pipe_to = ["| python ../../benchmark_parser.py"]

parameter_space = [target_executable, data_layout, container, container_size,
        type_index, operation_complexity, access_pattern, iterations, columns, [time_limit], pipe_to]

base_dir = "run_" + datetime.datetime.now().strftime('%Y-%m-%d-%H-%M-%S')

os.makedirs(base_dir)
os.makedirs(base_dir + "/app_conf")
os.makedirs(base_dir + "/output")
os.makedirs(base_dir + "/jobs")

ct = 0
f_num = 1

def get_job_str(num, cores):
    return '''#!/bin/bash
#MSUB -r run_job_%s
#MSUB -n %s
#MSUB -T %s 
#MSUB -e run_job_%s.e
#MSUB -o run_job_%s.o
#MSUB -A gen2287
#MSUB -q standard

set -x
ccc_mprun -f app_conf/%s.conf''' % (num, cores, int(time_limit) * 60, num, num, num)

def get_launcher_str():
    return '''#!/bin/bash
rm -f jobs/*~
rm -f jobs/*.e
rm -f jobs/*.o
rm -f app_conf/*~
for job in jobs/*
do
  ccc_msub ${job}
  sleep 10
done
'''

for combination in itertools.product(*parameter_space):
    if combination[9] == "raw":
        if combination[7] != "single":
            continue
    ct += 1
    with open(base_dir + "/app_conf/" + str(f_num) + ".conf", "a") as f:
        f.write("1 " + " ".join(list(combination)))
        f.write('\n')
    if (ct % 1000) == 0:
        ct = 0
        f_num += 1
for job in range(1, f_num + 1):
    with open(base_dir + "/jobs/run_job_" + str(job), "a") as f:
        cores = 1000
        if job == f_num:
            cores = ct
        f.write(get_job_str(job, cores))
        f.write('\n')
with open(base_dir + "/launcher.sh", "a") as f:
    f.write(get_launcher_str())
    f.write('\n')
