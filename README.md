<!--
@Author: Izhar Shaikh
@Date:   2017-02-13T15:02:53-05:00
@Email:  izharits@gmail.com
@Filename: README.md
@Last modified by:   Izhar Shaikh
@Last modified time: 2017-02-15T16:01:19-05:00
-->


## Advanced Systems Programming (ASP)
#### Processing EFTs with worker threads (in C++ with POSIX Threads)

In this	assignment,	you	are	going	to simulate electronic fund transfer (EFT) between bank accounts.

We will assume that	there	is just	one	bank and several accounts. Your	program	will take	an	 input	file in	the	form:

```
AccountNo1 <space> initialBalance1	  
AccountNo2 <space> initialBalance2	 
..
..
AccountNoN <space> initialBalanceN
Transfer <space> accountNoFrom1 <space> accountNoTo1 <space> Amount1
Transfer <space> accountNoFrom1	<space> accountNoTo2 <space> Amount2  
...
...  
Transfer <space>	accountNoFrom1 <space> accountNoTo1 <space> Amount1
```

which	first	lists	the accounts in the	system along with	the	initial	balances and then lists the	transfers	between	accounts. You	can	assume that	all	transfers	refer	to existing	accounts and	 all	initial	balances and the transfer	amounts	are	nonnegative	integers.	  

Your program should	take one more	parameter	to denote the number of worker threads that will	run	in parallel. Note	that the main	thread will	initialize the accounts,read the input file and	assign work	(EFTs) to	worker threads in	a	round-­‐robin	fashion	until	all	transfers	are processed.

It is possible that an account may be overdrawn and gets a negative value while processing.
Your program should output on the standard output the amount in each account (in the order specified in the input file) once all transfers are computed.

#### Usage of your program:
```
$ ./transfProg inputFile numWorkers
```

```
So, as an example,assuming the input file is as below:

1 1000
2 50
3 400
4 150
Transfer 1 2 200
Transfer 1 4 50
Transfer 2 3 100
```

your program should produce the following output regardless of the number of worker threads specified:

```
1 750
2 150
3 500
4 200
```

your solution should maximize concurrency, be free of race conditions and deadlocks,and produce the correct output.You can use mutexes and condition variables and/or semaphores for synchronizing your threads.


## Implementation

1. main() spawns the threads as soon as it receives inputFile and workerThreads as cmd line arguments.

2. Each worker has it's own worker queue and it's own ID. Once threads are spawned, they all wait for the jobs.

3. main() starts parsing the input file and starts putting account numbers and the balances in the accountPool.

4. Once main receives the first EFT Transfer request, it parses it and assigns it to the first thread, then reads the next request and assigns it to the second thread and so on (round-robin) fashion. This is achieved by main() using the workerQueue of each worker. Each workerQueue has a counting semaphore and a binary semaphore that is used for synchronizing mechanism with main().

5. As soon as any worker receives it's first EFT Transfer request, it goes onto processing it. For the processing, all workers have to synchronize among themselves as the accounts requested in each transfer request might be requested in other requests as well which are handled by other workers. For this, the Implementation involves an associated lock with each account in the account pool. Whenever two workers try to access the same account details, they will proceed with the locking scheme, and one of them will be able to get the access, while other will wait. Please check the thread function comments for exact Implementation details incorporated for avoiding deadlock.

6. Once main() is finished assigning jobs, main asks the workers to exit by assigning one last job, which is marked by both account numbers as non-negative numbers, one worker realizes the current job is the last job, it exits.

7. After all threads are successfully terminated, main() prints the accounts and their balances in the order they were received initially.
