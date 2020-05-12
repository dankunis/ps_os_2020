CFLAGS = -std=gnu11 -Wall -Werror -Wextra -O2

LIBS = -pthread

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: all clean task1 task2/task2 task3/task3_mutex task3/task3_atomic

all: task1/task1 task2/task2 task3/task3_mutex task3/task3_atomic

clean:
	$(RM) task1/thread*.txt task1/task1 task2/task2 task3/task3_mutex task3/task3_atomic

task1/task1: task1/task1.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

task2/task2: task2/task2.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

task3/task3_mutex: task3/task3_mutex.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

task3/task3_atomic: task3/task3_atomic.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^