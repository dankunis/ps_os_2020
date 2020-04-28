# Exercise Sheet 5

In this exercise sheet you will practice shared memory communication and
synchronization with semaphores. As with previous exercises, **make sure to
use the newer POSIX APIs** instead of the System V APIs.

# Task 1

In this task, you will implement the classic producer-consumer pattern using
multiple processes operating on shared memory.

Begin by reading `man 7 shm_overview`. Take note of additional linker flags
that may be required.

Create two programs, `task1_a` and `task1_b`.

The role of `task1_a` will be to manage the shared memory's creation and
subsequent destruction. To this end, it creates a shared memory segment and
writes into it a numeric parameter `N`, which is passed as an argument on the
shell. Additionally, `task1_a` creates a _named pipe_ (FIFO) and waits until
it can read the result of the computation (which will be performed by
`task1_b`) from it. After it has read the result, it prints it to the
console, cleans everything up and exits.

The process `task1_b` acts as the _parent_ of two child processes, the
_producer_ and the _consumer_. The parent begins by attaching to the shared
memory segment created by `task1_a`. It then creates the two child processes
and waits for their termination. Finally, it writes the result, which will be
stored in shared memory by the consumer, into the named pipe which `task1_a`
is waiting on, cleans up and exits.

The producer reads `N` from shared memory and iteratively writes all numbers in
`[1, N]` into a
**_ring buffer_ of size 10**, which is also stored in shared memory, and
then exits. For your ring buffer you can use a simple array, and access it
using the modulo operator, e.g. `buffer[i % 10]`.

The consumer reads `N` from shared memory and iteratively sums up `N`
subsequent numbers from the ring buffer, thereby effectively computing the
sum of all numbers in `[1, N]`. It then writes the resulting sum into the
shared memory segment and exits.

Example output:

```
TERMINAL 1                      TERMINAL 2

> ./task1_a 10
                                > ./task1_b
Result: 55
```

Analyze the obtained output. Is the computed result the expected value? Does
the computed value change across different runs? What happens when you try
larger values for `N`, for example 10,000 or 100,000? Try to explain the
behavior.

Notes:

- There is no need to attach multiple shared memory segments. Think about a
  good way of storing the entire context (`N`, the ring buffer and the
  result) inside a single shared memory segment.
- Make sure to use numeric data types that can store sufficiently large
  values, for example `uint64_t`, which is defined in `<stdint.h`>.
- This exercise MUST be implemented using the POSIX shared memory mechanisms.

# Task 2

In this task, you will use _semaphores_ to synchronize access to shared data
structures. Begin by reading `man 7 sem_overview`. Take note of any
additional compiler flags that may be required. This exercise MUST be
implemented using the POSIX semaphore facilities.

Implement the same program as before. However this time, ensure that the
consumer only reads each value in `[1, N]` exactly once from the ring buffer.
Likewise, ensure that the producer only overwrites values in the ring buffer
that have already been read by the consumer. Use two semaphores to implement
this behavior.

Measure the execution time of your solutions for Task 1 and Task 2 using
`/usr/bin/time`. What do you notice?

# Task 3

Answer the following questions:

- What is a race condition?
- Why do race conditions occur?
- Is the output of a program with a race condition always incorrect?
- If you run your program 100 times and always get the correct result, does
  that mean that there is no race condition?
- What is synchronization?
- Why are semaphores well suited for synchronizing producer-consumer patterns?
- Could Task 2 also be implemented using a single semaphore?
