% This MATLAB program tests the function read_barcode.

function retval = testreadbarcode();

% Octave-specific commands
isOctave = exist('OCTAVE_VERSION', 'builtin') ~= 0;
if isOctave
  pkg load signal;
end

clear;
close all;
clc;

barcodes = { ...
    '../data/sample-barcode-pics/letter_A_noisy.jpg' '1' 'A'
    '../data/sample-barcode-pics/letter_B_noisy.jpg' '1' 'B'
    '../data/sample-barcode-pics/letter_C_noisy.jpg' '1' 'C'
    '../data/sample-barcode-pics/letter_D_noisy.jpg' '1' 'D'
    '../data/sample-barcode-pics/letter_E_noisy.jpg' '1' 'E'
    '../data/sample-barcode-pics/letter_F_noisy.jpg' '1' 'F'
    '../data/sample-barcode-pics/letter_G_noisy.jpg' '1' 'G'
    '../data/sample-barcode-pics/letter_H_noisy.jpg' '1' 'H'
    '../data/sample-barcode-pics/letter_I_noisy.jpg' '1' 'I'
    '../data/sample-barcode-pics/letter_J_noisy.jpg' '1' 'J'
    '../data/sample-barcode-pics/letter_K_noisy.jpg' '1' 'K'
    '../data/sample-barcode-pics/letter_L_noisy.jpg' '1' 'L'
    '../data/sample-barcode-pics/letter_M_noisy.jpg' '1' 'M'
    '../data/sample-barcode-pics/letter_N_noisy.jpg' '1' 'N'
    '../data/sample-barcode-pics/letter_O_noisy.jpg' '1' 'O'
    '../data/sample-barcode-pics/letter_P_noisy.jpg' '1' 'P'
    ...
    '../data/sample-barcode-scans/input_letter_A_large_backwards.csv' '1' 'A'
    '../data/sample-barcode-scans/input_letter_A_large_forwards.csv' '1' 'A'
    '../data/sample-barcode-scans/input_letter_A_medium_backwards.csv' '1' 'A'
    '../data/sample-barcode-scans/input_letter_A_medium_forwards.csv' '1' 'A'
    '../data/sample-barcode-scans/input_letter_A_medium_forwards_broken.csv' '0' 'Nothing'
    '../data/sample-barcode-scans/input_letter_B_medium_backwards.csv' '1' 'B'
    '../data/sample-barcode-scans/input_letter_B_medium_forwards.csv' '1' 'B'
    '../data/sample-barcode-scans/input_letter_B_small_forwards.csv' '1' 'B'
    '../data/sample-barcode-scans/input_letter_C_large_backwards.csv' '1' 'C'
    '../data/sample-barcode-scans/input_letter_C_medium_backwards.csv' '1' 'C'
    '../data/sample-barcode-scans/input_letter_C_medium_forwards.csv' '1' 'C'
    '../data/sample-barcode-scans/input_letter_C_small_forwards.csv' '1' 'C'
    '../data/sample-barcode-scans/input_letter_D_medium_backwards.csv' '1' 'D'
    '../data/sample-barcode-scans/input_letter_D_medium_forwards.csv' '1' 'D'
    '../data/sample-barcode-scans/input_letter_E_medium_backwards.csv' '1' 'E'
    '../data/sample-barcode-scans/input_letter_E_medium_forwards.csv' '1' 'E'
    '../data/sample-barcode-scans/input_letter_F_medium_backwards.csv' '1' 'F'
    '../data/sample-barcode-scans/input_letter_F_medium_forwards.csv' '1' 'F'
    '../data/sample-barcode-scans/input_letter_G_medium_backwards.csv' '1' 'G'
    '../data/sample-barcode-scans/input_letter_G_medium_forwards.csv' '1' 'G'
    '../data/sample-barcode-scans/input_letter_H_medium_backwards.csv' '1' 'H'
    '../data/sample-barcode-scans/input_letter_H_medium_forwards.csv' '1' 'H'
    '../data/sample-barcode-scans/input_letter_I_medium_backwards.csv' '1' 'I'
    '../data/sample-barcode-scans/input_letter_I_medium_forwards.csv' '1' 'I'
    '../data/sample-barcode-scans/input_letter_I_small_forwards.csv' '1' 'I'
    '../data/sample-barcode-scans/input_letter_J_medium_backwards.csv' '1' 'J'
    '../data/sample-barcode-scans/input_letter_J_medium_forwards.csv' '1' 'J'
    '../data/sample-barcode-scans/input_letter_J_small_forwards.csv' '1' 'J'
    '../data/sample-barcode-scans/input_letter_K_medium_forwards.csv' '1' 'K'
    '../data/sample-barcode-scans/input_letter_L_medium_backwards.csv' '1' 'L'
    '../data/sample-barcode-scans/input_letter_L_medium_forwards.csv' '1' 'L'
    '../data/sample-barcode-scans/input_letter_M_medium_backwards.csv' '1' 'M'
    '../data/sample-barcode-scans/input_letter_M_medium_forwards.csv' '1' 'M'
    '../data/sample-barcode-scans/input_letter_N_medium_forwards.csv' '1' 'N'
    '../data/sample-barcode-scans/input_letter_O_medium_forwards.csv' '1' 'O'
    '../data/sample-barcode-scans/input_letter_P_medium_backwards.csv' '1' 'P'
    '../data/sample-barcode-scans/input_letter_P_medium_forwards.csv' '1' 'P'
    '../data/sample-barcode-scans/input_letter_Q_medium_forwards.csv' '1' 'Q'
    '../data/sample-barcode-scans/input_letter_R_medium_forwards.csv' '1' 'R'
    '../data/sample-barcode-scans/input_letter_S_medium_backwards.csv' '1' 'S'
    '../data/sample-barcode-scans/input_letter_S_medium_forwards.csv' '1' 'S'
    '../data/sample-barcode-scans/input_letter_T_medium_forwards.csv' '1' 'T'
    '../data/sample-barcode-scans/input_letter_U_medium_forwards.csv' '1' 'U'
    '../data/sample-barcode-scans/input_letter_V_medium_forwards.csv' '1' 'V'
    '../data/sample-barcode-scans/input_letter_W_medium_forwards.csv' '1' 'W'
    '../data/sample-barcode-scans/input_letter_X_medium_forwards.csv' '1' 'X'
    '../data/sample-barcode-scans/input_letter_Y_medium_forwards.csv' '1' 'Y'
    '../data/sample-barcode-scans/input_letter_Z_medium_forwards.csv' '1' 'Z'
    };

tests = 0;
failures = 0;

for i = 1:size(barcodes, 1)  % Number of rows
    tests = tests + 1;

    filename = char(barcodes(i, 1));
    barcode = readbarcode(filename);
    val = decodebarcode(barcode);

    assertTrue = strcmp('1', char(barcodes(i, 2)));
    if assertTrue
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

if failures ~= 0
  error('%d tests failed.', failures);
end

end

% This function returns the value of a barcode, or [] if the barcode is invalid.
function retval = decodebarcode(code)

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

  index = find(barcodeKeys == code);
  if isempty(index)
      index = find(barcodeKeys == str2num(fliplr(num2str(code))));
  end

  retval = [];
  if isempty(index) == false
    retval = barcodeValues(index);
  end

end
