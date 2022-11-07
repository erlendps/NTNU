% function, procedure declarations
declare ShowStream Enumerate GenerateOdd ListDivisorsOf ListPrimesUntil LazyEnumerate Primes in

% helper function for showing stream
proc {ShowStream List}
  case List of _|Tail then
    {Browse List.1}
    thread {ShowStream Tail} end
  else
    skip
  end
end

%%%%%%%%
% Task 2
%%%%%%%%

fun {Enumerate Start End}
  % helper function that enumerates from start to end, which we can later wrap
  % in a thread call.
  fun {Enum Start End}
    % check if we reached the end
    if (Start > End) then
      nil
    else
      % extend list
      Start|{Enum Start+1 End}
    end
  end in
  % wrap in thread for async
  thread {Enum Start End} end
end

%{ShowStream {Enumerate 1 10}}

fun {GenerateOdd Start End}
  % helper function for consuming a list and filtering out even numbers.
  fun {Filter List}
    case List of Head|Tail then
      % filter out even numbers
      if ({Int.isOdd Head}) then
        Head|{Filter Tail}
      else
        % else just call the function again
        {Filter Tail}
      end
    else
      nil
    end
  end in
  % wrap helper functino in thread for async execution
  thread {Filter {Enumerate Start End}} end
end

%{ShowStream {GenerateOdd 1 5}}

%%%%%%%%
% Task 3
%%%%%%%%

fun {ListDivisorsOf Number}
  % helper function that filters a list, creating a new list
  fun {Filter List}
    case List of Head|Tail then
      % check if its divisible by checking mod
      if ((Number mod Head) == 0) then
        Head|{Filter Tail}
      else
        % else recursively call on tail
        {Filter Tail}
      end
    else
      nil
    end
  end in
  % consumes Enumerate and produces a new list
  thread {Filter {Enumerate 1 Number}} end
end

%{ShowStream {ListDivisorsOf 20}}

fun {ListPrimesUntil N}
  % helper functions that checks if a number is a prime
  fun {IsPrime Num} L in
    % get list of divisors, if the first element is 1 and the last
    % is Num, then it's prime, else it's not prime.
    L = {ListDivisorsOf Num}
    case L of Head|Tail then
      if (Tail.1 == Num) then
        if (Head == 1) then
          true
        else
          false
        end
      else
        false
      end
    else
      false
    end
  end
  % helper function that consumes a stream, and collects the primes of that stream.
  fun {Filter List}
    case List of Head|Tail then
      % check if Head is a prime
      if ({IsPrime Head}) then
        % we then add it to the stream
        Head|{Filter Tail}
      else
        % just a recursive call
        {Filter Tail}
      end
    else
      nil
    end
  end in
  % base case, N < 2, 1 is not prime
  if (N < 2) then
    nil
  % base case N == 2, return a list only containing 2.
  elseif (N == 2) then
    2|nil
  % else start a thread and filter
  else
    thread {Filter {Enumerate 2 N}} end
  end
end

%{ShowStream {ListPrimesUntil 50}}

%%%%%%%%
% Task 4
%%%%%%%%

fun lazy {LazyEnumerate}
  % helper function, generate infinite list
  fun {Generate Next}
    Next|{Generate Next+1}
  end in
  % when called, a new thread will create the stream, starting from 1
  thread {Generate 1} end
end

%{Browse {List.take {LazyEnumerate} 10}}

fun lazy {Primes}
  fun {IsPrime Num} L in
    % get list of divisors, if the first element is 1 and the last
    % is Num, then it's prime, else it's not prime.
    L = {ListDivisorsOf Num}
    case L of Head|Tail then
      if (Head == Num) then
        true
      elseif ({And Tail.1 == Num Head == 1}) then
        true
      else
        false
      end
    else
      false
    end
  end
  % helper function that consumes a stream, and collects the primes of that stream.
  fun {Filter List}
    case List of Head|Tail then
      % check if Head is a prime
      if ({IsPrime Head}) then
        % we then add it to the stream
        Head|thread {Filter Tail} end
      else
        % just a recursive call
        thread {Filter Tail} end
      end
    else
      nil
    end
  end in
  
  % generate an infinite amount of primes
  thread {Filter {LazyEnumerate}} end
end

%{Browse {List.take {Primes} 10}}
