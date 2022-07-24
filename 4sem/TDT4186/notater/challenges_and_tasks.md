# Challenges and tasks of an OS
## Different views of and operating system
- abstractions
	- processes
	- virtual mem
	- file systems
- tasks
	- cpu scheduling, synchronization
	- inter-process communication
	- memory management
- problems
	- deadlocks
	- system security
- challenges
	- multiprocessor systems
	- cloud computing and virtualization


**A process** is a program in execution
This requires a process context which consists of:
- Memory
- Conetens of processor registers
- process state
- user id
- access permissions
- currently used resources

## Process model
- Techinal view (multiprogramming)
	- 1 instruction pointer
	- context switching
- Conceptional view (concurrent processes)
	- 4 independent sequential control flows
- Real time view (CPU multiplexing)
	- (Gantt Diagram) Only one process is active at any given point in time (on a single processor system)

## Process behaviour and states
**Process states**
- Running
	- process is currently being executed
- Ready
	- process is ready to run and waits for the CPU
- Blocked
	- Process waits for the completion of an I/O operation

## CPU scheduling
A single scheduling algorithm is characterized by the order of processes in the queue and the conditions under which the processes are added to the queue.
- Enables the coordination of concurrent processes
- Basic questions:
	- which sorts of events can cause preemption
	- in which order should processes be executed
- Objectives of a scheduling algorithm
	- user oriented -> short reaction times
	- system oriented -> optimal CPU utilization
- No single scheduling algorithm can fulfill all requirements

## Process synchronization
Need to synchronize processes so that they dont interfere with each other.
- Reason for the problem: **critical sections**
- Solution approach: **mutual exclusion**
	- Using the **mutex** abstraction

If one of the processes A or B is in between the calls to lock and unlock, the other cannot pass the lock and blocks at the lock until the critical section is left by the other process calling unlock

## Deadlocks
A rule can block all processes beacuse every process is yielding to the other (traffic rule analogy)