# MSE 110: Project 3 - Barcode Scanner

This project consists of a Lego Mindstorms EV3 ROBOTC program and a MATLAB program.

The ROBOTC program moves the robot forward to scan a barcode of arbitrary size, and saves the data to the datalog. The MATLAB program takes the data as input and decodes it to a barcode value.

Example output:
```
[DEBUG  ] Reading barcode from file: ../data/datalog-0.txt
[DEBUG  ] File extension detected: txt
[DEBUG  ] Number of peaks found in first search: 10
[DEBUG  ] Optimal peak distance: 20.0567
[DEBUG  ] Number of peaks found in optimal search: 10
[DEBUG  ] Distances between each peak: 40  103  108   35  102   42   40   39   38
[DEBUG  ] Normalized distances between each peak: 1  3  3  1  3  1  1  1  1
[INFO   ] Barcode value read: 133131111
[INFO   ] Barcode: Z (Equivalent to T reversed)
```

The MATLAB program is also capable of reading a barcode from an image file.

For instructions on setting up the dependencies for MATLAB or GNU Octave, see the README in the parent directory.
