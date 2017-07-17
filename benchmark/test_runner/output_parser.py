def parse_benchmark(rawString):
    print rawString

def parse_chrono_benchmark(rawString):
    print rawString

def parse_google_benchmark(rawString):
    print rawString

class OutputParser:
    parse = None
    def __init__(self, executable):
        if "unpack_benchmark" in executable:
            self.parse = parse_benchmark
        if "unpack_chrono_benchmark" in executable:
            self.parse = parse_chrono_benchmark
        if "unpack_google_benchmark" in executable:
            self.parse = parse_google_benchmark
