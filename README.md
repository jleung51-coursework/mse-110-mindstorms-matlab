# MSE 110: Projects

This repository contains the projects for _MSE 110: Mechatronics Design I_ at Simon Fraser University for the Summer 2018 semester.

The projects are coded in **Lego Mindstorms EV3 ROBOTC** and **MATLAB**.

## Setup

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

To test the compilation of the ROBOTC programs, run `make` and use any of the options that appear.  
To run the ROBOTC programs, use the ROBOTC IDEs.

To run the MATLAB programs, use the MATLAB or GNU Octave IDEs.
