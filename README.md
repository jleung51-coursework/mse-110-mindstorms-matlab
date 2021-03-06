# MSE 110: Projects

[![Build Status](https://api.travis-ci.com/jleung51-coursework/mse-110-mindstorms-matlab.svg?branch=master)](https://travis-ci.com/jleung51-coursework/mse-110-mindstorms-matlab)

This repository contains the projects for _MSE 110: Mechatronics Design I_ at Simon Fraser University for the Summer 2018 semester.

The projects are coded in **Lego Mindstorms EV3 ROBOTC** and **MATLAB**.

## Setup

### ROBOTC

To compile and execute the ROBOTC code without a ROBOTC IDE, set up the [ROBOTC dummy header](https://github.com/jleung51/utilities/tree/master/robotc_dummy) in the root directory of this repository.

### MATLAB

Project 3 requires [MATLAB](https://www.mathworks.com/products/matlab.html).

If you do not have a MATLAB licence, [GNU Octave](https://www.gnu.org/software/octave/) is a free, open-source alternative. The project files are fully compatible with GNU Octave.

Install GNU Octave, then install the development packages:
```
sudo apt install liboctave-dev
```

From the console in Octave, install the required packages for finding the data peaks in `read_barcode`:
```
pkg install -forge control
pkg install -forge signal
```

## Execution

To test the compilation of the ROBOTC programs in GCC, run `make` and activate the dummy code before using any of the Make options.
To run the ROBOTC programs, use the ROBOTC IDEs.

To run the MATLAB programs, use the MATLAB or GNU Octave IDEs.

# Main Contributors

* Aryanna Mason
* Jeffrey Leung
