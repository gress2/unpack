def parse_benchmark(rawString):
    contentAry = rawString.split()
    for idx in range(len(contentAry)):
        if contentAry[idx] == 'real':
            return float(contentAry[idx - 1])

def parse_chrono_benchmark(rawString):
    contentAry = rawString.split()
    return float(contentAry[len(contentAry) - 1])

def parse_google_benchmark(rawString):
    contentAry = rawString.split()
    aryLen = len(contentAry)
    # total run time / iterations / 1billion (to get seconds from nanoseconds)
    return (float(contentAry[aryLen - 5]) / float(contentAry[aryLen - 1])) / 1e9

class OutputParser:
    parse = None
    def __init__(self, executable):
        if "unpack_benchmark" in executable:
            self.parse = parse_benchmark
        if "unpack_chrono_benchmark" in executable:
            self.parse = parse_chrono_benchmark
        if "unpack_google_benchmark" in executable:
            self.parse = parse_google_benchmark
