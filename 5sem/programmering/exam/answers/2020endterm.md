# Exam 2020 Endterm

## Problem 1

Execution may fail. I dont believe this

## Problem 2

1 2 6 3 7 is not possible, it will always finish executing, and with this, B is bound to true.

%4 skip, but surely they say %6 B = false

## Problem 3

1 is printed and all threads terminate

## Problem 4

They realise the reversing of a list/stream S, in a demand-driven way. Both are lazy in the way that just calling the function would yield a future. R2 uses a lazy subroutine, but since R2 returns whatever FnX returns, when R2 is needed, FnX is also needed. This makes it so R1 and R2 are in essence the same. R1 and R2 calculate the same result, and for a lazy program I would use R1. This is because it simply creates a bigger overhead by using the laze keyword twice, and as mentionded above, it is wasted.

## Problem 5

```oz
fun lazy {LzyFilt Xs F}
  case Xs of nil then nil
  [] X|Xr then
    if {F X} then X|{LzyFilt Xr F}
    else {LzyFilt Xr F} end
  end
end
```

## Problem 6

The first output does ont change because it is not needed.

## Problem 7

Running S1 S2 in separate threads would, and throwing exeception there, would not have an affect on the main thread. If an exception occurs in the thread and not handled, the thread will simply terminate, and the main thread continue. So if S1 is wrapped in a thread, the main thread will never catch an exception, so it will neither raise the exception. S1 and S2 have no interaction, they are independent. However, if S2 raises an exception (not wrapped in a thread) it would terminate the main thread as well, since S2 is outside the try catch block. Similarily, if S1 is not wrapped in a thread and throws a exception, this will be caught by the main thread.

## Problem 8

Thread safe programs may result in deadlock.

## Problem 10

t.join ensures that log never prints null

## Problem 11

t.join ensures that log never prints null is correct. This is because a threads join method halts the calling threads execution, waiting for the thread to finish executing. The thread sets the result var to a string. The main thread waits for this with join, and finally logs this.

t.join is not a suspending method. If a thread suspends, it is because of a property of the thread itself, not the join method. The join method, as stated above, tells the main thread to wait for the thread to finish executing, meaning that it is in fact the calling thread (the one that call join) that may suspend.

ThreadsCommunicate is not an example of nondeterministic concurrency. We can easily see that in the thread body, the result is set to an value that always compute to the same value. Then, the join ensures that the result variable is set by the thread before logging.

Yes we can. Notice that for threads to start executing, you would have to call start or join on the thread. Since s is never started, it never changes the result variable.

It will not deadlock, both because of what I stated above and even if we add s.start, it will not deadlock since there are no restrictions (i.e locks) on the result variable. Any thread can write any (string) value to result at any time.
