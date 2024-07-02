% divisible (N,P) - checks if P is smaller then N and if N modoulus P
% recive the rest 0 it's divisible with any other number then 1 and N.
% Because N cant be 1 due to we initialize it with 2. and P is always
% smaller then N, wich means they can never be the same number.
% This means divisible(N,P) returns true if its divisible with rest 0
% other wise false.
divisible(N,P) :-
    P < N,
    N mod P =:= 0.

% if N mod p is not equal to 0. add 1 to the P to itirate aslong as P<N.
% And call for the predicate recursive.
divisible(N,P) :-
    P<N,
    Np is P + 1,
    divisible(N,Np).




% prime(x) check if x is a prime number.
% number 1 is not a prime number in my opinion, thats why X should be
% larger then 1. X is only divisible with 1 and X. X need to be and
% integer aswell. X must be an odd number. Number 2 is an even number
% and is always true when entered as input.
% A prime number cant be divisible with any other number then itself and
% 1, thats why we use negation on divisible to make sure that X is not
% divisible with any other number then 1 and X.

prime(2) :- !.
prime(X) :-integer(X),  X > 1 , X mod 2 =\= 0, \+divisible(X,2).










