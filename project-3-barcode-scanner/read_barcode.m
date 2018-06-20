% This function analyses an image of a barcode, detects the individual bars,
% and returns a number representing the barcode value.
%
% Parameters:
%   filename: Image file in the current directory with the barcode to be read
% Returns a number: Barcode value of the file given (e.g. 113133111).

function retval = read_barcode(filename);

img = double(imread(filename));
data = img(850, :);

% Calculate moving average
average_length = 3;
data_averaged = data(1:numel(data)-(average_length-1));
for i = 1:numel(data_averaged) - average_length
  data_averaged(i) = sum(data(i:i+average_length-1))/average_length;
end

% Calculate derivative
data_derivative = data_averaged(1:numel(data_averaged)-1);
for i = 1:numel(data_derivative)
  data_derivative(i) = data_averaged(i+1) - data_averaged(i);
  data_derivative(i) = abs(data_derivative(i));
end

% Locate peaks
[peaks, locations] = findpeaks( ...
    data_derivative, ...
    'MinPeakHeight', 40, ...
    'MinPeakDistance', 7);

%plot(data); hold on;
%plot(data_averaged); hold on;
%plot(data_derivative); hold on; plot(locations, peaks, 'or'); hold on;
    
% Calculate distance between each peak
distances = locations(1:numel(locations)-1);
for i = 1:numel(distances)
  distances(i) = locations(i+1) - locations(i);
end
distances_normalized = round(distances/min(distances));

barcode = str2num(strrep(num2str(distances_normalized), ' ', ''));
retval = barcode
return