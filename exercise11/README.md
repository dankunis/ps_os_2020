# Exercise Sheet 11

The following is a (slightly modified) _real_ exam from a previous year. It
should give you a general idea of what to expect from this year's exam.

This exercise sheet is optional, and provides up to 3 **bonus points**: These
points are not included in the total number of points reachable this semester (32),
but will count towards your personal final score (theoretically, it is thus
possible to achieve 35/32 points).

While the exam itself is not split into explicit tasks, we define the
following tasks for receiving the bonus points:

- **Task 1:** Implement the basic train scheduling behavior and output.
- **Task 2:** Implement the signal handler for printing the state.
- **Task 3:** Provide a fully functional solution that compiles without
  warnings, executes correctly producing only the expected output and
  finishes cleanly without leaking any resources.

**As usual, please mark your completed tasks in OLAT**.

---

## General Information

**Please read this description carefully.**

Talking, chatting, texting or any other form of live human to human
communication is not allowed and may lead to a negative grade. However, you
are allowed to use all other offline and online resources such as previous
homework solutions, manpages, StackOverflow, etc.

If you directly copy code from one of these sources, please add a
**citation**
as a comment before the code fragment. i.e.

```C
// The following code is from my solution to exercise 5.2
```

or

```C
// Source: https://stackoverflow.com/questions/14888027/mutex-lock-threads
```

If we find the same code in multiple exams without citation, we have to
assume cheating and you will receive a failing grade.

Your program **must compile** on ZID-GPL and include a Makefile. Also, if
something does not work as expected, provide comments in the source code
explaining what you think the problem is. It might improve your grade.

You have 1 hour and 45 minutes to complete this exercise.

### General Hints

- Don't forget you can load a newer version of GCC on ZID-GPL using e.g.
  `module load gcc/7.2.0`.
- Remember that you can use `valgrind` to check your program for memory
  leaks.
- Try to develop your program incrementally, and focus on the core
  functionality first.

## Exam Exercise - Train Station

Develop a program simulating a train station, with trains arriving and leaving.

The following table summarizes the situation which should be modelled:

|         | Arrival Time | Stop Time |
| ------- | :----------: | :-------: |
| Train 1 |      0s      |    8s     |
| Train 2 |      0s      |    4s     |
| Train 3 |      2s      |    3s     |
| Train 4 |      3s      |    7s     |
| Train 5 |      5s      |    3s     |
| Train 6 |      5s      |    5s     |
| Train 7 |      6s      |    2s     |

For example, the table indicates that train 3 arrives 2 seconds after the program start, and remains at the station for 3 seconds.

The station has **3 platforms** (platform 1, 2 and 3) which should be managed globally. Therefore, this management needs to be able to handle concurrent access.

Every train should be implemented with one thread. This thread first waits until its arrival time, and then tries to acquire any platform. If no platform is available the train repeatedly tries to get another platform.

When the train thread succeeds, it moves into the station and prints the following text:

```C
"X seconds after the start: train Y moves into the station at platform Z."
```

with the corresponding values for X, Y and Z.

After this, the train thread sleeps for the given stop time, releases the
platform and leaves the station. At this point, it should print the following message:

```C
"X seconds after the start: train Y leaves the station at platform Z."
```

Afterwards, the thread ends.

The complete program should end after all train threads have completed.

When the process receives a `SIGINT` signal, the main thread should print out the current state of the station, cancel all train threads and end the process.

The printout should look like this:

```
Platform 1: Train 4
Platform 2: Empty
Platform 3: Train 6
```

**Hints:**

- You can use `time` and `difftime` for printing times.
- Make sure to refresh your memory on `man signal-safety`.
