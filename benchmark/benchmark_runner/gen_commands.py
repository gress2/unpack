import itertools

executable = "python benchmark_runner.py"
target_executable = ["unpack_chrono_benchmark"]
data_layout = ["aos", "soa"]
container = ["vector"]
container_size = [str(x) for x in [2**0, 2**10, 2**20]]
type_index = [str(x) for x in [0,1,2,3,4,5,6,7,8,9,10]]
operation_complexity = ["simple", "complex"]
access_pattern = ["single", "independent", "combined"]
iterations = [str(2**x) for x in range(20)]
columns = ["raw", ""]

parameter_space = [[executable], target_executable, data_layout, container, container_size,
        type_index, operation_complexity, access_pattern, iterations, columns]

ct = 0
f_num = 1
for combination in itertools.product(*parameter_space):
    if combination[9] == "raw":
        if combination[7] != "single":
            continue
    ct += 1
    with open("app_conf/" + str(f_num) + ".conf", "a") as f:
        f.write(str(ct) + " " + " ".join(list(combination)))
        f.write('\n')
    if (ct % 1000) == 0:
        ct = 0
        f_num += 1
