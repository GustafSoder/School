:- consult(mondial).
:-debug.

%testing the built-in predicate write.
whatis(X):-
    write('X is '),
    write(X).
%--------------------------------------------
%edges
edge(a,b).
edge(a,c).
edge(b,c).
edge(c,d).
edge(c,e).
edge(d,e).
edge(f,g).
edge(g,h).

%Predicate allways finds all ways from node X to node Y in an acyclic graph. each found path is printed using Write. 
allways(X,Y) :-
    edge(X,Y),
    write(Y).

allways(X,Y) :-
    edge(Z,Y),
      allways(X,Z),
      write(Y).
%--------------------------------------------
% The next task is to write the predicate anyway(X, Y)that works for
% cyclic graphs as well. How ever, the path should not be printed this
% time, we only want to know if there is a path or not. 

edge2(d,a).
edge2(h,f).
edge2(X,Y) :- edge(X,Y).

% The predicate edge2 now describes a cyclic graph, which is the same
% graph as edge except it has two new edges.

anyway(X,Y) :- path(X,Y,[]).

path(X,Y,_) :- edge2(X,Y).

path(X,Y,L) :-
    edge(Z,Y),
    \+member(Y,L),
     path(X,Z,[Y|L]).


%--------------------------------------------
% This predicate can determine if the country is a citystate. Returns true if
% the population in a city, Pop, is atleast 75% of the country population,P. 
citystate(Country) :-
    country(Country,_,_,P),
    city(_,Country,_,Pop),
    integer(P),
    integer(Pop),
    Ppop is Pop/P,
    Ppop >=0.75.


%--------------------------------------------
% This is the solution to make the borders symmetrical. 
anycountry(X,Y,L):- borders(X,Y,L).
anycountry(X,Y,L):- borders(Y,X,L).

%This predicate is using setof, Wich makes a list and each element is the length of the border to each country. 
circumference(C,Country):- setof(L, Y^(anycountry(Country,Y,L)), Set), sumborder(Set,C).

%Calculate the sum of the borders of a country. 
sumborder([],0).
sumborder([H|T],N):- sumborder(T,X), N is X+H.


%%------------------------------------------
% This Predicate give us the countries that are not connected to the SEA and not divided in two or more continents. 
notAllowedCountries(Country,Continent) :-
    encompasses(Country,Continent,100),
    \+geo_sea(_,Country,_).

%Longest landlocked countries in the world. The predicate is true if the country has the longest border in its continent
%Not connected to sea and not a country divided in two or more continents. 
lll(Continent,Country):- 
    longestborder(Continent,Country).

%Calculate the longest border. Making a list of the length of the border in each country that are allowed. and use the 
%built-in predicate max_list wich will pick out the biggest value from the list representing the country with LLL. 
longestborder(Continent,Country):- 

    setof(C,listcontinents(Continent,C),Set),

    max_list(Set,MaxValue),

    circumference(MaxValue,Country).

% in this predicate we make sure we use the notAllowedCountries predicate
% and get the circumference of each country in the given continent. X, representing
% Length in this predicate. 
listcontinents(Continent,C):-
    notAllowedCountries(Country,Continent),
    setof(X,OtherCountry^(anycountry(Country,OtherCountry,X)),Set),
    sumborder(Set,C).

%------------------------------------------------------
%Print-outs

%?- allways(a,e).
%ce
%true ;
%bce
%true ;
%cde
%true ;
%bcde
%true ;
%false.

%?- allways(e,a).
%false.
    
%?- allways(e,a).
%false.

%?- anyway(b,a).
%false.

%?- citystate(Country).
%Country = ceuta ;
%Country = hong_kong ;
%Country = liechtenstein ;
%Country = melilla ;
%Country = saint_pierre_and_miquelon ;
%Country = singapore ;
%false.
    
%?- circumference(C,sweden).
%C = 2205.

%?- lll(Continent,Country).
%Continent = 'Africa',
%Country = mali ;
%Continent = 'America',
%Country = bolivia ;
%Continent = 'Asia',
%Country = mongolia ;
%Continent = 'Europe',
%Country = belarus ;
%false.
    
    





