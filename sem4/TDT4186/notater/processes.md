# Processes
## Unix-processes
- primary way to structure activites
- can create new processes in a fast and easy way
	- parent process -> child process
- form a process hierarchy

shells starts unix processes

swapper processes start the init and pagedaemon (memory management) processes. The init processes then can start getty processes. Getty processors manages physical or virtual terminals. When this detecs a connection, it prompts username and runs the "login" program for auth. When logged in it starts bash process. Each getty can spawn more child processes, and shells is a tool to talk to terminals and start new processes.

## unix shell
- A "shell" around the operating system "core"
- text based user interface to start commands (unix programs):
	- commands can be located anywhere in the file system
	- shell searches in directories given in the $PATH environment variable
- every executed command is a seprate child processes
- typically, the shell blocks (waits) until the last command has terminated
- it is possible suspend, continue and terminate commands, and to have commands executing in the background

## Standard I/O channels
- usually connected to the terminal in which the shell runs that started the process
	- stdin: standard input reads user input
	- stdout: standard output outputs text of the process
	- stderr: standard error, separate channel for error messages
- Almost all UNIX commands also accept files as input or output channels
- shells provide a simple syntax to redirect the standard I/O channels

## Process-OS interaction in unix
- the transistion from code executing in an application to code running in the kernel needs to be protected
- many cpus provide several executin modes:
	- "user mode": only restricted functionality is allowed
	- "kernel" or "supervisor mode": full acces to all hardware resources
- special machine instructions are provided to transition from user to kernel mode
	- executing such an instruction causes the CPU to change its current execution mode to kernel mode and jump to an address predetermined by the processor hardware: system call
