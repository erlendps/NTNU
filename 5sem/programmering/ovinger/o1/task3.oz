% task 3a
local X Y Z in  % declate X Y and Z in the local scope
  Y = 300   % point Y to 300
  Z = 30    % point Z to 30
  X = Y * Z % point X to Y * Z = 300 * 30

  {System.show X}
end


% task 3b
local X Y in
  X = "This is a string"
  thread {System.showInfo Y} end
  Y = X
end

% This works because of how threads works in Oz. From the Oz documentation it says that
% a thread will suspend its execution until all operands needed have a well-defined value.
% In this case, the thread suspends until Y is assigned X. This is useful since we then can start
% many threads in parallell. If for instance you start a thread with many operations, but depends on
% a value from the main thread in its last step, the thread can continue execution up until this point
% and then wait for the operand, meaning less idle time.
%
% Y = X makes Y point at X in essence. Since X points at a string, Y will also point to this string.
% It also means that X points at Y, and the the string points at X and therefor also Y.
% This works because Y has not been assigned a value yet. If for instance the line before was
% Y = 3, then the assignment Y = X would have thrown an error, since we're trying to say that
% 3 is Y and Y is "This is a string", but 3 is in fact not "This is a string".