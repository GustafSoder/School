% Script which uses MyRichardson to get an approximation length using the
% trapezoid rule.
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

% Methods
m=["rk1","rk2","rk3","rk4"];

% Number of rows in table
kmax=10;

% Define the function needed for arc length
g=@(z)sqrt(z(3,:).^2+z(4,:).^2);
% Loop over methods
for i=1:4
    % Select method
    method=m(i);
    
    % Initialize time step
    dt=h0;
    
    % Initialize maxstep
    maxstep=200;
    
    % Loop over approximations
    for k=1:kmax
      % Compute range
      [r, flag, t, tra]=range_rkx(param,v0,theta,method,dt,maxstep);
      % Save information
      a(k)=a3int(g,t,tra);
      % Decrease time step
      dt=dt/2;
      % Increase maxstep
      maxstep=maxstep*2;
    end
    
    % Run Richardsons techniques
    data=MyRichardson(a,i);
    
    % New figure
    h(i)=figure();
    
    % Print to screen
    rdifprint(data,i);
end