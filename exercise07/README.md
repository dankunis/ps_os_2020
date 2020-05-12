# Exercise Sheet 7

In this exercise sheet you will continue to use POSIX threads and test
different methods for mutual exclusion and synchronization.

# Task 1

Implement the single producer multiple consumer pattern as in last weeks'
[Exercise Sheet 6 Task 2](../exercise06/README.md), but increase the number
of consumers to 50 and use `pthread_spin_lock` to protect access to the
queue. What are the advantages and disadvantages compared to `pthread_mutex`?

Measure the execution time with `/usr/bin/time -v`, compare it to the
execution time of last week's task (also using 50 consumers) and interpret
the results.

# Task 2

Repeat Task 1, but use pthread _condition variables_ to signal the
availability of new elements. What are the advantages and disadvantages
compared to the other two approaches?

Again take measurements and interpret the results.

# Task 3

Write a program that initializes an integer value `X` to 0 and subsequently
creates 1,000 threads. Each of these newly created threads should execute a
loop of 10,000 iterations. In each of these iterations, the value `X` should
be increased by one. The main thread waits for all the other threads to
finish and then writes the final value of `X` to the output.

Then, implement two variants:

1. Protect access to `X` using a mutex.
2. Use C11 atomics to perform the increment operation.

How does the program behavior differ? Measure the execution time of both
versions.
