# Translation

```vsl
func main()
begin
    var iter, count
    iter:=2
    while iter < 40 do
    begin
        count:=collatz(iter)
        print iter, "converges in", count, "steps"
        iter:=iter+1
    end
    return 0
end

func collatz(n)
begin
    var steps
    steps := 0
    while n > 1 do
    begin
        var i
        i := n/2
        if n = i*2 then n := i
        else n := 3*n+1
        steps := steps + 1
    end
    return steps
end
```

```tac
func main ( )
    t1 = 2
    LmainLoopTest:
        if t1 >= 40 goto LmainLoopEnd
        param t1
        t2 = call collatz 1
        param "steps"
        param t2
        param "converges in"
        param t1
        call print 4
        t1 = t1 + 1
        jump LmainLoopTest
    LmainLoopEnd:

    return 0

func collatz ( n )
    t1 = 0
    LcollatzLoopTest:
        if n <= 1 goto LcollatzLoopEnd
        t2 = n / 2
        t3 = t2 * 2
        if n != t3 goto LcollatzElse
        n = t2
        jump LcollatzIfEnd
    LcollatzElse:
        t2 = 3 * n
        t3 = t2 + 1
        n = t3
    LcollatzIfEnd:
        t1 = t1 + 1
        jump LcollatzLoopTest
    LcollatzLoopEnd:

    return t1
```
