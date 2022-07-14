% ======= Databaze faktu ========
man(bart).
man(homer).
man(abraham).
man(clancy).
woman(mona).
woman(marge).
woman(liza).
woman(meggie).
woman(selma).
woman(patty).
woman(jacqueline).
parent(homer,bart).
parent(homer,liza).
parent(homer,meggie).
parent(abraham,homer).
parent(marge,bart).
parent(marge,liza).
parent(marge,meggie).
parent(mona,homer).
parent(jacqueline,marge).
parent(jacqueline,patty).
parent(jacqueline,selma).
married(homer,marge).
married(abraham,mona).
married(clancy,jacqueline).

father(X, Y) :- man(X), parent(X, Y).
mother(X, Y) :- woman(X), parent(X, Y).
son(X, Y) :- man(X), parent(Y, X).

% z je predkem y
ancestor(X, Y) :- parent(X, Y).
ancestor(X, Y) :- parent(X, Z), parent(Z, Y).


%sourozenci
%treba se naucit cut v dalsi hodine
sibling(X, Y) :- parent(Z, X), parent(Z, Y), X\=Y.
%sibling(X, Y) :- father(Z, X), father(Z, Y), X\=Y.


sister(X, Y) :- sibling(X, Y), woman(X).

aunt(Aunt, Child) :- parent(Z, Child),
			sister(Aunt, Z).

%x je synovec Y
nephew(X, Y) :- sibling(Y, Z), son(X, Z).  
