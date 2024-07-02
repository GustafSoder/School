% Zebra puzzle


% Create a list with 5 elements. if the input is for example list(X).
% You will recive a list with 5 random elements that prolog creates for
% you. You can also check if a list contains 5 elements. If the list
% contains 5 elements you recive True otherwise false.

list5(L):- length(L,5).

% Check if L1 is the same as E1 and L2 is the same as E2. If not remove
% the Head. and call the function again. Repeat until true.  This only
% works if the lists are of equal size. You recive true if the elements
% got the same position otherwise false.

eq_pos([L1|_],[L2|_],E1,E2) :- L1 = E1 , L2 = E2.
eq_pos([_|L1],[_|L2],E1,E2) :- eq_pos(L1,L2,E1,E2).

% Check if E1 and E2 has the neighbour position in two seperate lists,
% L1 and L2. Use the tail for one list and the whole list for the other.
% Call for the eq_pos rule to iterate through the lists until you find
% the element you search, once the elements are found, you recive true
% otherwise false.

eq_neigh_pos([_|L1],L2,E1,E2) :- eq_pos(L1,L2,E1,E2).
eq_neigh_pos(L1,[_|L2],E1,E2) :- eq_pos(L1,L2,E1,E2).

% Checks if E2 is on the right hand side of E1. Use eq_pos to itirate
% through the list. if E2 is on the right hand side of E1 it returns
% true, otherwise false.
eq_next_pos(L1,[_|L2],E1,E2) :- eq_pos(L1,L2,E1,E2).


% Zebra puzzle.
% Using the previously written predicates to write the rules 1 - 15 will
% help solving the puzzle.

zebra(Origin,Colour,Pet,Drink,Fruit) :-

% Rule 1, there are five houses in a row. using the list5 predicate will
% create a list of 5 elements. Every element is representing a house.
% Create a 5 element list for each category.

list5(Origin),
list5(Colour),
list5(Pet),
list5(Drink),
list5(Fruit),

% Rule9 they drink juice in the house in the middle. Is given already
% from the specification as a clue.
Drink = [_,_,juice,_,_],

% rule 10 The Swede lives in the house on the far left. Is given already
% from the specification as a clue same as rule 9.
Origin = [swede,_,_,_,_],

% rule 2 The Englishman eats peaches. Using the eq_pos due to the
% statements are at the same house.
eq_pos(Origin, Fruit, englishman, peach),

%rule 3 The Spaniard has a dog.
eq_pos(Origin,Pet, spaniard, dog),

%Rule 4 the German drinks Tea.
eq_pos(Origin, Drink, german, tea),

% Rule 5 The swede lives next to the green house. Here I'm using the
% eq_neigh_pos. Due to the swedes neighbour house is green.
eq_neigh_pos(Origin, Colour,swede, green),

% rule 6 The Blue house is next to the white house, on the right hand
% side. Here i,m making sure that I use the predicate, eq_next_pos to
% put in the white house first, and after I put in the blue. Then I'm
% making sure that the blue house is on the right.
eq_next_pos(Colour,Colour, white, blue),

%rule 7 The person who eats apples lives next to the one who drinks milk
eq_neigh_pos(Fruit,Drink, apple, milk),

%rule 8 The person who eats oranges has a horse.
eq_pos(Fruit, Pet, orange, horse),

%Rule 11 The person in the blue house has a hen.
eq_pos(Colour,Pet,blue,hen),


%rule 12 the person in the red house eats grapes.
eq_pos(Colour, Fruit, red, grape),

% Rule 13 The person who eats grapes lives next to the one who has a
% donkey.
eq_neigh_pos(Fruit, Pet, grape, donkey),

%rule 14 The american lives in a yellow house.
eq_pos(Origin, Colour, american, yellow),

%rule 15 The coffe drinker eats banana.
eq_pos(Drink, Fruit, coffe, banana).



