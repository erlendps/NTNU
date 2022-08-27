\insert '/home/erlendps/skole/NTNU/5sem/programmering/oz/loops.oz'

declare J X Jj L

{PrintHello 10}

X = 2.0

J = trondheim

Jj = 'trondheim, Norway'

{Show J}

local N in
  N = who(first: 'Erlend' last: 'Skaaden')
  L = person(name: N age: 21)

  {Show L}
end

local MyList MyOtherList CreateList in

  fun {CreateList N}
    if (N > 0) then
      N|{CreateList N-1}
    else
      nil
    end
  end

  % comment
  MyList = {CreateList 6}

  MyOtherList = 1|2|3|4|nil

  {Show MyList}
  {Show MyOtherList}

end