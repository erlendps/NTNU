declare QuadraticEquation Sum Length RightFold Quadratic LazyNumberGenerator TailSum TailFoldSum in

%%% TASK 1 %%%
proc {QuadraticEquation A B C ?RealSol ?X1 ?X2}
  D = B * B - 4.0 * A * C in
  % two real solutions (1 if D == 0, but bind to both X1 and X2)
  if D >= 0.0 then
    RealSol = true
    X1 = (~B + {Sqrt D}) / (2.0 * A)
    X2 = (~B - {Sqrt D}) / (2.0 * A)
  % complex solutions, just set RealSol to false
  else
    RealSol = false
  end
end

local RealSol X1 X2 in
{QuadraticEquation 2.0 1.0 ~1.0 RealSol X1 X2}
{System.show X1#X2#RealSol}
end
local RealSol X1 X2 in
{QuadraticEquation 2.0 1.0 2.0 RealSol X1 X2}
{System.show X1#X2#RealSol}
end

%%% TASK 2 %%%
% implementation in assignment 1 (for comparison, I have since become to enjoy pattern matching)

%fun {Length List}
%  if List == nil then % base case, list is empty
%    0
%  elseif List.2 == nil then % base case, we reached the end of the list
%    1
%  else
%    1 + {Length List.2} % recursion step, return 1 + the length of the remaining list
%  end
%end

% sums the elements of a list
%fun {Sum List}
  %% we have iterated every element, return 0
  %case List of nil then
    %0
  %% new element, sum it with the sum of the remainging list
  %[] El|Rest then
    %El + {Sum Rest}
  %end
%end

%{System.show {Sum [1 4 6 2 1]}}

%%% TASK 3 %%%

fun {RightFold List Op U}
  case List of nil then
    U
  [] El|Rest then
    {Op El {RightFold Rest Op U}}
  end
end

% Length, we add 1 to the right-fold for each element. Neutral element is 0
fun {Length List}
  {RightFold List fun {$ X Y} 1 + Y end 0}
end

% Length, we add X (the value of the element) to the right-fold for each element. Neutral element is 0
fun {Sum List}
  {RightFold List fun {$ X Y} X + Y end 0}
end

{System.show {Length [1 2 3 4 5 6]}}
{System.show {Sum [1 2 3 4 5 6]}}


%%% TASK 4 %%%
fun {Quadratic A B C}
  % just declare an anonymous function that takes in X as parameter and return it
  % The anonymous function calculates the quadtric function, given X.
  fun {$ X}
    A * X * X + B * X + C
  end
end

{System.show {{Quadratic 3 2 1} 2}}


%%% TASK 5 %%%
fun {LazyNumberGenerator StartValue}
  StartValue|fun {$} {LazyNumberGenerator StartValue+1} end
end

{System.show {{{{{{LazyNumberGenerator 0}.2}.2}.2}.2}.2}.1}


%%% TASK 6 %%%
% explanation for why it is not tail recursive is in the pdf file.
% notice we send in SumSoFar as a parameter.
% in the recursion call, we do the summation and send that as a parameter down.
fun {TailSum List SumSoFar}
  case List of nil then
    SumSoFar
  [] El|Rest then
    {TailSum Rest El+SumSoFar} 
  end
end

{System.show {TailSum [1 2 3 4 5 6] 0}}
