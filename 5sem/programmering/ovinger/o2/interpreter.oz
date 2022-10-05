\insert '/home/erlendps/skole/NTNU/5sem/programmering/ovinger/o2/List.oz'
declare Map Lex Tokenize Interpret ExpressionTree

% implementation of Map function
fun {Map List Function}
  case List of El|Rest then
    {Function El}|{Map Rest Function}
  else
    nil
  end
end

% task 1 a
fun {Lex Input}
  {String.tokens Input & }
end

%task 2 b
fun {Tokenize Lexemes}
  % map lexemes to their token, returns a list of tokens
  {Map Lexemes
    fun {$ Lex}
      if {String.isFloat Lex} then
        number({String.toFloat Lex})
      else
        case Lex of "+" then
          operator(type:plus)
        [] "-" then
          operator(type:minus)
        [] "*" then
          operator(type:multiply)
        [] "/" then
          operator(type:division)
        [] "p" then
          command(print)
        [] "d" then
          command(duplicate)
        [] "i" then
          command(inverse)
        [] "c" then
          command(clear)
        else
          {Exception.failure "Not a valid lexeme"}
        end
      end
    end
  }
end

fun {Interpret Tokens}

  fun {NextInStack Stack Tokens}
    % recursion base case, return the stack
    case Tokens of nil then Stack
    
    % case next element is a number, add to stack
    [] number(Float)|TokensRest then
      {NextInStack Float|Stack TokensRest} 
    
    % case next element is an operator. Do operator on the two top elements and add to stack
    % type = plus
    [] operator(type:plus)|TokensRest then
      Top|NextToTop|RestOfStack = Stack in
        {NextInStack (Top + NextToTop)|RestOfStack TokensRest}

    % type = minus
    [] operator(type:minus)|TokensRest then
      Top|NextToTop|RestOfStack = Stack in
        {NextInStack (Top - NextToTop)|RestOfStack TokensRest}

    % type = multiply
    [] operator(type:multiply)|TokensRest then
      Top|NextToTop|RestOfStack = Stack in
        {NextInStack (Top * NextToTop)|RestOfStack TokensRest}

    % type = division
    [] operator(type:division)|TokensRest then
      Top|NextToTop|RestOfStack = Stack in
        {NextInStack (Top / NextToTop)|RestOfStack TokensRest}
    
    % commands
    [] command(Command)|TokensRest then
      case Command of print then
        {Show Stack}
        {NextInStack Stack TokensRest}
      [] duplicate then
        Top|RestOfStack = Stack in
          {NextInStack Top|Top|RestOfStack TokensRest}
      [] inverse then
        Top|RestOfStack = Stack in
          {NextInStack (~Top)|RestOfStack TokensRest}
      [] clear then
        {NextInStack nil TokensRest}
      else
        {Exception.failure "Not a valid token"}
      end
    else
      {Exception.failure "Not a valid input"}
    end
  end in 

  try {NextInStack nil Tokens}
  catch _ then raise "Tokens empty" end
  end

end


fun {ExpressionTree Tokens}
  fun {ExpressionTreeInternal Tokens ExpressionStack}
    {Browse ExpressionStack}
    case Tokens of nil then {Peek ExpressionStack}
    [] number(Float)|TokensRest then
      {ExpressionTreeInternal TokensRest Float|ExpressionStack}
    
    [] operator(type:plus)|TokensRest then
      Top|NextToTop|RestOfStack = ExpressionStack in
        {ExpressionTreeInternal TokensRest plus(Top NextToTop)|RestOfStack}

    [] operator(type:minus)|TokensRest then
      Top|NextToTop|RestOfStack = ExpressionStack in
        {ExpressionTreeInternal TokensRest minus(Top NextToTop)|RestOfStack}

    [] operator(type:multiply)|TokensRest then
      Top|NextToTop|RestOfStack = ExpressionStack in
        {ExpressionTreeInternal TokensRest multiply(Top NextToTop)|RestOfStack}

    [] operator(type:division)|TokensRest then
      Top|NextToTop|RestOfStack = ExpressionStack in
        {ExpressionTreeInternal TokensRest division(Top NextToTop)|RestOfStack}

    else
      {Exception.failure "Not a valid token"}
    end
  end in
    try {ExpressionTreeInternal Tokens nil}
    catch _ then raise "Tokens are nil" end
    end
end

{Browse {Interpret {Tokenize {Lex "3 4 - 3 + 2 / 5 *"}}}}
{Browse {ExpressionTree {Tokenize {Lex "3 10 9 * - 7 +"}}}}