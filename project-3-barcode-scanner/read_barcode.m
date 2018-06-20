% This function analyses an image or CSV of a barcode, detects the individual
% bars, and returns a number representing the barcode value.
%
% Parameters:
%   filename: File in the current directory with the barcode to be read
% Returns a number: Barcode value of the file given (e.g. 113133111).

function retval = read_barcode(filename);

extension = filename([end-2:end]);
if extension == 'csv'
  data = csvread(filename);
elseif extension == 'jpg' || extension == 'jpeg' || extension == 'png'
  img = double(imread(filename));
  data = img(850, :);
  % Uncomment the line below to save the data to a CSV file
  % dlmwrite('input.csv', data);
else
  fprintf('The file extension to be read (%s) was invalid.\n', extension);
  exit(1);
end

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

% Reduce the values to multiples of their smallest bar length
distances_normalized = round(distances/min(distances));

% Concatenate all numbers in the array into a single string without whitespace
barcode = str2num(strrep(num2str(distances_normalized), ' ', ''));
retval = barcode;
return