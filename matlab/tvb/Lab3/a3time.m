% Script which uses MyRichardson to get an approximation of time from range_rkx
%
% PROGRAMMING by  Mathias Hallberg (c19mhg@cs.umu.se)
%                Gustaf Soderlund (et14gsd@cs.umu.se)
%               
%   2022-01-13  Finished the program
% Clean up

clear all

% Load parameters describing shot
a3f3

% Set initial time step
h0=1;

% Number of rows in table
kmax=10;

% Initialize time step
dt=h0;

% Select method
method = 'rk1';
% Initialize maxstep
maxstep=200;

% Loop over approximations
for k=1:kmax
  % Compute range
  [r, flag, t, tra]=range_rkx(param,v0,theta,method,dt,maxstep);
  % Save information
  a(k)=t(:, end);
  % Decrease time step
  dt=2^-k;
  % Increase maxstep
  maxstep=maxstep*2;
end

% Run Richardsons techniques
data=MyRichardson(a, 1);

% New figure
h(1)=figure();

% Print to screen
rdifprint(data, 1);


