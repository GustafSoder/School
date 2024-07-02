function y=MyChebyshev(n,x)

% A1F1 Evaluates the first n Chebyshev polynomials
%
% CALL SEQUENCE: y=a1f1(n,x)
%
% INPUT:
%   n    the number of polynomials
%   x    a vector of length m containing the sample points
%
% OUTPUT:
%   y     a matrix of dimension m by n such that y(i,j) = T(j,x(i))
%
% MINIMAL WORKING EXAMPLE: myChebyshevMWE1

% PROGRAMMING by Carl Christian Kjelgaard Mikkelsen (spock@cs.ume.se)
%                Mathias Hallberg (c19mhg@cs.umu.se)
%                Gustaf Söderlund (et14gsd@cs.umu.se)
%  2018-11-14 Skeleton extracted from working function
%  2020-11-04 Minor polishing ...
%  2021-11-28 Added calculations

% Determine number of element in x
m=numel(x);

% Reshape x as a column vector 
x=reshape(x,m,1);

% Allocate space for output y
y=ones(m,n);

% Initialize the first two columns of y

y(:,1)=ones(m,1);
y(:,2)=y(:,2).*x;

% Calculate all remaining columns of y
for i=3:n
    y(:,i)=2.*x.*y(:,i-1)-y(:,i-2);
end
