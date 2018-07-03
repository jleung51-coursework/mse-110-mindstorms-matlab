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
    'M' 'N' 'O' 'P' ...
    'Q' 'R' 'S' 'T' ...
    'U' 'V' 'W' 'X' ...
    'Y' 'Z' ];
barcode_keys = [ ...
   311113113 113113113 313113111 111133113 ...
   311133111 113133111 111113313 311113311 ...
   113113311 111133311 311111133 113111133 ...
   313111131 111131133 311131131 113131131 ...
   111111333 311111331 113111331 111131331 ...
   331111113 133111113 333111111 131131113 ...
   331131111 133131111 ];
barcode_reversibles = {'KU', 'NY', 'OX', 'QW', 'RV', 'TZ'};

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

code = read_barcode('datalog-0.txt');

index = find(barcode_keys == code);

if isempty(index)
    index = find(barcode_keys == str2num(fliplr(num2str(code))));
end

if isempty(index)
  fprintf('[INFO   ] Barcode not found.\n');
else
  value = barcode_values(index);

  equivalence = '';
  for i = 1:numel(barcode_reversibles)
      char_first = barcode_reversibles{i}(1);
      char_second = barcode_reversibles{i}(2);
      if strcmp(char_first, value)
          equivalence = ['(Equivalent to ', char_second, ' reversed)'];
      elseif strcmp(char_second, value)
          equivalence = ['(Equivalent to ', char_first, ' reversed)'];
      end
  end

  fprintf('[INFO   ] Barcode: %s %s\n', value, equivalence);
end
