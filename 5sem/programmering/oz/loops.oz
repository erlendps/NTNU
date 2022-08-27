declare PrintHello J

proc {PrintHello N}
  if N > 0 then
    {System.show 'Hello'}
    {PrintHello N-1}
  end
end
