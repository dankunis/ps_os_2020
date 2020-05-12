One advantage of thread condition is that thread can be informed while waiting until a certain condition is met.
In this case, for example, the consumer thread does not have to check for the size of the queue in an endless loop but can wait for the signal
of the producer. 

Compared to the other examples it also takes about `0.01` sys time and about `0.01` in user time which results in a slightly faster execution.
