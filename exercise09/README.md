# Exercise Sheet 9

In this exercise you will implement different types of memory management and
benchmark them using a custom benchmarking utility.

# Task 1

Create an application `membench`, which will be used to benchmark different
memory allocator implementations in this and the next exercise sheet.

The benchmark itself is rather simple: The program spawns `T` threads, which
all perform `N` iterations of a loop. In each iteration, a memory segment of
size `S` is allocated and then immediately free'd again.

The `membench` application should be flexible, and able to benchmark
different memory allocators without having to be recompiled. To this end, it
receives the name of a _shared library_ as its first parameter, from which it
will subsequently load the addresses of two functions, `my_malloc` and
`my_free`, dynamically at run-time. See the man pages for `dlopen` and `dlsym`
for more information. Also read up on how to compile a shared library with GCC.

To test your implementation, and as a baseline for the following tasks,
also create a shared library called `system_allocator.so`, which simply wraps
GLIBC's `malloc` and `free` in the aforementioned functions.

Other than the name of the shared library, the application should also be
able to read the parameters `T`, `N` and `S` from the command line. For
example

```
./membench ./system_allocator.so 100 10000 1024
```

would launch 100 threads, each of which performs 10000
allocations/deallocations of 1 KiB, using the functions `my_malloc` and
`my_free`, which are defined in the shared library `system_allocator.so`.

# Task 2

Implement a naive _best fit_ memory allocator (see lecture slides) and test
it using your `membench` application. The allocator should thus provide its
own versions of the `my_malloc` and `my_free` functions. Note that while you
will likely need an initial call to system `malloc` to obtain a chunk of
memory which your allocator can operate in, try to avoid making any further
calls to `malloc` after this point.

Use a single global allocator that is shared by all threads. Ensure proper
synchronization. Also make sure that your allocator doesn't leak any memory
(i.e., you need a corresponding call to `free` for the initial `malloc` that
you do). You may add additional functions to your allocator interface for this
purpose, however there might also be more elegant solutions.

Compare the performance of your allocator to the system allocator using
`/usr/bin/time` and `membench`. Experiment with different numbers of threads
and allocation sizes, and a number of allocations/deallocations that gives a
reasonable total running time (more than a second and less than a minute).

Use an optimized build for your benchmarking.

# Task 3

For this task, instead of a single global allocator, create a separate local
allocator for each thread.

For creating local instances of the allocator, you can use thread-local
storage in GCC using `__thread` (see e.g.
https://gcc.gnu.org/onlinedocs/gcc/Thread-Local.html for more information).

Compare the performance of Task 2 and 3 in `membench`, with a few different
numbers of threads and allocation sizes, and a number of
allocations/deallocations that gives a reasonable total running time (more
than a second and less than a minute).

Use an optimized build for your benchmarking.
