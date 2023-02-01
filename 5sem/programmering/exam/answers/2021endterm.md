# Exam 2021

## Problem 1

It could look like this:

```oz
declare proc {Insert Key Value TreeIn ?TreeOut}
  case TreeIn of nil then TreeOut = tree(Key Value nil nil)
  [] tree(K1 V1 T1 T2) then
    if Key == K1 then TreeOut = tree(Key Value T1 T2)
    elseif Key < K1 then T in
      TreeOut = tree(K1 V1 T T2)
      {Insert Key Value T1 T}
    else T in
      TreeOut = tree(K1 V1 T1 T)
      {Insert Key Value T2 T}
    end
  else skip end
end
```

## Problem 2

```oz
% Insert definition

local T1 T2 T3 in
T1 = {Insert 2 two nil}
T2 = {Insert 1 one T1}
T3 = {Insert 3 three T2}
{Browse T3} % would browse tree(2 two tree(1 one nil nil) tree(3 three nil nil))
end
```

## Problem 3

Produce produces an infinite stream, while consume consumes a stream.
In snippet 1, Consume will never be called since the main thread is busy producing
In snippet 2, the delay function is not in a thread, meaning that the displaying of numbers are deterministic.
In snippet 3, it has the same problem as in snippet 2.
In snippet 4, it has the same problem as snippet 1.
Snippet 5 will yield a non-deterministic sequence.
Snippet 5 is correct.

## Problem 4

```oz
fun {Length Xs}
  fun {IterLength Xs L}
    case Xs of nil then L
    [] _|Xr then
      {IterLength Xr L+1}
    end
  end in
  {IterLength Xs 0}
end
```

## Problem 5

Procedurs uses out variables for "returning"

```oz
proc {Length Xs ?I}
  case Xs of nil then I = 0
  [] _|Xr then PartialI in
    {Length Xr PartialI}
    I = 1 + PartialI
  end
end
```

## Problem 6

The first occuring output does not change because it is not needed.

'x_is#5' and 'y_is#10' is printed eventually.

## Problem 7

|Statement|line|
|---|---|
|This clause represents a statement that always reduces the size of the semantic stack|1|
|This clause makes the grammar ambiguious|2|
|This suspendable statement may directly change the environment of the next semantic stack content|4|
|This clause allows statements to be interpreted as expressions|6|
|This clause represents a non-suspendable statement|3|
|This clause represents a statement that only affects the SSA|5|

## Problem 8

The state of the abstract machine is:

semantic stack: $[(\{Q X\}, \{X \rightarrow v_1, Q \rightarrow v_3\})]$
SSA: $[v_1 = 1, v_2 = (proc \{\$\} \ \{Browse X\} \ end, \{X \rightarrow v_1\}), v_3 = (proc \{\$\} local X in X = 2 \{P\} \ end \ end, \{P \rightarrow v_2\})]$

## Problem 9

The log will always show "one" before "four".

## Problem 10

The situation that the effect, or output, of concurrent computation dpends on execution scheduling.

## Problem 11

Has a default global execution context.

## Problem 12

static scoping

## Problem 13

```prolog
mult(0, _, 0).
mult(succ(A), B, R) :- mult(A, B, Pr), add(Pr, A, R).
```

## Problem 14

In order:

1. false
2. R = succ(0)
3. R = 0
4. R = succ(succ(0))
5. X = succ(R)

## Problem 15

|S|Line|
|---|---|
|A directive to the interpreter|1|
|X in the answer is contrained to an interval|7|
|The head of a defined clause|2|
|A query to the interpreter|6|
|Two variables in the answer are constrained by a relationship|8|
|The variable must be constrained to bounded interval|3|
