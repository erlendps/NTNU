# Exam 2020 retake

## Problem 1

may be definitionally declarative when written in the declarative sequential kernel language even when extended with exceptions.

## Problem 2

both nr 1 and nr 4

## Problem 3

ambiguous

## Problem 4

static scoping
dynamic typing

## Problem 5

1) 2 *3* 4 = **24** is shown by Browse.
2) A takes in a list, a transform function and identity (base-case) element. The transform function has to take two parameters, where the first is the head of the list, while the second is a recursive call to A, with the tail as the new list. This recursive call will continue until the base case (list is nil) is reached, and the identity is returned.
3) An implementation:

```oz
fun {A L BO I}
  fun {RightFold List Transform Combine Null}
    case List of nil then Null
    [] Head|Tail then
      {Combine
        {Transform Head}
        {RightFold Tail Transform Combine Null}
      }
    end
  end in
  {RightFold L fun {$ Head} Head end BO I}
end
```

## Problem 6

```oz
1 )

fun {ReverseD DL}
  case DL of nil#nil then DL
  [] (Head1|Tail1)#(Head2|Tail2) then
    if Head1 == Head2 then
      DL
    else
      {ReverseD Tail1}|Head
    end
  end
end
```

## Problem 7

```oz
local RemoveNLast in
fun {RemoveNLast List N}
  fun {Length List}
    case List of nil then 0
    [] _|Tail then 1 + {Length Tail}
    else skip end
  end
  fun {TakeM List M}
    if M <= 0 then nil
    else
      List.1|{TakeM List.2 M-1}
    end
  end L in
  L = {Length List}
  if N >= L then nil
  else
    {TakeM List L-N}
  end
end
end
```

2 ) As one can see in the program, if N is greater than or equal to the length of the list, then it will just return nil, indicating that the whole list hast been removed.

3 )

```oz
% same code as above
{Browse {RemoveNLast [1 2 3 4 5 6] 3}}
% this will yield [1 2 3]
{Browse {RemoveNLast [1 2 3 4 5 6] 7}}
% this will yield nil
```

4 ) The computational complexity is O(n). Finding the length is a linear process, and also the removal of the last n elements are that as well. I believe this solution is quite efficient. I was thinking about first reversing the list, removing the first n elements and then reverse the resulting list back again, which would be way less efficient than my current solution.

## Problem 8

a )

The semantic stack is:
[({Q X}, {X -> v_1, Q -> v_3})]
The SSA is:
{v_1 = 1, v_2 = (proc {\$} {Browse X} end, {X -> v_1}), v_3 = (proc {\$ X} local X in X = 2 {P} end end, {P -> v_2})}

b )
The semantic stack is:
[(proc {\$ X} local X in X = 2 {P} end end, {P -> v_2})]
The SSA is:

1 is printed in the browser window.

## Problem 9

In P, the semantic of the if statement is that the statement B = 1 is expressed as the expression A.
For F, the if statement maps an expression to another expression. There also seems to be a bug here. A function has to return an expression, and in the function body, there is a missing else cause, meaning that if the expression A is false, it will throw an error because it has no expression it can return. This however is fine for the procedure, since it does not return a value.
