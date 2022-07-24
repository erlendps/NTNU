# Example exam
## 1 Processes
### 1.1
Before the first fork there is one process (parent), after the first fork there are then 2 processes. After the second fork, both the child and parent fork, resulting in a total of 4 processes running.
It only enters the if-sentence once. There we fork once more, bringing the total up to 5, and if we are in the parent wo do another fork. The total number of processes is then 6. The reason only one process can enter the if-sentence, its because it requires that the pid1 > 0, meaning that we come from the main parent process, and that pid2 = 0. There is only one pair of pids that accomplish this.

### 1.2
A will always be the first letter in the string. We then fork and get 2 processes. We then have a couple of scenarios. The child process could complete before the parent continues, we then get the output ABCBC. If both processes complete line 10 (context switch at line 10), and then the child completes, we get ABBCC. We could also have that the parent "completes" before the child (but since the parent will wait for the child, the child must complete before the parent can continue). We then get the same as the last one, ABBCC. We therefore have a total of 2 outputs: ABCBC ABBCC

## 2 System ccalls and the shell
### 2.1
fd: It has to check that the file is not a NON-BLOCKING file, in which the read would have blocked. It also has to check that the fd is indeed a fd, and that its suitable for reading (permissions, not a directory and such).
buf: It has to check that the given address is inside the executors available address space.
count: The count is not as important as the other two, but one should be aware of the max size of ssize, and also be aware how different distributions handle this.

### 2.2
No, its not possible. The cd command has to be internal because a shell itself needs to be able to change its current working directory. The shell process then needs a method to do this. If one were to do this as an external command, you would have to fork and execute cd in the child. However, now you changed the cwd for the child, not the parent. It's impossible to change the cwd for the shell process unless its a built-in command.

## 3 sync
### 3.1
```C
Semaphore S1 = 0;
Semaphore S2 = 0;
Semaphore S3 = 0;

f1 () {
	wait(S2);
	printf("3");
	printf("5");
	signal(S3);
}

f2 () {
	printf("2");
	signal(S2);
	wait(S1);
	printf("13");
	signal(S1);
}

f3 () {
	wait(S3)
	printf("7");
	printf("11");
	signal(S1);
}
```

### 3.2
It is not possible. You could have two cores beginning the same processes (that uses mutextes implemented in primitive C) at the exact same time, and they would not have time to read that the other process (core) has entered a critical section.

### 3.3
If the instruction x++ is executed at nearly the exact time, one of the threads might not have enogh time to write the result of x++ back to the memory before the second thread reads the (unupdated) value of x. Then it would seem that it only was incremtended once.

## 4 Deadlocks
### 4.1
Yes, this is possible. If the first line in each thread gets executed, this would result in a deadlock. T1 is waiting for L2 (which is held by T3), T2 is waiting for L1 (which is held by T1) and T3 is waiting for L3 (which is held by T2). We have a circular waiting, and therefore a deadlock.

### 4.2
A simple fix, that is not so effective, is to introduce a foruth mutex, that decides if a thread is allowed to wait for L1, L2 or L3. This would be the first line (and last, signaling) in each thread, and could look like this:

```C
Semaphore L1=1, L2=1, L3=1, L4=1;

// Thread 1:
wait(L4);
wait(L1);
wait(L2);
// critical section requiring L1 and L2 locked
signal(L2);
signal(L1);
signal(L4);

// Thread 2:
wait(L4);
wait(L3);
wait(L1);
// critical section requiring L3 and L1 locked
signal(L1);
signal(L3);
signal(L4);

// Thread 3:
wait(L4);
wait(L2);
wait(L3);
// critical section requiring L2 and L3 locked
signal(L3);
signal(L2);
signal(L4);
```

## 5 memory allocation
### 5.1
| Allocation | 0 | 2 | 4 | 6 | 8 | 10 | 12 | 14 | 16 | 18 | 20 | 22 | 24 | 26 | 28 | 30 |
| --- | --- | --- |---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | A | A | A | A |
| 2 | A | A | A | A | | | | | B | B | B | B|B|B|B|B|
| 3 | A|A|A|A|C||||B|B|B|B|B|B|B|B|
| 4 not allocated | |

### 5.2
| Refrence sequence | 	|5|3|5|1|2|5|4|6|1|
|---|---|---|---|---|---|---|---|---|---|---|
| Main memory |  frame 1|5|5|5|5|5|5|5|5|1|
|  | 			 frame 2| |3|3|3|2|2|2|6|6|
|  | 			 frame 3| | | |1|1|1|4|4|4|
| Control states|frame 1|0|1|0|1|2|0|1|2|0|
|  | 			 frame 2| |0|1|2|0|1|2|0|1|
|  | 			 frame 3| | | |0|1|2|0|1|2|

## 6 Virtual memory
### 6.1
0x0000BEEF has 00 as index. Start address is then 0x000000E0. The length is 0x2120FF > 0x00BEEF, so we have no memory trap. The physical address is therefore 0x000000E0 + 0x0000BEEF = 0x0000BFCF

0x1CEB00DA has 0x1C as index, which has start address 0x00010000. The length is 0xFFFFFF > 0xEB00DA so we have a valid access. The physical address is therefore 0x00010000 + 0xEB00DA = 0x00EC00DA.

### 6.2
4 most significant bits determine page.
0x6AB1 therefore resides in page 6, which has start address 0x4000. It has an offset of 0xAB1, which gives the physical addres 0x4AB1.

The 0xF1B7 resides in page 15, which has startaddress 0x5000. It has offset 0x1B7, and therefore has the physical address 0x51B7.

### 6.3
We get the largest page size with the largest offset. If we choose the first 4 bit as page identifier, we could potentially have an offset of $2^{28} = 268MBit$ pages. This however assumes that the systems least unit is 4 bit, but this is not normal (i think). If one were to consider a more natural smallest unit (1 byte), we would get $2^{24} = 16MBit = 2MB$ page sizes. 

Should be 2\*\*23

## 7 Scheduling
### 7.1
Done on own paper

### 7.2
done on own paper

## 8 I/O, disk scheduling and file systems
### 8.1
The use of lseek moves the pointer 3 bytes to the "right", meaning that it points to the character "d". The buffer then contains only the string "d", because when we read the fd, we read it with the offset given in lseek. It would likely not read all 1023 bytes, since its likely the string in the fd is terminated before that. Memset fills the first 1024 bytes from the memory address pointed at by \*buf with the constant 0. Its done to "reset" the memory. 

### 8.2
With the FIFO strategy, the disk scheduler will read 4, 7, 11, 3, 2, 13, 1, 15, 5, 6.

### 8.3
I assume that it literally uses as shown in the picture. We would then get 10 direct inodes, 1 single indirect and 2 double-indirect. The maximum then becomes:
$10*1KB + (1024/4)*1KB + (1024/4)*(1024/4)*1KB = 65802KB = 64MB$


