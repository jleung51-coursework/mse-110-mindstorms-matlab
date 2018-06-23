% This function analyses an image or CSV of a barcode, detects the individual
% bars, and returns a number representing the barcode value.
%
% Parameters:
%   filename: File in the current directory with the barcode to be read
% Returns a number: Barcode value of the file given (e.g. 113133111).

function retval = read_barcode(filename);
fprintf('[DEBUG  ] Reading barcode from file: %s\n', filename);

extension = filename([end-2:end]);
fprintf('[DEBUG  ] File extension detected: %s\n', extension);
if extension == 'csv'
  data = csvread(filename);
  data = data';

  % Set variables calibrated for ROBOTC scanning
  average_length = 30;
  min_peak_height = 0.6;
  min_peak_distance = 28;

elseif extension == 'jpg' || extension == 'jpeg' || extension == 'png'
  img = double(imread(filename));
  data = img(850, :);
  % Uncomment the line below to save the data to a CSV file
  % dlmwrite('input.csv', data);

  % Set variables calibrated for image scanning
  average_length = 3;
  min_peak_height = 40;
  min_peak_distance = 7;

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

% Locate peaks
[peaks, locations] = findpeaks( ...
    data_derivative, ...
    'MinPeakHeight', min_peak_height, ...
    'MinPeakDistance', min_peak_distance);

fprintf('[DEBUG  ] Number of peaks found: %d\n', numel(peaks));

%plot(data); hold on;
%plot(data_averaged); hold on;
%plot(data_derivative); hold on; plot(locations, peaks, 'or'); hold on;

% Calculate distance between each peak
distances = locations(1:numel(locations)-1);
for i = 1:numel(distances)
  distances(i) = locations(i+1) - locations(i);
end
fprintf('[DEBUG  ] Distances between each peak: %s\n', num2str(distances));

% Calculate the average of the smallest units
% Prevents the case where a slightly smaller minimum value makes
% a large value round up by mistake
min_distance = min(distances);
distance_unit_average = 0;
distance_unit_average_count = 0;

for i = 1:numel(distances)
  if min_distance*.5 <= distances(i) && distances(i) <= min_distance*1.5
    distance_unit_average = distance_unit_average + distances(i);
    distance_unit_average_count = distance_unit_average_count + 1;
   end
 end
distance_unit = distance_unit_average / distance_unit_average_count;
distance_unit = round(distance_unit);

% Reduce the values to multiples of the bar length
distances_normalized = round(distances/distance_unit);

fprintf( ...
    '[DEBUG  ] Normalized distances between each peak: %s\n', ...
    num2str(distances_normalized) ...
);

% Concatenate all numbers in the array into a single string
% without whitespace
barcode = str2num(strrep(num2str(distances_normalized), ' ', ''));
fprintf('[DEBUG  ] Barcode value read: %d\n', barcode);
retval = barcode;
return