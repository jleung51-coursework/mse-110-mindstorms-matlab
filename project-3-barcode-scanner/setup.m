% This MATLAB program installs necessary dependencies.

% Octave-specific commands
isOctave = exist('OCTAVE_VERSION', 'builtin') ~= 0;
if isOctave
  pkg install -forge control
  pkg install -forge signal
end
