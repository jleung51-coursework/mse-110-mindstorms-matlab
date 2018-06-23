% This MATLAB program installs necessary dependencies.

% Octave-specific commands
is_octave = exist('OCTAVE_VERSION', 'builtin') ~= 0;
if is_octave
  pkg install -forge control
  pkg install -forge signal
end