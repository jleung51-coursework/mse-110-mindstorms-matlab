% This MATLAB program tests the function read_barcode.

function retval = test_read_barcode();

% Octave-specific commands
is_octave = exist('OCTAVE_VERSION', 'builtin') ~= 0;
if is_octave
  pkg load signal;
end

clear;
close all;
clc;

barcodes = { ...
    'sample-barcode-pics/letter_A_noisy.jpg' '1' 'A'
    'sample-barcode-pics/letter_B_noisy.jpg' '1' 'B'
    'sample-barcode-pics/letter_C_noisy.jpg' '1' 'C'
    'sample-barcode-pics/letter_D_noisy.jpg' '1' 'D'
    'sample-barcode-pics/letter_E_noisy.jpg' '1' 'E'
    'sample-barcode-pics/letter_F_noisy.jpg' '1' 'F'
    'sample-barcode-pics/letter_G_noisy.jpg' '1' 'G'
    'sample-barcode-pics/letter_H_noisy.jpg' '1' 'H'
    'sample-barcode-pics/letter_I_noisy.jpg' '1' 'I'
    'sample-barcode-pics/letter_J_noisy.jpg' '1' 'J'
    'sample-barcode-pics/letter_K_noisy.jpg' '1' 'K'
    'sample-barcode-pics/letter_L_noisy.jpg' '1' 'L'
    'sample-barcode-pics/letter_M_noisy.jpg' '1' 'M'
    'sample-barcode-pics/letter_N_noisy.jpg' '1' 'N'
    'sample-barcode-pics/letter_O_noisy.jpg' '1' 'O'
    'sample-barcode-pics/letter_P_noisy.jpg' '1' 'P'
    ...
    'sample-barcode-scans/input_letter_A_large_backwards.csv' '1' 'A'
    'sample-barcode-scans/input_letter_A_large_forwards.csv' '1' 'A'
    'sample-barcode-scans/input_letter_A_medium_backwards.csv' '1' 'A'
    'sample-barcode-scans/input_letter_A_medium_forwards.csv' '1' 'A'
    'sample-barcode-scans/input_letter_K_medium_backwards.csv' '1' 'K'
    'sample-barcode-scans/input_letter_K_medium_forwards.csv' '1' 'K'
    'sample-barcode-scans/input_letter_A_medium_forwards_broken.csv' '0' 'Nothing'
    ...
    };

tests = 0;
failures = 0;

for i = 1:size(barcodes, 1)  % Number of rows
    tests = tests + 1;

    filename = char(barcodes(i, 1));
    barcode = read_barcode(filename);
    val = decode_barcode(barcode);

    assert_true = strcmp('1', char(barcodes(i, 2)));
    if assert_true
      if strcmp(val, char(barcodes(i, 3)))
        fprintf( ...
            '[SUCCESS] File %s decoded to barcode %s\n', ...
            filename, val ...
        );
      else
        failures = failures + 1;
        if val == []
          fprintf( ...
              '[FAILURE] File %s could not be decoded. The expected value was %s.\n', ...
              filename, char(barcodes(i, 3)) ...
          );
        else
          fprintf( ...
              '[FAILURE] File %s was decoded to %s but the expected value was %s.\n', ...
              filename, val, char(barcodes(i, 3)) ...
          );
        end
      end

    else
      if isempty(val)
        fprintf( ...
            '[SUCCESS] File %s decoded to broken barcode %s\n', ...
            filename, val ...
        );
      else
        failures = failures + 1;
        fprintf( ...
            '[FAILURE] File %s should have been a broken barcode but decoded to %d with the value %s\n', ...
            filename, barcode, val ...
        );
      end
    end

end

fprintf('\n');
fprintf('[INFO   ] --------------------------------------------------------\n');
if failures == 0
  fprintf('[INFO   ] TEST SUITE PASSED\n');
else
  fprintf('[INFO   ] TEST SUITE FAILED\n');
end
fprintf('[INFO   ] --------------------------------------------------------\n');
fprintf('[INFO   ] %d tests run.\n', tests);
if failures == 1
  fprintf('[INFO   ] %d test failed.\n', failures);
else
  fprintf('[INFO   ] %d tests failed.\n', failures);
end
fprintf('[INFO   ] --------------------------------------------------------\n');
fprintf('\n');

%{
if failures ~= 0
  error("%d tests failed.", failures);
end
%}

end

% This function returns the value of a barcode, or [] if the barcode is invalid.
function retval = decode_barcode(code)

  barcode_keys = [ ...
     311113113 113113113 313113111 111133113 ...
     311133111 113133111 111113313 311113311 ...
     113113311 111133311 311111133 113111133 ...
     313111131 111131133 311131131 113131131 ];
  barcode_values = [ ...
      'A' 'B' 'C' 'D' ...
      'E' 'F' 'G' 'H' ...
      'I' 'J' 'K' 'L' ...
      'M' 'N' 'O' 'P' ];

  index = find(barcode_keys == code);
  if isempty(index)
      index = find(barcode_keys == str2num(fliplr(num2str(code))));
  end

  retval = [];
  if isempty(index) == false
    retval = barcode_values(index);
  end

end
