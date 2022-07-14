%bod(X,Y)
%usecka(bod(X,Y), bod(A,B)).

%svisla(usecka).
svisla(usecka(bod(X,_),bod(X,_))) :- A \= B. %pokud to je bod tak vrat false

%vodorovna
vodorovna(usecka(bod(_,X),bod(-,X))) :- A \= B.

%delka (+usecka, -Delka)
delka(usecka(bod(Ax, Ay), bod(Bx, By)), Delka) :- 
	XDiff is Ax-Bx,
	YDiff is Ay-By,
	Delka is sqrt((XDiff*XDiff) + (YDiff*YDiff)).

spol(X1,X2) :- usecka(_,X), usecka(X,_).

ctyruhelnik(U1,I2,U3,U4) :- spol(U1,U2), spol(U2,U3), spol(U3,U4), spol(U4,U1).

/*
ctyruhelnik(
	usecka(bod(0,0), bod(0,1)),
	usecka(bod(0,1), bod(1,1)),
	usecka(bod(1,1), bod(1,0)),
	usecka(bod(1,0), bod(0,0)))*/
	

%ctyruhelnik(usecka(bod(0,0), bod(0,1)),usecka(bod(0,1), bod(1,1)),usecka(bod(1,1), bod(1,0)),usecka(bod(1,0), bod(0,0))).
