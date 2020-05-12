### Explain the program

Five philosophers sit around a table and want to eat. To eat each philosopher needs two chopsticks (on the right and left-hand side). On the table are 5 chopsticks (1 between every table). A philsopher can only eat if both (right and left chopstick) are available. If only one (or none) is available, the philosopher waits until both are available again. Once done eating, the philosopher puts down both chopsticks again and stops eating.

### How can a deadlock occur? Does it always happen?

In this scenario a deadlock can happen when every philosopher in a row starts picking up the chopstick on the right-hand side first (philosopher 1 - 5 takes chopstick 1 - 5). Then, trys to pick up the left chopstick, which is locked for each philosopher because every philospher already picked up the right chopstick. Thus every philsopher is waiting and no philospher is eating.

It only happens if every philosopher first picks up the right/left chopstick. Then all chopsticks are taken but no philosopher has the chance to pick up the other one in order to eat and free the chopsticks again. 
If not all threads are executed consecutively, thus one thread can use the right and left chopstick, a deadlock will not happen.

### Solution

The simplest solution, though not the most efficient, is to for every philosopher to first check that the left-hand side chopstick is available before tacking the other one. This can be achieved by making all philosophers with an even index to first pick up the left chopstick, then the right one and for the "odd-based" philosophers to do it the other way. Thus, if one fork is taken — the other will not be considered.