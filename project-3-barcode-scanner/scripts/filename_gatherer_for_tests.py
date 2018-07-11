#!/usr/bin/env python3
#
# This Python 3 program collects the filenames of the sample barcodes and
# prints them out in a format which can be copy and pasted into the
# test suite.

import os

def print_formatted_filenames(dir_name):
    filenames = os.listdir(os.path.abspath(__file__ + "/../" + dir_name))
    filenames.sort()

    for line in filenames:
        for char in line:
            if char.isupper():
              letter = char
              break
        if "broken" not in line:
            test_result = "1"
            for char in line:
                if char.isupper():
                    test_comparison = char
                    break
        else:
            test_result = "0"
            test_comparison = "Nothing"
        print("\'" + dir_name + "/" + line + "\' \'" + test_result +
                "\' \'" + test_comparison + "\'")

print_formatted_filenames("../data/sample-barcode-pics")
print("...")
print_formatted_filenames("../data/sample-barcode-scans")
