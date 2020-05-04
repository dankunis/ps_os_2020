# Exercise Sheet 6

In this exercise sheet you will work with POSIX threads and synchronize them
using mutexes.

# Task 1

Begin by skimming through `man pthreads`. Take note of additional compiler
flags that may be required.

Write a program which spawns 10 threads.

- Each thread creates a file called `threadN.txt` (where `N` is the thread
  number between 0 and 9) and then waits 2 seconds before exiting.
- After spawning all threads, the main thread waits 1 second and
  then decides for each thread whether to randomly cancel it, with a chance of
  50%. Write each decision to the console.
- If a thread is cancelled, it deletes its `threadN.txt` file.
- Finally, the main program waits for all threads to exit.

Example output:

```
 > ./task1
Thread 0 not cancelled
Thread 1 cancelled
Thread 2 cancelled
Thread 3 not cancelled
Thread 4 cancelled
Thread 5 not cancelled
Thread 6 not cancelled
Thread 7 not cancelled
Thread 8 not cancelled
Thread 9 cancelled
```

# Task 2

In this task you will revisit the producer-consumer pattern from last week,
this time with multiple consumers implemented through threads.

Your program should spawn 5 threads, the _consumers_, which all try to read
elements from a queue storing integers. For the queue, you can use the basic
implementation that is provided in [myqueue.h](myqueue.h).

- When a consumer thread successfully reads an element, it adds it to its
  local `sum`. When the element is `0`, it prints out the sum, returns it
  to the main thread and exits.
- The main thread acts as the _producer_. After spawning all 5 consumers, it
  feeds 10.000 entries of value `1` into the queue, followed by 5 entries of
  value `0`.
- The main thread then waits until all consumers have finished and computes
  the final sum from all the partial results, prints it to the console and
  exits.

To avoid race conditions, synchronize access to the queue by using the
`pthread_mutex` facilities.

Instead of a mutex, could a semaphore be used in this situation, like last
week?

# Task 3

In the provided file [philosophers.c](philosophers.c) you can find an
implementation of the classic
[Dining philosophers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem).
However, the implementation is flawed: In some situations multiple philosophers
will wait forever for a chopstick being released, constituting a
**deadlock**.

- Explain the program.
- How can a deadlock occur? Does it always happen?
- Change the program to prevent any potential deadlocks from occurring.
  Explain how your changes prevent deadlocks.
