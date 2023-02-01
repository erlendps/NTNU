# Exam 2020 midterm

## Problem 1

can only be achieved in a declarative programming language. Not Correct, in oz...

## Problem 2

Is required in all programming language translation/interpretation. Will produce tree-structured, think they meant "parser".

## Problem 3

local A in local B in A = B B = A if A then 3 else 0. Correct

## Problem 4

the grammas ir **not ambiguous**. Correct.

## Problem 5

The next state is identical. Correct.

## Problem 6

a#\_#\_. Correct.

## Problem 7

static scoping. Correct.

## Problem 8

may delay unification. Correct.

## Problem 9

1 and 3. Correct

## Problem 10

The thrid one. Correct.

## Problem 11

First we notice that Reverse2 is tail recursive, meaning the stack will have a constsant computational and space size. The runtime of the function is O(n), so it runs linearly with the size of the list to be reversed. Correct

## Problem 12

{FoldR [1 2 3] G1 nil}. Correct.

## Problem 13

[f o o | \_]#\_. Correct

## Problem 14

```oz
Ys = {FoldL Xs fun {$ S X} {G S {F X}} end S}
```

## Problem 15

Generecity: the ability to pass procedure values as arguments to a procedure call.
Instantiation: the ability to return procedure values as results from a procedure call.
Procedural abstraction: the ability to convert any staement into a procedure value
Embedding: The ability to put procedure values in data structures.

Correct.

## Problem 16

It will show something like '\_#(\_#(...)). Correct.

## Problem 17

```oz
fun {AppendD DL1 DL2}
  case DL1 of nil then DL2
  [] X|Xr then
    X|{AppendD Xr DL2}
  end
end
```
