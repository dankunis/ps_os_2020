CFLAGS = -std=gnu11 -pthread -Wall -Werror -Wextra -g
SOFLAGS = -fPIC -g
LDFLAGS = -pthread -ldl
SOLDFLAGS = -shared

.PHONY: all clean

all: membench system_allocator.so task2.so task3.so

clean:
	$(RM) membench system_allocator.so task2.so task3.so

membench: common/membench.c
	$(CC) $(CFLAGS) $^ -o $@

system_allocator.so: task1/system_allocator.c
	$(CC) $(SOFLAGS) $(CFLAGS) $^ -o $@ $(SOLDFLAGS)

task2.so: task2_3/task2_3.c
	$(CC) $(SOFLAGS) $(CFLAGS) $^ -o $@ $(SOLDFLAGS)

task3.so: task2_3/task2_3.c
	$(CC) $(SOFLAGS) $(CFLAGS) $^ -DPER_THREAD_ALLOCATOR -o $@ $(SOLDFLAGS)
