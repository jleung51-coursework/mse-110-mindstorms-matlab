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

function retval = readbarcode(filename);
fprintf('[DEBUG  ] Reading barcode from file: %s\n', filename);

extension = filename([end-2:end]);
fprintf('[DEBUG  ] File extension detected: %s\n', extension);
if strcmp('csv', extension) || strcmp('txt', extension)
  data = csvread(filename);
  data = data';

  % Set variables calibrated for ROBOTC scanning
  averageLength = 30;
  minPeakHeight = 0.09;
  minPeakDistanceBase = 25;

elseif strcmp('jpg', extension) || ...
    strcmp('jpeg', extension) || ...
    strcmp('png', extension)

  img = double(imread(filename));
  data = img(850, :);
  % Uncomment the line below to save the data to a CSV file
  % dlmwrite('input.csv', data);

  % Set variables calibrated for image scanning
  averageLength = 3;
  minPeakHeight = 40;
  minPeakDistanceBase = 7;

else
  fprintf('The file extension to be read (%s) was invalid.\n', extension);
  exit(1);
end

% Calculate moving average
dataAveraged = data(1:numel(data)-(averageLength-1));
for i = 1:numel(dataAveraged) - averageLength
  dataAveraged(i) = sum(data(i:i+averageLength-1))/averageLength;
end

% Calculate derivative
dataDerivative = dataAveraged(1:numel(dataAveraged)-1);
for i = 1:numel(dataDerivative)
  dataDerivative(i) = dataAveraged(i+1) - dataAveraged(i);
  dataDerivative(i) = abs(dataDerivative(i));
end

% Remove end data where the button press disrupts the scanner
dataDerivative( ...
    numel(dataDerivative) - round(numel(dataDerivative)*0.09) : ...
    numel(dataDerivative) ...
) = [];

% Locate first and last peaks
[peaks, locations] = findpeaks( ...
    dataDerivative, ...
    'MinPeakHeight', minPeakHeight, ...
    'MinPeakDistance', minPeakDistanceBase);
fprintf('[DEBUG  ] Number of peaks found in first search: %d\n', numel(peaks));

% Calculate the optimal peak distance (varies based on size of barcode)
totalUnits = 15;
range = locations(numel(locations)) - locations(1);
minPeakDistance = (range/totalUnits) * 0.55;
fprintf('[DEBUG  ] Optimal peak distance: %d\n', minPeakDistance);

% Locate actual peaks
[peaks, locations] = findpeaks( ...
    dataDerivative, ...
    'MinPeakHeight', minPeakHeight, ...
    'MinPeakDistance', minPeakDistance);
fprintf('[DEBUG  ] Number of peaks found in optimal search: %d\n', numel(peaks));

% Too many peaks
while numel(peaks) > 10
  index = find(peaks == min(peaks));
  index = index(1);
  fprintf('[DEBUG  ] Too many peaks, removing %d at index %d\n', min(peaks), index);
  peaks(index) = [];
  locations(index) = [];
end

%plot(data); hold on;
%plot(dataAveraged); hold on;
%plot(dataDerivative); hold on; plot(locations, peaks, 'or'); hold on;

% Calculate distance between each peak
distances = locations(1:numel(locations)-1);
for i = 1:numel(distances)
  distances(i) = locations(i+1) - locations(i);
end
fprintf('[DEBUG  ] Distances between each peak: %s\n', num2str(distances));

% Reduce the values to multiples of the bar length
distancesNormalized = round(distances/min(distances));

% All bar lengths are either 1 or 3
for i = 1:numel(distancesNormalized)
  if distancesNormalized(i) < 1
    distancesNormalized(i) = 1;
  elseif distancesNormalized(i) > 3
    distancesNormalized(i) = 3;
  elseif distancesNormalized(i) == 2
    distancesNormalized(i) = 3;
    if(distances(i) < 2*min(distances))
      distancesNormalized(i) = 1;
    end
  end
end

fprintf( ...
    '[DEBUG  ] Normalized distances between each peak: %s\n', ...
    num2str(distancesNormalized) ...
);

% Concatenate all numbers in the array into a single string
% without whitespace
barcode = str2num(strrep(num2str(distancesNormalized), ' ', ''));
fprintf('[INFO   ] Barcode value read: %d\n', barcode);
retval = barcode;
return
