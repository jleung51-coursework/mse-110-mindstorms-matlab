#!/bin/sh
# This sh script modifies all ROBOTC .c files in all subdirectories to use
# the dummy code, so it can run in an environment without the ROBOTC compiler.

# Regex:
# Replace //[0 or 1 char space]#define NOTROBOTC
# With    #define NOTROBOTC
find . -path '**/*.c' -exec sed -i 's/\/\/.*#define NOTROBOTC/#define NOTROBOTC/g' {} +

# Regex:
# Replace //[0 or 1 char space]#include "../robotc_dummy.h"
# With    #include "../robotc_dummy.h"
find . -path '**/*.c' -exec sed -i 's/\/\/.*#include \"..\/robotc_dummy.h\"/#include \"..\/robotc_dummy.h\"/g' {} +
