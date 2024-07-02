clc;clear;
% A1F2 Minimal working example for a1f1

% PROGRAMMING by Carl Christian Kjelgaard Mikkelsen
%                Mathias Hallberg (c19mhg@cs.umu.se)
%                Gustaf Söderlund (et14gsd@cs.umu.se)
%   2018-11-14 Skeleton extracted from working code
%   2021-11-28 Added working plot

% Set number of polynomials
n=11;

% Set number of sample points
x=linspace(-1,1,1000);

% Generate function values
y=MyChebyshev(n, x)

% Plot all graphs with one command
plot(x, y(:,end));
grid on;

% Adjust axis to make room for legend
axis([-1 1 -1.5 2.5]);

% Construct and display legend
str=[];
for i=0:n-1
    str=[str strcat("n=",string(i))];
end
legend(str);
