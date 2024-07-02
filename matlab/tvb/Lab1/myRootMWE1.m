% MyRoot Minimal working example for MyRoot

% PROGRAMMING by Gustaf Söderlund (et14gsd@cs.umu.se)
%                Mathias Hallberg (c19mhg@cs.umu.se)
%                
%   2021-11-28 completed the base for the minimal working example


clc;clear;
% T10 for computed values used by myRoot
p=[-1, 0, 50, 0, -400, 0, 1120, 0, -1280, 0, 512];
% Set brackets between -1 and 1 andd let max iterations be 101
a0=-1;b0=1;
maxit=101;
% Check size for array of polynomials
size = size(p);
% Set delta and eps.
delta=10^-10; eps=10^-10;
% Set brackets to check the roots
cosBrackets = linspace(0, pi, size(1,2));
brackets = -cos(cosBrackets);
% Initalize the output values to set size
x = zeros(1,size(1,2)-1); flag = zeros(1,size(1,2)-1);
it=zeros(1,size(1,2)-1); ab=zeros(1,size(1,2)-1);
bb=zeros(1,size(1,2)-1); yb=zeros(1,size(1,2)-1);
rebb=zeros(1,size(1,2)-1);
for i=1:size(1,2) - 1
    [x(i), flag(i), it(i), a, b, his, y, reb]=myRoot(p,brackets(i),brackets(i+1),delta,eps,maxit);
    ab(i)=a(it(i));
    bb(i)=b(it(i));
    yb(i)=y(it(i));
    rebb(i)=reb(it(i));
end

% Get the relative error using the algorithm
% (1/2)*abs(a-b)/min(abs(a),abs(b))
n=linspace(1,size(end) -1, size(end)-1);
relErr=zeros(1,size(end)-1);
for i=1:size(end)-1
    if sign(ab(1,i)) == sign(bb(1,i))

        relErr(1,i)=(0.5*abs(ab(1,i)-bb(1,i))/(min(abs(ab(1,i)), abs(bb(1,i)))));
    else
        relErr(1,i)=NaN;
    end 
end

% Calculate trust
trust=abs(yb(1,:))>abs(rebb(1,:));

% Select the data for printing
data=[n(1,:)' flag(1,:)' it(1,:)' ab(1,:)' bb(1,:)' x(1,:)' yb(1,:)' rebb(1,:)' relErr(1,:)' trust'];

% Define the column headingsh
colheadings = {'Idx','flag', 'iter','a','b','root','residual', 'REB', 'relative error', 'Trust'};

% Set the widths of the columns
wids=[6 6 6 12 12 12 12 12 16 8];
% Define the format specification
fms={'d','d','d','.4e','.4e', '.4e','.4e','.4e', '.4e', 'd'};

% Print the data nicely
displaytable(data,colheadings,wids,fms);
