% This function reads all barcodes from a set of filenames.
%
% Parameters:
%   filenames: Cell array of strings which are the filenames of the barcode
%     files in this directory to read.
% Returns an array of numbers: Barcode values of the files given, in the same
%   order.

function retval = save_barcodes(filenames);

barcodeKeys = filenames;
for i = 1:numel(filenames)
  barcodeKeys(i) = read_barcode(filenames{i});
end

barcodeKeysNum = cell2mat(barcodeKeys);
retval = barcodeKeysNum;

return;
