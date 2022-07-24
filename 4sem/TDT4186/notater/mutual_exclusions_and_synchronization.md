# Mutual Exclusion and Synchronization
Problem: **Race Condition**
- A race condtiotion is a situation in which multiple processes access shared data concurrently and at least one of the processes manipulates the data
	- when a race condition occurs, the resulting value of the shared data is dependent on the order of access by the processes
	- the result is thereofore not predictable and can also be incorrect in case of overlapping access
- To avoid this, concurrent processes need to be synchronized

## Synchronization
- The coordination of the cooperation of processes is called synchronization
	- synchronization creates an order for the activities of concurrent processes
	- sequence of activities

### Critical selection
- In the case of a race condition, N processes compete for the acces to shared data
- the code fragments accessing these critical data are called critical sections

**Problem**
- we need to ensure that only a single processes can be in the critical section at the same time

**Solution 1: Lock Variables**
- Lock variable is an abstract data type with two operations: acquire and release
- acquire
	- blocks a process until the specified lock is open
	- then locks the lock itself "from the inside"
- release
	- opens the specified lock without blocking calling process

**Implementation**
- A process takes a waiting number before it is allowed to enter the critical section
- Admission in order of the waiting numbers
	- when leaving the critical section, its waiting number is invalidated

**Problem**
- The algorithm cannot guarantee that a waiting number is given to only one process
- in this case, a PID decides about the priority

### Active waiting
- processes "waits" when checking if the critical section is locked. It is better to have passive waiting

### Passive waiting
- Processes release the CPU while they wait for events
	-	in case of synchronization, a process "blocks itself" waiting for an event
		-	the process is entered into a waiting queue
	- when the event occurs, one of the processes waiting for it is unblocked (there can be more than one waiting)
- the waiting phase of a process is realized as a blocking phase
	- the process schedule is updated
	- another process in state READY will be moved to state RUNNING
- with the start of the blocking phase of a process, its CPU burst ends

### Semaphores
A semaphore is defined as "a non-negative integer number" with two atomic operations

**P**(from "prolaag" = "decrement", also down or wait)
- if the semaphore has the value 0, the process calling P is blocked
- otherwise, the semaphore value is decremented
	
**V**(from "verhoog" = "increment" also up or signal)
- a process waiting for the semaphore is unblocked
- otherwise, the semaphore is incremented by 1
	