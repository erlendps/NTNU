# Resources and Computer Architecture
## Computers as they are no more
- Von Neumann
	- Addressable inified memory for code and data
	- I/O devices in the same or a different address range
	- Optional: interrupts notify CPU of the completion of an I/O operation
	- Optional: I/O devices can use DMA to transfer data to memory without CPU interaction
	- Instruction memory and data memory are in the same physical main memory

### Async execution: interrupts
- Access to I/O devices is often slow
	- Polling sends a command and then waits until the device returns data
- With interrupts, the device notifies the program when data is ready
	- this changes the control flow the CPU executes
	- more complex to develop software for

### Going more into details
- components of the computer are connected by buses:
	- address bus: identify component
	- Data bus: transerfer information
	- Control bus: meta-information
- CPU has control over the bus
	- exception: DMA

### Getting a bit more real
- Simple model of executiion only works efficiently if the speed of memory is equal the speed of the CPU
	- not the case until 1980s
- Memory speed only improved ~6%/year
- Today: "memory gap"
	- CPU speed ~ 10000x faster, but memory speed only ~ 10x faster

## Introducing a memory hierarchy
- Idea: introduce caches
	- small, but fast intermediate levels of memory
- Caches can only hold a partial copy of the whole memory
	- Unified caches vs separate instruction and data caches
	- expensive to manufacture
	- Later: introduction to multiple levels of cache (L1, L2, ...)
		- each on bigger but slower than the previous one
- Caches work efficiently due to loaclity principles
	- temporal locality: a program accessing some part of memory is likely to access the same memory soon thereafter
	- spatial locality: a program accessing some part of memory is likely to access nearby memory text

**The further away from the CPU**
- increasing size
- decreasing speed

## Memory impact: non-functional properties
**Memory has a large influence on non-functional properties of a system**
- Average, best and worst case performance, throughput and latencies
- Power and energy consumption
- Reliability and security

**Non-functional properties depnd on many parameters of memory, e.g**
- Cache architecture
- memory type
- alignment and aliasing of data

## When one processor is not enough
- Moores law
	- observation that the **number of transistors** in a dense integrated circuit doubles about every two years
	- Accordingly, increase in CPU speed due to smaller semiconductor structures
- This development is htting physical limitations
	- CPU frequencies "stuck" at ~3 GHz
	- energy consumption is additional limiting factor
- what can we do with all these transistors?
	- bigger caches - energy hungry and prone to faults
	- Put more processors on a chip
		- earlier high-end systems already used multiple separate processor chips
- old as well as new problems:
	- memory througput now has to satisfy demands of n processors
	- software now has to support execution on multiple processors
	- caches need to be coherent so they hold the same copies of main memory data.

## More processors, more memories
- Memory throughput now has to satisfy demands of n processirs
	- provide each processor with its own main memory
	- NUMA
		- non-unified-memory-architecture
- And new problems show up:
	- how to access data in another CPUs memory
	- who decides which CPU is allowed to use the bus
	- is a common bus still efficient

## On-chip communication
- Use high-speed networks instead of conventional buses
	- using ideas from computer networking
	- on-chip network can achieve high througput and low latencies
- example on-chip ring network connecting 6 CPUs, a system controller and a GPU

## Heterogeneous systems: GPGPUs
- In modern computers, not only CPUs can execute code
- **GPGPUs** (general purpose graphics processing units)
	- massively parallel processors for typical parallel tasks
	- 3D graphics, signal processing, machine learning, bitcoin mining
	- few features for protection security
- Traditionally, GPUs were accessible to a single program only (in Unix: X window server) for drawing
	- other programs had to ask the X server for services
- In modern systems, multiple programs want direct access to the GPGPU
	- How can the os multiplex the GPGPU safely and securely?

## Security
- theres another important non-functional property
- Multiple programs running simultaneously
	- e.g a online banking application and a video player
- How can be avoid the video player accessing memory of the banking app
	- e.g your account number and password which the video player could share online
- Restrict access to non permitted memory ranges
	- the memory management unit (MMU) only makes memory ranges visible to a running program "belonging" to it.

## The MMU
- Idea: intercept "virtual" addresses generated by the CPU
	- MMU checks for "allowed" addresses
	- it translates allowed addresses to "physical" addresses in main memory using a translation table
- Problem: translation table for each single address would be large
	- split memory into pages of identical size
	- apply the same translation to all addresses in the page: page table
- MMUs were originally separate ICs sitting between CPU and RAM
	- or even realised using discrete components
	- higher integration due to Moores law -> fit on CPU chip now

### Page table structure
- split memory into pages of identical size
- apply the same translation to all addresses in the page
- find a compromise page size allowing flexibility and efficiency
	- typically severeal kB
- 32 bit CPU has roughly 1 million pages
	- use sparse multi-level page tables -> reduce page table size

### The memory translation process
- MMU splits virtual address coming from the CPU into three parts
	- 10 bits (31-22) page directory entry (PDE) number
	- 10 bits (21-12) page translation entry (PTE) number
	- 12 bits page offset inside the references page
- Translation process
	1. Read PDE entry from directory: -> address of one page table
	2. Read PTE entry from table: -> physical base address of memory page
	3. Add offset from original virtual address to obtain the complete physical memory address

### Speeding up translation
- Where is the page table stored?
	- can be several MB in size -> does not fit on CPU chip
	- page directory and page tables are in main memory
- Using virtual memory address translation requires thrre main memory accesses
	- same idea as with regular slow memory access: use cache
- MMU uses a special cache on the CPU chip_ the translation lookaside buffer (TLB)
	- caches commonly used PTEs
	- locality principle at work again

## What about the OS?
- new hardware capabilities have to be used efficiently
- the OS has to manage and multiplex the related resources
	- OS has to adapt to new hardware capabilities
	- it has to provide code for all new capabilities
	- these often interact with other parts of the system, making the overall OS more complex
- A modern OS aslo has to ensure adherence to non-functional requirements (security, energy, real-time)
	- the OS has to do more bookkeeping and statistics
	- some of the non-functional properties contradict each other
	- unexpected problems may show up
- Finally, the OS itself has to be efficient