# Intro to operation systems
## Definitions
"Software that controls the operation of a computer and directs the processing of programs)" (Merriam-Webster)
"The operating system is software that manages every part of a computer system - all hardware and all other software. To be specific, it controls every file, every device, every section of main memory, every nanosecond of processing time, and every network connection. It controls who can use the system and how." (Encyclopedia)
- Is an interface between applications and hardware
- Commen ideas
	- The OS serves the users and their programs, it is never an end to itself
	- The OS has to know the hardware in detail and provides suitable abstractions to apllication programs
- Hardware and application requirements determine the services provided by an OS
	- From this, its structure and functionality are derived

## Serial processing
- Non-interactive programming of computers
	- commonly in machine language
	- input via punched card reader
	- output via printer
	- errors indicated using lamps
- Compute time scheduled with a paper calendar
	- Waste of compute time due to overallocation or termination of programs due to errors
- Minimal CPU utilisation
	- Most of the time was spent waiting for slow I/O devices
- First system software as reusable program libraries
	- Linker, loader, debugger, device drivers

## Simple batch systems (from 1955)
- Reduced frequency of operator interactions
- First operating systems: "resident monitors"
	- interpretation of job control commands
	- loading and execution of programs
	- device control
- Added control cards to the stack of punch cards

The "monitor" stayed resident in memory and executed one application after the other. First sign of operating system
- Problems due to erroneous applications
	- program doesn't terminate
	- writes in the memory of the resident monitor
	- accesses card reader directly and interprets control commands as data
- Solutions
	- Addition of a timer circuit generating interrupts
	- Traps for erroneous programs
		- fence register to realise primitive memory protection
		- Priveleged operating mode of the CPU
			- deactivates fence register
			- allows input and output

## The input/output bottleneck
- **Problem**: CPU is faster then card reader and printer
	- valuable compute time is wasted by active waiting
- **Solution 1**: off line processing
	- Enabled by magnetic tape drives
	- Parallelisation of I/O using multiple satellite computers
- **Solution 2**: spooling
	- Enabled by magnetic disk driver (random access) and direct memory access (DMA)
	- Computation and I/O can now overlap
	- Requires rules for processor allocation

## Multiprogramming (from 1965)
- Despite spooling, a single program does not utilise the CPU efficiently
	- System operation alternates between CPU bursts and I/O bursts, during with the CPU has to wait
- With multiprogramming, the CPU works on multiple jobs at the same time
- Start with program A,doing CPU burst, then needs to do a I/O job. The program then starts another program B that works with the CPU

** The operating system becomes increasingly more complex:**
- Handling concurrent I/O activites
- managing the main memory for multiple programs
- internal management of programs in execution (processes)
- processor scheduling
- multi user operation: security and accounting

What we need:
- Memory management
	- programs to be started need an assigned memory range
- Memory protection
	- Simple fence register is no logner sufficient to isolate processes from each other
	- Solution: use a simple memory management unit MMU
- Process management
	- Every "program in execution" has its own context
	- When switching between processes, the context has to be switched as well

## Dialog computing (from 1970)
- New I/O devices enable interactive software
	- keyboard, screen, later mouse
- Time sharing operation
	- enables acceptable response times for interactive users
	- Timer interrupts ensure the preemption of processes which run (too) long
- System programs enable interactive software development
	- editor, shell, compiler, debugger
- Disks and file systems allow to access programs and data at any time