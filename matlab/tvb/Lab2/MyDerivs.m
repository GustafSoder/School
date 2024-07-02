function fp=MyDerivs(y,h)

% MyDerivs  Computes approximations of derivatives
%
% CALL SEQUENCE: fp=a2f2(y)
%
% INPUT:
%   y     a one dimensional array of function values, y = f(x)
%   h     the spacing between the sample points x
%
% OUTPUT
%   fp    a one dimension array such that fp(i) approximates f'(x(i))
%
% ALGORITHM: Space central and asymmetric finite difference as needed
%
% MINIMAL WORKING EXAMPLE: MyDerivsMWE

% PROGRAMMING by Carl Christian Kjelgaard Mikkelsen (spock@cs.umu.se)
%   2018-11-26 Extracted from a working code

% Extract the number of points
m=numel(y);

% The exercise is pointless unless there are at least 3 points
if m<3
    return;
end

% Allocate space for derivatives
fp=zeros(size(y));

% Do asymmetric approximation of the derivative at the left endpoint
fp(1)=(-3*(y(1))+4*(y(2))-y(3))/(2*h);
% Do space central approximation of all derivatives at the internal points
% Do a for-loop *before* you attempt to do this as an array operation
for i=2:m-1
    fp(i)=(y(i+1)-y(i-1))/(2*h);
end

% Do asymmetric approximation of the derivatives at the right endpoint
fp(m)=(y(m-2)-4*y(m-1)+3*y(m))/(2*h);