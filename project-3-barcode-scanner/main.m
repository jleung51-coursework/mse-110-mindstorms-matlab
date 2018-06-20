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

% Uncomment the following lines to decode and print out the barcode keys of
% the files in this directory
%{
barcode_filenames = cell(numel(barcode_values), 1);
for i = 1:numel(barcode_filenames)
  barcode_filenames{i} = [ ...
      strcat('sample-barcodes/letter_', num2str(barcode_values(i)), '_noisy.jpg') ...
  ];
end
barcode_keys = save_barcodes(barcode_filenames)
%}

%code = read_barcode('input.csv');  % TODO: Implement CSV functionality
code = 311111133;

index = find(barcode_keys == code);
if index == -1
  fprintf('Barcode not found.\n');
else
  fprintf('Barcode: %s\n', barcode_values(index));
end