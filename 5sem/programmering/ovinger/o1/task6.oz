% task 6
% recursive function that calculates the factorial of a given number N
declare Factorial

fun {Factorial N} % function declaration
  if {Or N == 0 N == 1} then % check N==0 or N==1
    1         % base case reached, return 1
  else
    N * {Factorial N-1} % else return N * the factorial of N-1
  end
end

{System.showInfo {Factorial 5}}