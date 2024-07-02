child(rhea,uranus).
child(rhea,gaia).
child(chronus,uranus).
child(chronus,gaia).
child(phoebe,uranus).
child(phoebe,gaia).
child(coeus,uranus).
child(coeus,gaia).
child(iapetus,uranus).
child(iapetus,gaia).
child(tethys,uranus).
child(tethys,gaia).
child(oceanus,uranus).
child(oceanus,gaia).
child(hera,chronus).
child(hera,rhea).
child(hades,chronus).
child(hades,rhea).
child(demeter,chronus).
child(demeter,rhea).
child(poseidon,chronus).
child(poseidon,rhea).
child(zeus,chronus).
child(zeus,rhea).
child(leto,phoebe).
child(leto,coeus).
child(epimetheus,iapetus).
child(atlas,iapetus).
child(pleione,tethys).
child(pleione,oceanus).
child(hephaestus,zeus).
child(hephaestus,hera).
child(ares,zeus).
child(ares,hera).
child(persephone,zeus).
child(persephone,demeter).
child(artemis,zeus).
child(artemis,leto).
child(apollo,zeus).
child(apollo,leto).
child(dione,epimetheus).
child(dryope,atlas).
child(dryope,pleione).
child(maia,atlas).
child(maia,pleione).
child(aphrodite,zeus).
child(aphrodite,dione).
child(hermes,zeus).
child(hermes,maia).
child(eros,ares).
child(eros,aphrodite).
child(hermaphroditus,aphrodite).
child(hermaphroditus,hermes).
child(athena,zeus).
child(pan, hermes).
child(pan, dryope).
female(gaia).
female(rhea).
female(phoebe).
female(tethys).
female(hera).
female(demeter).
female(leto).
female(pleione).
female(persephone).
female(aphrodite).
female(athena).
female(artemis).
female(dione).
female(dryope).
female(maia).

%Rules:

%C is a daughter of P if C is the child of P, and C is female.
daughter(C,P) :- child(C,P), female(C).

%Gp is the grandparent of C if (for any P) C is the child of P, and P is the child of Gp
grandchild(C,Gp) :- child(C,P), child(P,Gp).

%C is a son of P if C is the child of P, and C is not female.
son(C,P) :- child(C,P), \+ female(C).
% \+ is the "not" operator.

%Note: This does not work!
son2(C,P) :- \+ female(C), child(C,P).
%You should never start with \+.
%It would be impossible to start by finding all things that are not female.

%X and Y are in a family if X is the child of Y, or Y is the child of X.
% The two last Family rules are the extended version that checks
% siblings and partners aswell.
family(X,Y) :- child(X,Y).
family(X,Y) :- child(Y,X).

family(X,Y) :- sibling(X,Y).
family(X,Y) :- partner(X,Y).
%It is also possible to use ; for "or", but it is often better to just make multiple rules.

% X is a titan if X is the child of Y, and Y is Uranus.
% The extended version of this checks the childs of uranus directly
% instead of making "y is uranus etc".
titan(X) :- child(X,uranus).
% == is the "equals" operator. \= is "not equals".


% M is the mother of C if M is the parent of C and M is female.
mother(M, C) :- child(C,M) , female(M).

%true if A and B have the same child X.
partner(A,B) :- child(X,A), child(X,B), A\=B.

%X must be a man / son  with no children.

bachelor(X) :- child(X,_),\+ female(X), \+ child(_,X).

% A and B are siblings if two children have the same parents X and Y, X
% is not equal to Y and A is not equal to B.

sibling(A,B) :- child(A,X), child(B,X), child(A,Y), child(B,Y), X\=Y, A\=B.



% Printouts from prompt:

%Is Atlas female?
%   ?- female(atlas).
%   false.

%Who is the parent of both Apollo and Ares?
%   ?- child(apollo,P), child(ares,P).
%   P = zeus ;
%   false.

% Pan is a child of Hermes and Dryope.  Add these two facts to the file.

% Write a query that answers the questionis Tethys a female?
%   ?- female(tethys).
%   true.

%Write a query that answers the questionwhich gods are children of
%Rhea and Chronus?This shouldbe written on one line, and you should
%get five answers.
%    ?- child(X, rhea), child(X,chronus).
%    X = hera ;
%    X = hades ;
%    X = demeter ;
%    X = poseidon ;
%    X = zeus ;
%    false.

% Write a query that answers the question who is the mother of Phoebe?
%   ?- mother(M, phoebe).
%   M = gaia.


% Write a query that answers the question which gods are partners of
% Hermes?You should receive two answers.
%   ?- partner(hermes,B).
%   B = aphrodite ;
%   B = dryope.

% Write a query that answers the question who are bachelors? You Should
% receive seven unique answers.

%   ?- bachelor(X).
%   X = hades ;
%   X = hades ;
%   X = poseidon ;
%   X = poseidon ;
%   X = hephaestus ;
%   X = hephaestus ;
%   X = apollo ;
%   X = apollo ;
%   X = eros ;
%   X = eros ;
%   X = hermaphroditus ;
%   X = hermaphroditus ;
%   X = pan ;
%   X = pan.

% Write a query that answers the questionwho is a sibling of Apollo?You should receive one
% unique answer, but it can be repeated.
%   ?- sibling(apollo,B).
%   B = artemis ;
%   B = artemis ;
%   false.


% Extend the definition of family(X, Y)so that it is also true
% if X and Y are partners or siblings.Use the rules you have written.
% Write a query that answers the question who is a part of Ares family?
% You should receive five unique answers.
%
%   ?- family(ares,Y).
%   Y = zeus ;
%   Y = hera ;
%   Y = eros ;
%   Y = hephaestus ;
%   Y = hephaestus ;
%   Y = aphrodite.

% Write a query, without creating a new rule, that answers the
% question which godesses have children with more than one partner?
% There can be duplicates among the answers.
%
%   ?- partner(Man, Kvinna), female(Kvinna),child(Kid,Kvinna),\+family(Kid,Man).
%   Man = ares,
%   Kvinna = aphrodite,
%   Kid = hermaphroditus ;
%   Man = hermes,
%   Kvinna = aphrodite,
%   Kid = eros ;
%   false.


