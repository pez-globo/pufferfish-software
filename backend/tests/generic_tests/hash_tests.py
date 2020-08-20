"""Script to test time complexity of crc and hash functions."""

import hashlib
import os
import time
import sys


from ventserver.util import crc


functions = {
    'crc': crc.compute_reflected_crc,
    'sha1': hashlib.sha1,
    'md5': hashlib.md5,
    'blake2b': hashlib.blake2b,
    'blake2s': hashlib.blake2s
}

timings = {func:list() for func in functions}

data = []
data_size = 32

while data_size <= 1024:
    data.append(bytes(os.urandom(data_size)))
    data_size *= 2

print("Actual sizes of generated chunks ...")
for each in data:
    print(f"{len(each)} bytes: {sys.getsizeof(each)}")

for each in data:
    for func in functions:
        if func == 'crc':
            start = time.perf_counter()
            _ = functions[func](each)
            end = time.perf_counter()
            timings[func].append(end - start)
        else:
            m = hashlib.new(func)
            start = time.perf_counter()
            m.update(each)
            _ = m.digest()
            end = time.perf_counter()
            timings[func].append(end - start)
            del m
    print(f"Completed {len(each)}")
    

print("Time taken in seconds ...")
print("bytes\t\t32\t\t64\t\t128\t\t256\t\t512\t\t1024")

for func in timings:
    timings_ = "\t".join(["%f"%each for each in timings[func]])
    print(f"{func}\t\t{timings_}")
