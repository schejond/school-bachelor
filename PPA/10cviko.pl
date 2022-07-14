factorial(0, 1).
factorial(N, Res) :- 
	N1 is N-1,
	factorial(N1, R1),
	Res is N * R1.

fibonacci(1, 1).
fibonacci(2, 1).
fibonacci(N, R) :-
	% fibonacci (N-1) + fibonacci(N-2),
	N1 is N-1,
	N2 is N-2,
	fibonacci(N1, R1),
	fibonacci(N2, R2),
	R is R1 + R2.

%-----------------------------------------------------
%contains(Lst, Elem).
contrains([], Elem) :- fail.
contains([Elem|_], Elem).
contains([H|T], Elem) :-
	contains(T, Elem).

%not contains
not_contains([], Elem) :- true.
not_contains([Elem|_], Elem) :- fail.
not_contains([H|T], Elem) :-
	H \= Elem,
	not_contains(T, Elem).

%length
myLength([], 0).
myLength([_|T], Res) :-
	myLength(T, R),
	Res is R + 1.

myLengthS([],0).
myLengthS(S, 1) :- number(S) ; atom(S).
myLength([H|T], Res) :-
	myLengthS(H, R1),
	myLengthS(T, R2),
	Res is R1 + R2.
myLengthS(_,1).	
%myLengthS([bod(1,1), [2,3],4,[5,6,[7]]], R).
% ! zakazuje back tracking

%append
myAppend([], Elem, [Elem]).
myAppend([H|T], Elem, [H|R]) :-
	myAppend(T, Elem, R).

myLast(Lst, X) :- myAppend(_,X,Lst).

%myAppendLst
%myAppendLst([], Lst, Lst).
myAppendLst(Lst, [], Lst).
myAppendLst([H|T], L2, [H|R]) :-
	myAppendLst(T, L2, R).

myDelete([], _, []).
myDelete)[E|T], E, R) :- mydelete(T,E,R) !.%chci to projit cele, co kdyby jich tam bylo vic
myDelete([H|T],E,[H|R]) :- myDelete(T, E, R).

myReverse([], []).
myReverse([H|T], Res) :-
	myReverse(T, R),
	myAppend(R, H, Res).

myReverseAcc(Lst, Res) :- myreverseAux(Lst, [], Res).

myReverseAux([], Acc, Acc).
myReverseAux([H|T], Acc, Res) :-
	myReverseAux(T, [H|Acc], Res).
appendToAll([], _, []).
appendToAll([H|T], E, [R1|R2]) :- 
	myAppend(H, E, R1),
	appendToAll(T, E,R2).
