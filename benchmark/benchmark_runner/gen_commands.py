import itertools

executable = "python benchmark_runner.py"
target_executable = ["unpack_chrono_benchmark", "unpack_google_benchmark", "unpack_benchmark"]
data_layout = ["aos", "soa"]
container = ["vector"]
container_size = [str(x) for x in [2**0, 2**10, 2**20]]
type_index = [str(x) for x in [0,1,2,3,4]]
operation_complexity = ["simple", "complex"]
access_pattern = ["single", "independent", "combined"]
iterations = [str(2**x) for x in range(20)]

parameter_space = [[executable], target_executable, data_layout, container, container_size,
        type_index, operation_complexity, access_pattern, iterations]

ct = 0

for combination in itertools.product(*parameter_space):
    ct += 1
    with open("com" + str(ct / 1000) + ".sh", "a") as f:
        f.write(" ".join(list(combination)))
        f.write('\n')
