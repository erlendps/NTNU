% Task 1
payment(0, []). % base case for recursion
payment(Amount, [Head|Tail]) :-
    % extract info from head
    Head = coin(AmountNeeded, ValOfCoin, AmountAvail),
    % constrain AmountNeeded (for search)
    AmountNeeded in 0..AmountAvail,
    % find the next amount to find by subtracting from
    % the current amount
    NextAmount #= Amount - AmountNeeded * ValOfCoin,
    % recursive call
    payment(NextAmount, Tail).


% TASK 2
:- use_module(library(clpfd)).
distance(c1, c2, 10, 1). distance(c1, c3, 0, 0). distance(c1, c4, 7, 1).
distance(c1, c5, 5, 1). distance(c2, c3, 4, 1). distance(c2, c4, 12, 1).
distance(c2, c5, 20, 1). distance(c3, c4, 0, 0). distance(c3, c5, 0, 0).
distance(c4, c5, 0, 0). distance(c2, c1, 10, 1). distance(c3, c1, 0, 0).
distance(c4, c1, 7, 1). distance(c5, c1, 5, 1). distance(c3, c2, 4, 1).
distance(c4, c2, 12, 1). distance(c5, c2, 20, 1). distance(c4, c3, 0, 0).
distance(c5, c3, 0, 0). distance(c5, c4, 0, 0).

% base case for recursion
path(Cabin1, Cabin2, Path, TotalDistance, _) :-
    distance(Cabin1, Cabin2, TotalDistance, 1),
    Path = [Cabin1, Cabin2].

path(Cabin1, Cabin2, Path, TotalDistance, Seen) :-
    % check if we have reached the end
    not(Cabin1 = Cabin2),
    % find a path between two cabins
    distance(Cabin1, X, Distance, 1),
    % check if we already have visted this cabin
    not(member(X, Seen)),
    % append X to seen list
    append(Seen, [X], NewSeen),
    % append Cabin1 to the SubPath
    append([Cabin1], SubPath, Path),
    % update totaldistance
    TotalDistance #= Distance + SubDistance,
    % recursive call
    path(X, Cabin2, SubPath, SubDistance, NewSeen).

% main predicate
plan(Cabin1, Cabin2, Path, TotalDistance) :-
    % call to recursive function
    path(Cabin1, Cabin2, Path, TotalDistance, [Cabin1]).

% task 2.1
findShortestPath([], _, _).
findShortestPath([PathsHead|PathsTail], ShortestPath, ShortestDistance) :-
    PathsHead = [Path, TotalDistance],
    (   TotalDistance < ShortestDistance ->
    	ShortestPath = Path,
        ShortestDistance = TotalDistance
    ;   true
    ),
    findShortestPath(PathsTail, ShortestPath, ShortestDistance).

shortestPath([PathsHead|PathsTail], ShortestPath, ShortestDistance) :-
    PathsHead = [Path, TotalDistance],
    ShortestPath = Path,
    ShortestDistance = TotalDistance,
    findShortestPath(PathsTail, ShortestPath, ShortestDistance).
                  
bestplan(Cabin1, Cabin2, Path, Distance) :-
    bagof([Path, TotalDistance],
          plan(Cabin1, Cabin2, _, TotalDistance),
          Paths
    ),
    shortestPath(Paths, Path, Distance).

