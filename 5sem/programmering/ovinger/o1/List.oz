% declare all functions
declare Length Take Drop Append Member Position Push Peek Pop

% task 7a
fun {Length List}
  if List == nil then % base case, list is empty
    0
  elseif List.2 == nil then % base case, we reached the end of the list
    1
  else
    1 + {Length List.2} % recursion step, return 1 + the length of the remaining list
  end
end

% task 7b
fun {Take List Count}
  if Count >= {Length List} then % base case if Count is larger or equal to the length of the list
    List                         % we then just return the list
  else
    if Count == 0 then % recursion base case, terminate the list with nil
      nil
    else
      List.1|{Take List.2 Count-1} % recursively create a list
    end
  end
end

% task 7c
fun {Drop List Count}
  if Count >= {Length List} then nil % base case, if Count is larger or equal to length of list return nil
  else
    if Count == 0 then % recursion base case, we reached the the index we want to split at
      List             % the remaing list is then the original list minus the first Count elements
    else
      {Drop List.2 Count-1} % recursion step
    end
  end
end

% task 7d
fun {Append List1 List2} 
  if List1.2 == nil then % recursion base case, we reached the end of List1, and we can return list head
    List1.1|List2        % concatenated with List2. We take advantage of how lists are represented in Oz 
  else
    List1.1|{Append List1.2 List2} % recursion step, have to iterate through entire list1
  end
end

% task 7e
fun {Member List Element}
  if List.1 == Element then true % recursion base case, element found
  elseif List.2 == nil then false % recursion base case, element not found
  else {Member List.2 Element} % recursion step, check if Element is in tail of List
  end
end

% task 7f I assume lists are 1-indexed
fun {Position List Element}
  if List.1 == Element then % recursion base case, assumed the element exists in list
    1
  else
    1 + {Position List.2 Element} % recursion step, this represenets number of iterations basically
  end
end

% TASK 8 %
% task 8a
fun {Push List Element}
  Element|List % take advantage of how lists are represented in Oz, concatenate first element with the rest of the list
end

% task 8b
fun {Peek List}
  if List == nil then % check if list is nil
    nil
  else
    List.1 % return the first element of the list
  end 
end

% task 8c
fun {Pop List}
  List.2 % this will be the list without the first element
end
