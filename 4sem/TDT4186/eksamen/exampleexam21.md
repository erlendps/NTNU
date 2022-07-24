# Example Exam 2021
## 1 Processes
### 1.1
Before first iteration we have 1 process; the main parent process (say pid 100).
After the first iteration in the for loop, we have 2 processes, pid 100 and pid 101. After the second iteration, both processes forks and creates a new processes, in total 4 processes. After the third and final iteration, each of the 4 processes creates a new child with fork, bringing the total up to 8 processes.

### 1.2
It writes A before forking, so the string would always start with A. It then forks, creating a new process. Which processes that continues is not fixed. The child can finish first, creating the string ABC, then the parent could continue, and adding BC to the string, meaning one legal output is ABCBC. The parent could also continue first, creating the string AB, before it waits for the completion of its child. The child then produces BC. The child is then killed, and the parent continues writing C at last. We then get the string ABBCC. There are also other variations, e.g a context switch from child to parent after line 10, but this would yield the same result as the last one, ABBCC. The legal outputs are therefore ABCBC and ABBCC.

## 2 Shell pipelines
Firstly, on a pipe, its pipe[1] that refers to the write end, so #define WRITE 2 should instead be defined to 1. Having it at to would not throw an error, but you would access the wrong memory location, giving unexpected results.
pipe2 should be initialized as an array with 2 elements, instead of one. Im not certain it would make a difference, since in pipe() the pipe is created and should also create the write end of the pipe even though our pipe2 is not an array of 2 elements (same reasoning as above, writing pipe2[1] would give you the memory data that lies 4 bytes next to pipe2[0]). But it looks better, and its best to allocate the memory for it. 
In line 11

## 3 Threading
### 3.1
Say that thd->proc_info is not null at the start, and enters the if statement. Then a context switch happens before fputs is run. proc_info would then be set to null, then a new context switch happens, and fputs gets ran. fputs would then write null to the stream, not the original string. This could potentially lead to problems. 

### 3.2
No it does not. You could still enter the if-statement and before the wait command, a context switch to thread 2 happens and occupies (and writes null to proc_info) the mutex. When thread 2 signal the lock, a context switch can happen again to thread 1, and it would again write null to the stream. A better solution would have been to acquire the lock before the if-statement. 

## 4 Deadlocks
### 4.1
yes, suppose the following interleaving. Thread 1 acquires lock 1 and a context switch to thread 2 happens. T2 acquires L3 and a context switch to T3 happens. T3 acquires L2 and a context switch happens to T1. T1 cannot acquire L2, since its held by T3. T3 cannot acquire L3 since its held by T2 and T2 cannot acquire L1 since its held by T1. We have a circular waiting and thereofre a deadlock.

### 4.2
Introduce a 4th lock, L4, that each thread must acquire before trying to acquire any of the other locks.

## 5 memory management
### 5.1
S1: C needs to be allocated 4 MB of memory. Since the algorithm is best-fit, and the section 16-22 (8 MB) is already partioned into two blocks of 4MB, C is allocated 16 and 18.
S2: D needs to be allocated 16 MB of memory, since the first section of 16MB (0-14) already has a single block used, D is allocated 16-30.
S3: E needs to be allocated 16MB, however, the memories current state does not allow for a 16MB block to be allocated, so no allocation happens.
S4: F needs 8MB. The first 16MB section does not have enough space, so F allocates the second 16MB section. Since it only needs 8MB, it partitions the 16MB to two 8MB blocks, and F allocates 16-22.

### 5.2
External fragmentation is unused memory outside of the data memory that cannot be used since its to small or other reasons. This could happen in a linked memory allocation strategy. 
Internal fragmentation is unused memory inside a data block. This cannot be used by others, since its already allocated to a consumer. The buddy algorithm removes external fragmentation, but adds internal fragmentation.

### 5.3
TLB, translation lookaside buffer is used to reduce the time it takes to lookup a virtual address (and map it to a physical address). It works in many ways the same as a cache, by saving the latest virtual address lookups in the buffer. When a new logical address lookup happens, the MMU also checks its TLB. If there is a record that matches, it returns the address.

## 6 Scheduling
done on own paper.

## 7 File I/O
### 7.1
It would output ar#. lseek moves the pointer 5 bytes to the right, placing it at a. Then read only reads 3 bytes, namely a, r and #. These 3 bytes are placed in buf[0], buf[1] and buf[2], and is terminated by buffer[3] = 0 (this is the same as setting buffer[3] = '\\0'.

### 7.2
It will be dee. It opens the fd in write only and append mode. Also because of the trunc flag, all the content on the file before is truncated to length 0 (removed in other words), so that we have a "new" file.

## 8 File systems
### 8.1
From the direct entries, we get $2048*10 = 20KB$.
From the single-indrect entry, we get: $(2048/4)*2048 = 1MB$
From the double-indrect entry, we get: $(2048/4)*(2048/4)*2048 = 536MB$
In total we get approx 537,02MB as the maximum size

### 8.2
The files that use 2kB uses 100% of the block, i.e wastes 0 space. The files that use 1,5kB wastes $1-1.5kB/2kB=0.25$ 25% of the space. The total fraction is then $(0+0.25)/2=0.125$, meaning 12,5% of the space is wasted.

### 8.3
No it would not. If we reduce the block size to 1kB, both the 2kB and 1.5kB files would need 2 blocks. Of those two blocks, the 2kB file would fill the entire block (fraction of 0 space is wasted), while the 1,5kB would only use 75% of the space, meaning a waste fraction of 0.25. This would yield the same total space wasted.

## 9 Security
### 9.1
Passwords are stored in /etc/shadow. Here it can check either by (depending on the implementation in the system) encrypting the inputted password and checking it against the recorded value, or by hashing the inputted password and checking it against the recorded hash in /etc/shadow.

### 9.2
Since you are root, you have access to everything. You could change for example the login process to also capture the input and log it to a file. I suppose you also can change the way passwords are stored, by e.g encrypting them with a weak encryption algorithm (DES) and then decrypt the password.

## 10 storage systems
### 10.1
RAID0 makes the whole disk available for storage. So 8 disk all with RAID0 would yield 8TB of storage.
RAID1 mirrors disk, so you "lose" half of the capacity. 8 disks would yield 4TB.
In RAID5, approximately 1 disk of your total amount of disk would be used for the parity bits. So 8 disks would yield 7TB.

### 10.2
RAID0: disks can perform reads in parallell, so given that the data is evenly distributed in the 8 disks, we would get a throughput of 800 reads/sec
RAID1: Would also give 800 reads/sec. 4 disks would provide 400 reads/sec, but since its mirrored, you can access the "same" disk in parallell, providing an additional 400 reads/sec.
RAID5: 