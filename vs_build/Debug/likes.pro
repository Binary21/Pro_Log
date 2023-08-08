hangout(X,Y) :- friends(X,Y),intown(X),intown(Y).
friends(X,Y) :- likes(X,Z),likes(Y,Z).
likes(bill,movies).
likes(sally,movies).
likes(bob,pizza).
likes(joe,movies).
intown(bill).
intown(sally).
intown(bob).
