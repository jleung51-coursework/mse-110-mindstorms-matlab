% This MATLAB program reads a CSV file representing image values from reading
% a barcode, and decodes it into one of several preset values.

% Octave-specific commands
is_octave = exist('OCTAVE_VERSION', 'builtin') ~= 0;
if is_octave
  pkg load signal;
end

clear;
close all;
clc;

barcode_values = [ ...
    'A' 'B' 'C' 'D' ...
    'E' 'F' 'G' 'H' ...
    'I' 'J' 'K' 'L' ...
    'M' 'N' 'O' 'P' ];
barcode_keys = [ ...
   311113113 113113113 313113111 111133113 ...
   311133111 113133111 111113313 311113311 ...
   113113311 111133311 311111133 113111133 ...
   313111131 111131133 311131131 113131131 ];

barcodes = {
    'sample-barcode-scans/input_letter_A_large_backwards.csv' ...
    'sample-barcode-scans/input_letter_A_large_forwards.csv' ...
    'sample-barcode-scans/input_letter_A_medium_backwards.csv' ...
    'sample-barcode-scans/input_letter_A_medium_forwards.csv' ...
    'sample-barcode-scans/input_letter_K_medium_backwards.csv' ...
    'sample-barcode-scans/input_letter_K_medium_forwards.csv' };

for i = 1:numel(barcodes)
    code = read_barcode(barcodes{i});
    index = find(barcode_keys == code);
    if isempty(index)
        index = find(barcode_keys == str2num(fliplr(num2str(code))));
    end
    if isempty(index)
      fprintf('[FAILURE] File: %s | Barcode not found.\n', barcodes{i});
    else
      fprintf('[SUCCESS] File: %s | Barcode: %s\n', barcodes{i}, barcode_values(index));
    end
end