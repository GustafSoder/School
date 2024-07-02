% Script which uses MyRichardson to get an approximation of of the 
% angle to shoot to reach 15000 m 
%
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
kmax=6;

eps=0;delta=10^-10;
deg=linspace(0,45,1001);
rad=deg.*pi/180;
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
        table=compute_range(param,v0,rad,method,dt,maxstep, 'false');
        for j=1:numel(table(2,:))
            if (table(2,j)>15000)
                thetapos=j;
                break
            end
        end
      % Compute range
      r0=15000-range_rkx(param,v0,table(1,thetapos-1),method,dt,maxstep);
      r1=15000-range_rkx(param,v0,table(1,thetapos),method,dt,maxstep);
      % Save information
      thetafunc=@(theta)15000-range_rkx(param,v0,theta,method,dt,maxstep);
      [x,~,~,~,~,~,~]=bisection(thetafunc,table(1,thetapos-1),table(1,thetapos),r0,r1,delta,eps,10001,'true');
      a(k)=x;
      % Decrease time step
      dt=2^-k;
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