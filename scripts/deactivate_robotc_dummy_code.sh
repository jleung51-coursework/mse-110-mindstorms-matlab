#!/bin/sh
# This sh script modifies all ROBOTC .c files in all subdirectories to stop
# using the dummy code, so it can run in the ROBOTC environment.

# Regex:
# Replace [Beginning of string]#define NOTROBOTC
# With    // #define NOTROBOTC
find . -path '**/*.c' -exec sed -i 's/^#define NOTROBOTC/\/\/ #define NOTROBOTC/g' {} +

# Regex:
# Replace [Beginning of string]#include "../robotc_dummy.h"
# With    // #include "../robotc_dummy.h"
find . -path '**/*.c' -exec sed -i 's/^#include \"..\/robotc_dummy.h\"/\/\/ #include \"..\/robotc_dummy.h\"/g' {} +
