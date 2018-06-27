% This function analyses an image or CSV of a barcode, detects the individual
% bars, and returns a number representing the barcode value.
%
% Octave-specific dependencies:
%   signal: Required to find the changes in color.
%     The command `pkg load signal` must be executed beforehand.
%
% Parameters:
%   filename: File in the current directory with the barcode to be read
% Returns a number: Barcode value of the file given (e.g. 113133111).

function retval = read_barcode(filename);
fprintf('[DEBUG  ] Reading barcode from file: %s\n', filename);

extension = filename([end-2:end]);
fprintf('[DEBUG  ] File extension detected: %s\n', extension);
if strcmp('csv', extension)
  data = csvread(filename);
  data = data';

  % Set variables calibrated for ROBOTC scanning
  average_length = 30;
  min_peak_height = 0.4;
  min_peak_distance_base = 25;

elseif strcmp('jpg', extension) || ...
    strcmp('jpeg', extension) || ...
    strcmp('png', extension)

  img = double(imread(filename));
  data = img(850, :);
  % Uncomment the line below to save the data to a CSV file
  % dlmwrite('input.csv', data);

  % Set variables calibrated for image scanning
  average_length = 3;
  min_peak_height = 40;
  min_peak_distance_base = 7;

else
  fprintf('The file extension to be read (%s) was invalid.\n', extension);
  exit(1);
end

% Calculate moving average
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

% Remove end data where the button press disrupts the scanner
data_derivative( ...
    numel(data_derivative) - round(numel(data_derivative)*0.09) : ...
    numel(data_derivative) ...
) = [];

% Locate first and last peaks
[peaks, locations] = findpeaks( ...
    data_derivative, ...
    'MinPeakHeight', min_peak_height, ...
    'MinPeakDistance', min_peak_distance_base);
fprintf('[DEBUG  ] Number of peaks found in first search: %d\n', numel(peaks));

% Calculate the optimal peak distance (varies based on size of barcode)
total_units = 15;
range = locations(numel(locations)) - locations(1);
min_peak_distance = (range/total_units) * 0.85;
fprintf('[DEBUG  ] Optimal peak distance: %d\n', min_peak_distance);

% Locate actual peaks
[peaks, locations] = findpeaks( ...
    data_derivative, ...
    'MinPeakHeight', min_peak_height, ...
    'MinPeakDistance', min_peak_distance);
fprintf('[DEBUG  ] Number of peaks found in optimal search: %d\n', numel(peaks));

while numel(peaks) > 10
  index = find(peaks == min(peaks));
  index = index(1);
  fprintf('[DEBUG  ] Too many peaks, removing %d at index %d\n', min(peaks), index);
  peaks(index) = [];
  locations(index) = [];
end;

%plot(data); hold on;
%plot(data_averaged); hold on;
%plot(data_derivative); hold on; plot(locations, peaks, 'or'); hold on;

% Calculate distance between each peak
distances = locations(1:numel(locations)-1);
for i = 1:numel(distances)
  distances(i) = locations(i+1) - locations(i);
end
fprintf('[DEBUG  ] Distances between each peak: %s\n', num2str(distances));

% Reduce the values to multiples of the bar length
distances_normalized = round(distances/min(distances));

% All bar lengths are either 1 or 3
for i = 1:numel(distances_normalized)
  if distances_normalized(i) < 1
    distances_normalized(i) = 1;
  elseif distances_normalized(i) > 3
    distances_normalized(i) = 3;
  elseif distances_normalized(i) == 2
    distances_normalized(i) = 3;
    if(distances(i) < 2*min(distances))
      distances_normalized(i) = 1;
    end
  end
end

fprintf( ...
    '[DEBUG  ] Normalized distances between each peak: %s\n', ...
    num2str(distances_normalized) ...
);

% Concatenate all numbers in the array into a single string
% without whitespace
barcode = str2num(strrep(num2str(distances_normalized), ' ', ''));
fprintf('[INFO   ] Barcode value read: %d\n', barcode);
retval = barcode;
return
