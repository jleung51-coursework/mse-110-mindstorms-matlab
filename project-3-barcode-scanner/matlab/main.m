% This MATLAB program reads a CSV file representing image values from reading
% a barcode, and decodes it into one of several preset values.

% Octave-specific commands
isOctave = exist('OCTAVE_VERSION', 'builtin') ~= 0;
if isOctave
  pkg load signal;
end

clear;
close all;
clc;

barcodeValues = [ ...
    'A' 'B' 'C' 'D' ...
    'E' 'F' 'G' 'H' ...
    'I' 'J' 'K' 'L' ...
    'M' 'N' 'O' 'P' ...
    'Q' 'R' 'S' 'T' ...
    'U' 'V' 'W' 'X' ...
    'Y' 'Z' ];
barcodeKeys = [ ...
   311113113 113113113 313113111 111133113 ...
   311133111 113133111 111113313 311113311 ...
   113113311 111133311 311111133 113111133 ...
   313111131 111131133 311131131 113131131 ...
   111111333 311111331 113111331 111131331 ...
   331111113 133111113 333111111 131131113 ...
   331131111 133131111 ];
barcodeReversibles = {'KU', 'NY', 'OX', 'QW', 'RV', 'TZ'};

% Uncomment the following lines to decode and print out the barcode keys of
% the files in this directory
%{
barcodeFilenames = cell(numel(barcodeValues), 1);
for i = 1:numel(barcodeFilenames)
  barcodeFilenames{i} = [ ...
      strcat('../data/sample-barcodes/letter_', num2str(barcodeValues(i)), '_noisy.jpg') ...
  ];
end
barcodeKeys = savebarcodes(barcodeFilenames)
%}

code = readbarcode('../data/datalog-0.txt');

index = find(barcodeKeys == code);

if isempty(index)
    index = find(barcodeKeys == str2num(fliplr(num2str(code))));
end

if isempty(index)
  fprintf('[INFO   ] Barcode not found.\n');
else
  value = barcodeValues(index);

  equivalence = '';
  for i = 1:numel(barcodeReversibles)
      charFirst = barcodeReversibles{i}(1);
      charSecond = barcodeReversibles{i}(2);
      if strcmp(charFirst, value)
          equivalence = ['(Equivalent to ', charSecond, ' reversed)'];
      elseif strcmp(charSecond, value)
          equivalence = ['(Equivalent to ', charFirst, ' reversed)'];
      end
  end

  fprintf('[INFO   ] Barcode: %s %s\n', value, equivalence);
end
