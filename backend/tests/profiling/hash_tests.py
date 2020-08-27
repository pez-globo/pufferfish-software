"""Script to test time complexity of crc and hash functions."""

import hashlib
import os
import time
import sys
from crcmod import predefined as predefinedcrc
from typing import Optional, Dict, List, Any, Tuple, TypeVar, Callable

from ventserver.util import crc


def gen_data(low: int = 32, high: int = 64,
    increment: int = 2, multiplier: bool = True,
    additionals: Optional[List[int]] = None
) -> Dict[int, bytes]:
    """Generate a dictionary of random bytes of given size."""
    
    assert (high > low), "high should be greater than low." 
    data: dict = dict()
    while low <= high:
        data[low] = bytes(os.urandom(low))
        if multiplier:
            low *= increment
        else:
            low += increment

    if additionals:
        assert (isinstance(additionals,list)), "additionals should be a list instance."
        for each in additionals:
            data[each] = bytes(os.urandom(each))

    return data


def calculate_timings(data: Dict[int, bytes], 
    functions: Dict[str, Callable[[bytes], bytes]], 
    iterations: int = 1000
) -> Tuple[Dict[str, Dict[int, float]], Dict[str, Dict[int, float]]]:
    """Calculate the time complexity for given hash functions and data."""
    
    print(f"Processing for {iterations} iterations.")
    timings: Dict[str,Dict[int, float]] = {_func:dict() for _func in functions}
    timings_avg: Dict[str,Dict[int, float]] = {_func:dict() for _func in functions}
    
    for each in data:
        for func in functions:
            if 'crc' in func:
                test_time = list()
                for i in range(iterations):
                    start = time.perf_counter()
                    _ = functions[func](data[each])
                    end = time.perf_counter()
                    test_time.append(end - start)
                timings[func][each] = sum(test_time)
                timings_avg[func][each] = sum(test_time)/len(test_time)
            else:
                test_time = list()
                for i in range(iterations):
                    hash_func = hashlib.new(func)
                    start = time.perf_counter()
                    hash_func.update(data[each])
                    _ = hash_func.digest()
                    end = time.perf_counter()
                    test_time.append(end - start)
                    del hash_func
                timings[func][each] = sum(test_time)
                timings_avg[func][each] = sum(test_time)/len(test_time)
        print(f"Completed {each}")

    return timings, timings_avg

def print_timings(timings: Dict[Any, Dict[int, float]], 
    timings_avg: Dict[Any, Dict[int, float]]
):
    """Print the time generated by calculate_timings."""
    byte_sizes = list(timings.values())[0].keys()
    print("Time taken in seconds total(avg))...")
    bytes_header = "\t\t\t".join([str(byte_size) for byte_size in byte_sizes])
    print("bytes\t\t" + bytes_header)

    for func in timings:
        timings_ = "\t".join(["{0:f}({1:f})".format(total, avg) for \
         total, avg in zip(timings[func].values(), timings_avg[func].values())])
        print(f"{func}\t\t{timings_}")

def main():

    functions: Dict[str, Callable[[bytes], bytes]] = {
        'pf_crc': lambda data: crc.compute_reflected_crc(data, \
            crc.CRC32C_REFLECTED_TABLE),
        'mod_crc': predefinedcrc.mkCrcFun('crc-32c'),
        'sha1': hashlib.sha1,
        'md5': hashlib.md5,
        'blake2b': hashlib.blake2b,
        'blake2s': hashlib.blake2s
    }

    low: int = 32
    high: int = 1024
    increment: int = 2
    iterations: List[int] = [1000, 5000, 10000, 15000]
    data: Dict[str,List[float]] = gen_data(low = low, high = high, \
        increment = increment, additionals = [20])
    
    # print actual size of data objects in the memory
    print("Actual sizes of generated chunks ...")
    for each in data.values():
        print(f"{len(each)} bytes: {sys.getsizeof(each)}")

    # measure timings
    for iteration in iterations:
        timings_total, timings_avg = calculate_timings(data, \
            functions, iterations=iteration)

        print_timings(timings_total, timings_avg) 

if __name__ == '__main__':
    main()
