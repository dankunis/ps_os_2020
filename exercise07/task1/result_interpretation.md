Measure speed using`/usr/bin/time -v task1`.

User-time is the amount of CPU time spent in user-mode code within the process (actual time used in executing the process).
Sys-time is the amount of CPU time spent in the kernel within the process. This is CPU time spent in system calls withing the kernel. 

The spin lock solution in most cases end up with system time of 0.00 seconds and user time of 0.03. On the other hand the mutex solution
has a lower user time but a higher system time (both around 0.01) 

This result says that the spin lock solution spends less time within the kernel but more time in user-space, in contrary to the mutex solution.
