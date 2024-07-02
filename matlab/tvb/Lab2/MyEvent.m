% MyEvent.m, which simulates a impact of a projactile on a hill.
%
% PROGRAMMING by Carl Christian Kjelgaard Mikkelsen (spock@cs.umu.se)
%                Mathias Hallberg (c19mhg@cs.umu.se)
%                Gustaf Söderlund (et14gsd@cs.umu.se)
%   2021-12-6  Finished the working code
clear;clc;
% Load shells models
load shells.mat

% Specify shell and enviroment
param=struct('mass',10,'cali',0.088,'drag',@(x)mcg7(x),'atmo',@(x)atmosisa(x),'grav',@(x)9.82,'wind',@(t,x)[0, 0]);

% Set the muzzle velocity and the elevation of the gun
v0=780; theta=60*pi/180; 

% Select the method which will be used to integrate the trajectory
method='rk2'; 

% Select the basic time step size and the maximum number of time steps
dt=0.1; maxstep=2000;

% Compute the range of the shell
% Where t is our point of comparison
[r, ~, t, tra]=range_rkx(param,v0,theta,method,dt,maxstep);
% Calculate hermites approximation of t->x(t) and t->y(t)
% Maximum number of iterations
a=0;b=numel(tra(1,:));
maxit=b;

% Allocate space
n=zeros(maxit,1); mre=zeros(maxit,1);
t=linspace(t(1,1),t(1,end),100*maxit+1);

% Number of sample points
n(maxit)=10*maxit;
    
% Sample points
s=linspace(a,b,n(maxit)+1);

x=@(t)MyPiecewiseHermite(s,tra(1,:),tra(3,:),t);
y=@(t)MyPiecewiseHermite(s,tra(2,:),tra(4,:),t);
% Below follows a long sequence of commands which demonstrates how to get
% a very nice plot of the trajectory automatically
hold on;
% Obtain the coordinates of the corners of the screen
screen=get(groot,'Screensize'); 

% Isolate the width and height of the screen measured in pixels
sw=screen(3); sh=screen(4);

% Obtain a handle to a new figure
hFig=gcf;

% Set the position of the desired window
set(hFig,'Position',[0 sh/4 sw/2 sh/2]);

% Plot the trajectory of the shell and the hill
hillSpace=linspace(0,16000, 2000);
hillValues =a2f6(hillSpace);
hill=plot(hillSpace,hillValues);
shell=plot(x(t),y(t));

func=@(t) y(t)-a2f6(x(t));
% Run bisection 100000 times to get a approximation as good as possible
root=bisection(func,t(round(numel(t)/2)),t(end),func(t(2)),func(t(end)),0,0,100000,0);

% Plot impact point.
impact=plot(x(root), y(root), 'k*');

%Legends for lines
legend([shell,hill,impact],'Shell','Hill','Impact');
ylabel('height (meters)'); xlabel('distance (meters)');
% Turn of the major grid lines and set the axis
grid ON; axis([0 16000 -2000 10500]); grid MINOR;

