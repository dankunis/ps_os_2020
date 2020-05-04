# Task 3

- What is a race condition?

    It's a state of the system when thr result relies on the order of the code execution leading to a undetermined result.

- Why do race conditions occur?

    When the developer (unexpectedly) depends on the relative timing of events. Relies on order of execution where it is not determined.

- Is the output of a program with a race condition always incorrect?

    Because it is undetermined — it relies on the order of execution and cannot be predicted.

- If you run your program 100 times and always get the correct result, does
  that mean that there is no race condition?
  
    No, it means you are (un)lucky enough that the results reproduces, but there are absolutely no guarantee that the result will be the same next time.
  
- What is synchronization?

    It's a way to coordinate processes that use shared data.

- Why are semaphores well suited for synchronizing producer-consumer patterns?

    They are very easy to use and it solves the problem.  

- Could Task 2 also be implemented using a single semaphore?

    Yes, I changed the implementation.