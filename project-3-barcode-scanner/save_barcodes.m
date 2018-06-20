% This function reads all barcodes from a set of filenames.
%
% Parameters:
%   filenames: Cell array of strings which are the filenames of the barcode
%     files in this directory to read.
% Returns an array of numbers: Barcode values of the files given, in the same
%   order.

function retval = save_barcodes(filenames);

barcode_keys = filenames;
for i = 1:numel(filenames)
  barcode_keys(i) = read_barcode(filenames{i});
end

barcode_keys_num = cell2mat(barcode_keys);
retval = barcode_keys_num;

return;