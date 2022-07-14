r() :- reconsult('11cviko.pl').
/*---------------------------------*/
%mergesort
mylength([_|T], Res) :-
	mylength(T, Subresult),
	Res is 1 + Subresult.

%rozpuli seznam
halve(Lst, L1, L2) :-
	mylength(Lst, Len),
	Len2 is Len//2,
	halve(Lst, Len2, -L1, -L2).

halve(Lst, 0, [], Lst).
%celociselne deleni jsou 2 lomitka
halve([H|T], N, , [H|L1], L2) :-
	N1 is N - 1,
	halve(T, N1, L1, L2).

%merge(L1, L2, -Res).
merge([H1|T1], [H2|T2], [H1|Res]) :-
	H1 =< H2,
	merge(T1, [H2|T2], Res).

merge([H1|T1], [H2|T2], [H2|Res]) :-
	H1 > H2,
	merge([H1|T1, T2, Res).
merge([], L2, L2).
merge(L1, [], L1).

%merge_sort
merge_sort([], []).
merge_sort([H], [H]) :- !.
merge_sort([H1,H2|T], Sorted) :-
	halve([H1,H2|T], L1, L2),
	merge_sort(L1, Sorted1),
	merge_sort(L2, Sorted2),
	merge(Sorted1, Sorted2, Sorted).

/*-------------------------------------------------------------*/
%bst
%val, bstL, bstR
bstfind([H, _, _], H).
bstfind([H ,L, _], E) :-
	H > E,
	bstfind(L, E).
bstfind([H ,_, R], E) :-
	H < E,
	bstfind(R, E).

bstinorder([], []).
bstinorder([H,L,R], Res) :-
	bstinorder(L, InL),
	bstinorder(R, InR),
	myappendlist(InL, [H|InR], Res).

bstinsert([], E, [E, [], []]).
bstinsert([H, L, R], H, [H, L, R]).
bstinsert([H, L, R], E, [H, NewL, R]) :-
	E < H,
	bstinsert(L, E, NewL).
bstinsert([H, L, R], E, [H, L, NewR]) :-
	E > H,
	bstinsert(R, E, NewR).

/*--------------------------*/
mynth([H|_], 1, H).
mynth([_|T, N, Res)  :-
	N1 is N-1,
	mynth(T, N1, Res).
/*----------------------------*/
%pak se delali matice (nty sloupec atd)

%graph
%todo

