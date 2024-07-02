clc;clear;
% Define a nice function
f=@(x)exp(x).*sin(x);

% Define the derivative fp (fprime) of f
fp=@(x)exp(x).*(sin(x)+cos(x));

% Interval
a=0; b=pi;

% Number of subintervals
n=100;

% Sample points for plotting
s=linspace(a,b,n+1);

% Plot the graph
h=figure; plot(s,f(s)); 

% Hold the graph
hold on;
% Turn on grid
grid on;

% Axis tight
axis tight

% /////////////////////////////////////////////////////////////////////////
%  Illustration of Runge's theorem
% /////////////////////////////////////////////////////////////////////////

% Initial search bracket
x0=a;
x1=b;

% The function values corresponding to the initial search bracket
fp0=fp(a);
fp1=fp(b);

% Tolerances and maxit for bisection.
eps=10^-10; delta=10^-10; maxit=101;
% Run the bisection algorithm to find the zero c of fp
c=bisection(fp, x0, x1, fp0, fp1, delta, eps, maxit, false);

% Define the tangent at this point; this a constant function.
w=@(x)ones(size(x))*f(c);

% Plot the tangent
plot(s, w(s));
    
% /////////////////////////////////////////////////////////////////////////
%  Illustration of the mean value theorem
% /////////////////////////////////////////////////////////////////////////

% Define points for corde
x0=2; x1=pi;

% Compute corresponding function values
f0=f(x0);
f1=f(x1);

% Define the linear function which connects (x0,f0) with (x1,f1)
h=f(x0);
b=-((f1-f0)/(x1-x0)*2)+h;
p=@(s) ((f1-f0)/(x1-x0)).*s+b;


% Plot the straight line between (x0,f0) with (x1,f1)
plot(s,p(s))

% Compute the slope of the corde
yp=(f1-f0)/(x1-x0);

% Define an auxiliary function which is zero when fp equals yp
g=@(s) yp - fp(s);

% Run the bisection algorithm to find a zero c of g
c=bisection(g, x0, x1, g(x0), g(x1), delta, eps, maxit, false);

% Define the line which is tangent to the graph of f at the point (c,f(c))
h=f(c)
b=-(yp*c)+h;
q=@(s)yp.*s+b;

% Plot the tangent line
plot(s,q(s));

% Labels
xlabel('x'); ylabel('y');

% Print the figure to a file
print('MyZeroTheorems','-depsc2');