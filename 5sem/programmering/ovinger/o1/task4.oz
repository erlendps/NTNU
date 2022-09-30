% task 4a
% functions that returns max of two numbers
declare Max % declate function name

fun {Max Number1 Number2} % function definiton
  if Number1 > Number2 then
    Number1
  else
    Number2
  end
end

% task 4b
% procedure that prints the max of two values

declare PrintGreater

proc {PrintGreater Number1 Number2}
  {System.show {Max Number1 Number2}} % use function defined above to get the max of 2 numbers
end

{PrintGreater 4 7}